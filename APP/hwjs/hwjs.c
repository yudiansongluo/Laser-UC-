#include "hwjs.h"
#include "SysTick.h"
#include "function.h"
#include "usart.h"
#include "time.h"
#include "voice.h"
#include "module.h"

u8 irflag=77;
u8 readflg;
u32 hw_jsm=0x00de21ee;	  //定义一个32位数据变量，保存接收码
u8  hw_jsbz;  //定义一个8位数据的变量，用于指示接收标志
u8 data_u8[4];

u8 Lead_Low_Flag=0;
u8 Lead_High_Flag=0;

uint32_t exit_line[8]={EXTI_Line9,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line15,EXTI_Line7,EXTI_Line14,0,};
uint16_t exit_io[8] = {GPIO_Pin_9,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_15,GPIO_Pin_7,GPIO_Pin_14,0,};
uint8_t  exit_PortSource[8] = {0x01,0x00,0x00,0x02,0x00,0x00,0x01,0x02};
uint8_t  exit_PinSource[8] = {0x09,0x11,0x12,0x13,0x15,0x07,0x14,0};
GPIO_TypeDef* exit_AB[8] = {GPIOB,GPIOA,GPIOA,GPIOC,GPIOA,GPIOA,GPIOB,GPIOC};
uint32_t RCC_que[8] = {RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOC,RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOA,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOC};


void Hwjs_PinInit(GPIO_TypeDef*GPIO,uint16_t GPIO_Pin, u8 flag)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_que[flag]|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=exit_io[flag];//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(exit_AB[flag],&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(exit_PortSource[flag],  exit_PinSource[flag]); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(exit_line[flag]);
	
	EXTI_InitStructure.EXTI_Line=exit_line[flag];
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

void Hwjs_NVIVInit(uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;   //打开全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  NVIC_IRQChannelSubPriority; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* 函 数 名         : Hwjs_Init
* 函数功能		   : 红外端口初始化函数	  时钟端口及外部中断初始化 
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Hwjs_InitA15()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line15);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* 设置NVIC参数 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //打开全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);
}

void Hwjs_InitC13()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(GPIO_Pin_13);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=GPIO_Pin_13;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}

void Hwjs_InitA12()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(GPIO_Pin_12);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=GPIO_Pin_12;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}


void Hwjs_InitA11()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(GPIO_Pin_11);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=GPIO_Pin_11;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}

void Hwjs_InitB14()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line14);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}


void Hwjs_InitA7()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line7);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* 设置NVIC参数 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //打开全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);
}

void Hwjs_InitB9()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(GPIO_Pin_9);
	
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=GPIO_Pin_9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}



void ShakeB0()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}


/*******************************************************************************
* 函 数 名         : HW_jssj
* 函数功能		   : 高电平持续时间，将记录的时间保存在t中返回，其中一次大约20us 
* 输    入         : 无
* 输    出         : t
*******************************************************************************/
void Lc12s_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//SET引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//SET引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	


}

/*******************************************************************************
* 函 数 名         : HW_jssj
* 函数功能		   : 高电平持续时间，将记录的时间保存在t中返回，其中一次大约20us 
* 输    入         : 无
* 输    出         : t
*******************************************************************************/
u8 HW_jssj(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin )
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIO,GPIO_Pin)==1)//高电平
	{
		t++;
		delay_us(20);
		if(t>=250) return t;//超时溢出
	}
	return t;
}



	
/*******************************************************************************
* 函 数 名         : Lead_Low_Time
* 函数功能		   : 检测那一路被击中，并将被击中部位取出来。 
* 输    入         : 无
* 输    出         : 击中部位标志
*******************************************************************************/
void	Lead_Low_Time(GPIO_TypeDef* GPIO,uint16_t GPIO_Pin )//判断中断在那个脚，或许可以用这个程序：https://blog.csdn.net/weibo1230123/article/details/80798200?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164630881416780261986784%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164630881416780261986784&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_ulrmf~default~default-5-80798200.pc_search_insert_ulrmf&utm_term=stm32+exti&spm=1018.2226.3001.4187
{	
	u16 Low_time = 0 ;
	TIM_SetCounter(TIM4,0);
	while(GPIO_ReadInputDataBit(GPIO,GPIO_Pin) == 0)
	{
		Low_time =TIM_GetCounter(TIM4);
	}
	if((Low_time>1700) && (Low_time<1900))			//判断低电平脉宽是否在8.5ms-9.5ms之间
	{
			Lead_Low_Flag=1;				
	}
}		





void EXIT_15(u8 Lead_High_Flag)
{
		u8 Tim=0,Ok=0,Data,Num=0;

		while(1)
		{
		 if(GPIO_ReadInputDataBit(exit_AB[Lead_High_Flag],exit_io[Lead_High_Flag])==1)
			{  
				 Tim=HW_jssj(exit_AB[Lead_High_Flag],exit_io[Lead_High_Flag]);//获得此次高电平时间
				 if(Tim>=250) break;//不是有用的信号
				 if((Tim>=150 && Tim<250))
					 {
						Ok=1;//收到起始信号
					 }
				 else if(Tim>=60 && Tim<150)		//60~90   73~95
					 {
						Data=1;//收到数据 1
							Num++;
					 }
				else if(Tim>=10 && Tim<60)  //10~50  17~39
					 {
						Data=0;//收到数据 0
						 Num++;
					 }
				else
					break;//不是有用的信号
				 if(Ok==1)
					 {
						hw_jsm<<=1;
						hw_jsm+=Data;
						if(Num>=32)
							{
								hw_jsbz=1;
									break;
							}
						}
			}	
   }
}


