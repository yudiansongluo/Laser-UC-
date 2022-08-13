#include "stm32f10x.h"
#include "bsp_fun.h"


/*
*********************************************************************************************************
*	�� �� ��: GPIO_Configuration
*	����˵��: ����PA9ΪTXD��PA10��RXD
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ�� GPIOA clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

	/* ��ֹ�ϵ������� */
	GPIO_SetBits(GPIOA, GPIO_Pin_9);

	/* ����PA9Ϊ������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����PA10Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}





/*
*********************************************************************************************************
*	�� �� ��: TIM2_Configuration
*	����˵��: ����TIM1
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//��ʱ��

  	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	//������������Ϊ TIM2 ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָ�Ϊ0
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM2 ���ϼ���ģʽ

  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//��ʼ��
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	//��� TIM2 ���жϴ�����λ
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   		//����TIM2���ж� 
        
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2 ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: TIM3_Configuration
*	����˵��: ����TIM1
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  	TIM_TimeBaseStructure.TIM_Period = 65535 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	//������������Ϊ TIM3 ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָ�Ϊ0
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM3 ���ϼ���ģʽ

  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);    

}



/*
*********************************************************************************************************
*	�� �� ��: Delay_Ms
*	����˵��: ��ʱ
*	��    �Σ�nTime,��λΪuS
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay_Ms(volatile u32 nTime)
{ 

	u16 tmp;

	tmp = TIM_GetCounter(TIM3);		//��� TIM3 ��������ֵ
	
	if(tmp + nTime <= 65535)
		while( (TIM_GetCounter(TIM3) - tmp) < nTime );
	else
	{
		TIM_SetCounter(TIM3, 0);//���� TIM3 �������Ĵ���ֵΪ0
		while( TIM_GetCounter(TIM3) < nTime );
	}
			
}


/*
*********************************************************************************************************
*	�� �� ��: SendOneByte
*	����˵��: ģ�⴮�ڷ���һ�ֽ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SendOneByte(u8 datatoSend)
{
	u8 i, tmp;

	// ��ʼλ
	TXD_low();	//��TXD�����ŵĵ�ƽ�õ�
	Delay_Ms(SendingDelay);	

	for(i = 0; i < 8; i++)
	{
		tmp	= (datatoSend >> i) & 0x01;

		if(tmp == 0)
		{
			TXD_low();
			Delay_Ms(SendingDelay);	//0		
		}
		else
		{
			TXD_high();
			Delay_Ms(SendingDelay);	//1		
		}	
	}
	
	// ����λ
	TXD_high();//��TXD�����ŵĵ�ƽ�ø�
	Delay_Ms(SendingDelay);	
}

