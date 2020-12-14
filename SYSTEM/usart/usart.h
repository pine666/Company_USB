#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"
#include "FreeRTOS.h"  
#include "string.h"
#include "semphr.h"  
#include "task.h"
#include "queue.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2017/6/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			6064  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8 ucDebug_RX_BUF[USART_REC_LEN];
extern u16 ucDebug_RX_CNT;

extern UART_HandleTypeDef USART_DEBUGHandler; //UART句柄
extern SemaphoreHandle_t DEBUG_BinarySemaphore;	//二值信号量句柄,用于接收成功标志


//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void DEBUG_Send_Data(u8 *buf,u16 len);
void DEBUG_Receive_Data(u8 *buf,u16 len);
#endif
