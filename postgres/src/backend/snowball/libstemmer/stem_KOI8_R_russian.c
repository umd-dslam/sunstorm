/* Generated by Snowball 2.1.0 - https://snowballstem.org/ */

#include "header.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int russian_KOI8_R_stem(struct SN_env * z);
#ifdef __cplusplus
}
#endif
static int r_tidy_up(struct SN_env * z);
static int r_derivational(struct SN_env * z);
static int r_noun(struct SN_env * z);
static int r_verb(struct SN_env * z);
static int r_reflexive(struct SN_env * z);
static int r_adjectival(struct SN_env * z);
static int r_adjective(struct SN_env * z);
static int r_perfective_gerund(struct SN_env * z);
static int r_R2(struct SN_env * z);
static int r_mark_regions(struct SN_env * z);
#ifdef __cplusplus
extern "C" {
#endif


extern struct SN_env * russian_KOI8_R_create_env(void);
extern void russian_KOI8_R_close_env(struct SN_env * z);


#ifdef __cplusplus
}
#endif
static const symbol s_0_0[3] = { 0xD7, 0xDB, 0xC9 };
static const symbol s_0_1[4] = { 0xC9, 0xD7, 0xDB, 0xC9 };
static const symbol s_0_2[4] = { 0xD9, 0xD7, 0xDB, 0xC9 };
static const symbol s_0_3[1] = { 0xD7 };
static const symbol s_0_4[2] = { 0xC9, 0xD7 };
static const symbol s_0_5[2] = { 0xD9, 0xD7 };
static const symbol s_0_6[5] = { 0xD7, 0xDB, 0xC9, 0xD3, 0xD8 };
static const symbol s_0_7[6] = { 0xC9, 0xD7, 0xDB, 0xC9, 0xD3, 0xD8 };
static const symbol s_0_8[6] = { 0xD9, 0xD7, 0xDB, 0xC9, 0xD3, 0xD8 };

static const struct among a_0[9] =
{
{ 3, s_0_0, -1, 1, 0},
{ 4, s_0_1, 0, 2, 0},
{ 4, s_0_2, 0, 2, 0},
{ 1, s_0_3, -1, 1, 0},
{ 2, s_0_4, 3, 2, 0},
{ 2, s_0_5, 3, 2, 0},
{ 5, s_0_6, -1, 1, 0},
{ 6, s_0_7, 6, 2, 0},
{ 6, s_0_8, 6, 2, 0}
};

static const symbol s_1_0[2] = { 0xC0, 0xC0 };
static const symbol s_1_1[2] = { 0xC5, 0xC0 };
static const symbol s_1_2[2] = { 0xCF, 0xC0 };
static const symbol s_1_3[2] = { 0xD5, 0xC0 };
static const symbol s_1_4[2] = { 0xC5, 0xC5 };
static const symbol s_1_5[2] = { 0xC9, 0xC5 };
static const symbol s_1_6[2] = { 0xCF, 0xC5 };
static const symbol s_1_7[2] = { 0xD9, 0xC5 };
static const symbol s_1_8[2] = { 0xC9, 0xC8 };
static const symbol s_1_9[2] = { 0xD9, 0xC8 };
static const symbol s_1_10[3] = { 0xC9, 0xCD, 0xC9 };
static const symbol s_1_11[3] = { 0xD9, 0xCD, 0xC9 };
static const symbol s_1_12[2] = { 0xC5, 0xCA };
static const symbol s_1_13[2] = { 0xC9, 0xCA };
static const symbol s_1_14[2] = { 0xCF, 0xCA };
static const symbol s_1_15[2] = { 0xD9, 0xCA };
static const symbol s_1_16[2] = { 0xC5, 0xCD };
static const symbol s_1_17[2] = { 0xC9, 0xCD };
static const symbol s_1_18[2] = { 0xCF, 0xCD };
static const symbol s_1_19[2] = { 0xD9, 0xCD };
static const symbol s_1_20[3] = { 0xC5, 0xC7, 0xCF };
static const symbol s_1_21[3] = { 0xCF, 0xC7, 0xCF };
static const symbol s_1_22[2] = { 0xC1, 0xD1 };
static const symbol s_1_23[2] = { 0xD1, 0xD1 };
static const symbol s_1_24[3] = { 0xC5, 0xCD, 0xD5 };
static const symbol s_1_25[3] = { 0xCF, 0xCD, 0xD5 };

