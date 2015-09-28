//更改V_FREE值为5V modfied by LC 2015.08.05 23:10

#include "reg_24.h"


#define cli()  _asm("cli")
#define sei()  _asm("sei")
#define nop()  _asm("nop")
#define stop() _asm("stop")

#define uchar unsigned char
#define uint  unsigned int

//#define debug 1

#define DG_LED1   P04
#define DG_LED2   P05
#define DG_LED3   P06
#define DG_LED4   P26

/* uint dg_ad0;
uint dg_ad1;
uint dg_ad2; */

//LED
#define LED_PORT      P04 
#if debug
#define LED_ON        LED_PORT=1
#define LED_OFF       LED_PORT=0  //  modfied by LC 2015.07.21 11:55
#else
#define LED_ON        LED_PORT=0
#define LED_OFF       LED_PORT=1  //  modfied by LC 2015.07.21 11:55   	
#endif

//控制脚
#define P12V6_CTRL    P26
#define P12V6_ON      P12V6_CTRL=1
#define P12V6_OFF     P12V6_CTRL=0

#define P8V4_CTRL     P06
#define P8V4_ON       P8V4_CTRL=1
#define P8V4_OFF      P8V4_CTRL=0

#define P8V4_CTRL1    P20
#define P8V4C1_ON     P8V4_CTRL1=0
#define P8V4C1_OFF    P8V4_CTRL1=1

#define P4V2_CTRL     P05
#define P4V2_ON       P4V2_CTRL=1
#define P4V2_OFF      P4V2_CTRL=0

#define P4V2_CTRL1    P21
#define P4V2C1_ON     P4V2_CTRL1=0
#define P4V2C1_OFF    P4V2_CTRL1=1

//AD口
#define P12V6_ADCH    2
#define P8V4_ADCH     1
#define P4V2_ADCH     0


//系统状态
volatile uchar sys_status;
#define FREE          0//未插入电池状态
#define FALUT         1//故障
#define PRE_CHARGE    2//预充
#define FULL_CHARGE   3//全充
#define ALONE_CHARGE  4//单冲
#define FULL          5//充满

#define STATUS_MAX 1//到这个 计数次数切换
volatile uchar status_count;//同一状态连续计数值
volatile uchar last_status;

//电压阈值定义,阈值与实际电压值偏高约0.12V
#define V_FAULT_MIN   0  //0V 
#define V_FREE_MIN    205//1*1024/5 1V 小于该值直接可判定为断线或短路
#define V_FAULT_MID   471// 2.3*1024/5 2V
#define V_FAULT_MID_ADD 492//2.4V 低压故障到预充有0.1V缓冲
#define V_PRE_CHARGE  676// 3.3V*1024/5 3V

// #define V_FREE_MIN    205//1*1024/5 1V 小于该值直接可判定为断线或短路
// #define V_FAULT_MID   717// 3.5*1024/5 2V
// #define V_FAULT_MID_ADD 737//3.6V 低压故障到预充有0.1V缓冲
// #define V_PRE_CHARGE  809// 3.95*1024/5 3V

#define V_FULL        864//4.24*1024/5 4.24V //改为4.22V modfied by LC 2015.07.30 22:23
#define V_FULL_CHARGE 874   //4.27新增串冲电压，增加充电速度add by LC 2015.07.30 22:22
#define V_FULL_ADD    885//4.32V 充满到充电状态有0.08V缓冲
#define V_FAULT_SUB   926//4.44V 故障到取消故障 0.08V   //改为4.52V modfied by LC 2015.07.30 22:07
#define V_FAULT_MAX   946//4.52*1024/5 4.52V //大于该值判定为过压故障  //改为4.62V 946 modfied by LC 2015.07.30 22:07
#define V_FREE        1024// 6*1024/5  6V  //又原来的6V阈值改为5V 6V处于2S和3S判断临界点上，太危险了 add by LC 2015.08.05 23:07

#define AD_4V2_OFF    82// 0.4*1024/5  最后一个电压恒定偏移量
#define AD_NEXT_OFF    10//0.05*1024/5  0.05V 单次采样与平均值的压差不能超过0.05V
//系统变量
volatile uint AD_12V6_value;
volatile uint AD_8V4_value;
volatile uint AD_4V2_value;

