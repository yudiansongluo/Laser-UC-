#include "hwjs.h"
#include "SysTick.h"
#include "function.h"
#include "usart.h"
#include "time.h"
#include "voice.h"
#include "module.h"

u8 irflag=77;
u8 readflg;
u32 hw_jsm=0x00de21ee;	  //����һ��32λ���ݱ��������������
u8  hw_jsbz;  //����һ��8λ���ݵı���������ָʾ���ձ�־
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
	
	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_que[flag]|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=exit_io[flag];//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(exit_AB[flag],&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(exit_PortSource[flag],  exit_PinSource[flag]); //ѡ��GPIO�ܽ������ⲿ�ж���·
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
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  NVIC_IRQChannelSubPriority; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* �� �� ��         : Hwjs_Init
* ��������		   : ����˿ڳ�ʼ������	  ʱ�Ӷ˿ڼ��ⲿ�жϳ�ʼ�� 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Hwjs_InitA15()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line15);
	
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* ����NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

void Hwjs_InitC13()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(GPIO_Pin_13);
	
	/* �����ⲿ�жϵ�ģʽ */ 
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


	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(GPIO_Pin_12);
	
	/* �����ⲿ�жϵ�ģʽ */ 
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

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(GPIO_Pin_11);
	
	/* �����ⲿ�жϵ�ģʽ */ 
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

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line14);
	
	/* �����ⲿ�жϵ�ģʽ */ 
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

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line7);
	
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	/* ����NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

void Hwjs_InitB9()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); //ѡ��GPIO�ܽ������ⲿ�ж���·
	EXTI_ClearITPendingBit(GPIO_Pin_9);
	
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=GPIO_Pin_9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}



void ShakeB0()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}


/*******************************************************************************
* �� �� ��         : HW_jssj
* ��������		   : �ߵ�ƽ����ʱ�䣬����¼��ʱ�䱣����t�з��أ�����һ�δ�Լ20us 
* ��    ��         : ��
* ��    ��         : t
*******************************************************************************/
void Lc12s_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//SET����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	/* ����GPIOʱ�Ӽ��ܽŸ���ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//SET����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	


}

/*******************************************************************************
* �� �� ��         : HW_jssj
* ��������		   : �ߵ�ƽ����ʱ�䣬����¼��ʱ�䱣����t�з��أ�����һ�δ�Լ20us 
* ��    ��         : ��
* ��    ��         : t
*******************************************************************************/
u8 HW_jssj(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin )
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIO,GPIO_Pin)==1)//�ߵ�ƽ
	{
		t++;
		delay_us(20);
		if(t>=250) return t;//��ʱ���
	}
	return t;
}



	
/*******************************************************************************
* �� �� ��         : Lead_Low_Time
* ��������		   : �����һ·�����У����������в�λȡ������ 
* ��    ��         : ��
* ��    ��         : ���в�λ��־
*******************************************************************************/
void	Lead_Low_Time(GPIO_TypeDef* GPIO,uint16_t GPIO_Pin )//�ж��ж����Ǹ��ţ�����������������https://blog.csdn.net/weibo1230123/article/details/80798200?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164630881416780261986784%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164630881416780261986784&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_ulrmf~default~default-5-80798200.pc_search_insert_ulrmf&utm_term=stm32+exti&spm=1018.2226.3001.4187
{	
	u16 Low_time = 0 ;
	TIM_SetCounter(TIM4,0);
	while(GPIO_ReadInputDataBit(GPIO,GPIO_Pin) == 0)
	{
		Low_time =TIM_GetCounter(TIM4);
	}
	if((Low_time>1700) && (Low_time<1900))			//�жϵ͵�ƽ�����Ƿ���8.5ms-9.5ms֮��
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
				 Tim=HW_jssj(exit_AB[Lead_High_Flag],exit_io[Lead_High_Flag]);//��ô˴θߵ�ƽʱ��
				 if(Tim>=250) break;//�������õ��ź�
				 if((Tim>=150 && Tim<250))
					 {
						Ok=1;//�յ���ʼ�ź�
					 }
				 else if(Tim>=60 && Tim<150)		//60~90   73~95
					 {
						Data=1;//�յ����� 1
							Num++;
					 }
				else if(Tim>=10 && Tim<60)  //10~50  17~39
					 {
						Data=0;//�յ����� 0
						 Num++;
					 }
				else
					break;//�������õ��ź�
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


//void EXTI15_10_IRQHandler(void)	  //����ң���ⲿ�ж�
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
//		Lead_Low_Flag=0;//��־λ����
//	EXIT_15( Lead_High_Flag);}

//}

//void EXTI9_5_IRQHandler(void)	  //����ң���ⲿ�ж�
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

void EXTI15_10_IRQHandler(void)	  //����ң���ⲿ�ж�
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
		Lead_Low_Flag=0;//��־λ����
	EXIT_15( Lead_High_Flag);}

}


void EXTI9_5_IRQHandler(void)	  //����ң���ⲿ�ж�
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







































