#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "rs232.h"
#include "dwin.h"
#include "dma.h"
#include "sdram.h"
#include "pcf8574.h"
#include "mpu.h"
#include "malloc.h"
#include "w25qxx.h"  
#include "ff.h"
#include "exfuns.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "diskio.h"
#include "notepad.h"
#include "rtc.h"
#include "math.h"
#include "string.h"
/************************************************
 ALIENTEK 阿波罗STM32H7开发板 实验59
 U盘实验(Host)实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

/*********创建任务*************/
//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

/*********换页任务**********/
//任务优先级
#define CHANGEPAGE_TASK_PRIO		5
//任务堆栈大小	
#define CHANGEPAGE_STK_SIZE 		128
//任务句柄
TaskHandle_t ChangePageTask_Handler;
//任务函数
void changepage_task(void *pvParameters);

/***********页面数据更新****************/
//任务优先级
#define PAGEDATAPROCESS_TASK_PRIO 		4
//任务堆栈大小	
#define PAGEDATAPROCESS_STK_SIZE  		256 
//任务句柄
TaskHandle_t PageDataProcess_Handler;
//任务函数
void pagedataProcess_task(void *pvParameters);

/************调试用任务***************/
//任务优先级
#define DEBUGPROCESS_TASK_PRIO 		3
//任务堆栈大小	
#define DEBUGPROCESS_STK_SIZE  		512 
//任务句柄
TaskHandle_t DebugProcess_Handler;
//任务函数
void debugProcess_task(void *pvParameters);


//二值信号量句柄
SemaphoreHandle_t BinarySemaphore;	//二值信号量句柄


/***********所有需要设定的全局变量和宏定义，会放在一个头文件中*****************/

//u8 TEXT[100]={"YOU ARE RIGHT YOU KNOW!\r\n"}; 
#define PI 3.1415926
#define POINT_NUM 60
u8 SinBuf[60];
u8 *buf;
CurseData cursedata;



/****************/
//使用sine函数计算sine值，构建数组
void CreateSineVal(u8 flag)
{
	double i;
	u8 point;
	switch(flag)
	{
		case 1:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+10*(sin(i)));
				}
				break;
		case 2:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+20*(sin(i)));
				}
				break;
		case 3:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+30*(sin(i)));
				}
				break;
		case 4:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+40*(sin(i)));
				}
				break;
		case 5:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+50*(sin(i)));
				}
				break;
		case 6:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+60*(sin(i)));
				}
				break;	
		case 7:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+70*(sin(i)));
				}
				break;
		case 8:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+80*(sin(i)));
				}
				break;		
		case 9:	for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+90*(sin(i)));
				}
				break;
		case 10: for(point=0;point<POINT_NUM;point++)
				{
					i=(((double)point/POINT_NUM)*2*PI);
					SinBuf[point]=(u8)(100+100*(sin(i)));
				}
				break;	
		default : break;
	}
}

