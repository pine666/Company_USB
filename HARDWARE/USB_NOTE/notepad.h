#ifndef __NOTEPAD_H
#define __NOTEPAD_H 	
#include "malloc.h"
#include "sys.h"
#include "ff.h"
#include "string.h"
#include "exfuns.h"
#include "rtc.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "ff.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-���±� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
//�������ı༭����.Ҳ�����½��ı��ļ��Ĵ�С.���ߴ�һ�����ļ�������������ӵĳ���.
#define NOTEPAD_EDIT_LEN	7168	
extern USBH_HandleTypeDef hUSBHost;
void USB_USER_Init(void);
u8 noteWrite(void *buf,u16 len);
u8  Ascii2Hex(u8 *hex);
u16 Hex2Ascii(u8 hex);
#endif