//void EXTI15_10_IRQHandler(void)	  //红外遥控外部中断
//{
//	u32 Temp;
//	Temp = EXTI->PR;
//	switch(Temp)
//  {
//    case  EXTI_PR_PR15:
//			Lead_Low_Time(exit_AB[4],exit_io[4]);
//			Lead_High_Flag = 4;
//      EXTI_ClearITPendingBit(EXTI_Line15);
//      break;
//    case EXTI_PR_PR14:
//			Lead_Low_Time(exit_AB[6],exit_io[6]);
//			Lead_High_Flag = 6;
//      EXTI_ClearITPendingBit(EXTI_Line14);
//      break;
//		case EXTI_PR_PR13:
//			Lead_Low_Time(exit_AB[3],exit_io[3]);
//			Lead_High_Flag = 3;
//      EXTI_ClearITPendingBit(EXTI_Line13);
//      break;
//		case EXTI_PR_PR12:
//			Lead_Low_Time(exit_AB[2],exit_io[2]);
//			Lead_High_Flag = 2;
//      EXTI_ClearITPendingBit(EXTI_Line12);
//      break;
//		case EXTI_PR_PR11:
//			Lead_Low_Time(exit_AB[1],exit_io[1]);
//			Lead_High_Flag = 1;
//      EXTI_ClearITPendingBit(EXTI_Line11);
//      break;
//    default:break;
//  }
//	if(Lead_Low_Flag==1){
//		 irflag=Lead_High_Flag;
//		Lead_Low_Flag=0;//标志位清零
//	EXIT_15( Lead_High_Flag);}

//}

//void EXTI9_5_IRQHandler(void)	  //红外遥控外部中断
//{
//	u32 Temp;
//	Temp = EXTI->PR;
//	switch(Temp)
//  {
//    case  EXTI_PR_PR7:
//			Lead_Low_Time(exit_AB[5],exit_io[5]);
//			Lead_High_Flag = 5;
//      EXTI_ClearITPendingBit(EXTI_Line7);
//      break;
//    case EXTI_PR_PR9:
//			Lead_Low_Time(exit_AB[0],exit_io[0]);
//			Lead_High_Flag = 0;
//      EXTI_ClearITPendingBit(EXTI_Line9);
//      break;
//    default:break;
//  }
//	if(Lead_Low_Flag==1){
//		irflag=Lead_High_Flag;
//		Lead_Low_Flag=0;
//	EXIT_15( Lead_High_Flag);}
//}

void EXTI15_10_IRQHandler(void)	  //红外遥控外部中断
{
	if (EXTI_GetITStatus(EXTI_Line15) != RESET)  
	{
			Lead_Low_Time(exit_AB[4],exit_io[4]);
			Lead_High_Flag = 4;
      EXTI_ClearITPendingBit(EXTI_Line15);
	}
		if (EXTI_GetITStatus(EXTI_Line14) != RESET)  
	{
			Lead_Low_Time(exit_AB[6],exit_io[6]);
			Lead_High_Flag = 6;
      EXTI_ClearITPendingBit(EXTI_Line14);
	}
		if (EXTI_GetITStatus(EXTI_Line13) != RESET)  
	{
			Lead_Low_Time(exit_AB[3],exit_io[3]);
			Lead_High_Flag = 3;
      EXTI_ClearITPendingBit(EXTI_Line13);
	}
		if (EXTI_GetITStatus(EXTI_Line12) != RESET)  
	{
			Lead_Low_Time(exit_AB[2],exit_io[2]);
			Lead_High_Flag = 2;
      EXTI_ClearITPendingBit(EXTI_Line12);
	}
		if (EXTI_GetITStatus(EXTI_Line11) != RESET)  
	{
			Lead_Low_Time(exit_AB[1],exit_io[1]);
			Lead_High_Flag = 1;
      EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(Lead_Low_Flag==1){
		 irflag=Lead_High_Flag;
		Lead_Low_Flag=0;//标志位清零
	EXIT_15( Lead_High_Flag);}

}


void EXTI9_5_IRQHandler(void)	  //红外遥控外部中断
{
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)  
	{
			Lead_Low_Time(exit_AB[5],exit_io[5]);
			Lead_High_Flag = 5;
      EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)  
	{
			Lead_Low_Time(exit_AB[5],exit_io[5]);
			Lead_High_Flag = 5;
      EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if(Lead_Low_Flag==1)
		{
		irflag=Lead_High_Flag;
		Lead_Low_Flag=0;
		EXIT_15( Lead_High_Flag);
		}
}







































