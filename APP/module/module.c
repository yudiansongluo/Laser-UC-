#include "function.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "SysTick.h"
#include "usart.h"
#include "module.h"
#include "hwjs.h"
#include "nrf24l01.h"
#include "spi.h"
#include "gps.h"
#include "voice.h"

u8 order[] = {0xaa,0x5a,0x01,0xC8,0x01,0xC8,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0xB3};
u8 flag_que[] = {0x04,0x10,0x08,0x80,0x40,0x30,0x20};
u8 voice_que[] = {0x12,0x14,0x13,0x0f,0x11,0x10,0x15};
u8 zhongdan_que[] = {0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16};
u8 bodybuff[255];
u8 hit = 0;

u8 netflag=1;
u8 netflag1=1;

u8 netflag2=1;
u8 netflag3=1;
u8 Lc12s_flag = 1;
char string[]="";
char string3[]="";
char *dest_string="";
char *dest_string1="";
char*p;
char*NetMessage;
u8 i,s;
int fport,bytes_length,bytes_length3;
u8 lorasendflag=0;	
u8 IrRecSendflag=0;
u8 Voiceflag=0;
u8 data=0;
u8  y=0;
u8 port=0;
u16 USART1_RX_STA=0;       //?????????	
u8 USART1_RX_BUF[USART1_REC_LEN]; 
u8 test=2;
u8 count=0;
u8 count1=0;
u8 beat_count=0;
u8 INITFLAG=1;
uint32_t jk=0;
u8 Res;
u8 netdata[]={22,61,97,78,78,ID_Num1,ID_Num2,0x58};
u8 flagdata[] = {0x58,0x63};
char *delim=",";
char dest[10] = {""};
char dest1[10] = {""};	
uint8_t converted[8];
uint8_t converted3[8];
u8 rx_buf[33]={0};
u16 color=0;

char str[20]="";
char str1[20]="";
char str2[20]="";
char str3[20]="";
char str0[20]="";
char str4[100]="";
char str5[100]="";
u8 a[40]={0};
char *ret;
u8 kkkk;
char *merged;
char *merged1;



//����
char joinCmd [] = "at+join\r\n";
//������Ϣ
char testCmd [] = "at+send=lora:1:5A00\r\n";

char atCmd [] ="at+set_config=lora:confirm:1\r\n";

char position []	= "at+send=lora:1:008000002e3e04d109980d1201c88a\r\n"; //456
////ʿ������
//char position[]	= "at+send=lora:1:008000002e3e04d109980d12007b8a\r\n"; //123

//char position[]	= "at+send=lora:1:008000002e3e04d109980d12000c8a\r\n"; //12

//char position[]	= "at+send=lora:1:008000002e3e04d109980d1200188a\r\n"; //24

//char position[]	= "at+send=lora:1:008000002e3e04d109980d12005e8a\r\n"; //94
//#define ID_Num1  0x00
//#define ID_Num2  0x0C //12 

//#define ID_Num1  0x00
//#define ID_Num2  0x18

//#define ID_Num1  0x00
//#define ID_Num2  0x5E

char FeedbackOrder[]="OK Join Success\r\n";
//char FeedbackOrder1[]="OK \r\nat";
char FeedbackOrder1[]="OK";
const int bufferSize1=15;
const int bufferSize2=6;
//������Ϣ1,ping��Ϣ
unsigned char buffer1[]={0x00,0x80,0x00,0x00,0x2e,0x3e,0x04,0xd1,0x09,0x98,0x0d,0x12,ID_Num1,ID_Num2,0x8A};
//������Ϣ2��������Ϣ
unsigned char buffer2[]={0x40,0x00,0x00,ID_Num1,ID_Num2,0x5b};

