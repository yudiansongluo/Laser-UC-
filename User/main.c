#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"
#include "hwjs.h"
#include "string.h"
#include "hexify.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "nrf24l01.h"
#include "spi.h"
#include "gps.h"
#include "module.h"
#include "function.h"
#include "time.h"
#include "voice.h"   //���⴮��

const int testdataSize=15;
unsigned char testdata[]={0x00,0x10,0x00,0x00,0x2e,0x3e,0x04,0xd1,0x09,0x98,0x0d,0x12,ID_Num1,ID_Num2,0x8A};
char *smerged;

int main()
{
//	u8 netdata3[]={22,61,97,78,78,0x01,0xc8,0x58};
//	u8 hh[]={22,3,55,0x11};
//	char ll[4]={'\0'};
//	u8 string2[]="A7:00001";
	
	//initial
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	USART1_Init(115200);
	USART3_Init(9600);

	//��·�����ʼ��	
	Hwjs_InitA7();
	Hwjs_InitA15();
	Hwjs_InitC13();
	Hwjs_InitB9( );
//	Hwjs_InitA8( );
	Hwjs_InitA12();
	Hwjs_InitA11();
	Hwjs_InitB14();
	ShakeB0();
	Lc12s_GPIO_Init();
////	Lc12s_test1(netdata3[7]);
////	Lc12s_test(netdata3);
	Lc12s_Init();
//	Lc12s(netdata3);


		
			/* IO���� */
	GPIO_Configuration();


//	/* TIM3��ʼ�������ڷ��� */
//	TIM3_Configuration();


	//gps��ʼ������ģ�鲨���ʸ�Ϊ115200     
	delay_us(50);
	USART2_Init(9600);
	clrStruct();
	USART2_printf("$CCCAS,1,5*55\r\n");
	delay_us(50);
	USART2_printf("$CCCAS,1,5*55\r\n");
	delay_us(50);
	USART2_Init(115200);	 //���ڳ�ʼ��Ϊ9600
	delay_us(50);
	


	//��ʼ����ʱ��ʱ��Ϊ10000*��360-1+1��/72M=50ms������ֵΪ10000������10000�����¼���
	TIM4_Init(10000-1,360-1);  //��ʱ50ms
	TIM3_Configuration(10000-1,72-1);
	//108MHZ��ʱ��Ƶ��

	
//	TIM4_Init(10000-1,540-1);  //��ʱ50ms
//	Delay_Ms(1000);

	//gps��������
	gpsinit ();
	
//	//Lora ������ʼ����ģ������
//	Jion_Net();
	Senddata(0xE7);
	delay_ms(200);
	Senddata(ready);
//	smerged=newMergedData(testdata,testdataSize);
//	USART1_printf("%s",smerged);
	//������ʾ��
	while(1)
	{
		//�յ�����������Ϣ������	
		Usart_Receive();

		//GPS������Ϣ��
		Gps_RecToSend();	

		//����ģ�������Ϣ��
		Usart3_Receive();

		//����ģ�鷢��������Ϣ��	
	  USART3_Send();

		//������д���
		IR_Poll();	

		//��ѯGPS��	
		parseGpsBuffer();
		printGpsBuffer();

	}
}



