// this will be optimised and rewritten

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"
#include "common.h"

extern unsigned char sppc64[];
extern unsigned int sppc64_len;

typedef struct
{
    const char *version;
    const char *product;
    const char *generated_key;
    const char *key_type;
} office_license_key_t;

struct Software
{
    const char *product;
    const char *generated_key;
};

office_license_key_t office_license_keys[] = {
    {"v15.0 (2013)", "AccessRetail", "B7RFY-7NXPK-Q4342-Y9X2H-3JX4X", "Retail"},
    {"v15.0 (2013)", "AccessRuntimeRetail", "X3XNB-HJB7K-66THH-8DWQ3-XHGJP", "Bypass"},
    {"v15.0 (2013)", "AccessVolume", "9MF9G-CN32B-HV7XT-9XJ8T-9KVF4", "MAK"},
    {"v15.0 (2013)", "ExcelRetail", "NT889-MBH4X-8MD4H-X8R2D-WQHF8", "Retail"},
    {"v15.0 (2013)", "ExcelVolume", "Y3N36-YCHDK-XYWBG-KYQVV-BDTJ2", "MAK"},
    {"v15.0 (2013)", "GrooveRetail", "BMK4W-6N88B-BP9QR-PHFCK-MG7GF", "Retail"},
    {"v15.0 (2013)", "GrooveVolume", "RN84D-7HCWY-FTCBK-JMXWM-HT7GJ", "MAK"},
    {"v15.0 (2013)", "HomeBusinessPipcRetail", "2WQNF-GBK4B-XVG6F-BBMX7-M4F2Y", "OEM-Perp"},
    {"v15.0 (2013)", "HomeBusinessRetail", "YWD4R-CNKVT-VG8VJ-9333B-RCW9F", "Subscription"},
    {"v15.0 (2013)", "HomeStudentARMRetail", "B92QY-NKYFQ-6KTKH-VWW2Q-3PB3B", "OEM-ARM"},
    {"v15.0 (2013)", "HomeStudentPlusARMRetail", "QPG96-CNT7M-KH36K-KY4HQ-M7TBR", "OEM-ARM"},
    {"v15.0 (2013)", "HomeStudentRetail", "6WW3N-BDGM9-PCCHD-9QPP9-P34QG", "Subscription"},
    {"v15.0 (2013)", "InfoPathRetail", "RV7NQ-HY3WW-7CKWH-QTVMW-29VHC", "Retail"},
    {"v15.0 (2013)", "InfoPathVolume", "C4TGN-QQW6Y-FYKXC-6WJW7-X73VG", "MAK"},
    {"v15.0 (2013)", "LyncAcademicRetail", "6MDN4-WF3FV-4WH3Q-W699V-RGCMY", "PrepidBypass"},
    {"v15.0 (2013)", "LyncEntryRetail", "N42BF-CBY9F-W2C7R-X397X-DYFQW", "PrepidBypass"},
    {"v15.0 (2013)", "LyncRetail", "89P23-2NK2R-JXM2M-3Q8R8-BWM3Y", "Retail"},
    {"v15.0 (2013)", "LyncVolume", "3WKCD-RN489-4M7XJ-GJ2GQ-YBFQ6", "MAK"},
    {"v15.0 (2013)", "MondoRetail", "VNWHF-FKFBW-Q2RGD-HYHWF-R3HH2", "Subscription"},
    {"v15.0 (2013)", "MondoVolume", "2YNYQ-FQMVG-CB8KW-6XKYD-M7RRJ", "MAK"},
    {"v15.0 (2013)", "OneNoteFreeRetail", "4TGWV-6N9P6-G2H8Y-2HWKB-B4FF4", "Bypass"},
    {"v15.0 (2013)", "OneNoteRetail", "3KXXQ-PVN2C-8P7YY-HCV88-GVGQ6", "Retail"},
    {"v15.0 (2013)", "OneNoteVolume", "JDMWF-NJC7B-HRCHY-WFT8G-BPXD9", "MAK"},
    {"v15.0 (2013)", "OutlookRetail", "9N4RQ-CF8R2-HBVCB-J3C9V-94P4D", "Retail"},
    {"v15.0 (2013)", "OutlookVolume", "HNG29-GGWRG-RFC8C-JTFP4-2J9FH", "MAK"},
    {"v15.0 (2013)", "PersonalPipcRetail", "9CYB3-NFMRW-YFDG6-XC7TF-BY36J", "OEM-Perp"},
    {"v15.0 (2013)", "PersonalRetail", "2NCQJ-MFRMH-TXV83-J7V4C-RVRWC", "Retail"},
    {"v15.0 (2013)", "PowerPointRetail", "HVMN2-KPHQH-DVQMK-7B3CM-FGBFC", "Retail"},
    {"v15.0 (2013)", "PowerPointVolume", "47DKN-HPJP7-RF9M3-VCYT2-TMQ4G", "MAK"},
    {"v15.0 (2013)", "ProPlusRetail", "N3QMM-GKDT3-JQGX6-7X3MQ-4GBG3", "Retail"},
    {"v15.0 (2013)", "ProPlusVolume", "QKHNX-M9GGH-T3QMW-YPK4Q-QRP9V", "MAK"},
    {"v15.0 (2013)", "ProfessionalPipcRetail", "CF9DD-6CNW2-BJWJQ-CVCFX-Y7TXD", "OEM-Perp"},
    {"v15.0 (2013)", "ProfessionalRetail", "MBQBN-CQPT6-PXRMC-TYJFR-3C8MY", "Retail"},
    {"v15.0 (2013)", "ProjectProRetail", "WPY8N-PDPY4-FC7TF-KMP7P-KWYFY", "Subscription"},
    {"v15.0 (2013)", "ProjectProVolume", "WFCT2-NBFQ7-JD7VV-MFJX6-6F2CM", "MAK"},
    {"v15.0 (2013)", "ProjectStdRetail", "NTHQT-VKK6W-BRB87-HV346-Y96W8", "Subscription"},
    {"v15.0 (2013)", "ProjectStdVolume", "3CNQX-T34TY-99RH4-C4YD2-KWYGV", "MAK"},
    {"v15.0 (2013)", "PublisherRetail", "TWNCJ-YR84W-X7PPF-6DPRP-D67VC", "Retail"},
    {"v15.0 (2013)", "PublisherVolume", "DJPHV-NCJV6-GWPT6-K26JX-C7GX6", "MAK"},
    {"v15.0 (2013)", "SPDRetail", "3NY6J-WHT3F-47BDV-JHF36-2343W", "PrepidBypass"},
    {"v15.0 (2013)", "StandardRetail", "V6VWN-KC2HR-YYDD6-9V7HQ-7T7VP", "Retail"},
    {"v15.0 (2013)", "StandardVolume", "9TN6B-PCYH4-MCVDQ-KT83C-TMQ7T", "MAK"},
    {"v15.0 (2013)", "VisioProRetail", "NVK2G-2MY4G-7JX2P-7D6F2-VFQBR", "Subscription"},
    {"v15.0 (2013)", "VisioProVolume", "YN7CF-XRH6R-CGKRY-GKPV3-BG7WF", "MAK"},
    {"v15.0 (2013)", "VisioStdRetail", "NCRB7-VP48F-43FYY-62P3R-367WK", "Subscription"},
    {"v15.0 (2013)", "VisioStdVolume", "RX63Y-4NFK2-XTYC8-C6B3W-YPXPJ", "MAK"},
    {"v15.0 (2013)", "WordRetail", "NB77V-RPFQ6-PMMKQ-T87DV-M4D84", "Retail"},
    {"v15.0 (2013)", "WordVolume", "RPHPB-Y7NC4-3VYFM-DW7VD-G8YJ8", "MAK"},
    {"v15.0 (O365)", "O365BusinessRetail", "Y9NF9-M2QWD-FF6RJ-QJW36-RRF2T", "SubTest"},
    {"v15.0 (O365)", "O365HomePremRetail", "3NMDC-G7C3W-68RGP-CB4MH-4CXCH", "SubTest1"},
    {"v15.0 (O365)", "O365ProPlusRetail", "H8DN8-Y2YP3-CR9JT-DHDR9-C7GP3", "Subscription2"},
    {"v15.0 (O365)", "O365SmallBusPremRetail", "2QCNB-RMDKJ-GC8PB-7QGQV-7QTQJ", "Subscription2"},
    {"v16.0 (O365)", "O365BusinessRetail", "Y9NF9-M2QWD-FF6RJ-QJW36-RRF2T", "SubTest"},
    {"v16.0 (O365)", "O365EduCloudRetail", "W62NQ-267QR-RTF74-PF2MH-JQMTH", "Subscription"},
    {"v16.0 (O365)", "O365HomePremRetail", "3NMDC-G7C3W-68RGP-CB4MH-4CXCH", "SubTest1"},
    {"v16.0 (O365)", "O365ProPlusRetail", "H8DN8-Y2YP3-CR9JT-DHDR9-C7GP3", "Subscription2"},
    {"v16.0 (2016)", "AccessRetail", "WHK4N-YQGHB-XWXCC-G3HYC-6JF94", "Retail"},
    {"v16.0 (2016)", "AccessRuntimeRetail", "RNB7V-P48F4-3FYY6-2P3R3-63BQV", "PrepidBypass"},
    {"v16.0 (2016)", "AccessVolume", "JJ2Y4-N8KM3-Y8KY3-Y22FR-R3KVK", "MAK"},
    {"v16.0 (2016)", "ExcelRetail", "RKJBN-VWTM2-BDKXX-RKQFD-JTYQ2", "Retail"},
    {"v16.0 (2016)", "ExcelVolume", "FVGNR-X82B2-6PRJM-YT4W7-8HV36", "MAK"},
    {"v16.0 (2016)", "HomeBusinessPipcRetail", "2WQNF-GBK4B-XVG6F-BBMX7-M4F2Y", "OEM-Perp"},
    {"v16.0 (2016)", "HomeBusinessRetail", "HM6FM-NVF78-KV9PM-F36B8-D9MXD", "Retail"},
    {"v16.0 (2016)", "HomeStudentARMRetail", "PBQPJ-NC22K-69MXD-KWMRF-WFG77", "OEM-ARM"},
    {"v16.0 (2016)", "HomeStudentPlusARMRetail", "6F2NY-7RTX4-MD9KM-TJ43H-94TBT", "OEM-ARM"},
    {"v16.0 (2016)", "HomeStudentRetail", "PNPRV-F2627-Q8JVC-3DGR9-WTYRK", "Retail"},
    {"v16.0 (2016)", "HomeStudentVNextRetail", "YWD4R-CNKVT-VG8VJ-9333B-RC3B8", "Retail"},
    {"v16.0 (2016)", "MondoRetail", "VNWHF-FKFBW-Q2RGD-HYHWF-R3HH2", "Subscription"},
    {"v16.0 (2016)", "MondoVolume", "FMTQQ-84NR8-2744R-MXF4P-PGYR3", "MAK"},
    {"v16.0 (2016)", "OneNoteFreeRetail", "XYNTG-R96FY-369HX-YFPHY-F9CPM", "Bypass"},
    {"v16.0 (2016)", "OneNoteRetail", "FXF6F-CNC26-W643C-K6KB7-6XXW3", "Retail"},
    {"v16.0 (2016)", "OneNoteVolume", "9TYVN-D76HK-BVMWT-Y7G88-9TPPV", "MAK"},
    {"v16.0 (2016)", "OutlookRetail", "7N4KG-P2QDH-86V9C-DJFVF-369W9", "Retail"},
    {"v16.0 (2016)", "OutlookVolume", "7QPNR-3HFDG-YP6T9-JQCKQ-KKXXC", "MAK"},
    {"v16.0 (2016)", "PersonalPipcRetail", "9CYB3-NFMRW-YFDG6-XC7TF-BY36J", "OEM-Perp"},
    {"v16.0 (2016)", "PersonalRetail", "FT7VF-XBN92-HPDJV-RHMBY-6VKBF", "Retail"},
    {"v16.0 (2016)", "PowerPointRetail", "N7GCB-WQT7K-QRHWG-TTPYD-7T9XF", "Retail"},
    {"v16.0 (2016)", "PowerPointVolume", "X3RT9-NDG64-VMK2M-KQ6XY-DPFGV", "MAK"},
    {"v16.0 (2016)", "ProPlusRetail", "GM43N-F742Q-6JDDK-M622J-J8GDV", "Retail"},
    {"v16.0 (2016)", "ProPlusVolume", "FNVK8-8DVCJ-F7X3J-KGVQB-RC2QY", "MAK"},
    {"v16.0 (2016)", "ProfessionalPipcRetail", "CF9DD-6CNW2-BJWJQ-CVCFX-Y7TXD", "OEM-Perp"},
    {"v16.0 (2016)", "ProfessionalRetail", "NXFTK-YD9Y7-X9MMJ-9BWM6-J2QVH", "Retail"},
    {"v16.0 (2016)", "ProjectProRetail", "WPY8N-PDPY4-FC7TF-KMP7P-KWYFY", "Subscription"},
    {"v16.0 (2016)", "ProjectProVolume", "PKC3N-8F99H-28MVY-J4RYY-CWGDH", "MAK"},
    {"v16.0 (2016)", "ProjectProXVolume", "JBNPH-YF2F7-Q9Y29-86CTG-C9YGV", "MAKC2R"},
    {"v16.0 (2016)", "ProjectStdRetail", "NTHQT-VKK6W-BRB87-HV346-Y96W8", "Subscription"},
    {"v16.0 (2016)", "ProjectStdVolume", "4TGWV-6N9P6-G2H8Y-2HWKB-B4G93", "MAK"},
    {"v16.0 (2016)", "ProjectStdXVolume", "N3W2Q-69MBT-27RD9-BH8V3-JT2C8", "MAKC2R"},
    {"v16.0 (2016)", "PublisherRetail", "WKWND-X6G9G-CDMTV-CPGYJ-6MVBF", "Retail"},
    {"v16.0 (2016)", "PublisherVolume", "9QVN2-PXXRX-8V4W8-Q7926-TJGD8", "MAK"},
    {"v16.0 (2016)", "SkypeServiceBypassRetail", "6MDN4-WF3FV-4WH3Q-W699V-RGCMY", "PrepidBypass"},
    {"v16.0 (2016)", "SkypeforBusinessEntryRetail", "4N4D8-3J7Y3-YYW7C-73HD2-V8RHY", "PrepidBypass"},
    {"v16.0 (2016)", "SkypeforBusinessRetail", "PBJ79-77NY4-VRGFG-Y8WYC-CKCRC", "Retail"},
    {"v16.0 (2016)", "SkypeforBusinessVolume", "DMTCJ-KNRKR-JV8TQ-V2CR2-VFTFH", "MAK"},
    {"v16.0 (2016)", "StandardRetail", "2FPWN-4H6CM-KD8QQ-8HCHC-P9XYW", "Retail"},
    {"v16.0 (2016)", "StandardVolume", "WHGMQ-JNMGT-MDQVF-WDR69-KQBWC", "MAK"},
    {"v16.0 (2016)", "VisioProRetail", "NVK2G-2MY4G-7JX2P-7D6F2-VFQBR", "Subscription"},
    {"v16.0 (2016)", "VisioProVolume", "NRKT9-C8GP2-XDYXQ-YW72K-MG92B", "MAK"},
    {"v16.0 (2016)", "VisioProXVolume", "G98Q2-B6N77-CFH9J-K824G-XQCC4", "MAKC2R"},
    {"v16.0 (2016)", "VisioStdRetail", "NCRB7-VP48F-43FYY-62P3R-367WK", "Subscription"},
    {"v16.0 (2016)", "VisioStdVolume", "XNCJB-YY883-JRW64-DPXMX-JXCR6", "MAK"},
    {"v16.0 (2016)", "VisioStdXVolume", "B2HTN-JPH8C-J6Y6V-HCHKB-43MGT", "MAKC2R"},
    {"v16.0 (2016)", "WordRetail", "P8K82-NQ7GG-JKY8T-6VHVY-88GGD", "Retail"},
    {"v16.0 (2016)", "WordVolume", "YHMWC-YN6V9-WJPXD-3WQKP-TMVCV", "MAK"},
    {"v16.0 (2019)", "Access2019Retail", "WRYJ6-G3NP7-7VH94-8X7KP-JB7HC", "Retail"},
    {"v16.0 (2019)", "Access2019Volume", "6FWHX-NKYXK-BW34Q-7XC9F-Q9PX7", "MAK-AE"},
    {"v16.0 (2019)", "AccessRuntime2019Retail", "FGQNJ-JWJCG-7Q8MG-RMRGJ-9TQVF", "PrepidBypass"},
    {"v16.0 (2019)", "Excel2019Retail", "KBPNW-64CMM-8KWCB-23F44-8B7HM", "Retail"},
    {"v16.0 (2019)", "Excel2019Volume", "8NT4X-GQMCK-62X4P-TW6QP-YKPYF", "MAK-AE"},
    {"v16.0 (2019)", "HomeBusiness2019Retail", "QBN2Y-9B284-9KW78-K48PB-R62YT", "Retail"},
    {"v16.0 (2019)", "HomeStudentARM2019Retail", "DJTNY-4HDWM-TDWB2-8PWC2-W2RRT", "OEM-ARM"},
    {"v16.0 (2019)", "HomeStudentPlusARM2019Retail", "NM8WT-CFHB2-QBGXK-J8W6J-GVK8F", "OEM-ARM"},
    {"v16.0 (2019)", "HomeStudent2019Retail", "XNWPM-32XQC-Y7QJC-QGGBV-YY7JK", "Retail"},
    {"v16.0 (2019)", "Outlook2019Retail", "WR43D-NMWQQ-HCQR2-VKXDR-37B7H", "Retail"},
    {"v16.0 (2019)", "Outlook2019Volume", "RN3QB-GT6D7-YB3VH-F3RPB-3GQYB", "MAK-AE"},
    {"v16.0 (2019)", "Personal2019Retail", "NMBY8-V3CV7-BX6K6-2922Y-43M7T", "Retail"},
    {"v16.0 (2019)", "PowerPoint2019Retail", "HN27K-JHJ8R-7T7KK-WJYC3-FM7MM", "Retail"},
    {"v16.0 (2019)", "PowerPoint2019Volume", "29GNM-VM33V-WR23K-HG2DT-KTQYR", "MAK-AE"},
    {"v16.0 (2019)", "ProPlus2019Retail", "BN4XJ-R9DYY-96W48-YK8DM-MY7PY", "Retail"},
    {"v16.0 (2019)", "ProPlus2019Volume", "T8YBN-4YV3X-KK24Q-QXBD7-T3C63", "MAK-AE"},
    {"v16.0 (2019)", "Professional2019Retail", "9NXDK-MRY98-2VJV8-GF73J-TQ9FK", "Retail"},
    {"v16.0 (2019)", "ProjectPro2019Retail", "JDTNC-PP77T-T9H2W-G4J2J-VH8JK", "Retail"},
    {"v16.0 (2019)", "ProjectPro2019Volume", "TBXBD-FNWKJ-WRHBD-KBPHH-XD9F2", "MAK-AE"},
    {"v16.0 (2019)", "ProjectStd2019Retail", "R3JNT-8PBDP-MTWCK-VD2V8-HMKF9", "Retail"},
    {"v16.0 (2019)", "ProjectStd2019Volume", "RBRFX-MQNDJ-4XFHF-7QVDR-JHXGC", "MAK-AE"},
    {"v16.0 (2019)", "Publisher2019Retail", "4QC36-NW3YH-D2Y9D-RJPC7-VVB9D", "Retail"},
    {"v16.0 (2019)", "Publisher2019Volume", "K8F2D-NBM32-BF26V-YCKFJ-29Y9W", "MAK-AE"},
    {"v16.0 (2019)", "SkypeforBusiness2019Retail", "JBDKF-6NCD6-49K3G-2TV79-BKP73", "Retail"},
    {"v16.0 (2019)", "SkypeforBusiness2019Volume", "9MNQ7-YPQ3B-6WJXM-G83T3-CBBDK", "MAK-AE"},
    {"v16.0 (2019)", "SkypeforBusinessEntry2019Retail", "N9722-BV9H6-WTJTT-FPB93-978MK", "PrepidBypass"},
    {"v16.0 (2019)", "Standard2019Retail", "NDGVM-MD27H-2XHVC-KDDX2-YKP74", "Retail"},
    {"v16.0 (2019)", "Standard2019Volume", "NT3V6-XMBK7-Q66MF-VMKR4-FC33M", "MAK-AE"},
    {"v16.0 (2019)", "VisioPro2019Retail", "2NWVW-QGF4T-9CPMB-WYDQ9-7XP79", "Retail"},
    {"v16.0 (2019)", "VisioPro2019Volume", "33YF4-GNCQ3-J6GDM-J67P3-FM7QP", "MAK-AE"},
    {"v16.0 (2019)", "VisioStd2019Retail", "263WK-3N797-7R437-28BKG-3V8M8", "Retail"},
    {"v16.0 (2019)", "VisioStd2019Volume", "BGNHX-QTPRJ-F9C9G-R8QQG-8T27F", "MAK-AE"},
    {"v16.0 (2019)", "Word2019Retail", "JXR8H-NJ3MK-X66W8-78CWD-QRVR2", "Retail"},
    {"v16.0 (2019)", "Word2019Volume", "9F36R-PNVHH-3DXGQ-7CD2H-R9D3V", "MAK-AE"},
    {"v16.0 (2021)", "Access2021Retail", "P286B-N3XYP-36QRQ-29CMP-RVX9M", "Retail"},
    {"v16.0 (2021)", "AccessRuntime2021Retail", "MNX9D-PB834-VCGY2-K2RW2-2DP3D", "Bypass"},
    {"v16.0 (2021)", "Access2021Volume", "JBH3N-P97FP-FRTJD-MGK2C-VFWG6", "MAK-AE"},
    {"v16.0 (2021)", "Excel2021Retail", "V6QFB-7N7G9-PF7W9-M8FQM-MY8G9", "Retail"},
    {"v16.0 (2021)", "Excel2021Volume", "WNYR4-KMR9H-KVC8W-7HJ8B-K79DQ", "MAK-AE"},
    {"v16.0 (2021)", "HomeBusiness2021Retail", "JM99N-4MMD8-DQCGJ-VMYFY-R63YK", "Subscription"},
    {"v16.0 (2021)", "HomeStudent2021Retail", "N3CWD-38XVH-KRX2Y-YRP74-6RBB2", "Subscription"},
    {"v16.0 (2021)", "OneNoteFree2021Retail", "CNM3W-V94GB-QJQHH-BDQ3J-33Y8H", "Bypass"},
    {"v16.0 (2021)", "OneNote2021Retail", "NB2TQ-3Y79C-77C6M-QMY7H-7QY8P", "Retail"},
    {"v16.0 (2021)", "OneNote2021Volume", "THNKC-KFR6C-Y86Q9-W8CB3-GF7PD", "MAK-AE"},
    {"v16.0 (2021)", "Outlook2021Retail", "4NCWR-9V92Y-34VB2-RPTHR-YTGR7", "Retail"},
    {"v16.0 (2021)", "Outlook2021Volume", "JQ9MJ-QYN6B-67PX9-GYFVY-QJ6TB", "MAK-AE"},
    {"v16.0 (2021)", "Personal2021Retail", "RRRYB-DN749-GCPW4-9H6VK-HCHPT", "Retail"},
    {"v16.0 (2021)", "PowerPoint2021Retail", "3KXXQ-PVN2C-8P7YY-HCV88-GVM96", "Retail1"},
    {"v16.0 (2021)", "PowerPoint2021Volume", "39G2N-3BD9C-C4XCM-BD4QG-FVYDY", "MAK-AE"},
    {"v16.0 (2021)", "ProPlus2021Retail", "8WXTP-MN628-KY44G-VJWCK-C7PCF", "Retail"},
    {"v16.0 (2021)", "ProPlus2021Volume", "RNHJY-DTFXW-HW9F8-4982D-MD2CW", "MAK-AE1"},
    {"v16.0 (2021)", "ProPlusSPLA2021Volume", "JRJNJ-33M7C-R73X3-P9XF7-R9F6M", "MAK-AE"},
    {"v16.0 (2021)", "Professional2021Retail", "DJPHV-NCJV6-GWPT6-K26JX-C7PBG", "Retail"},
    {"v16.0 (2021)", "ProjectPro2021Retail", "QKHNX-M9GGH-T3QMW-YPK4Q-QRWMV", "Retail"},
    {"v16.0 (2021)", "ProjectPro2021Volume", "HVC34-CVNPG-RVCMT-X2JRF-CR7RK", "MAK-AE1"},
    {"v16.0 (2021)", "ProjectStd2021Retail", "2B96V-X9NJY-WFBRC-Q8MP2-7CHRR", "Retail"},
    {"v16.0 (2021)", "ProjectStd2021Volume", "3CNQX-T34TY-99RH4-C4YD2-KW6WH", "MAK-AE"},
    {"v16.0 (2021)", "Publisher2021Retail", "CDNFG-77T8D-VKQJX-B7KT3-KK28V", "Retail1"},
    {"v16.0 (2021)", "Publisher2021Volume", "2KXJH-3NHTW-RDBPX-QFRXJ-MTGXF", "MAK-AE"},
    {"v16.0 (2021)", "SkypeforBusiness2021Retail", "DVBXN-HFT43-CVPRQ-J89TF-VMMHG", "Retail"},
    {"v16.0 (2021)", "SkypeforBusiness2021Volume", "R3FCY-NHGC7-CBPVP-8Q934-YTGXG", "MAK-AE"},
    {"v16.0 (2021)", "Standard2021Retail", "HXNXB-J4JGM-TCF44-2X2CV-FJVVH", "Retail"},
    {"v16.0 (2021)", "Standard2021Volume", "2CJN4-C9XK2-HFPQ6-YH498-82TXH", "MAK-AE"},
    {"v16.0 (2021)", "StandardSPLA2021Volume", "BQWDW-NJ9YF-P7Y79-H6DCT-MKQ9C", "MAK-AE"},
    {"v16.0 (2021)", "VisioPro2021Retail", "T6P26-NJVBR-76BK8-WBCDY-TX3BC", "Retail"},
    {"v16.0 (2021)", "VisioPro2021Volume", "JNKBX-MH9P4-K8YYV-8CG2Y-VQ2C8", "MAK-AE"},
    {"v16.0 (2021)", "VisioStd2021Retail", "89NYY-KB93R-7X22F-93QDF-DJ6YM", "Retail"},
    {"v16.0 (2021)", "VisioStd2021Volume", "BW43B-4PNFP-V637F-23TR2-J47TX", "MAK-AE"},
    {"v16.0 (2021)", "Word2021Retail", "VNCC4-CJQVK-BKX34-77Y8H-CYXMR", "Retail"},
    {"v16.0 (2021)", "Word2021Volume", "BJG97-NW3GM-8QQQ7-FH76G-686XM", "MAK-AE"},
    {"v16.0 (2024)", "Access2024Retail", "P6NMW-JMTRC-R6MQ6-HH3F2-BTHKB", "Retail"},
    {"v16.0 (2024)", "Access2024Volume", "CXNJT-98HPP-92HX7-MX6GY-2PVFR", "MAK-AE"},
    {"v16.0 (2024)", "Excel2024Retail", "82CNJ-W82TW-BY23W-BVJ6W-W48GP", "Retail"},
    {"v16.0 (2024)", "Excel2024Volume", "7Y287-9N2KC-8MRR3-BKY82-2DQRV", "MAK-AE"},
    {"v16.0 (2024)", "Home2024Retail", "N69X7-73KPT-899FD-P8HQ4-QGTP4", "Retail"},
    {"v16.0 (2024)", "HomeBusiness2024Retail", "PRKQM-YNPQR-77QT6-328D7-BD223", "Retail"},
    {"v16.0 (2024)", "Outlook2024Retail", "2CFK4-N44KG-7XG89-CWDG6-P7P27", "Retail"},
    {"v16.0 (2024)", "Outlook2024Volume", "NQPXP-WVB87-H3MMB-FYBW2-9QFPB", "MAK-AE"},
    {"v16.0 (2024)", "PowerPoint2024Retail", "CT2KT-GTNWH-9HFGW-J2PWJ-XW7KJ", "Retail"},
    {"v16.0 (2024)", "PowerPoint2024Volume", "RRXFN-JJ26R-RVWD2-V7WMP-27PWQ", "MAK-AE"},
    {"v16.0 (2024)", "ProjectPro2024Retail", "GNJ6P-Y4RBM-C32WW-2VJKJ-MTHKK", "Retail"},
    {"v16.0 (2024)", "ProjectPro2024Volume", "WNFMR-HK4R7-7FJVM-VQ3JC-76HF6", "MAK-AE1"},
    {"v16.0 (2024)", "ProjectStd2024Retail", "C2PNM-2GQFC-CY3XR-WXCP4-GX3XM", "Retail"},
    {"v16.0 (2024)", "ProjectStd2024Volume", "F2VNW-MW8TT-K622Q-4D96H-PWJ8X", "MAK-AE"},
    {"v16.0 (2024)", "ProPlus2024Retail", "VWCNX-7FKBD-FHJYG-XBR4B-88KC6", "Retail"},
    {"v16.0 (2024)", "ProPlus2024Volume", "4YV2J-VNG7W-YGTP3-443TK-TF8CP", "MAK-AE1"},
    {"v16.0 (2024)", "SkypeforBusiness2024Volume", "XKRBW-KN2FF-G8CKY-HXVG6-FVY2V", "MAK-AE"},
    {"v16.0 (2024)", "Standard2024Volume", "GVG6N-6WCHH-K2MVP-RQ78V-3J7GJ", "MAK-AE1"},
    {"v16.0 (2024)", "VisioPro2024Retail", "HGRBX-N68QF-6DY8J-CGX4W-XW7KP", "Retail"},
    {"v16.0 (2024)", "VisioPro2024Volume", "GBNHB-B2G3Q-G42YB-3MFC2-7CJCX", "MAK-AE"},
    {"v16.0 (2024)", "VisioStd2024Retail", "VBXPJ-38NR3-C4DKF-C8RT7-RGHKQ", "Retail"},
    {"v16.0 (2024)", "VisioStd2024Volume", "YNFTY-63K7P-FKHXK-28YYT-D32XB", "MAK-AE"},
    {"v16.0 (2024)", "Word2024Retail", "XN33R-RP676-GMY2F-T3MH7-GCVKR", "Retail"},
    {"v16.0 (2024)", "Word2024Volume", "WD8CQ-6KNQM-8W2CX-2RT63-KK3TP", "MAK-AE"}};

