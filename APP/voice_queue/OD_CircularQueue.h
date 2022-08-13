#ifndef __OD_CIRCULARQUEUE_H__
#define __OD_CIRCULARQUEUE_H__
#include "SysTick.h"

typedef struct CircularQueue_STR
{
  u8   *BuffHead;    //缓冲区头
  u16  WritePtr;     //写入指针index
  u16  ReadPtr;      //读取指针index
  u16  Count;        //可用数据长度
  u16  BuffSize;     //最大缓冲区大小
}CirQueue_Str;

extern void OD_QueueInit(CirQueue_Str *queue, u8 *bufHead, u16 bufSize);
extern u8 OD_QueueEmpty(CirQueue_Str *queue);
extern u8 OD_QueueFull(CirQueue_Str *queue);
extern void OD_QueueClear(CirQueue_Str *queue);
extern void OD_EnQueue(CirQueue_Str *queue, u8 dat);
extern u16 OD_DeQueue(CirQueue_Str *queue);
#endif  /*__OD_CIRCULARQUEUE_H__*/