//lora������
void Jion_Net(){
	USART_ClearFlag(USART1,USART_FLAG_TC);
	//������	
	USART1_printf("%s",joinCmd);
	//�����յ����ط�����	
	while(netflag==1)
	{
		if(USART1_FINISH)
		{
//			USART1_FINISH = 0;
			//		ret=strstr(string ,FeedbackOrder);
			USART3_printf("%s",joinCmd);
			//�����ɹ�	
			if(strncmp(string ,FeedbackOrder,2)==0)
			{							

				netflag=0;
				//��ջ���
				for(jk=0;jk<jkl;jk++)
				{
					string[jk]=0x00;
				}
				jkl = 0;
			}
			else
			{
//				USART1_printf("%s",joinCmd);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
//				Delay_Ms(1000);
				for(jk=0;jk<jkl;jk++)
				{
					string[jk]=0x00;
				}
				jkl = 0;
				}
			USART1_FINISH = 0;
		}
		else
		{
			for(jk=0;jk<jkl;jk++)
				{
					string[jk]=0x00;
				}
				jkl = 0;
				USART1_FINISH = 0;
				USART1_printf("%s",joinCmd);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
		}
	}
	

	//ʿ������;
	USART1_printf("%s",position);
	USART3_printf("%d",position);
	
	while(netflag1==1)
	{
		if(USART1_FINISH)
		{
//			USART1_FINISH = 0;
			//		ret=strstr(string ,FeedbackOrder);
//			USART3_printf("%d",strncmp(string ,FeedbackOrder1,4));
			if(strncmp(string ,FeedbackOrder1,2)==0)
			{							
				netflag1=0;
				for(jk=0;jk<jkl;jk++)
				{
					string[jk]=0x00;
				}
				jkl = 0;
			}
			else
			{
//				USART1_printf("%s",position);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//			USART3_printf("%d",position);
			for(jk=0;jk<jkl;jk++)
			{
				string[jk]=0x00;
			}
			jkl = 0;
			USART1_FINISH = 0;
			}	
		}
		//����û�յ�����֮ǰ����������ʿ��
		else
		{
				for(jk=0;jk<jkl;jk++)
				{
					string[jk]=0x00;
				}
				jkl = 0;
			USART1_printf("%s",position);
			USART1_FINISH = 0;
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
		}
	}
	USART3_printf("%s",position);
}

//����1�յ����ص����ݣ����д���
void Usart_Receive()
{
	if(USART1_FINISH)
	{
		USART1_FINISH = 0;
//		USART3_printf("111 : %s\r\n",string);
		//�յ������ݰ����� ����
		if(strstr(string,":"))
		{
			lorasendflag=1;
			dest_string = strstr(string,":");
			strncpy(dest, dest_string+1, 6);//�ָ�
			//USART1_printf("333 : %s\r\n",dest);
			bytes_length=strlen(dest) / 2;
//			converted[0]=atoi(dest);
			strHexToUint8Array(dest,converted,bytes_length);
			converted[0]=atoi(converted);
			//USART1_printf("converted:%d\r\n",converted[0]);
		}
		//�յ������ݰ����� at��
		if(strstr(string,"at"))
		{
			dest_string1 = strstr(string,"=");
			p=strtok(dest_string1, delim);
			//	printf("string : %s\r\n",p);
			strncpy(dest1, p+1, 16);
			fport=atoi(dest1);    //�ַ���ת��������ȡ�˿ںţ�
			//USART1_printf("fport:%d\r\n",fport);
			//printf("222 : %s\r\n",dest1);
		}
		for(jk=0;jk<jkl;jk++)
		{
			string[jk]=0x00;
		}
		jkl = 0;
	}
}


	//���ױ�������Ϣ����
