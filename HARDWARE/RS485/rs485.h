#ifndef __RS485_H
#define __RS485_H
#include "sys.h"
#include "FreeRTOS.h"  
#include "string.h"
#include "semphr.h"  
#include "task.h"
#include "queue.h"
#include "pcf8574.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//RS485驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define RS485_BUFFERSIZE  64
#define DAQ_RX_SIZE 200
#define RS485_memcpy memcpy

extern UART_HandleTypeDef USART_RS485Handler;  	//USART2句柄(用于RS485)
extern SemaphoreHandle_t  RS485_BinarySemaphore;	//二值信号量句柄,用于接收成功标志
  	  
//Data Acquisition(DAQ)采集信号
//采集数据接收、个数
extern u8 ucRS485_Buf[DAQ_RX_SIZE];			//用于复制接收，外部处理接收数据
extern u8 ucDAQ_RX_Cnt; 					//计采集接收到的数据



void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);	
void RS485_TX_Set(u8 en);
#endif
