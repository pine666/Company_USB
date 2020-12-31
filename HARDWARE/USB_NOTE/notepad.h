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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//APP-记事本 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/20
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
//定义最大的编辑长度.也就是新建文本文件的大小.或者打开一个旧文件后最大允许增加的长度.
#define NOTEPAD_EDIT_LEN	7168	
extern USBH_HandleTypeDef hUSBHost;
void USB_USER_Init(void);
u8 noteWrite(void *buf,u16 len);
u8  Ascii2Hex(u8 *hex);
u16 Hex2Ascii(u8 hex);
#endif























