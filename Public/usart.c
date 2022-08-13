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
	 


//int fputc(int ch,FILE *p)  //º¯ÊýÄ¬ÈÏµÄ£¬ÔÚÊ¹ÓÃprintfº¯ÊýÊ±×Ô¶¯µ÷ÓÃ
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
	char buffer[USART1_REC_LEN+1];  // Êý¾Ý³¤¶È
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
	char buffer[USART_REC_LEN+1];  // Êý¾Ý³¤¶È
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
	char buffer[USART_REC_LEN3+1];  // Êý¾Ý³¤¶È
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
* º¯ Êý Ãû         : USART1_Init
* º¯Êý¹¦ÄÜ		   : USART1³õÊ¼»¯º¯Êý
* Êä    Èë         : bound:²¨ÌØÂÊ
* Êä    ³ö         : ÎÞ
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //´ò¿ªÊ±ÖÓ
 
	
	/*  ÅäÖÃGPIOµÄÄ£Ê½ºÍIO¿Ú */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //´®¿ÚÊä³öPA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //¸´ÓÃÍÆÍìÊä³ö
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ³õÊ¼»¯´®¿ÚÊäÈëIO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //´®¿ÚÊäÈëPA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //Ä£ÄâÊäÈë
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ³õÊ¼»¯GPIO */
	

   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
	USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
	
	USART_Cmd(USART1, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
//	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï

	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//×ÓÓÅÏÈ¼¶0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢	
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	
}

/*******************************************************************************
* º¯ Êý Ãû         : USART1_IRQHandler
* º¯Êý¹¦ÄÜ		   : USART1ÖÐ¶Ïº¯Êý
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ÎÞ
*******************************************************************************/ 
//int USART1_Handle_Callback(char * y, char * (*Callback)(char *)){
//	Callback(y);
//}
void USART1_IRQHandler(void){                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//	uint8_t ucTemp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    //°Ñ½ÓÊÜµÄÊý¾Ý¸øucTemp,ÔÙÍ¨¹ýUSART_SendData·¢ËÍ»ØÈ¥
		string[jkl++] =USART_ReceiveData(USART1);


		//USART_SendData(USART1,ucTemp);

	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){

		USART1_FINISH=1;
		USART1->SR;
		USART1->DR;
		
	}
} 	

 
#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
char USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  
  
void USART2_Init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART2, ENABLE);                    //Ê¹ÄÜ´®¿Ú 


	CLR_Buf();//Çå¿Õ»º´æ
}

void USART2_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 Res;
//#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓ½ÚÅÄÊý¶¨ÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
//	OSIntEnter();    
//#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//È·¶¨ÊÇ·ñÊÕµ½"GPRMC/GNRMC"ÕâÒ»Ö¡Êý¾Ý
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //Çå¿Õ
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//±£´æÊý¾Ý
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //Çå¿Õ				
		}	
				
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	  		 
  } 
#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓ½ÚÅÄÊý¶¨ÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
	OSIntExit();  											 
#endif
}


u8 Hand(char *a)                   // ´®¿ÚÃüÁîÊ¶±ðº¯Êý
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ´®¿Ú»º´æÇåÀí
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //Çå¿Õ
  point1 = 0;                    
}

void clrStruct()//??GPS??????
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //Çå¿Õ
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


u8 USART_RX_BUF3[USART_REC_LEN3];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART_RX_STA3=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	

/*******************************************************************************
* º¯ Êý Ãû         : USART3_Init
* º¯Êý¹¦ÄÜ		   : USART3³õÊ¼»¯º¯Êý
* Êä    Èë         : bound:²¨ÌØÂÊ
* Êä    ³ö         : ÎÞ
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
   //GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	
	/*  ÅäÖÃGPIOµÄÄ£Ê½ºÍIO¿Ú */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //´®¿ÚÊä³ö
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //¸´ÓÃÍÆÍìÊä³ö
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* ³õÊ¼»¯´®¿ÚÊäÈëIO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //´®¿ÚÊäÈë
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //Ä£ÄâÊäÈë
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* ³õÊ¼»¯GPIO */
	

   //USART3 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
	USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú3
	
	USART_Cmd(USART3, ENABLE);  //Ê¹ÄÜ´®¿Ú3 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		


	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//´®¿Ú3ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//×ÓÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
}

/*******************************************************************************
* º¯ Êý Ãû         : USART3_IRQHandler
* º¯Êý¹¦ÄÜ		   : USART3ÖÐ¶Ïº¯Êý
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ÎÞ
*******************************************************************************/ 
/*void USART3_IRQHandler(void)                	//´®¿Ú3ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 r;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï
	{
		r =USART_ReceiveData(USART3);//(USART3->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		if((USART_RX_STA3&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART_RX_STA3&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				if(r!=0x0a)USART_RX_STA3=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART_RX_STA3|=0x8000;	//½ÓÊÕÍê³ÉÁË 
			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(r==0x0d)USART_RX_STA3|=0x4000;
				else
				{
					USART_RX_BUF3[USART_RX_STA3&0X3FFF]=r ;
					USART_RX_STA3++;
					if(USART_RX_STA3>(USART_REC_LEN3-1))USART_RX_STA3=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	  
				}		 
			}
		}   		
	} 
	
} */	

void USART3_IRQHandler(void){                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//	uint8_t ucTemp;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    //°Ñ½ÓÊÜµÄÊý¾Ý¸øucTemp,ÔÙÍ¨¹ýUSART_SendData·¢ËÍ»ØÈ¥
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







