/*****************************
Name:	MC20P24 Head File
Author:	lxf
Date:	2011.01.07
*****************************/

typedef struct bits
{
unsigned char b0:
    1;
unsigned char b1:
    1;
unsigned char b2:
    1;
unsigned char b3:
    1;
unsigned char b4:
    1;
unsigned char b5:
    1;
unsigned char b6:
    1;
unsigned char b7:
    1;
} BITS;

typedef union bit_char
{
    unsigned char byte;
    BITS b;
} BIT_CHAR;

volatile unsigned char const T0CNT @0x00;
volatile unsigned char T0DATA      @0x01;

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4         	 : 1;	
    unsigned char bit5         	 : 1; 	
    unsigned char bit6_7         : 2;
  } bitn;
} t0con @ 0x02;
	
#define T0CON	t0con.byte	
#define T0F		t0con.bitn.bit0	
#define T0E		t0con.bitn.bit1	
#define T0CLR	t0con.bitn.bit3	
#define T0PS	t0con.bitn.bit6_7	

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1_7         : 7;
  } bitn;
} mcr @ 0x03;
	
#define MCR		mcr.byte	
#define LVRE	mcr.bitn.bit0	

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2_3         : 2;
    unsigned char bit4_7         : 4;	
  } bitn;
} btcon @ 0x0C;
	
#define BTCON	btcon.byte	
#define DVCLR	btcon.bitn.bit0	
#define BTCLR	btcon.bitn.bit1	
#define BTPS	btcon.bitn.bit2_3	 
#define WDTE	btcon.bitn.bit4_7	

volatile unsigned char const BTCNT 	@0x0D;

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0         : 1;
    unsigned char bit1         : 1;
    unsigned char bit2         : 1;
    unsigned char bit3         : 1;
    unsigned char bit4         : 1;	
    unsigned char bit5         : 1; 
    unsigned char bit6         : 1; 	
    unsigned char bit7         : 1; 	
  } bitn;
} p0 @ 0x10;

#define P0	p0.byte		
#define P00 p0.bitn.bit0
#define P01 p0.bitn.bit1
#define P02 p0.bitn.bit2
#define P03 p0.bitn.bit3
#define P04 p0.bitn.bit4
#define P05 p0.bitn.bit5
#define P06 p0.bitn.bit6
#define P07 p0.bitn.bit7

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0         : 1;
    unsigned char bit1         : 1;
    unsigned char bit2         : 1;
    unsigned char bit3         : 1;
    unsigned char bit4         : 1;	
    unsigned char bit5         : 1; 
    unsigned char bit6         : 1; 	
    unsigned char bit7         : 1; 	
  } bitn;
} p1 @ 0x11;

#define P1	p1.byte		
#define P10 p1.bitn.bit0
#define P11 p1.bitn.bit1
#define P12 p1.bitn.bit2

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0         : 1;
    unsigned char bit1         : 1;
    unsigned char bit2         : 1;
    unsigned char bit3         : 1;
    unsigned char bit4         : 1;	
    unsigned char bit5         : 1; 
    unsigned char bit6         : 1; 	
    unsigned char bit7         : 1; 	
  } bitn;
} p2 @ 0x12;

#define P2	p2.byte		
#define P20 p2.bitn.bit0
#define P21 p2.bitn.bit1
#define P22 p2.bitn.bit2
#define P23 p2.bitn.bit3
#define P24 p2.bitn.bit4
#define P25 p2.bitn.bit5
#define P26 p2.bitn.bit6

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0_1         : 2;
    unsigned char bit2_3         : 2;
    unsigned char bit4_5         : 2;
    unsigned char bit6_7         : 2;	
  } bitn;
} p0conh @ 0x16;

#define P0CONH	p0conh.byte		
#define P04C p0conh.bitn.bit0_1
#define P05C p0conh.bitn.bit2_3
#define P06C p0conh.bitn.bit4_5
#define P07C p0conh.bitn.bit6_7

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0_1         : 2;
    unsigned char bit2_3         : 2;
    unsigned char bit4_5         : 2;
    unsigned char bit6_7         : 2;	
  } bitn;
} p0conl @ 0x17;

#define P0CONL	p0conl.byte		
#define P00C p0conl.bitn.bit0_1
#define P01C p0conl.bitn.bit2_3
#define P02C p0conl.bitn.bit4_5
#define P03C p0conl.bitn.bit6_7

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0         : 1;
    unsigned char bit1         : 1;
    unsigned char bit2         : 1;
    unsigned char bit3         : 1;
    unsigned char bit4         : 1;	
    unsigned char bit5         : 1; 
    unsigned char bit6         : 1; 	
    unsigned char bit7         : 1; 
  } bitn;
} p0pnd @ 0x18;

#define P0PND p0pnd.byte		
#define INT0F p0pnd.bitn.bit0
#define INT0E p0pnd.bitn.bit1
#define INT1F p0pnd.bitn.bit2
#define INT1E p0pnd.bitn.bit3

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0_1         : 2;
    unsigned char bit2_3         : 2;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;    	
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;		
  } bitn;
} p1con @ 0x19;

#define P1CON p1con.byte		
#define P10C  p1con.bitn.bit0_1
#define P11C  p1con.bitn.bit2_3
#define P10OD p1con.bitn.bit6
#define P11OD p1con.bitn.bit7

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0_1         : 2;
    unsigned char bit2_3         : 2;
    unsigned char bit4_6         : 3;
    unsigned char bit7           : 1;		
  } bitn;
} p2conh @ 0x1A;

#define P2CONH	p2conh.byte		
#define P24C p2conh.bitn.bit0_1
#define P25C p2conh.bitn.bit2_3
#define P26C p2conh.bitn.bit4_6

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0_1         : 2;
    unsigned char bit2_3         : 2;
    unsigned char bit4_5         : 2;
    unsigned char bit6_7         : 2;	
  } bitn;
} p2conl @ 0x1B;

#define P2CONL	p2conl.byte		
#define P20C p2conl.bitn.bit0_1
#define P21C p2conl.bitn.bit2_3
#define P22C p2conl.bitn.bit4_5
#define P23C p2conl.bitn.bit6_7

volatile unsigned char PWMDATA 		@0x22;

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;	
    unsigned char bit6_7         : 2;
  } bitn;
} pwmcon @ 0x23;

#define PWMCON  pwmcon.byte	
#define PWMIF	pwmcon.bitn.bit0	
#define PWMIE	pwmcon.bitn.bit1	
#define PWMCE	pwmcon.bitn.bit2	
#define PWMCLR	pwmcon.bitn.bit3	
#define PWMDRS	pwmcon.bitn.bit4	
#define PWMPS	pwmcon.bitn.bit6_7	

volatile union
{
  unsigned char byte;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1_2         : 2;
    unsigned char bit3           : 1;
    unsigned char bit4_7         : 4;
  } bitn;
} adcon @ 0x27;
	
#define ADCON   adcon.byte	
#define ADCE	adcon.bitn.bit0	
#define ADPS	adcon.bitn.bit1_2	
#define EOC		adcon.bitn.bit3	
#define ADCH	adcon.bitn.bit4_7	

volatile unsigned char const ADDATAH 	@0x28;
volatile unsigned char const ADDATAL 	@0x29;	
	
	
	