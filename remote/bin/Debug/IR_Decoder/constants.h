#include <stdint.h>
#include <string>

//protocol IDs
const int protocol_nec = 0;
const int protocol_rc5 = 1 ;
const int protocol_rc6 = 2 ;
const int protocol_rcmm = 3 ;
const int protocol_xmp1 = 4 ;
const int protocol_rc5_57 = 5 ;
const int protocol_nec_short = 6;
const int protocol_sony = 7 ;
const int protocol_panasonic = 8 ;
const int protocol_jvc = 9 ;
const int protocol_rc5_38 = 10 ;
const int protocol_sharp = 11 ;
const int protocol_rca_38 = 12 ;
const int protocol_rca_57 = 13 ;
const int protocol_mitsubishi = 14 ;
const int protocol_konka = 15 ;
const int protocol_xiomi = 16 ;
const int protocol_mce = 17 ;

//protocol names
std::string static protocol[]={"NEC","RC5","RC6","RCMM","XMP1","RC5_57","NEC_SHORT","SONY","PANASONIC","JVC","RC5_38","SHARP","RCA_38","RCA_57","MITSUBUISHI","KONKA"};

//protocol constants
const int NEC_HEADER_MARK = 342;
const int NEC_HEADER_SPACE = 171;
const int NEC_MARK = 21;
const int NEC_HIGH_SPACE = 60;
const int NEC_LOW_SPACE = 21;

const int NEC_SHORT_HEADER_MARK = 171;
const int NEC_SHORT_HEADER_SPACE = 171;
const int NEC_SHORT_MARK = 21;
const int NEC_SHORT_HIGH_SPACE = 60;
const int NEC_SHORT_LOW_SPACE = 21;

const int RC5_SLICE = 32;
const int RC5_SLICE_ = 64;

const int RC5_38_SLICE = 32;
const int RC5_38_SLICE_ = 64;

const int RC5_57_SLICE = 52;
const int RC5_57_SLICE_ = 104;

const int RC6_HEADER_MARK = 96;
const int RC6_HEADER_SPACE = 32;
const int RC6_SLICE = 16;
const int RC6_SLICE_ = 32;

const int RCMM_HEADER_MARK = 15;
const int RCMM_HEADER_SPACE = 10;
const int RCMM_MARK = 6;
const int RCMM_SPACE0 = 10;
const int RCMM_SPACE1 = 16;
const int RCMM_SPACE2 = 22;
const int RCMM_SPACE3 = 28;

const int XMP1_MARK = 6;
const int XMP1_SPACE0 = 29;
const int XMP1_SPACE1 = 34;
const int XMP1_SPACE2 = 39;
const int XMP1_SPACE3 = 44;
const int XMP1_SPACE4 = 49;
const int XMP1_SPACE5 = 54;
const int XMP1_SPACE6 = 60;
const int XMP1_SPACE7 = 65;
const int XMP1_SPACE8 = 70;
const int XMP1_SPACE9 = 75;
const int XMP1_SPACE10 = 81;
const int XMP1_SPACE11 = 86;
const int XMP1_SPACE12 = 91;
const int XMP1_SPACE13 = 96;
const int XMP1_SPACE14 = 101;
const int XMP1_SPACE15 = 107;
const int XMP1_GAP = 500;

const int SONY_HEADER_MARK = 96;
const int SONY_HEADER_SPACE = 24;
const int SONY_HIGH_MARK = 48;
const int SONY_LOW_MARK = 24;
const int SONY_SPACE = 24;

const int PANA_HEADER_MARK = 128;
const int PANA_HEADER_SPACE = 64;
const int PANA_MARK = 16;
const int PANA_HIGH_SPACE = 48;
const int PANA_LOW_SPACE = 16;

const int JVC_HEADER_MARK = 320;
const int JVC_HEADER_SPACE = 160;
const int JVC_MARK = 20;
const int JVC_HIGH_SPACE = 60;
const int JVC_LOW_SPACE = 20;

const int SHARP_MARK = 12;
const int SHARP_HIGH_SPACE = 63;
const int SHARP_LOW_SPACE = 25;
const int SHARP_GAP_SPACE = 1520;

const int RCA38_HEADER_MARK = 152;
const int RCA38_HEADER_SPACE = 152;
const int RCA38_MARK = 19;
const int RCA38_HIGH_SPACE = 76;
const int RCA38_LOW_SPACE = 38;

const int RCA57_HEADER_MARK = 228;
const int RCA57_HEADER_SPACE = 228;
const int RCA57_MARK = 29;
const int RCA57_HIGH_SPACE = 114;
const int RCA57_LOW_SPACE = 57;

const int MITSUBISHI_HEADER_MARK = 304;
const int MITSUBISHI_HEADER_SPACE = 152;
const int MITSUBISHI_MARK = 20;
const int MITSUBISHI_HIGH_SPACE = 56;
const int MITSUBISHI_LOW_SPACE = 18;

const int KONKA_HEADER_MARK = 114;
const int KONKA_HEADER_SPACE = 152;
const int KONKA_MARK = 19;
const int KONKA_HIGH_SPACE = 95;
const int KONKA_LOW_SPACE = 57;