static const struct among a_1[26] =
{
{ 2, s_1_0, -1, 1, 0},
{ 2, s_1_1, -1, 1, 0},
{ 2, s_1_2, -1, 1, 0},
{ 2, s_1_3, -1, 1, 0},
{ 2, s_1_4, -1, 1, 0},
{ 2, s_1_5, -1, 1, 0},
{ 2, s_1_6, -1, 1, 0},
{ 2, s_1_7, -1, 1, 0},
{ 2, s_1_8, -1, 1, 0},
{ 2, s_1_9, -1, 1, 0},
{ 3, s_1_10, -1, 1, 0},
{ 3, s_1_11, -1, 1, 0},
{ 2, s_1_12, -1, 1, 0},
{ 2, s_1_13, -1, 1, 0},
{ 2, s_1_14, -1, 1, 0},
{ 2, s_1_15, -1, 1, 0},
{ 2, s_1_16, -1, 1, 0},
{ 2, s_1_17, -1, 1, 0},
{ 2, s_1_18, -1, 1, 0},
{ 2, s_1_19, -1, 1, 0},
{ 3, s_1_20, -1, 1, 0},
{ 3, s_1_21, -1, 1, 0},
{ 2, s_1_22, -1, 1, 0},
{ 2, s_1_23, -1, 1, 0},
{ 3, s_1_24, -1, 1, 0},
{ 3, s_1_25, -1, 1, 0}
};

static const symbol s_2_0[2] = { 0xC5, 0xCD };
static const symbol s_2_1[2] = { 0xCE, 0xCE };
static const symbol s_2_2[2] = { 0xD7, 0xDB };
static const symbol s_2_3[3] = { 0xC9, 0xD7, 0xDB };
static const symbol s_2_4[3] = { 0xD9, 0xD7, 0xDB };
static const symbol s_2_5[1] = { 0xDD };
static const symbol s_2_6[2] = { 0xC0, 0xDD };
static const symbol s_2_7[3] = { 0xD5, 0xC0, 0xDD };

static const struct among a_2[8] =
{
{ 2, s_2_0, -1, 1, 0},
{ 2, s_2_1, -1, 1, 0},
{ 2, s_2_2, -1, 1, 0},
{ 3, s_2_3, 2, 2, 0},
{ 3, s_2_4, 2, 2, 0},
{ 1, s_2_5, -1, 1, 0},
{ 2, s_2_6, 5, 1, 0},
{ 3, s_2_7, 6, 2, 0}
};

static const symbol s_3_0[2] = { 0xD3, 0xD1 };
static const symbol s_3_1[2] = { 0xD3, 0xD8 };

static const struct among a_3[2] =
{
{ 2, s_3_0, -1, 1, 0},
{ 2, s_3_1, -1, 1, 0}
};

