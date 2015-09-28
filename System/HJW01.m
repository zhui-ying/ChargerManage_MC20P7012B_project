
C source file reg_24.h:

unsigned char T0CNT at 0x0000
unsigned char T0DATA at 0x0001
union t0con at 0x0002
union mcr at 0x0003
union btcon at 0x000c
unsigned char BTCNT at 0x000d
union p0 at 0x0010
union p1 at 0x0011
union p2 at 0x0012
union p0conh at 0x0016
union p0conl at 0x0017
union p0pnd at 0x0018
union p1con at 0x0019
union p2conh at 0x001a
union p2conl at 0x001b
unsigned char PWMDATA at 0x0022
union pwmcon at 0x0023
union adcon at 0x0027
unsigned char ADDATAH at 0x0028
unsigned char ADDATAL at 0x0029

C source file int_24.h:

(no globals)

void SWI_ISR() lines 2 to 5 at 0x1003-0x1003
    (no locals)

void INT1_ISR() lines 7 to 11 at 0x1004-0x1006
    (no locals)

void INT0_ISR() lines 13 to 17 at 0x1007-0x1009
    (no locals)

void T0INT_ISR() lines 19 to 31 at 0x100a-0x101c
    (no locals)

void PWMINT_ISR() lines 33 to 36 at 0x101d-0x101d
    (no locals)

C source file HJW01.c:

unsigned char sys_status at 0x0041
unsigned char status_count at 0x0040
unsigned char last_status at 0x003f
unsigned int AD_12V6_value at 0x003d
unsigned int AD_8V4_value at 0x003b
unsigned int AD_4V2_value at 0x0039
unsigned char time_125us at 0x0038
unsigned char time_10ms at 0x0037
unsigned char time_charge_10ms at 0x0036
unsigned char get_ad_flag at 0x0035
unsigned char AD12V6_FLAG at 0x0034

void Delay() lines 115 to 120 at 0x101e-0x1045
    static argument unsigned int ms at 0x004b
    static auto unsigned char i at 0x004a

void SysInit() lines 122 to 158 at 0x1046-0x1084
    (no locals)

int AD_Check() lines 166 to 178 at 0x1085-0x10ad
    static argument unsigned char AD_ch at 0x005c
    static auto unsigned int AD_Value at 0x005a

int AbsSub() lines 186 to 190 at 0x10ae-0x10dd
    static argument int a at 0x0059
    static argument int b at 0x005b

void GetADValue() lines 198 to 276 at 0x10de-0x1263
    static auto int AD_12_temp at 0x004c
    static auto int AD_4_temp at 0x004e
    static auto int AD_8_temp at 0x0050
    static auto int AD_12_temp_avg at 0x0052
    static auto int AD_4_temp_avg at 0x0054
    static auto int AD_8_temp_avg at 0x0056
    static auto unsigned char i at 0x0058

void Execute() lines 284 to 396 at 0x1264-0x135e
    (no locals)

void main() lines 398 to 550 at 0x135f-0x15ee
    static auto unsigned char status_temp at 0x0049