volatile uchar time_125us;
volatile uchar time_10ms;
// volatile uchar time_free_10ms;//空缺模式下的计数器0 -- 100
// volatile uchar time_full_10ms;//充满和故障模式下的计数器
volatile uchar time_charge_10ms;//全充模式下的计数器
// volatile uchar time_alcharge_10ms;//单充模式下的计数器
// volatile uchar time_pre_10ms;//预充模式下的计时器

volatile uchar get_ad_flag;//AD采集标志位
volatile uchar AD12V6_FLAG;//第一颗电池是否存在标志位 0 没有 1 有

#include "int_24.h"

//0x01 40us
void Delay(uint ms) 
{       
       	uchar i;
       	while(ms--)
       	for(i = 0; i<150; i++);           
}

void SysInit(void)
{
       	//IO口定义 P00 P01 P02定义为AD输入，其他全部为普通输出
       	P0CONL = 0xBF;
       	P0CONH = 0xAA;
       	P1CON = 0x0A;
       	P2CONL = 0xAA;
       	P2CONH = 0x4A;

       	BTCON = 0XA0; //关看门狗
       	MCR = 0x00;   //LVR关
       	
       	//AD口初始化
       	ADCON = 0x82;//Fsys/4  8M/2/4=1M ADC输入时钟
       	//定时器初始化
       	T0CON = 0x80;//Fsys/8  8M/2/8=0.5M 定时器计时频率
       	T0E = 1;      //开定时器
       	
/*             	AD_12V6_value = V_FREE;
       	AD_8V4_value = 0;
       	AD_4V2_value = 0; */
       	sys_status = FREE;
       	
       	time_125us = 0;
       	time_10ms = 0;
       	time_charge_10ms = 0;
       	AD12V6_FLAG = 1;//默认3S模式
       	get_ad_flag = 0;
       	       	
       	       	P12V6_OFF;
       	P8V4_OFF;
       	P8V4C1_OFF;
       	P4V2_OFF;
       	P4V2C1_OFF; 
       	       	LED_OFF;
       	
}