/*************main函数***************/
int main(void)
{	
	Cache_Enable();                			//打开L1-Cache
	MPU_Memory_Protection();        		//保护相关存储区域
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	RS232_Init(115200);						//RS232初始化
	MYDMA_Config();							//RS232——DMA初始化
	LED_Init();								//初始化LED
	SDRAM_Init();                   		//初始化SDRAM
	RTC_Init();
    W25QXX_Init();				   		 	//初始化W25Q256
 	my_mem_init(SRAMIN);		    		//初始化内部内存池
	my_mem_init(SRAMEX);		    		//初始化外部内存池
	my_mem_init(SRAMDTCM);		    		//初始化CCM内存池 
	exfuns_init();				            //为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1); 		            //挂载SPI FLASH. 
	USB_USER_Init();
	PageNo=1;
	PageChangeFlag=1;
    //创建开始任务
	xTaskCreate((TaskFunction_t )start_task,            //任务函数
				(const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
    while (1)
    {
      USBH_Process(&hUSBHost);
		if(RS232Receiveflag==1)
		{
			Dwin_RxDeal(Dwin_RX_Buf);

			
			RS232Receiveflag=0;
			RS232_RX_CNT=0;

		}
		if(PageChangeFlag)
		{				
			address=0x0084;
			data[0]=0x5A;
			data[1]=0x01;
			data[2]=0x00;
			data[3]=PageNo;
			Dwin_CmdCreate(address,data,DWIN_WRITE,4);
			RS232_Send_Data(Dwin_Cmd,8+4);
			delay_ms(40);
			PageChangeFlag=0;
			if(PageNo==1||PageNo==3||PageNo==6)
			{
				address=0x0301;
				data[0]=0x00;
				data[1]=0x00;
				for(i=0;i<8;i++)
				{					
					Dwin_CmdCreate(address,data,DWIN_WRITE,2);
					RS232_Send_Data(Dwin_Cmd,8+2);	
					Clear_Flag=0;
					address=address+2;
//					delay_ms(50);
				}
			}
			if(ZoomInFlag==1)
			{
				switch(ZoomInNo)
				{
					case 1: break;
					case 2:	break;
					case 3:	break;
					case 4:	break;
					case 5:	break;
				
				}
			}
		}

		if(PageNo==1)
		{	
			CreateSineVal(flag++);
			sinbuf=SinBuf;
			for(i=0;i<6;i++)
			{
				//这里的数据复制，以后包装一下
				mymemcpy(cursedata.Channel0Data,sinbuf,10);
				mymemcpy(cursedata.Channel1Data,sinbuf,10);
				mymemcpy(cursedata.Channel2Data,sinbuf,10);
				mymemcpy(cursedata.Channel3Data,sinbuf,10);
				mymemcpy(cursedata.Channel4Data,sinbuf,10);
				mymemcpy(cursedata.Channel5Data,sinbuf,10);
				mymemcpy(cursedata.Channel6Data,sinbuf,10);
				mymemcpy(cursedata.Channel7Data,sinbuf,10);
				Dwin_Curve(cursedata,10);
				delay_ms(40);
				sinbuf+=10;
			}
//			i=0;
//			j=0;
//				for(i=0;i<8;i++)				
//				{
//					CreateSineVal(flag++);
//					for(j=0;j<10;j++)
//					{
//						Dwin_Curve(cursedata,POINT_NUM);
//						delay_ms(40);
//					}
//					channel++;
////					sinbuf++;
//				}
	
	
		}
		else if(PageNo==3)
		{
//			address=0x0301;
//			data[0]=0x00;
//			data[1]=0x00;
//			Dwin_CmdCreate(address,data,DWIN_WRITE,2);
//			RS232_Send_Data(Dwin_Cmd,8+2);		
			
//			Clear_Flag=0;
//			for(i=0;i<8;i++)
//			{
//				CreateSineVal(10);
//				Dwin_Curve(SinBuf,POINT_NUM,i);
//			}
			
		}
		
		if(flag>10)flag=1;
        LED0_Toggle;		
	}		
}
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建二值信号量
	RS232_BinarySemaphore=xSemaphoreCreateBinary();	
    //创建换页任务
    xTaskCreate((TaskFunction_t )changepage_task,             
                (const char*    )"changepage_task",           
                (uint16_t       )CHANGEPAGE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )CHANGEPAGE_TASK_PRIO,        
                (TaskHandle_t*  )&ChangePageTask_Handler);   
    //创建页面数据更新任务
    xTaskCreate((TaskFunction_t )pagedataProcess_task,     
                (const char*    )"pagedataProcess_task",   
                (uint16_t       )PAGEDATAPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )PAGEDATAPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&PageDataProcess_Handler);
    //创建调试任务
    xTaskCreate((TaskFunction_t )debugProcess_task,     
                (const char*    )"debugProcess_task",   
                (uint16_t       )DEBUGPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )DEBUGPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&DebugProcess_Handler); 				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//换页函数
extern SemaphoreHandle_t RS232_BinarySemaphore;	//二值信号量句柄,用于接收成功标志

