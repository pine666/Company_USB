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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//RS485��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define RS485_BUFFERSIZE  64
#define DAQ_RX_SIZE 200
#define RS485_memcpy memcpy

extern UART_HandleTypeDef USART_RS485Handler;  	//USART2���(����RS485)
extern SemaphoreHandle_t  RS485_BinarySemaphore;	//��ֵ�ź������,���ڽ��ճɹ���־
  	  
//Data Acquisition(DAQ)�ɼ��ź�
//�ɼ����ݽ��ա�����
extern u8 ucRS485_Buf[DAQ_RX_SIZE];			//���ڸ��ƽ��գ��ⲿ�����������
extern u8 ucDAQ_RX_Cnt; 					//�Ʋɼ����յ�������



void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);	
void RS485_TX_Set(u8 en);
#endif