/*******************************************************************************
* 函 数 名 ：AD_Check
* 函数功能 ：检测某一路的当前电压，一次检测约60us
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
int AD_Check(uchar AD_ch)
{
       	uint AD_Value;
       	ADCH=AD_ch; //选择通道
       	ADPS=1; //AD时钟频率4分频 1MHz
       	nop();
       	nop();
       	nop();
       	ADCE=1; //启动AD
       	while(EOC==0);//AD转换中
       	AD_Value=ADDATAH*4 + (ADDATAL&0x03);//获取AD值
       	return AD_Value;
}

/*******************************************************************************
* 函 数 名 ：AbsSub
* 函数功能 ：求差的绝对值
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
int AbsSub(int a,int b)
{
       	if(a>=b) return a-b;
       	if(a<b) return b-a;
}

/*******************************************************************************
* 函 数 名 ：GetADValue
* 函数功能 ：获取三路可用AD值，更新全局变量 AD_12V6_value,AD_8V4_value,AD_4V2_value
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void GetADValue(void)
{
       	int AD_12_temp;
       	int AD_4_temp;
       	int AD_8_temp;
       	
       	int AD_12_temp_avg=0;
       	int AD_4_temp_avg=0;
       	int AD_8_temp_avg=0;
       	
       	uchar i=0;
       	//三路分别10次读取AD值，变读边求平均值，并将采样电压与平均值比较，一旦出现电压与平均差的过大，
       	//就放弃前面的读取，重新计算平均值
       	while(i<6)
       	{
       	       	//取AD
       	       	AD_4_temp = AD_Check(P4V2_ADCH);
       	       	AD_8_temp = AD_Check(P8V4_ADCH);
       	       	AD_12_temp = AD_Check(P12V6_ADCH);
       	       	
       	         //    	AD_12_temp = AD_12_temp*3;
       	       	AD_12_temp = AD_12_temp*2 + AD_12_temp*24/25;//提高精度
       	       	AD_8_temp = AD_8_temp<<1;
       	       	AD_4_temp = AD_4_temp<<1;
       	       	
       	       	//求差
       	       	// AD_12_temp = AD_12_temp - AD_8_temp;
       	       	// AD_8_temp = AD_8_temp - AD_4_temp;
       	       	       	       	if(AD_12_temp > AD_8_temp)
       	       	       	       	{
       	       	       	       	       	AD_12_temp = AD_12_temp - AD_8_temp;
       	       	       	       	}else
       	       	       	       	{
       	       	       	       	       	AD_12_temp = 0;
       	       	       	       	}
       	       	       	       	if(AD_8_temp > AD_4_temp)
       	       	       	       	{
       	       	       	       	       	AD_8_temp = AD_8_temp - AD_4_temp;
       	       	       	       	}
       	       	       	       	else
       	       	       	       	{
       	       	       	       	       	AD_8_temp = 0;
       	       	       	       	}
       	       	if(AD_4_temp > AD_4V2_OFF)
       	       	{
       	       	       	AD_4_temp = AD_4_temp - AD_4V2_OFF;//减去恒定偏压  
       	       	}else
       	       	{
       	       	       	AD_4_temp = 0;
       	       	}
       	       	
       	       	
       	       	if(i>0)
       	       	{
       	       	       	//一旦出现跳变过大，以当前采样为开始，重新采样
       	       	       	if((AbsSub(AD_12_temp_avg,AD_12_temp) > AD_NEXT_OFF) || (AbsSub(AD_8_temp_avg,AD_8_temp) > AD_NEXT_OFF) || (AbsSub(AD_4_temp_avg,AD_4_temp) > AD_NEXT_OFF)) 
       	       	       	{
       	       	       	       	i=1; 
       	       	       	       	AD_12_temp_avg = AD_12_temp;
       	       	       	       	AD_4_temp_avg = AD_4_temp;
       	       	       	       	AD_8_temp_avg = AD_8_temp;     	       	       	       	
       	       	       	       	continue;
       	       	       	}
       	       	}
       	       	//求平均值
       	       	AD_12_temp_avg = (AD_12_temp_avg + AD_12_temp)>>1;
       	       	AD_8_temp_avg = (AD_8_temp_avg + AD_8_temp)>>1;
       	       	AD_4_temp_avg = (AD_4_temp_avg + AD_4_temp)>>1;
       	       	i++;
       	}
       	
       	//根据当前输出状态更新有效AD值      	
       	if(P12V6_CTRL == 1) 
       	AD_12V6_value = AD_12_temp_avg;
       	if(((P12V6_CTRL == 1) && (P8V4_CTRL1 == 1)) || (P8V4_CTRL == 1)) 
       	AD_8V4_value = AD_8_temp_avg;
       	if(((P12V6_CTRL == 1) && (P8V4_CTRL1 == 1) && (P4V2_CTRL1 == 1)) || ((P8V4_CTRL == 1) && (P4V2_CTRL1 == 1)) || (P4V2_CTRL == 1))
       	AD_4V2_value = AD_4_temp_avg;  	
}      	

/*******************************************************************************
* 函 数 名 ：Execute
* 函数功能 根据当前状态执行输出模式,注意P12V6，P8V4,P4V2只能有一路导通，10ms执行一次
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void Execute(void)
{
       	//先将所有IO输出口置零，LED口除外
       	P12V6_OFF;
       	P8V4_OFF;
       	P8V4C1_OFF;
       	P4V2_OFF;
       	P4V2C1_OFF; 
       	
       	//根据当前状态执行
       	
       	time_charge_10ms++;
       	if(time_charge_10ms >= 100) time_charge_10ms = 0;//1s周期定时
       	if(time_charge_10ms == 95 && sys_status != FREE)//定时采样 在900ms的位置定时采样,在free时需要发两个脉冲
       	{
       	       	if(AD12V6_FLAG == 0)//没有第一颗电池
       	       	{
       	       	       	P8V4_ON; //P8V4导通10ms          	
       	       	}else
       	       	{
       	       	       	P12V6_ON; //P12V6导通10ms        	       	       	       	
       	       	}
       	       	get_ad_flag = 1;
       	}  
       	       	
       	       	
       	/*全充模式，P12V6或P8V4导通，，占空 100%*/
       	if(sys_status == FULL_CHARGE)
       	{
       	       	if(AD12V6_FLAG == 0)//没有第一颗电池
       	       	{
       	       	       	P8V4_ON;
       	       	}else
       	       	{
       	       	       	P12V6_ON;
       	       	}
       	       	LED_ON;
       	       	return;
       	}
       	
       	/*单充模式,任意两路或一路以25%占空比充，占空 200ms/1s*/
       	if(sys_status == ALONE_CHARGE)
       	{
       	       	
       	       	if((AD_12V6_value < V_FULL_ADD) && (AD12V6_FLAG == 1))//第一颗慢充
       	       	{
       	       	       	if(time_charge_10ms >= 0 && time_charge_10ms < 25) //0 -- 20
       	       	       	{
       	       	       	       	P12V6_ON;
       	       	       	       	P8V4C1_ON;
       	       	       	}
       	       	}
       	       	
       	       	if(AD_8V4_value < V_FULL_ADD)//第二颗慢充
       	       	{
       	       	       	if(time_charge_10ms >= 30 && time_charge_10ms < 55) //30 -- 50
       	       	       	{
       	       	       	       	P8V4_ON;
       	       	       	       	P4V2C1_ON;
       	       	       	}
       	       	}
       	       	
       	       	if(AD_4V2_value < V_FULL_ADD)//第三颗慢充
       	       	{
       	       	       	if(time_charge_10ms >= 60 && time_charge_10ms < 85) // 60 -- 80
       	       	       	{
       	       	       	       	P4V2_ON;
       	       	       	}
       	       	}
       	       	LED_ON;
       	       	return;
       	}
       	
       	/*预充状态，P12V6或P8V4导通，占空 200ms/2s*/
       	if(sys_status == PRE_CHARGE)
       	{
       	       	if(AD12V6_FLAG == 0)//没有第一颗电池
       	       	{
       	       	       	if(time_charge_10ms >= 0 && time_charge_10ms < 10) P8V4_ON; //P8V4导通200ms              	       	
       	       	}else
       	       	{
       	       	       	if(time_charge_10ms >= 0 && time_charge_10ms < 10) P12V6_ON; //P12V6导通200ms            	       	       	       	
       	       	}
       	       	       	       	LED_ON;
       	       	return;
       	}
       	
       	/*空缺态,P12V6,P8V4轮流导通，占空 10ms/1s */
       	if(sys_status == FREE)
       	{
       	       	if(time_charge_10ms == 95) 
       	       	{
               	       	P8V4_ON; //P8V4导通10ms
               	 get_ad_flag = 1;
       	       	}
       	       	LED_OFF;
       	       	return;
       	}
       	
       	/*故障以及充满状态，P12V6或P8V4导通，占空 10ms/1s*/
       	if(sys_status == FALUT || sys_status == FULL)
       	{
       	       	if(sys_status == FALUT)
       	       	{
               	       	       	if(time_charge_10ms%50 == 0) LED_PORT = !LED_PORT;//故障闪动
       	       	}else
       	       	{
       	       	       	LED_OFF;
       	       	}
       	       	
       	       	return;
       	}
}