static const symbol s_4_0[1] = { 0xC0 };
static const symbol s_4_1[2] = { 0xD5, 0xC0 };
static const symbol s_4_2[2] = { 0xCC, 0xC1 };
static const symbol s_4_3[3] = { 0xC9, 0xCC, 0xC1 };
static const symbol s_4_4[3] = { 0xD9, 0xCC, 0xC1 };
static const symbol s_4_5[2] = { 0xCE, 0xC1 };
static const symbol s_4_6[3] = { 0xC5, 0xCE, 0xC1 };
static const symbol s_4_7[3] = { 0xC5, 0xD4, 0xC5 };
static const symbol s_4_8[3] = { 0xC9, 0xD4, 0xC5 };
static const symbol s_4_9[3] = { 0xCA, 0xD4, 0xC5 };
static const symbol s_4_10[4] = { 0xC5, 0xCA, 0xD4, 0xC5 };
static const symbol s_4_11[4] = { 0xD5, 0xCA, 0xD4, 0xC5 };
static const symbol s_4_12[2] = { 0xCC, 0xC9 };
static const symbol s_4_13[3] = { 0xC9, 0xCC, 0xC9 };
static const symbol s_4_14[3] = { 0xD9, 0xCC, 0xC9 };
static const symbol s_4_15[1] = { 0xCA };
static const symbol s_4_16[2] = { 0xC5, 0xCA };
static const symbol s_4_17[2] = { 0xD5, 0xCA };
static const symbol s_4_18[1] = { 0xCC };
static const symbol s_4_19[2] = { 0xC9, 0xCC };
static const symbol s_4_20[2] = { 0xD9, 0xCC };
static const symbol s_4_21[2] = { 0xC5, 0xCD };
static const symbol s_4_22[2] = { 0xC9, 0xCD };
static const symbol s_4_23[2] = { 0xD9, 0xCD };
static const symbol s_4_24[1] = { 0xCE };
static const symbol s_4_25[2] = { 0xC5, 0xCE };
static const symbol s_4_26[2] = { 0xCC, 0xCF };
static const symbol s_4_27[3] = { 0xC9, 0xCC, 0xCF };
static const symbol s_4_28[3] = { 0xD9, 0xCC, 0xCF };
static const symbol s_4_29[2] = { 0xCE, 0xCF };
static const symbol s_4_30[3] = { 0xC5, 0xCE, 0xCF };
static const symbol s_4_31[3] = { 0xCE, 0xCE, 0xCF };
static const symbol s_4_32[2] = { 0xC0, 0xD4 };
static const symbol s_4_33[3] = { 0xD5, 0xC0, 0xD4 };
static const symbol s_4_34[2] = { 0xC5, 0xD4 };
static const symbol s_4_35[3] = { 0xD5, 0xC5, 0xD4 };
static const symbol s_4_36[2] = { 0xC9, 0xD4 };
static const symbol s_4_37[2] = { 0xD1, 0xD4 };
static const symbol s_4_38[2] = { 0xD9, 0xD4 };
static const symbol s_4_39[2] = { 0xD4, 0xD8 };
static const symbol s_4_40[3] = { 0xC9, 0xD4, 0xD8 };
static const symbol s_4_41[3] = { 0xD9, 0xD4, 0xD8 };
static const symbol s_4_42[3] = { 0xC5, 0xDB, 0xD8 };
static const symbol s_4_43[3] = { 0xC9, 0xDB, 0xD8 };
static const symbol s_4_44[2] = { 0xCE, 0xD9 };
static const symbol s_4_45[3] = { 0xC5, 0xCE, 0xD9 };

