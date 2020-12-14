#ifndef __RS232_H
#define __RS232_H
#include "sys.h"
#include "FreeRTOS.h"  
#include "string.h"
#include "semphr.h"  
#include "task.h"
#include "queue.h"
//#include "stdio.h"


//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define BUFFER_SIZE 	24			//��������������ÿ�ν��ܿɲ����
#define EN_USART2_RX 	1			//0,������;1,����.
#define DWIN_RX_SIZE	12
#define RS232_memcpy memcpy

extern UART_HandleTypeDef USART_RS232Handler;  //USART2���(����RS232)
extern SemaphoreHandle_t RS232_BinarySemaphore;	//��ֵ�ź������,���ڽ�����ɱ�־


extern u8 RS232_RX_BUF[BUFFER_SIZE];  	//���ջ���,���64���ֽ�.
extern u8 RS232_RX_CNT;					//���յ������ݳ���

extern u8 ucRS232_Buf[DWIN_RX_SIZE];				//�����ⲿ����������
extern u8 ucDwin_RX_Cnt; 				//�Ƶ��������յ�������




void RS232_Init(u32 bound);
void RS232_Send_Data(u8 *buf,u16 len);
void RS232_Receive_Data(u8 *buf,u16 len);
#endif