void IR_Poll()
{
	u8 Ir_temp = 77;
	if(hw_jsbz==1)	
	{
		hw_jsbz=0;	  
		Ir_temp = irflag;
	//�ָ��4���ֽ�
		data_u8[0] = (hw_jsm >> 24) & 0xff;	 
		data_u8[1] = (hw_jsm >> 16) & 0xff; 	   
		data_u8[2] = (hw_jsm >> 8)  & 0xff;
		data_u8[3] =  hw_jsm       & 0xff;	
		hw_jsm=0;				
		//��С�˱仯
		data_u8[0]= reverse_bit8(data_u8[0]);
		data_u8[1]= reverse_bit8(data_u8[1]);
		data_u8[2]= reverse_bit8(data_u8[2]);
		data_u8[3]= reverse_bit8(data_u8[3]);
		//��Ϣ����������Ϣ��
		buffer2[1]=data_u8[0];//0x01
		buffer2[2]=data_u8[2];//0xc8
			
		// ���в�λ��ʾforward 80; back  40;	r_leg 20;l_leg 10; right 08; left 04; 
		// 5�Ǳ��� 4��ǰ��.3�����֣�1�����֣�2�����ȣ�7������,6�Ǹ���,
		buffer2[0]=flag_que[Ir_temp];
		buffer1[2]++;
		Senddata(voice_que[Ir_temp]);
		delay_ms(400);
		Senddata(zhongdan_que[Ir_temp]);
//		GPIO_SetBits(GPIOB,GPIO_Pin_0);
//		delay_ms(1000);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		bodybuff[++hit] = flag_que[Ir_temp];
		//����������Ϣ2
		merged=newMergedData(buffer2,bufferSize2);
		USART1_printf("%s",merged);
		Ir_temp =77;
		free(merged);//�ڴ漰ʱ�ͷţ�
	}
}


//������Ϣ
void updating()
{
	if(hit>1)
	{
			buffer2[0] =  bodybuff[i];
			merged=newMergedData(buffer2,bufferSize2);
			USART1_printf("%s",merged);
			hit = hit - 1;
		}
	}



//gps��Ϣ����
void Gps_RecToSend()
{
	// GPS��Ч���±�־ 
	if(gpssendflag==1)
	{
	gpssendflag=0;
		
	//longtidu����
	buffer1[4] = gpsposition[6];
	buffer1[5] = gpsposition[7];
	buffer1[6] = gpsposition[4];
	buffer1[7] = gpsposition[5];

	//latitudeγ��
	buffer1[8] = gpsposition[2];
	buffer1[9] = gpsposition[3];
	buffer1[10] = gpsposition[0];
	buffer1[11] = gpsposition[1];

	merged=newMergedData(buffer1,bufferSize1);
	USART1_printf("%s",merged);
//	USART3_printf("%s",merged);
	free(merged);
	}
}

void Lc12s_Init()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	
	for(int i = 0;i<17;i++)
	{
		UART_PutChar(USART3,order[i]);
		delay_ms(3);
	}
	delay_ms(1000);
	delay_ms(1000);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
}


//�����֮����շ�
void Usart3_Receive()
{
	//lora recevive
	if(USART3_FINISH)
	{
		USART3_FINISH = 0;

		
//		char buf[8];
 //ȥ�����ź���λ
			if(string3[0] == 0x16 && string3[7] == 0x4d )
			{
				buffer1[2]++;
				buffer2[0]=0x01;
				buffer2[1] = string3[1];
				buffer2[2] = string3[3];
				Senddata(zhengwang);
//				USART_Send(voice8,sizeof(voice8));
				merged=newMergedData(buffer2,bufferSize2);
				USART1_printf("%s",merged);
				for(int m =0 ;m<=5;m++)
				{
					delay_ms(1000);	
				}
				for (int j =0;j<jk3;j++)
				{
					string3[j] = 0x00;
				}
				jk3=0;
				//��������
				delay_ms(500);	
				UART_PutChar( USART3,flagdata[1]);
			
//			USART3_printf("333%d\r\n",buffer1);
				free(merged);	
				delay_ms(100);				
			}
			//�����ж�2�����յ�ǹ��������Ϣ
			if(string3[0] == 0x0b && string3[7] == 0x4d)
			{
					
				buffer1[1]= string3[1];
				merged1=newMergedData(buffer1,bufferSize1);
				USART1_printf("%s",merged1);
				for(int m =0 ;m<=5;m++)
				{
					delay_ms(1000);	
				}
				free(merged1);	
				//��������
				for (int j =0;j<jk3;j++)
				{
					string3[j] = 0x00;
				}
				jk3=0;
				delay_ms(500);	
				UART_PutChar( USART3,flagdata[0]);
				delay_ms(100);
			}
			if(jk3 >= 8)
			{
				for (int j =0;j<jk3;j++)
				{
					string3[j] = 0x00;
				}
				jk3=0;
			}

  
	
	}
}