static const struct among a_4[46] =
{
{ 1, s_4_0, -1, 2, 0},
{ 2, s_4_1, 0, 2, 0},
{ 2, s_4_2, -1, 1, 0},
{ 3, s_4_3, 2, 2, 0},
{ 3, s_4_4, 2, 2, 0},
{ 2, s_4_5, -1, 1, 0},
{ 3, s_4_6, 5, 2, 0},
{ 3, s_4_7, -1, 1, 0},
{ 3, s_4_8, -1, 2, 0},
{ 3, s_4_9, -1, 1, 0},
{ 4, s_4_10, 9, 2, 0},
{ 4, s_4_11, 9, 2, 0},
{ 2, s_4_12, -1, 1, 0},
{ 3, s_4_13, 12, 2, 0},
{ 3, s_4_14, 12, 2, 0},
{ 1, s_4_15, -1, 1, 0},
{ 2, s_4_16, 15, 2, 0},
{ 2, s_4_17, 15, 2, 0},
{ 1, s_4_18, -1, 1, 0},
{ 2, s_4_19, 18, 2, 0},
{ 2, s_4_20, 18, 2, 0},
{ 2, s_4_21, -1, 1, 0},
{ 2, s_4_22, -1, 2, 0},
{ 2, s_4_23, -1, 2, 0},
{ 1, s_4_24, -1, 1, 0},
{ 2, s_4_25, 24, 2, 0},
{ 2, s_4_26, -1, 1, 0},
{ 3, s_4_27, 26, 2, 0},
{ 3, s_4_28, 26, 2, 0},
{ 2, s_4_29, -1, 1, 0},
{ 3, s_4_30, 29, 2, 0},
{ 3, s_4_31, 29, 1, 0},
{ 2, s_4_32, -1, 1, 0},
{ 3, s_4_33, 32, 2, 0},
{ 2, s_4_34, -1, 1, 0},
{ 3, s_4_35, 34, 2, 0},
{ 2, s_4_36, -1, 2, 0},
{ 2, s_4_37, -1, 2, 0},
{ 2, s_4_38, -1, 2, 0},
{ 2, s_4_39, -1, 1, 0},
{ 3, s_4_40, 39, 2, 0},
{ 3, s_4_41, 39, 2, 0},
{ 3, s_4_42, -1, 1, 0},
{ 3, s_4_43, -1, 2, 0},
{ 2, s_4_44, -1, 1, 0},
{ 3, s_4_45, 44, 2, 0}
};

static const symbol s_5_0[1] = { 0xC0 };
static const symbol s_5_1[2] = { 0xC9, 0xC0 };
static const symbol s_5_2[2] = { 0xD8, 0xC0 };
static const symbol s_5_3[1] = { 0xC1 };
static const symbol s_5_4[1] = { 0xC5 };
static const symbol s_5_5[2] = { 0xC9, 0xC5 };
static const symbol s_5_6[2] = { 0xD8, 0xC5 };
static const symbol s_5_7[2] = { 0xC1, 0xC8 };
static const symbol s_5_8[2] = { 0xD1, 0xC8 };
static const symbol s_5_9[3] = { 0xC9, 0xD1, 0xC8 };
static const symbol s_5_10[1] = { 0xC9 };
static const symbol s_5_11[2] = { 0xC5, 0xC9 };
static const symbol s_5_12[2] = { 0xC9, 0xC9 };
static const symbol s_5_13[3] = { 0xC1, 0xCD, 0xC9 };
static const symbol s_5_14[3] = { 0xD1, 0xCD, 0xC9 };
static const symbol s_5_15[4] = { 0xC9, 0xD1, 0xCD, 0xC9 };
static const symbol s_5_16[1] = { 0xCA };
static const symbol s_5_17[2] = { 0xC5, 0xCA };
static const symbol s_5_18[3] = { 0xC9, 0xC5, 0xCA };
static const symbol s_5_19[2] = { 0xC9, 0xCA };
static const symbol s_5_20[2] = { 0xCF, 0xCA };
static const symbol s_5_21[2] = { 0xC1, 0xCD };
static const symbol s_5_22[2] = { 0xC5, 0xCD };
static const symbol s_5_23[3] = { 0xC9, 0xC5, 0xCD };
static const symbol s_5_24[2] = { 0xCF, 0xCD };
static const symbol s_5_25[2] = { 0xD1, 0xCD };
static const symbol s_5_26[3] = { 0xC9, 0xD1, 0xCD };
static const symbol s_5_27[1] = { 0xCF };
static const symbol s_5_28[1] = { 0xD1 };
static const symbol s_5_29[2] = { 0xC9, 0xD1 };
static const symbol s_5_30[2] = { 0xD8, 0xD1 };
static const symbol s_5_31[1] = { 0xD5 };
static const symbol s_5_32[2] = { 0xC5, 0xD7 };
static const symbol s_5_33[2] = { 0xCF, 0xD7 };
static const symbol s_5_34[1] = { 0xD8 };
static const symbol s_5_35[1] = { 0xD9 };

