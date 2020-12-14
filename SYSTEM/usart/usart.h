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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2017/6/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			6064  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8 ucDebug_RX_BUF[USART_REC_LEN];
extern u16 ucDebug_RX_CNT;

extern UART_HandleTypeDef USART_DEBUGHandler; //UART���
extern SemaphoreHandle_t DEBUG_BinarySemaphore;	//��ֵ�ź������,���ڽ��ճɹ���־


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void DEBUG_Send_Data(u8 *buf,u16 len);
void DEBUG_Receive_Data(u8 *buf,u16 len);
#endif