void changepage_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;	
	while(1)
	{

		if(RS232_BinarySemaphore!=NULL)
		{
			err=xSemaphoreTake(RS232_BinarySemaphore,1000);	//获取信号量
			if(err==pdTRUE)										//获取信号量成功
			{				
				Dwin_RxDeal(Dwin_RX_Buf);
				RS232_RX_CNT=0;//串口接收缓冲区清零
			}
			else if(err==pdFALSE)
			{
				LED1_Toggle;
				vTaskDelay(50);
			}
			if(PageChangeFlag)
			{	
							
				address=0x0084;
				data[0]=0x5A;
				data[1]=0x01;
				data[2]=0x00;
				data[3]=PageNo;
				Dwin_CmdCreate(address,data,DWIN_WRITE,4);
				RS232_Send_Data(Dwin_Cmd,8+4);
				vTaskDelay(40);
				PageChangeFlag=0;
				if(PageNo==1||PageNo==3)
				{
					ClearCurse();
				}
				
			}
		}
		
	}
}

//页面数据更新任务
void pagedataProcess_task(void *pvParameters)
{
	u8 flag;
	u8 i,j;
	u8 *BigBuf;
	while (1)
    {
		if(PageNo==1)
		{	
			CreateSineVal(flag++);
			sinbuf=SinBuf;
			for(i=0;i<6;i++)
			{
				mymemcpy(cursedata.Channel0Data,sinbuf,10);
				mymemcpy(cursedata.Channel1Data,sinbuf,10);
				mymemcpy(cursedata.Channel2Data,sinbuf,10);
				mymemcpy(cursedata.Channel3Data,sinbuf,10);
				mymemcpy(cursedata.Channel4Data,sinbuf,10);
				mymemcpy(cursedata.Channel5Data,sinbuf,10);
				mymemcpy(cursedata.Channel6Data,sinbuf,10);
				mymemcpy(cursedata.Channel7Data,sinbuf,10);
				Dwin_Curve(cursedata,10);
				vTaskDelay(40);
				sinbuf+=10;
			}

			if(flag>10)flag=1;
		}
		else if(PageNo==3)
		{
			address=0x0301;
			data[0]=0x00;
			data[1]=0x00;
			for(i=0;i<8;i++)
			{					
				Dwin_CmdCreate(address,data,DWIN_WRITE,2);
				RS232_Send_Data(Dwin_Cmd,8+2);	
				address=address+2;
//				vTaskDelay(40);
			}
			vTaskDelay(500);
			
		}
		else if(PageNo==6)
		{
			BigBuf=mymalloc(SRAMIN,1200*sizeof(u8));
			memset(BigBuf,0x64,1200);
			if(*BigBuf==0x64)
			{
				//这里的数据复制，以后包装一下,算了，包装更麻烦，不包装了
				mymemcpy(cursedata.Channel0Data,BigBuf,10);
				mymemcpy(cursedata.Channel1Data,BigBuf,10);
				mymemcpy(cursedata.Channel2Data,BigBuf,10);
				mymemcpy(cursedata.Channel3Data,BigBuf,10);
				mymemcpy(cursedata.Channel4Data,BigBuf,10);
				mymemcpy(cursedata.Channel5Data,BigBuf,10);
				mymemcpy(cursedata.Channel6Data,BigBuf,10);
				mymemcpy(cursedata.Channel7Data,BigBuf,10);
				Dwin_Curve(cursedata,10);
				vTaskDelay(40);
				BigBuf+=10;
			}
				
		}
		else if(PageNo==4)
		{
			address=0x5040;
			for(j=0;j<3;j++)
			{
				for(i=0;i<8;i++)
					data[i]=cnstr[j][i];
				Dwin_CmdCreate(address,data,DWIN_WRITE,8);
				RS232_Send_Data(Dwin_Cmd,8+8);
				vTaskDelay(400);
			}
			
		}
    }
}

//调试任务
void debugProcess_task(void *pvParameters)
{
	vTaskList(InfoBuffer);							//获取所有任务的信息
	printf("%s\r\n",InfoBuffer);					//通过串口打印所有任务的信息
	printf("/**************************结束**************************/\r\n");
	while(1)
	{
		USBH_Process(&hUSBHost);
        vTaskDelay(10);                           //延时1s，也就是1000个时钟节拍	
	}
}




 