static const struct among a_5[36] =
{
{ 1, s_5_0, -1, 1, 0},
{ 2, s_5_1, 0, 1, 0},
{ 2, s_5_2, 0, 1, 0},
{ 1, s_5_3, -1, 1, 0},
{ 1, s_5_4, -1, 1, 0},
{ 2, s_5_5, 4, 1, 0},
{ 2, s_5_6, 4, 1, 0},
{ 2, s_5_7, -1, 1, 0},
{ 2, s_5_8, -1, 1, 0},
{ 3, s_5_9, 8, 1, 0},
{ 1, s_5_10, -1, 1, 0},
{ 2, s_5_11, 10, 1, 0},
{ 2, s_5_12, 10, 1, 0},
{ 3, s_5_13, 10, 1, 0},
{ 3, s_5_14, 10, 1, 0},
{ 4, s_5_15, 14, 1, 0},
{ 1, s_5_16, -1, 1, 0},
{ 2, s_5_17, 16, 1, 0},
{ 3, s_5_18, 17, 1, 0},
{ 2, s_5_19, 16, 1, 0},
{ 2, s_5_20, 16, 1, 0},
{ 2, s_5_21, -1, 1, 0},
{ 2, s_5_22, -1, 1, 0},
{ 3, s_5_23, 22, 1, 0},
{ 2, s_5_24, -1, 1, 0},
{ 2, s_5_25, -1, 1, 0},
{ 3, s_5_26, 25, 1, 0},
{ 1, s_5_27, -1, 1, 0},
{ 1, s_5_28, -1, 1, 0},
{ 2, s_5_29, 28, 1, 0},
{ 2, s_5_30, 28, 1, 0},
{ 1, s_5_31, -1, 1, 0},
{ 2, s_5_32, -1, 1, 0},
{ 2, s_5_33, -1, 1, 0},
{ 1, s_5_34, -1, 1, 0},
{ 1, s_5_35, -1, 1, 0}
};

static const symbol s_6_0[3] = { 0xCF, 0xD3, 0xD4 };
static const symbol s_6_1[4] = { 0xCF, 0xD3, 0xD4, 0xD8 };

static const struct among a_6[2] =
{
{ 3, s_6_0, -1, 1, 0},
{ 4, s_6_1, -1, 1, 0}
};

static const symbol s_7_0[4] = { 0xC5, 0xCA, 0xDB, 0xC5 };
static const symbol s_7_1[1] = { 0xCE };
static const symbol s_7_2[1] = { 0xD8 };
static const symbol s_7_3[3] = { 0xC5, 0xCA, 0xDB };

static const struct among a_7[4] =
{
{ 4, s_7_0, -1, 1, 0},
{ 1, s_7_1, -1, 2, 0},
{ 1, s_7_2, -1, 3, 0},
{ 3, s_7_3, -1, 1, 0}
};

static const unsigned char g_v[] = { 35, 130, 34, 18 };

static const symbol s_0[] = { 0xC5 };