const char *office_get_license_key_for_product(const char *product_name)
{
    for (int i = 0; i < sizeof(office_license_keys) / sizeof(office_license_keys[0]); i++)
    {
        if (strcmp(office_license_keys[i].product, product_name) == 0)
        {
            return office_license_keys[i].generated_key;
        }
    }
    return NULL;
}

struct Software software[] = {
    {"SQL Server 2022 Enterprise", "J4V48-P8MM4-9N3J9-HD97X-DYMRM"},
    {"SQL Server 2022 Enterprise Core", "2Q48Q-PB48J-DRCVN-GB844-X2H4Q"},
    {"SQL Server 2022 Standard", "FG86G-CHH2T-CB7NJ-XT7D2-V8V4X"},
    {"SQL Server 2022 Web", "2R97W-F4XNT-T6MYV-3TKB7-6X3JM"},
    {"SQL Server 2022 Developer", "22222-00000-00000-00000-00000"},
    {"SQL Server 2019 Enterprise", "HMWJ3-KY3J2-NMVD7-KG4JR-X2G8G"},
    {"SQL Server 2019 Standard", "PMBDC-FXVM3-T777P-N4FY8-PKFF4"},
    {"32-bit (x86) Windows XP Pro VL SP3 ISO Activation key", "XCYBK-2B3KV-G8T8F-WXJM7-WCTYT"},
    {"64-bit (x64) Windows XP Pro VL SP2 ISO Activation key", "VCFQD-V9FX9-46WVH-K3CD4-4J3JM"}};