void Lc12s_test1(u8 data)
{
	while(1)
	{
	for (int i=0;i<8;i++)
				{
					UART_PutChar( USART3,data);
					delay_ms(3000);
				}
	}
}

void Lc12s_test(u8 data[])
{
	while(1)
	{
	for (int i=0;i<8;i++)
				{
					UART_PutChar( USART3,data[i] );
					delay_ms(3);
				}
	delay_ms(3000);
	}
}

void Lc12s(uint8_t data[])
	//����Ҫ����һ����ʱ��ʱ������ʱ�ͷ����˴η��ͣ���һ��ʱ���ڽ���,��δ����
{
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//�������ݰ�	

	//�����յ�����
	while (Lc12s_flag == 1)
	{
		if(USART3_FINISH)
		{
			USART3_FINISH = 0;
			//�ж϶Է��Ƿ���յ���������յ��Ͳ��ڷ���
			if(string3[0] == 0x58|| string3[0] == 0x63)//88Ϊǹ��99Ϊͷ��
			{
				Lc12s_flag = 0; //���ͱ�־λ����
				for(int j =0;j<jk3;j++)
				{
					string3[i] = 0;//��ջ���
				}
				jk3=0;
			}
			//�����ж�1�����յ�ͷ����������Ϣ
      //��δ���ý��պ���
			else
			{
				for(int j =0;j<jk3;j++)
				{
					string3[i] = 0;//��ջ���
				}
				jk3=0;
				for (int i=0;i<8;i++)
				{
					UART_PutChar( USART3,data[i] );
					delay_ms(3);
				}
				delay_ms(1000);
				USART1_FINISH = 0;
			} 
		}	
		else
		{
				for(int j =0;j<jk3;j++)
				{
					string3[i] = 0;//��ջ���
				}
				jk3=0;
			//δ�յ��ظ�������Э��
			for (int i=0;i<8;i++)
			{
				UART_PutChar( USART3,data[i] );
				delay_ms(3);
			}
			delay_ms(1000);
			USART1_FINISH = 0;
		}
	}
}

void USART3_Send()
{
//		USART3_printf( "%d",netdata );
//	int i=0;
//	for (i=0;i<7;i++)
//	{
//	  UART_PutChar( USART3,netdata[i] );
//	}
//	
//////  USART3_printf( "%d",0xc7 );
//	delay_ms(1000);	
	// send message from nrf2401
//	USART3_printf( "%s",netdata );
	if(lorasendflag==1)
	{
		//22 means send to head;
		//11 means send to gun;
		//78 means people number
//		netdata[1]=fport;
		netdata[2]=	converted[0];
		
		//send to head 
		if(fport==3)
		{
			if(netdata[2] == 0x00)
			{
				Senddata(ready);
			}
			if(netdata[2] == 0x01)
			{
				Senddata(zhengwang);
			}
//			netdata[0]=22;
//			Lc12s(netdata);
//			int i=0;
//			for (i=0;i<7;i++)
//			{
//				UART_PutChar( USART3,netdata[i] );
//			}
		}

		//send to gun 
		if(fport==5)
		{
			netdata[0]=11;
			Lc12s(netdata);
//			int i=0;
//			for (i=0;i<7;i++)
//			{
//				UART_PutChar( USART3,netdata[i] );
//			}			
		}
		lorasendflag=0;
		netdata[1]=0;
		netdata[2]=	0;
	}
}




//gpsԭʼ���ݴ���
void GPS_LogIn()
{
	parseGpsBuffer();
	printGpsBuffer();
}



