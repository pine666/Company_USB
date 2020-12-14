#ifndef __RS232_H
#define __RS232_H
#include "sys.h"
#include "FreeRTOS.h"  
#include "string.h"
#include "semphr.h"  
#include "task.h"
#include "queue.h"
//#include "stdio.h"


//如果想串口中断接收，设置EN_USART2_RX为1，否则设置为0
#define BUFFER_SIZE 	24			//最大接受数据量，每次接受可不足此
#define EN_USART2_RX 	1			//0,不接收;1,接收.
#define DWIN_RX_SIZE	12
#define RS232_memcpy memcpy

extern UART_HandleTypeDef USART_RS232Handler;  //USART2句柄(用于RS232)
extern SemaphoreHandle_t RS232_BinarySemaphore;	//二值信号量句柄,用于接收完成标志


extern u8 RS232_RX_BUF[BUFFER_SIZE];  	//接收缓冲,最大64个字节.
extern u8 RS232_RX_CNT;					//接收到的数据长度

extern u8 ucRS232_Buf[DWIN_RX_SIZE];				//用于外部读接收数据
extern u8 ucDwin_RX_Cnt; 				//计迪文屏接收到的数据




void RS232_Init(u32 bound);
void RS232_Send_Data(u8 *buf,u16 len);
void RS232_Receive_Data(u8 *buf,u16 len);
#endif


