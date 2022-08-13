/*******************************************************************************
* Copyright (C), 2014-2015, www.njodin.com
*
* �ļ���: OD_CircularQueue.c
* ���ݼ���: ѭ�����в���
*
* �ļ���ʷ:
* �汾��    ����      ����      ˵��
* v0.1    2014.7.15  candy    �����ļ�
*
*******************************************************************************/
#include "OD_CircularQueue.h"

/*******************************************************************************
*  �������� OD_QueueInit
*  ��  ��:  queue�����нṹ��bufHead����������ͷ��bufSize����󳤶�
*  ��  ��:  ��
*  ����˵���� ѭ�����еĳ�ʼ��
*******************************************************************************/
void OD_QueueInit(CirQueue_Str *queue, u8 *bufHead, u16 bufSize)
{
  queue->BuffHead = bufHead;
  queue->BuffSize = bufSize;
  queue->ReadPtr = 0;
  queue->WritePtr = 0;
  queue->Count = 0;
}

/*******************************************************************************
*  �������� OD_QueueEmpty
*  ��  ��:  queue�����нṹ
*  ��  ��:  �����Ƿ�Ϊ��
*  ����˵���� �ж϶����Ƿ�Ϊ��
*******************************************************************************/
u8 OD_QueueEmpty(CirQueue_Str *queue)
{
  return (queue->Count == 0);
}

/*******************************************************************************
*  �������� OD_QueueFull
*  ��  ��:  queue�����нṹ
*  ��  ��:  �����Ƿ�Ϊ��
*  ����˵���� �ж϶����Ƿ�Ϊ��
*******************************************************************************/
u8 OD_QueueFull(CirQueue_Str *queue)
{
  return (queue->Count == queue->BuffSize);
}

/*******************************************************************************
*  �������� OD_QueueClear
*  ��  ��:  queue�����нṹ
*  ��  ��:  ��
*  ����˵���� ѭ�����е����
*******************************************************************************/
void OD_QueueClear(CirQueue_Str *queue)
{
  queue->ReadPtr = 0;
  queue->WritePtr = 0;
  queue->Count = 0;
}

/*******************************************************************************
*  �������� OD_EnQueue
*  ��  ��:  queue�����нṹ��dat����ӵ�����
*  ��  ��:  ��
*  ����˵���� ѭ�����е����
*******************************************************************************/
void OD_EnQueue(CirQueue_Str *queue, u8 dat)
{
//  if (OD_QueueFull(queue))
//  {
//    DEBUG_WARNING("The data is overflow the circular queue\r\n");
//    return;
//  }
  *(queue->BuffHead + queue->WritePtr) = dat;
  __disable_irq();
  queue->Count++;
  queue->WritePtr = (queue->WritePtr + 1) % queue->BuffSize; //ѭ��������ƣ���ֹ�ڴ��˷�
  __enable_irq();
}

/*******************************************************************************
*  �������� OD_DeQueue
*  ��  ��:  queue�����нṹ
*  ��  ��:  ��
*  ����˵���� ѭ�����еĳ���
*******************************************************************************/
u16 OD_DeQueue(CirQueue_Str *queue)
{
  u8 temp;
  
//  if (OD_QueueEmpty(queue))
//  {
//    DEBUG_WARNING("The queue is empty\r\n");
//    return 0xFFFF;
//  }
  
  temp = *(queue->BuffHead + queue->ReadPtr);  
  __disable_irq();
  queue->Count--;  
  queue->ReadPtr = (queue->ReadPtr + 1) % queue->BuffSize;  
  __enable_irq();
  return temp;
}