char *get_office_edition()
{
    HKEY hKey;
    LPCWSTR subKey = L"SOFTWARE\\Microsoft\\Office\\ClickToRun\\Configuration";
    LPCWSTR valueName = L"ProductReleaseIds";
    static char officeEdition[256];
    WCHAR data[256];
    DWORD dataSize = sizeof(data);

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegGetValueW(hKey, NULL, valueName, RRF_RT_REG_SZ, NULL, data, &dataSize) == ERROR_SUCCESS)
        {
            WideCharToMultiByte(CP_ACP, 0, data, -1, officeEdition, sizeof(officeEdition), NULL, NULL);
            MessageBoxA(NULL, officeEdition, "Installed Office Suite", MB_OK | MB_ICONINFORMATION);
            RegCloseKey(hKey);
            return officeEdition;
        }
        RegCloseKey(hKey);
    }
    MessageBoxA(NULL, "Failed to retrieve Office edition.", "Error", MB_OK | MB_ICONERROR);
    return NULL;
}

char *getLicenseKey(char *productName)
{
    for (int i = 0; i < sizeof(office_license_keys) / sizeof(office_license_keys[0]); i++)
    {
        if (strcmp(office_license_keys[i].product, productName) == 0)
        {
            return (char *)office_license_keys[i].generated_key;
        }
    }
    return NULL;
}

