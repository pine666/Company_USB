#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
#include "rs232.h"
#include "rs485.h"
#include "dwin.h"
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
#include "rng.h"
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
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

/*********换页任务**********/
//任务优先级
#define CHANGEPAGE_TASK_PRIO		5
//任务堆栈大小	
#define CHANGEPAGE_STK_SIZE 		256
//任务句柄
TaskHandle_t ChangePageTask_Handler;
//任务函数
void changepage_task(void *pvParameters);

/***********页面数据更新****************/
//任务优先级
#define PAGEDATAPROCESS_TASK_PRIO 		4
//任务堆栈大小	
#define PAGEDATAPROCESS_STK_SIZE  		512 
//任务句柄
TaskHandle_t PageDataProcess_Handler;
//任务函数
void pagedataProcess_task(void *pvParameters);

/************调试用任务***************/
//任务优先级
#define DEBUGPROCESS_TASK_PRIO 		3
//任务堆栈大小	
#define DEBUGPROCESS_STK_SIZE  		256
//任务句柄
TaskHandle_t DebugProcess_Handler;
//任务函数
void debugProcess_task(void *pvParameters);

/************串口1收发任务***************/
//任务优先级
#define USART1PROCESS_TASK_PRIO 		2
//任务堆栈大小	
#define  USART1PROCESS_STK_SIZE  		512
//任务句柄
TaskHandle_t Usart1Process_Handler;
//任务函数
void usart1Process_task(void *pvParameters);

//二值信号量句柄
EventGroupHandle_t Dwin_EventGroupHandler;	//事件标志组句柄

/***********所有需要设定的全局变量和宏定义，会放在一个头文件中*****************/

//u8 TEXT[100]={"YOU ARE RIGHT YOU KNOW!\r\n"}; 
#define PI 3.1415926
#define POINT_NUM 60
u8 SinBuf[60]={0};
u8 *sinbuf=NULL;
u8 *buf=NULL;
extern s_DwinCurseData s_CurseData;
char const cnstr[9]={"这部分用"};


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
	RS485_Init(115200);						//RS485初始化
	LED_Init();								//初始化LED
	SDRAM_Init();                   		//初始化SDRAM
	RTC_Init();								//RTC时钟初始化
	RNG_Init();								//RNG,随机生成器初始化
    W25QXX_Init();				   		 	//初始化W25Q256
 	my_mem_init(SRAMIN);		    		//初始化内部内存池
	// my_mem_init(SRAMEX);		    		//初始化外部内存池
	my_mem_init(SRAMDTCM);		    		//初始化CCM内存池 
	exfuns_init();				            //为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1); 		            //挂载SPI FLASH. 
	USB_USER_Init();
	ucPageNo=1;
    //创建开始任务
	xTaskCreate((TaskFunction_t )start_task,            //任务函数
				(const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建二值信号量
	RS232_BinarySemaphore=xSemaphoreCreateBinary();
	RS485_BinarySemaphore=xSemaphoreCreateBinary();
	DEBUG_BinarySemaphore=xSemaphoreCreateBinary();
	//创建事件标志组
	Dwin_EventGroupHandler=xEventGroupCreate();	
	xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ClearScreenFlag);//开始时页面切换
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
    //创建调试任务
    xTaskCreate((TaskFunction_t )usart1Process_task,     
                (const char*    )"usart1Process_task",   
                (uint16_t       )USART1PROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART1PROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Usart1Process_Handler); 				
				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//换页函数
//换页如何保证他会立即执行，且过程只能够
void changepage_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	EventBits_t EventValue;	
	u8 i,j;
	while(1)
	{	

		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志				
		if(EventValue&ChangePageFlag)
		{ 					
			usAddress=0x0084;
			ucData[0]=0x5A;
			ucData[1]=0x01;
			ucData[2]=0x00;
			ucData[3]=ucPageNo;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,4);
			RS232_Send_Data(ucDwin_Cmd,8+4); 
			xEventGroupClearBits(Dwin_EventGroupHandler,ChangePageFlag);	
			if(EventValue&ClearScreenFlag)
			{
				vTaskDelay(40);
				ClearCurse();
				xEventGroupClearBits(Dwin_EventGroupHandler,ClearScreenFlag);					
			}
			if(EventValue&EventPageClearFlag)
			{
				//先把屏幕的时间和文本清为初始值
				usAddress=0x5020;
				for(i=0;i<10;i++)
				{		
					for(j=0;j<7;j++)
						ucData[j]=0x00;					
					Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
					RS232_Send_Data(ucDwin_Cmd,8+7);	
					usAddress=usAddress+4;
				}
			}
		}
		if(RS232_BinarySemaphore!=NULL)
		{

			err=xSemaphoreTake(RS232_BinarySemaphore,0);	//获取信号量
			if(err==pdTRUE)										//获取信号量成功
			{			
				Dwin_RxDeal();
				ucDwin_RX_Cnt=0;//串口接收缓冲区清零	
			}						
		}
		Dwin_DataManager();	
		vTaskDelay(200);		
	}
}

