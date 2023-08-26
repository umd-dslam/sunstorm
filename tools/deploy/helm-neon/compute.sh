# Substitute variables in spec file and start compute node
set -eux

SPEC_FILE_ORIGINAL=/config/spec-template.json
SPEC_FILE=/tmp/spec.json

echo "Waiting pageserver become ready."
while ! nc -z pageserver 6400; do
    sleep 1;
done
echo "Page server is ready."

tenant_id=$(curl -s http://pageserver:9898/v1/tenant/ | jq -r '.[0].id')
if [ $tenant_id = "null" ]; then
    echo "ERROR: Tenant id is null"
    exit 1
fi

timelines=$(curl -s http://pageserver:9898/v1/tenant/${tenant_id}/timeline/)
region_id_to_timeline_id=$(echo ${timelines} | jq '[.[] | {key: .region_id, value: .timeline_id}] | from_entries')

timeline_id=$(echo ${region_id_to_timeline_id} | jq -r ".[\"${REGION}\"]")
if [ $timeline_id = "null" ]; then
    echo "ERROR: Timeline id is null"
    exit 1
fi

echo "Region id: ${REGION}"
echo "Tenant id: ${tenant_id}"
echo "Timeline id: ${timeline_id}"

echo "Overwrite variables in spec file"
sed "s/TENANT_ID/${tenant_id}/" ${SPEC_FILE_ORIGINAL} > ${SPEC_FILE}
sed -i "s/TIMELINE_ID/${timeline_id}/" ${SPEC_FILE}
sed -i "s/SAFEKEEPERS_ADDR/${SAFEKEEPERS_ADDR}/" ${SPEC_FILE}
sed -i "s/REGION/${REGION}/" ${SPEC_FILE}

cat ${SPEC_FILE}

echo "Start compute node"
/usr/local/bin/compute_ctl --pgdata /data/compute  \
    -C "postgresql://cloud_admin@localhost:55433/postgres"    \
    -b /usr/local/bin/postgres                                \
    -S /tmp/spec.json