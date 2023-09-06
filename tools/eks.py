import argparse
import itertools
import time
import subprocess
import yaml

from collections import namedtuple
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from typing import List
from rich.console import Console
from utils import get_regions, get_context, COLORS


BASE_PATH = Path(__file__).parent.resolve() / "deploy"

CONSOLE = Console()

RegionInfo = namedtuple("RegionInfo", ["name", "color", "is_global"])


def run_command(cmd: List[str], info: RegionInfo, dry_run: bool):
    CONSOLE.log(f"Running: {' '.join(cmd)}")

    if dry_run:
        return

    with subprocess.Popen(cmd, stdout=subprocess.PIPE) as proc:
        for line in proc.stdout:
            decoded = line.decode("utf-8").rstrip("\n")
            CONSOLE.print(
                f"[bold]\[{info.name}][/bold] {decoded}",
                style=info.color,
                highlight=False,
            )


def create_eks_cluster(info: RegionInfo, dry_run: bool):
    eks_config_file = BASE_PATH / f"eks/{info.name}.yaml"

    # Create the cluster
    run_command(
        [
            "eksctl",
            "create",
            "cluster",
            "--config-file",
            eks_config_file.as_posix(),
        ],
        info,
        dry_run,
    )

    if not dry_run:
        CONSOLE.log(
            f"EKS cluster in {info.name} is up. "
            f"Context: {get_context(BASE_PATH, info.name)}."
        )


def delete_eks_cluster(info: RegionInfo, dry_run: bool):
    eks_config_file = BASE_PATH / f"eks/{info.name}.yaml"

    if info.is_global:
        # The cluster deletion will be stuck if the EBS CSI driver
        # is not deleted first.
        with open(eks_config_file, "r") as yaml_file:
            eks_config = yaml.safe_load(yaml_file)
        run_command(
            [
                "eksctl",
                "delete",
                "addon",
                "--name",
                "aws-ebs-csi-driver",
                "--cluster",
                eks_config["metadata"]["name"],
                "--region",
                info.name,
            ],
            info,
            dry_run,
        )

    # Delete the cluster
    run_command(
        [
            "eksctl",
            "delete",
            "cluster",
            "--config-file",
            eks_config_file.as_posix(),
        ],
        info,
        dry_run,
    )

    CONSOLE.log(f"EKS cluster in {info.name} is deleted.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "action",
        choices=["create", "delete"],
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print the commands that would be executed without actually executing them.",
    )
    args = parser.parse_args()

    if args.action == "create":
        action_fn = create_eks_cluster
    elif args.action == "delete":
        action_fn = delete_eks_cluster
    else:
        raise ValueError(f"Unknown action: {args.action}")

    regions, global_region = get_regions(BASE_PATH)
    regions = set(regions)
    regions.add(global_region)

    colors = itertools.cycle(COLORS)

    with ThreadPoolExecutor() as executor:
        infos = [
            RegionInfo(region, next(colors), region == global_region)
            for region in regions
        ]
        tasks = [executor.submit(action_fn, info, args.dry_run) for info in infos]
        with CONSOLE.status("[bold green]Waiting..."):
            while any(task.running() for task in tasks):
                time.sleep(1)

        # Consume the exceptions if any
        for task in tasks:
            task.result()