static int r_mark_regions(struct SN_env * z) {
    z->I[1] = z->l;
    z->I[0] = z->l;
    {   int c1 = z->c;
        {   
            int ret = out_grouping(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        z->I[1] = z->c;
        {   
            int ret = in_grouping(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        {   
            int ret = out_grouping(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        {   
            int ret = in_grouping(z, g_v, 192, 220, 1);
            if (ret < 0) goto lab0;
            z->c += ret;
        }
        z->I[0] = z->c;
    lab0:
        z->c = c1;
    }
    return 1;
}

static int r_R2(struct SN_env * z) {
    if (!(z->I[0] <= z->c)) return 0;
    return 1;
}

static int r_perfective_gerund(struct SN_env * z) {
    int among_var;
    z->ket = z->c;
    if (z->c <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((25166336 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0;
    among_var = find_among_b(z, a_0, 9);
    if (!(among_var)) return 0;
    z->bra = z->c;
    switch (among_var) {
        case 1:
            {   int m1 = z->l - z->c; (void)m1;
                if (z->c <= z->lb || z->p[z->c - 1] != 0xC1) goto lab1;
                z->c--;
                goto lab0;
            lab1:
                z->c = z->l - m1;
                if (z->c <= z->lb || z->p[z->c - 1] != 0xD1) return 0;
                z->c--;
            }
        lab0:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
        case 2:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_adjective(struct SN_env * z) {
    z->ket = z->c;
    if (z->c - 1 <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((2271009 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0;
    if (!(find_among_b(z, a_1, 26))) return 0;
    z->bra = z->c;
    {   int ret = slice_del(z);
        if (ret < 0) return ret;
    }
    return 1;
}

static int r_adjectival(struct SN_env * z) {
    int among_var;
    {   int ret = r_adjective(z);
        if (ret <= 0) return ret;
    }
    {   int m1 = z->l - z->c; (void)m1;
        z->ket = z->c;
        if (z->c <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((671113216 >> (z->p[z->c - 1] & 0x1f)) & 1)) { z->c = z->l - m1; goto lab0; }
        among_var = find_among_b(z, a_2, 8);
        if (!(among_var)) { z->c = z->l - m1; goto lab0; }
        z->bra = z->c;
        switch (among_var) {
            case 1:
                {   int m2 = z->l - z->c; (void)m2;
                    if (z->c <= z->lb || z->p[z->c - 1] != 0xC1) goto lab2;
                    z->c--;
                    goto lab1;
                lab2:
                    z->c = z->l - m2;
                    if (z->c <= z->lb || z->p[z->c - 1] != 0xD1) { z->c = z->l - m1; goto lab0; }
                    z->c--;
                }
            lab1:
                {   int ret = slice_del(z);
                    if (ret < 0) return ret;
                }
                break;
            case 2:
                {   int ret = slice_del(z);
                    if (ret < 0) return ret;
                }
                break;
        }
    lab0:
        ;
    }
    return 1;
}

static int r_reflexive(struct SN_env * z) {
    z->ket = z->c;
    if (z->c - 1 <= z->lb || (z->p[z->c - 1] != 209 && z->p[z->c - 1] != 216)) return 0;
    if (!(find_among_b(z, a_3, 2))) return 0;
    z->bra = z->c;
    {   int ret = slice_del(z);
        if (ret < 0) return ret;
    }
    return 1;
}

static int r_verb(struct SN_env * z) {
    int among_var;
    z->ket = z->c;
    if (z->c <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((51443235 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0;
    among_var = find_among_b(z, a_4, 46);
    if (!(among_var)) return 0;
    z->bra = z->c;
    switch (among_var) {
        case 1:
            {   int m1 = z->l - z->c; (void)m1;
                if (z->c <= z->lb || z->p[z->c - 1] != 0xC1) goto lab1;
                z->c--;
                goto lab0;
            lab1:
                z->c = z->l - m1;
                if (z->c <= z->lb || z->p[z->c - 1] != 0xD1) return 0;
                z->c--;
            }
        lab0:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
        case 2:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

static int r_noun(struct SN_env * z) {
    z->ket = z->c;
    if (z->c <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((60991267 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0;
    if (!(find_among_b(z, a_5, 36))) return 0;
    z->bra = z->c;
    {   int ret = slice_del(z);
        if (ret < 0) return ret;
    }
    return 1;
}

static int r_derivational(struct SN_env * z) {
    z->ket = z->c;
    if (z->c - 2 <= z->lb || (z->p[z->c - 1] != 212 && z->p[z->c - 1] != 216)) return 0;
    if (!(find_among_b(z, a_6, 2))) return 0;
    z->bra = z->c;
    {   int ret = r_R2(z);
        if (ret <= 0) return ret;
    }
    {   int ret = slice_del(z);
        if (ret < 0) return ret;
    }
    return 1;
}

static int r_tidy_up(struct SN_env * z) {
    int among_var;
    z->ket = z->c;
    if (z->c <= z->lb || z->p[z->c - 1] >> 5 != 6 || !((151011360 >> (z->p[z->c - 1] & 0x1f)) & 1)) return 0;
    among_var = find_among_b(z, a_7, 4);
    if (!(among_var)) return 0;
    z->bra = z->c;
    switch (among_var) {
        case 1:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            z->ket = z->c;
            if (z->c <= z->lb || z->p[z->c - 1] != 0xCE) return 0;
            z->c--;
            z->bra = z->c;
            if (z->c <= z->lb || z->p[z->c - 1] != 0xCE) return 0;
            z->c--;
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
        case 2:
            if (z->c <= z->lb || z->p[z->c - 1] != 0xCE) return 0;
            z->c--;
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
        case 3:
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
            break;
    }
    return 1;
}

extern int russian_KOI8_R_stem(struct SN_env * z) {
    {   int c1 = z->c;
        while(1) {
            int c2 = z->c;
            while(1) {
                int c3 = z->c;
                z->bra = z->c;
                if (z->c == z->l || z->p[z->c] != 0xA3) goto lab2;
                z->c++;
                z->ket = z->c;
                z->c = c3;
                break;
            lab2:
                z->c = c3;
                if (z->c >= z->l) goto lab1;
                z->c++;
            }
            {   int ret = slice_from_s(z, 1, s_0);
                if (ret < 0) return ret;
            }
            continue;
        lab1:
            z->c = c2;
            break;
        }
        z->c = c1;
    }
    
    {   int ret = r_mark_regions(z);
        if (ret < 0) return ret;
    }
    z->lb = z->c; z->c = z->l;


    {   int mlimit4;
        if (z->c < z->I[1]) return 0;
        mlimit4 = z->lb; z->lb = z->I[1];
        {   int m5 = z->l - z->c; (void)m5;
            {   int m6 = z->l - z->c; (void)m6;
                {   int ret = r_perfective_gerund(z);
                    if (ret == 0) goto lab5;
                    if (ret < 0) return ret;
                }
                goto lab4;
            lab5:
                z->c = z->l - m6;
                {   int m7 = z->l - z->c; (void)m7;
                    {   int ret = r_reflexive(z);
                        if (ret == 0) { z->c = z->l - m7; goto lab6; }
                        if (ret < 0) return ret;
                    }
                lab6:
                    ;
                }
                {   int m8 = z->l - z->c; (void)m8;
                    {   int ret = r_adjectival(z);
                        if (ret == 0) goto lab8;
                        if (ret < 0) return ret;
                    }
                    goto lab7;
                lab8:
                    z->c = z->l - m8;
                    {   int ret = r_verb(z);
                        if (ret == 0) goto lab9;
                        if (ret < 0) return ret;
                    }
                    goto lab7;
                lab9:
                    z->c = z->l - m8;
                    {   int ret = r_noun(z);
                        if (ret == 0) goto lab3;
                        if (ret < 0) return ret;
                    }
                }
            lab7:
                ;
            }
        lab4:
        lab3:
            z->c = z->l - m5;
        }
        {   int m9 = z->l - z->c; (void)m9;
            z->ket = z->c;
            if (z->c <= z->lb || z->p[z->c - 1] != 0xC9) { z->c = z->l - m9; goto lab10; }
            z->c--;
            z->bra = z->c;
            {   int ret = slice_del(z);
                if (ret < 0) return ret;
            }
        lab10:
            ;
        }
        {   int m10 = z->l - z->c; (void)m10;
            {   int ret = r_derivational(z);
                if (ret < 0) return ret;
            }
            z->c = z->l - m10;
        }
        {   int m11 = z->l - z->c; (void)m11;
            {   int ret = r_tidy_up(z);
                if (ret < 0) return ret;
            }
            z->c = z->l - m11;
        }
        z->lb = mlimit4;
    }
    z->c = z->lb;
    return 1;
}

extern struct SN_env * russian_KOI8_R_create_env(void) { return SN_create_env(0, 2); }

extern void russian_KOI8_R_close_env(struct SN_env * z) { SN_close_env(z, 0); }
