#include "stm32f10x.h"
#include "bsp_fun.h"


/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: 配置PA9为TXD，PA10做RXD
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOA clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

	/* 防止上电后的误判 */
	GPIO_SetBits(GPIOA, GPIO_Pin_9);

	/* 配置PA9为推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置PA10为浮空输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}





/*
*********************************************************************************************************
*	函 数 名: TIM2_Configuration
*	功能说明: 配置TIM1
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//打开时钟

  	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	//设置了用来作为 TIM2 时钟频率除数的预分频值
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割为0
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM2 向上计数模式

  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//初始化
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	//清除 TIM2 的中断待处理位
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   		//开启TIM2的中断 
        
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2 全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: TIM3_Configuration
*	功能说明: 配置TIM1
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  	TIM_TimeBaseStructure.TIM_Period = 65535 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	//设置了用来作为 TIM3 时钟频率除数的预分频值
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割为0
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM3 向上计数模式

  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);    

}



/*
*********************************************************************************************************
*	函 数 名: Delay_Ms
*	功能说明: 延时
*	形    参：nTime,单位为uS
*	返 回 值: 无
*********************************************************************************************************
*/
void Delay_Ms(volatile u32 nTime)
{ 

	u16 tmp;

	tmp = TIM_GetCounter(TIM3);		//获得 TIM3 计数器的值
	
	if(tmp + nTime <= 65535)
		while( (TIM_GetCounter(TIM3) - tmp) < nTime );
	else
	{
		TIM_SetCounter(TIM3, 0);//设置 TIM3 计数器寄存器值为0
		while( TIM_GetCounter(TIM3) < nTime );
	}
			
}


/*
*********************************************************************************************************
*	函 数 名: SendOneByte
*	功能说明: 模拟串口发送一字节数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SendOneByte(u8 datatoSend)
{
	u8 i, tmp;

	// 开始位
	TXD_low();	//将TXD的引脚的电平置低
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
	
	// 结束位
	TXD_high();//将TXD的引脚的电平置高
	Delay_Ms(SendingDelay);	
}