//页面数据更新任务
void pagedataProcess_task(void *pvParameters)
{
	u8 flag=1,random;
	u16 i,j,k,inc;
	// u8 size;
	u8 *BigBuf;
	u8 *Usart1Buf;
	u8 *NotepadBuf;
	BaseType_t err=pdFALSE;
	EventBits_t EventValue;
	// u16 usart1cnt;
	while (1)
    {
		
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志
		//欢迎界面
		if(ucPageNo==0)			//若进入页面数据加载时，页面还是为0（欢迎界面），则换页为1
		{
			ucPageNo=1;
			xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);
		}
		//首页
		else if(ucPageNo==1)
		{			
			usAddress=0x5001;
			for(i=0;i<20;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//获取[0,100]区间的随机数
				if(i%2)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,20);
			RS232_Send_Data(ucDwin_Cmd,8+20); 
			CreateSineVal(flag++);
			sinbuf=SinBuf;
			Dwin_DrawCurse(60,sinbuf,10);
			if(flag>10)flag=1;
		}
		//事件记录页，点进去会跳转到Page5
		else if(ucPageNo==3)
		{

			Dwin_FultEventDisplay();
		}
		else if(ucPageNo==5)
		{
			if((EventValue&FaultPageChangeFlag)||(EventValue&QUITFLAG))
			{
				k=0;
//				usart1cnt=ucDebug_RX_CNT;					
				ucDebug_RX_CNT=0;//串口接收缓冲区清零
				Usart1Buf=mymalloc(SRAMIN,800*sizeof(u8));
				if(Usart1Buf!=NULL)
				{
//						mymemcpy(Usart1Buf,ucDebug_RX_BUF,usart1cnt);
					for(i=0;i<800;i++)
					{
						Usart1Buf[i]=ucDebug_RX_BUF[k];
						if(i%2)k=k+7;
						else k=k+8;
					}
					Dwin_DrawCurse(800,Usart1Buf,10);
					xEventGroupClearBits(Dwin_EventGroupHandler,QUITFLAG|FaultPageChangeFlag);	
					MYFREE(SRAMIN,Usart1Buf);
				}
			}
			if(EventValue&ToDownloadFlag)
			{
				NotepadBuf=mymalloc(SRAMIN,6000*sizeof(u8));
				if(NotepadBuf!=NULL)
				{
					mymemset(NotepadBuf,0x64,6000);
					if(noteWrite(NotepadBuf,6000)==0)
					{
						LED1_Toggle;
						//这里需要一个屏幕提醒下载成功
					}
				}
				xEventGroupClearBits(Dwin_EventGroupHandler,ToDownloadFlag);	
			}
			if(EventValue&ToPrintFlag)
			{
				xEventGroupClearBits(Dwin_EventGroupHandler,ToPrintFlag);	
			}

		}
		else if(ucPageNo==6)
		{
			BigBuf=mymalloc(SRAMIN,1200*sizeof(u8));
			if(BigBuf!=NULL)
			{
				k=0;
				for(i=0;i<1200;i++)
				{
					if(ucZoomInNo==0)break;
					switch(ucZoomInNo)
					{
						case 1:	inc=0;break;
						case 2:	inc=1200;break;
						case 3:	inc=2400;break;
						case 4:	inc=3600;break;
						case 5:	inc=4800;break;
						default:break;
					}
					BigBuf[i]=ucDebug_RX_BUF[inc+(k++)];
				}
				if(ucZoomInNo!=0)
				{
					Dwin_DrawCurse(1200,BigBuf,10);
					ucZoomInNo=0;
				}
			}
			MYFREE(SRAMIN,BigBuf);				
		}		
    }
}

//调试任务
void debugProcess_task(void *pvParameters)
{
//	EventBits_t EventValue;
//	u8 err;
	while(1)
	{
		USBH_Process(&hUSBHost);
//		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志		
//		if(EventValue&ToDownloadFlag_2)
//		{

//			xEventGroupClearBits(Dwin_EventGroupHandler,ToDownloadFlag_2);					
//		}
        vTaskDelay(10);                          		
	}
}
//串口1收发任务
void usart1Process_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	while(1)
	{
//		if(DEBUG_BinarySemaphore!=NULL)
//		{
//			err=xSemaphoreTake(DEBUG_BinarySemaphore,1000);	//获取信号量
//			if(err==pdTRUE)										//获取信号量成功
//			{				
//				DEBUG_Send_Data(ucDebug_RX_BUF,ucDebug_RX_CNT);
//				ucDebug_RX_CNT=0;//串口接收缓冲区清零	
//			}						
//		}
		LED1_Toggle;	
		vTaskDelay(10);  
	
	
	}

}








 