void main(void)
{
       	uchar status_temp;
       	uint AD_value_test;//add by LC 2015.07.20 14:11
    //   Delay(100);
       	SysInit();
       	       	Delay(100);
       	cli();
       	while(1)
       	{
       	       	if(time_10ms >= 1)//时基为10ms
       	       	{
       	       	       	time_10ms = 0;
       	       	       	//执行
       	       	       	Execute();
       	       	       	//AD采样
       	       	       	if(get_ad_flag == 1)
       	       	       	{
               	       	       	      //       	LED_ON;
       	       	       	       	get_ad_flag = 0;
       	       	       	       	Delay(4);//延时等待电压稳定约2ms
       	       	       	       	GetADValue();

       	       	       	       	
       	       	       	       	//空缺判断,断路故障判断
       	       	       	       	if((AD_8V4_value > V_FREE) || (AD_4V2_value > V_FREE) || ((AD_12V6_value > V_FREE) && (AD12V6_FLAG == 1)))
       	       	       	       	{
       	       	       	       	       	status_temp = FALUT;
       	       	       	       	       	if(P12V6_CTRL == 1)//3S模式
       	       	       	       	       	{
       	       	       	       	       	       	if(AD_12V6_value > V_FREE && AD_8V4_value < V_FREE_MIN && AD_4V2_value < V_FREE_MIN)
       	       	       	       	       	       	{
       	       	       	       	       	       	       	status_temp = FREE;
       	       	       	       	       	       	}
       	       	       	       	       	}else //2S模式
       	       	       	       	       	{
       	       	       	       	       	       	if(AD_8V4_value > V_FREE && AD_4V2_value < V_FREE_MIN)
       	       	       	       	       	       	{
       	       	       	       	       	       	       	status_temp = FREE;
       	       	       	       	       	       	       	//AD12V6_FLAG = 1;//默认3S模式
       	       	       	       	       	       	}
       	       	       	       	       	}
       	       	       	       	       	
       	       	       	       	}
       	       	       	       	//故障判断  任意一路电压出现大于 V_FAULT_MAX 的值
       	       	       	       	else if((AD_8V4_value>V_FAULT_MAX) || (AD_4V2_value>V_FAULT_MAX) || ((AD_12V6_value>V_FAULT_MAX) && (AD12V6_FLAG == 1)))
       	       	       	       	{
       	       	       	       	       	status_temp = FALUT;
       	       	       	       	}
       	       	       	       	//故障判断  任意一路电压出现小于 V_FAULT_MID 的值
       	       	       	       	else if((AD_8V4_value<V_FAULT_MID) || (AD_4V2_value<V_FAULT_MID) || ((AD_12V6_value<V_FAULT_MID) && (AD12V6_FLAG == 1)))
       	       	       	       	{
       	       	       	       	       	status_temp = FALUT;
       	       	       	       	}
       	       	       	       	//预充判断  任意一路出现小于 V_PRE_CHARGE 的值
       	       	       	       	else if((AD_8V4_value<V_PRE_CHARGE) || (AD_4V2_value<V_PRE_CHARGE) || ((AD_12V6_value<V_PRE_CHARGE) && (AD12V6_FLAG == 1)))
       	       	       	       	{
       	       	       	       	       	if(sys_status == FALUT)
       	       	       	       	       	{
       	       	       	       	       	       	if((AD_8V4_value>V_FAULT_MID_ADD) && (AD_4V2_value>V_FAULT_MID_ADD) && ((AD_12V6_value>V_FAULT_MID_ADD) || (AD12V6_FLAG == 0)))//需要满足预充的最小值
       	       	       	       	       	       	status_temp = PRE_CHARGE;      	       	       	       	       	       	       	       	       	       	
       	       	       	       	       	}else
       	       	       	       	       	{
       	       	       	       	       	       	status_temp = PRE_CHARGE;      	
       	       	       	       	       	}

       	       	       	       	}
       	       	       	       	//全充判断  所有路电压均小于 V_FULL
       	       	       	       	else if((AD_8V4_value<V_FULL_CHARGE) && (AD_4V2_value<V_FULL_CHARGE) && ((AD_12V6_value<V_FULL_CHARGE) || (AD12V6_FLAG == 0)))
       	       	       	       	{
       	       	       	       	       	status_temp = FULL_CHARGE;
       	       	       	       	}
       	       	       	       	//充满判断  所有电压均大于等于 V_FULL_ADD
       	       	       	       	else if((AD_8V4_value>=V_FULL_ADD) && (AD_4V2_value>=V_FULL_ADD) && ((AD_12V6_value>=V_FULL_ADD) || (AD12V6_FLAG == 0)))
       	       	       	       	{
       	       	       	       	       	if(sys_status == FALUT)//上一状态为fault，除非所有电压值均低于 V_FAULT_SUB
       	       	       	       	       	{
       	       	       	       	       	       	if((AD_8V4_value<V_FAULT_SUB) && (AD_4V2_value<V_FAULT_SUB) && ((AD_12V6_value<V_FAULT_SUB) || (AD12V6_FLAG == 0)))
       	       	       	       	       	       	       	status_temp = ALONE_CHARGE;
       	       	       	       	       	}else
       	       	       	       	       	{
       	       	       	       	       	       	status_temp = FULL;
       	       	       	       	       	}
       	       	       	       	}
       	       	       	       	//分充判断 某一路出现大于 V_FULL 的情况
       	       	       	       	else if((AD_8V4_value>=V_FULL) || (AD_4V2_value>=V_FULL) || ((AD_12V6_value>=V_FULL) && (AD12V6_FLAG == 1)))
       	       	       	       	{
       	       	       	       	       	if(sys_status == FULL)//上一状态为full,为电压跌落的，必须判断是否有一路电压值小于V_FULL
       	       	       	       	       	{
       	       	       	       	       	       	if((AD_8V4_value<V_FULL) || (AD_4V2_value<V_FULL) || ((AD_12V6_value<V_FULL) && (AD12V6_FLAG == 1)))
       	       	       	       	       	       	       	status_temp = ALONE_CHARGE;
       	       	       	       	       	}else if(sys_status == FALUT)//上一状态为fault，除非所有电压值均低于 V_FAULT_SUB
       	       	       	       	       	{
       	       	       	       	       	       	if((AD_8V4_value<V_FAULT_SUB) && (AD_4V2_value<V_FAULT_SUB) && ((AD_12V6_value<V_FAULT_SUB) || (AD12V6_FLAG == 0)))
       	       	       	       	       	       	       	status_temp = ALONE_CHARGE;
       	       	       	       	       	}else
       	       	       	       	       	{
       	       	       	       	       	       	status_temp = ALONE_CHARGE;
       	       	       	       	       	}
       	       	       	       	       	
       	       	       	       	}
       	       	       	       	
       	       	       	       	if(status_temp == last_status)//当前状态与上次状态相同，则状态值自加
       	       	       	       	{
       	       	       	       	       	status_count++;
       	       	       	       	}
       	       	       	       	
       	       	       	       	if(status_temp != last_status)//当前状态与上次状态不同，则状态计数为1，并修改上次状态
       	       	       	       	{
       	       	       	       	       	status_count = 1;
       	       	       	       	       	last_status = status_temp;
       	       	       	       	}
       	       	       	       	//空缺态进入其他态,判断 3S模式 和 2S模式
       	       	       	       	if((sys_status == FREE) && (status_temp != FREE) && (status_count >= STATUS_MAX))
       	       	       	       	{
       	       	       	       	       	//先将所有IO输出口置零，LED口除外
       	       	       	       	       	P12V6_OFF;
       	       	       	       	       	P8V4_OFF;
       	       	       	       	       	P8V4C1_OFF;
       	       	       	       	       	P4V2_OFF;
       	       	       	       	       	P4V2C1_OFF; 
       	       	       	       	       	Delay(600);//延时待完全插入
       	       	       	       	       	
       	       	       	       	       	P12V6_ON;
       	       	       	       	       	Delay(20);
       	       	       	       	       	GetADValue();//获取采样值
       	       	       	       	       	if(AD_12V6_value < V_FREE)//表示有3S
       	       	       	       	       	{
       	       	       	       	       	       	AD12V6_FLAG = 1;
       	       	       	       	       	       	status_temp = FULL;//如果是3S模式，将系统状态设为full重新采集，避免LED乱闪
       	       	       	       	       	}else
       	       	       	       	       	{
       	       	       	       	       	       	AD12V6_FLAG = 0;
       	       	       	       	       	}
       	       	       	       	}
       	       	       	       	
       	       	       	       	if((status_count >= STATUS_MAX))               	       	       	       	
       	       	       	       	{
       	       	       	       	       	status_count = 0;
       	       	       	       	       	sys_status = status_temp;
       	       	       	       	}
       	       	       	       	       	       	       	       	
       	       	       	       	       	       	       	       	if(sys_status == FREE)
       	       	       	       	{
               	       	       	       	       	AD12V6_FLAG = 0;//改为默认2S模式，避免从free状态到充电状态出现fault modfied by LC 2015.07.30 21:47
       	       	       	       	       //      	AD_12V6_value = V_PRE_CHARGE;
       	       	       	       	       	AD_12V6_value =  V_FREE;
       	       	       	       	}
       	       	       	}
       	       	       	
       	       	}
       	}
}
