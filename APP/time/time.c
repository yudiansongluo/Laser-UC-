#include "time.h"
#include "led.h"
#include "usart.h"
#include "module.h"

int TIM3_count = 0;

void TIM3_Configuration(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  TIM_TimeBaseStructure.TIM_Period = per ;
  TIM_TimeBaseStructure.TIM_Prescaler = psc ;	//������������Ϊ TIM3 ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָ�Ϊ0
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM3 ���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM3,ENABLE);    

}

//void Delay_Us( u16 xus)
//{ 

//	TIM_Cmd(TIM3,ENABLE); //������ʱ��
//	TIM3->CNT = 0;
//	while(TIM3->CNT < xus);
//	TIM3->CNT = 0;
//	TIM_Cmd(TIM3,DISABLE); //�رն�ʱ��
//			
//}

//void Delay_Us( u16 xus)
//{ 

//	TIM_Cmd(TIM4,ENABLE); //������ʱ��
//	TIM3->CNT = 0;
//	while(((TIM3->CNT)) < xus);
//	TIM3->CNT = 0;
//	TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��
//			
//}


//void Delay_Ms(u16 xms)
//{
//	int i;
//	for(i=0;i<xms;i++)
//	{
//		Delay_Us(1000);
//	}
//}

/*******************************************************************************
* �� �� ��         : TIM4_Init
* ��������		   : TIM4��ʼ������
* ��    ��         : per:��װ��ֵ
					 psc:��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}

//void TIM3_IRQHandler(void)
//{
//	TIM3_count++;
//	if( TIM3_count ==  10)
//	{
//		updating();
//		TIM3_count = 0;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
//}
/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if( beat_count!=0){
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{	TIM4_Init(10000-1,36000-1);  //��ʱ5us
		netflag2=1;
		USART1_printf("%s",merged);
	}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}


