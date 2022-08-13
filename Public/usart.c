#include "usart.h"		 
#include "stdarg.h"
#include "SysTick.h"
u16 point1 = 0;
char rxdatabufer;
_SaveData Save_Data;
int Flag,j;
	char *gpsdelim=".";
	char gpsdest[10] = {""};
		char gpsdest1[10] = {""};
	char*gpsp;
		char*gpsp1;
		char * gpsdest_string;
		u16 lathalf1=0;
		u16 lathalf2=0;
		u16 loghalf1=0;
		u16 loghalf2=0;
u8 gpssendflag=0;
	u8	gpsposition[8]={0};
uint8_t USART1_FINISH;
uint32_t jkl=0;
uint8_t USART1_FINISH=0;
	 


//int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
//{
//	USART_SendData(USART1,(u8)ch);	
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//}
//void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch)
//{
//	uint8_t High, Low;
//	
//	High = (ch&0XFF00)>>8;
//	Low = ch&0XFF;
//	
//	USART_SendData(pUSARTx,High);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//	
//	USART_SendData(pUSARTx,Low);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
//}


//void MyUSART_SendArr(uint16_t *str,uint8_t count)
//{
//	uint8_t i=0;
//	for(i=0;i<count;i++)
//	{
//		Usart_SendHalfWord(USART3,*(str+i));
//	}
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
//}



	void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data){
    USART_SendData(USARTx, Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}

}
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str){  
    while (0 != *str){
        UART_PutChar(USARTx, *str);  
        str++;  
    }
}
void UART_PutBytes (USART_TypeDef* USARTx, uint8_t *str, uint8_t length){
		int i=0;
    for(i=0;i<length;i++){
        UART_PutChar(USARTx, *str);  
        str++;  
    }
}
	

void USART1_printf (char *fmt, ...){ 
	char buffer[USART1_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART1_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART1_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


 void USART2_printf (char *fmt, ...){ 
	char buffer[USART_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr,fmt);  
	vsnprintf(buffer, USART_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
 
 void USART3_printf (char *fmt, ...){ 
	char buffer[USART_REC_LEN3+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART_REC_LEN3+1, fmt, arg_ptr);
	while ((i < USART_REC_LEN3) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
//	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	
}

/*******************************************************************************
* �� �� ��         : USART1_IRQHandler
* ��������		   : USART1�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
//int USART1_Handle_Callback(char * y, char * (*Callback)(char *)){
//	Callback(y);
//}
void USART1_IRQHandler(void){                	//����1�жϷ������
//	uint8_t ucTemp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    //�ѽ��ܵ����ݸ�ucTemp,��ͨ��USART_SendData���ͻ�ȥ
		string[jkl++] =USART_ReceiveData(USART1);


		//USART_SendData(USART1,ucTemp);

	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){

		USART1_FINISH=1;
		USART1->SR;
		USART1->DR;
		
	}
} 	

 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART2_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 


	CLR_Buf();//��ջ���
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
//#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
//	OSIntEnter();    
//#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//��������
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //���				
		}	
				
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	  		 
  } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}


u8 Hand(char *a)                   // ��������ʶ����
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
  point1 = 0;                    
}

void clrStruct()//??GPS??????
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

#endif	



uint8_t USART3_FINISH;
uint32_t jk3=0;
uint8_t USART3_FINISH=0;


u8 USART_RX_BUF3[USART_REC_LEN3];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA3=0;       //����״̬���	

/*******************************************************************************
* �� �� ��         : USART3_Init
* ��������		   : USART3��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		


	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ����
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
}

/*******************************************************************************
* �� �� ��         : USART3_IRQHandler
* ��������		   : USART3�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
/*void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 r;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		r =USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������
		if((USART_RX_STA3&0x8000)==0)//����δ���
		{
			if(USART_RX_STA3&0x4000)//���յ���0x0d
			{
				if(r!=0x0a)USART_RX_STA3=0;//���մ���,���¿�ʼ
				else USART_RX_STA3|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(r==0x0d)USART_RX_STA3|=0x4000;
				else
				{
					USART_RX_BUF3[USART_RX_STA3&0X3FFF]=r ;
					USART_RX_STA3++;
					if(USART_RX_STA3>(USART_REC_LEN3-1))USART_RX_STA3=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		
	} 
	
} */	

void USART3_IRQHandler(void){                	//����1�жϷ������
//	uint8_t ucTemp;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    //�ѽ��ܵ����ݸ�ucTemp,��ͨ��USART_SendData���ͻ�ȥ
		string3[jk3] =USART_ReceiveData(USART3);
		jk3 = jk3 +1;



//		USART_SendData(USART3,ucTemp);

	}
	else if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET){

		USART3_FINISH=1;
		USART3->SR;
		USART3->DR;
		
	}
} 	







