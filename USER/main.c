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

/************看门狗任务***************/

/************页面查看任务***************/

//二值信号量句柄
EventGroupHandle_t Dwin_EventGroupHandler;	//事件标志组句柄
/***********所有需要设定的全局变量和宏定义，会放在一个头文件中*****************/

#define PI 3.1415926
#define POINT_NUM 60
u8 SinBuf[60]={0};
u8 *sinbuf=NULL;
u8 *buf=NULL;
extern s_DwinCurseData s_CurseData;

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

	//系统初始化，把一些全局变量初始化（原则上，全局变量只在一个地方可以更改，其他位置仅适用（防止内存管理问题））
	ucPageType=1;
	ucAcquireNo=1;
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
	xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//开始时页面切换
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

//换页过程不能让其中有任何任务切换（即：使此任务不会进入阻塞态）
//尝试先清屏
void changepage_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	EventBits_t EventValue;	
	u8 tempValue;
	u8 i,j;
	while(1)
	{	
		if(RS232_BinarySemaphore!=NULL)
		{
			
			err=xSemaphoreTake(RS232_BinarySemaphore,0);	//获取信号量
			if(err==pdTRUE)										//获取信号量成功
			{	
				Dwin_RxDeal();				
				ucDwin_RX_Cnt=0;//串口接收缓冲区清零	
			}						
		}
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志
		/***************菜单切换******************/		
		if(EventValue&ChangePageFlag)
		{ 
			ucPageNo=Dwin_CaculatePage();	//通过菜单去换页的情况
			ClearCurse();							//换页必清屏
			if(ucPageType==1||ucPageType==2||ucPageType==4)
			{
				//先给采集板标数
				usAddress=0x5040;
				for(i=0;i<5;i++)
				{				
					ucData[0]=0x00;
					ucData[1]=5*ucAcquirePage+(i+1);
					Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,2);
					RS232_Send_Data(ucDwin_Cmd,8+2);
					usAddress+=1;
				}
				if(ucPageType==1)
				{
					if(EventValue&CurseIsDrawingFlag)						
						xEventGroupSetBits(Dwin_EventGroupHandler,StopDrawCurseFlag);//需要放大页面且发生页面切换	
				}	
				if(ucPageType==4)
				{	
					//获得当前最新的页码数
					tempValue=AT24CXX_ReadOneByte(AT_EVENTNO);
					if(ucEventNo!=tempValue)ucEventNo=tempValue;
					ucEventNoDiv=ucEventNo/10;
					ucEventNoRem=ucEventNo%10;

					if(EventValue&CurseIsDrawingFlag)						
						xEventGroupSetBits(Dwin_EventGroupHandler,StopDrawCurseFlag);//需要放大页面且发生页面切换	
					if(EventValue&FaultPageRefreshFlag)
					{
						ucFaultPageNo=ucEventNoDiv+1;
						xEventGroupClearBits(Dwin_EventGroupHandler,FaultPageRefreshFlag);
					}
					else if(EventValue&ReturnBackEventPageFlag)
					{	
						ucFaultPageNo=ucFaultPageNo;
						xEventGroupClearBits(Dwin_EventGroupHandler,ReturnBackEventPageFlag);
					}
					//先把屏幕的时间和文本清为初始值				

					//再把事件标志（图标）给复位
					//
				}	
			}
			
			//换页			
			usAddress=0x0084;
			ucData[0]=0x5A;
			ucData[1]=0x01;
			ucData[2]=0x00;
			ucData[3]=ucPageNo;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,4);
			RS232_Send_Data(ucDwin_Cmd,8+4); 
			xEventGroupClearBits(Dwin_EventGroupHandler,ChangePageFlag);
		}

		/*****************进入波形页面**************/
		if(EventValue&EnterCursePageFlag)
		{

			ucPageType=0;
			ClearCurse();
			//换页			
			usAddress=0x0084;
			ucData[0]=0x5A;
			ucData[1]=0x01;
			ucData[2]=0x00;
			ucData[3]=ucPageNo;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,4);
			RS232_Send_Data(ucDwin_Cmd,8+4); 
			xEventGroupClearBits(Dwin_EventGroupHandler,EnterCursePageFlag);
		}
		Dwin_DataManager();	
		vTaskDelay(200);		
	}
}