int create_sppc(const char *path)
{
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, "Failed to create file", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);

    DWORD written;
    WriteFile(hFile, sppc64, sppc64_len, &written, NULL);
    CloseHandle(hFile);

    if (written == sppc64_len)
    {
        MessageBoxA(NULL, "Successfully created file", "Success", MB_OK | MB_ICONINFORMATION);
        return 0;
    }
    else
    {
        MessageBoxA(NULL, "Failed to write data to file", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}

int create_unattended_office_xml()
{
    const char *config_xml =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Configuration>\n"
        "    <Add OfficeClientEdition=\"64\">\n"
        "        <Product ID=\"O365ProPlusRetail\">\n"
        "            <Language ID=\"en-us\"/>\n"
        "        </Product>\n"
        "    </Add>\n"
        "    <Display Level=\"None\" AcceptEULA=\"TRUE\"/>\n"
        "    <Property Name=\"AUTOACTIVATE\" Value=\"0\"/>\n"
        "</Configuration>\n";

    FILE *file = fopen("C:/Windows/Temp/config.xml", "w");
    if (file == NULL)
    {
        return -1;
    }

    fputs(config_xml, file);
    fclose(file);
    return 0;
}

void ohook_cb(const char *seq, const char *req, void *arg)
{
#ifdef _WIN32
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;

    if (MessageBoxA(NULL, "Do you want to download Office 365?", "Info", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        system_download_file("https://c2rsetup.officeapps.live.com/c2r/download.aspx?ProductreleaseID=O365ProPlusRetail&platform=x64&language=en-us&version=O16GA", "C:/Windows/Temp/OfficeSetup.exe");

        /*
        MessageBoxA(NULL, "Create unattended Office configuration XML", "Info", MB_OK | MB_ICONINFORMATION);
        create_unattended_office_xml();
        */

        MessageBoxA(NULL, "Start Office installation", "Info", MB_OK | MB_ICONINFORMATION);
        system_execute_command_elevated("/c start /wait C:/Windows/Temp/OfficeSetup.exe");
    }

    MessageBoxA(NULL, "Create sppc.dll symlink\nmklink \"%ProgramFiles%\\Microsoft Office\\root\\vfs\\System\\sppcs.dll\" \"%windir%\\System32\\sppc.dll\"", "Info", MB_OK | MB_ICONINFORMATION);
    system_execute_command_elevated("/c mklink \"%ProgramFiles%\\Microsoft Office\\root\\vfs\\System\\sppcs.dll\" \"%windir%\\System32\\sppc.dll\"");

    char path[MAX_PATH];
    const char *programFiles = getenv("ProgramFiles");

    if (programFiles == NULL)
    {
        MessageBoxA(NULL, "Error: ProgramFiles environment variable not set.", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    snprintf(path, MAX_PATH, "%s\\Microsoft Office\\root\\vfs\\System\\sppcs.patched.dll", programFiles);

    char patched_msg[MAX_PATH];
    snprintf(patched_msg, sizeof(patched_msg), "Create patched %s by ohook", path);
    MessageBoxA(NULL, patched_msg, "Info", MB_OK | MB_ICONINFORMATION);

    if (create_sppc(path) != 0)
    {
        MessageBoxA(NULL, "Failed to create sppc.dll.", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    char command[MAX_PATH];
    snprintf(command, MAX_PATH, "/c copy /y \"%s\" \"%s\\Microsoft Office\\root\\vfs\\System\\sppcs.dll\"", path, programFiles);
    MessageBoxA(NULL, command, "Overwrite sppcs.dll with sppcs.patched.dll", MB_OK | MB_ICONINFORMATION);
    system_execute_command_elevated(command);

    char *officeEdition = get_office_edition();
    char msg[256];
    snprintf(msg, sizeof(msg), "Office Edition: %s", officeEdition);
    MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);

    MessageBoxA(NULL, "Get Generic Key", "Info", MB_OK | MB_ICONINFORMATION);

    char *licenseKey = getLicenseKey(officeEdition);
    snprintf(msg, sizeof(msg), "Matching Generic Key: %s", licenseKey);
    MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);

    if (licenseKey != NULL)
    {
        MessageBoxA(NULL, "Activate Office", "Info", MB_OK | MB_ICONINFORMATION);
        char command[512];
        snprintf(command, sizeof(command), "/c slmgr /ipk %s", licenseKey);
        system_execute_command_elevated(command);
    }
    else
    {
        MessageBoxA(NULL, "Failed to retrieve license key.", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }
#else
    printf("Unsupported OS\n");
#endif
}

void webview_office_activation_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t webview = (webview_t)arg;

#ifdef _WIN32
    if (!system_check_admin_privileges())
    {
        MessageBoxA(NULL, "Administrator privileges required for Office activation", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    char *office_edition = get_office_edition();
    if (!office_edition)
    {
        MessageBoxA(NULL, "Failed to retrieve Office edition", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    char *license_key = getLicenseKey(office_edition);
    if (license_key)
    {
        char message[512];
        snprintf(message, sizeof(message), "Found Office edition: %s\nUsing license key: %s", office_edition, license_key);
        MessageBoxA(NULL, message, "Office Activation", MB_OK | MB_ICONINFORMATION);

        // Create sppc64.dll for activation bypass
        const char *sppc_path = "C:\\Windows\\System32\\sppc64.dll";
        if (create_sppc(sppc_path) == 0)
        {
            MessageBoxA(NULL, "Successfully created sppc64.dll bypass", "Info", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MessageBoxA(NULL, "Failed to create sppc64.dll bypass", "Warning", MB_OK | MB_ICONWARNING);
        }

        // Apply license key
        char command[512];
        snprintf(command, sizeof(command), "/c slmgr /ipk %s", license_key);
        system_execute_command_elevated(command);

        MessageBoxA(NULL, "Office activation completed", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "No matching license key found for this Office edition", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }
#else
    printf("Office activation is only supported on Windows\n");
#endif
}