//页面数据更新任务
void pagedataProcess_task(void *pvParameters)
{
	u8 flag=1,random;
	u16 i,j;
	u16 *usTransBuf;
	// u8 size;
	u8 *NotepadBuf;
	// BaseType_t err=pdFALSE;
	EventBits_t EventValue;
	// u16 usart1cnt;
	while (1)
    {
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志
		/***************菜单页****************/
		//实时监测
		if(ucPageType==1)
		{	
			if(EventValue&StopDrawCurseFlag)		//跳出概略图曲线显示循环后需要清标志位
				xEventGroupClearBits(Dwin_EventGroupHandler,StopDrawCurseFlag);	
			usAddress=0x5010;
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
			Dwin_DrawRealTimeCurse(60,sinbuf);
			if(flag>10)flag=1;
		}
		//实时数据,32个数据，分为7/7/9/9去传输（测试发现，每次传输的变量数超过14，可能10都会有，就会有数值无法显示）
		else if(ucPageType==2)
		{
			usAddress=0x5050;
			for(i=0;i<14;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//获取[0,100]区间的随机数	
				if(i%2)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,14);
			RS232_Send_Data(ucDwin_Cmd,8+14);
			usAddress=0x5057;
			for(i=0;i<14;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//获取[0,100]区间的随机数	
				if(i%2)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,14);
			RS232_Send_Data(ucDwin_Cmd,8+14); 
			usAddress=0x505E;
			for(i=0;i<18;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//获取[0,100]区间的随机数	
				if(i&0x0001)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,18);
			RS232_Send_Data(ucDwin_Cmd,8+18);
			usAddress=0x5067;
			for(i=0;i<18;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//获取[0,100]区间的随机数	
				if(i%2)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,18);
			RS232_Send_Data(ucDwin_Cmd,8+18);
			vTaskDelay(400);
		}
		//系统设置
		else if(ucPageType==3)
		{
			//存储设置信息到24C64中
		}
		//事件记录页，点进去会跳转到Page18、事件ucEventNo在0~59，其页数6，为除数+1
		//事件记录有2种显示状态，一种是点事件记录图标，显示最新一页数据，另一种是选择一页进行概略图查看后返回后，到之前显示的页面
		else if(ucPageType==4)
		{	
			
			if(EventValue&StopDrawCurseFlag)		//跳出概略图曲线显示循环后需要清标志位
				xEventGroupClearBits(Dwin_EventGroupHandler,StopDrawCurseFlag);	
	
			if(EventValue&FaultPageUpFlag)
			{
				ucFaultPageNo-=1;
				xEventGroupClearBits(Dwin_EventGroupHandler,FaultPageUpFlag);
			}
			else if(EventValue&FaultPageDownFlag)
			{
				ucFaultPageNo+=1;
				xEventGroupClearBits(Dwin_EventGroupHandler,FaultPageDownFlag);
			}
			if(ucFaultPageNo>6)ucFaultPageNo=1;
			else if(ucFaultPageNo<1)ucFaultPageNo=6;			//限制范围

			Dwin_FaultEventDisplay(); 							//默认先显示最新的事件	

			vTaskDelay(500);			
							
		}
		/*************非菜单页***************/
		//波形概略图
		else if(ucPageType==0)	
		{
			if(ucPageNo==18)
			{
				if((EventValue&FaultPageChangeFlag)||(EventValue&QuitFlag))
				{
					Dwin_Page5_Display();
					xEventGroupClearBits(Dwin_EventGroupHandler,QuitFlag|FaultPageChangeFlag);	
				}
				if(EventValue&ToDownloadFlag)
				{
					NotepadBuf=mymalloc(SRAMIN,6000*sizeof(u8));
					usTransBuf=mymalloc(SRAMIN,12000*sizeof(u16));
					if(NotepadBuf!=NULL)
					{
						Dwin_FlashDataRead(NotepadBuf,4);
						j=0;
						for(i=0;i<12000;i++)
						{
							if(i%2)
								usTransBuf[i]='\0';

							else
								usTransBuf[i]=Hex2Ascii(NotepadBuf[i]);

						}
						if(noteWrite(usTransBuf,12000)==0)
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
			//波形具体图
			else if(ucPageNo==19)
			{					
				if(EventValue&ZoomInFlag)
				{ 	
					xEventGroupClearBits(Dwin_EventGroupHandler,ZoomInFlag);
					if(EventValue&StopDrawCurseFlag)
						xEventGroupClearBits(Dwin_EventGroupHandler,StopDrawCurseFlag);
					if(EventValue&ZoomPageUpFlag)
					{
						ucZoomInNo-=1;
						xEventGroupClearBits(Dwin_EventGroupHandler,ZoomPageUpFlag);
					}					
					else if(EventValue&ZoomPageDownFlag)
					{
						ucZoomInNo+=1;
						xEventGroupClearBits(Dwin_EventGroupHandler,ZoomPageDownFlag);
					}
					if(ucZoomInNo>5)ucZoomInNo=1;
					else if(ucZoomInNo<1)ucZoomInNo=5;
					Dwin_Page6_Display();	
				}
			}	
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
	// BaseType_t err=pdFALSE;
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
		vTaskDelay(1000);  
	
	
	}

}








 


