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
 ALIENTEK ������STM32H7������ ʵ��59
 U��ʵ��(Host)ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/*********��������*************/
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

/*********��ҳ����**********/
//�������ȼ�
#define CHANGEPAGE_TASK_PRIO		5
//�����ջ��С	
#define CHANGEPAGE_STK_SIZE 		256
//������
TaskHandle_t ChangePageTask_Handler;
//������
void changepage_task(void *pvParameters);

/***********ҳ�����ݸ���****************/
//�������ȼ�
#define PAGEDATAPROCESS_TASK_PRIO 		4
//�����ջ��С	
#define PAGEDATAPROCESS_STK_SIZE  		512 
//������
TaskHandle_t PageDataProcess_Handler;
//������
void pagedataProcess_task(void *pvParameters);

/************����������***************/
//�������ȼ�
#define DEBUGPROCESS_TASK_PRIO 		3
//�����ջ��С	
#define DEBUGPROCESS_STK_SIZE  		256
//������
TaskHandle_t DebugProcess_Handler;
//������
void debugProcess_task(void *pvParameters);

/************����1�շ�����***************/
//�������ȼ�
#define USART1PROCESS_TASK_PRIO 		2
//�����ջ��С	
#define  USART1PROCESS_STK_SIZE  		512
//������
TaskHandle_t Usart1Process_Handler;
//������
void usart1Process_task(void *pvParameters);

//��ֵ�ź������
EventGroupHandle_t Dwin_EventGroupHandler;	//�¼���־����

/***********������Ҫ�趨��ȫ�ֱ����ͺ궨�壬�����һ��ͷ�ļ���*****************/

//u8 TEXT[100]={"YOU ARE RIGHT YOU KNOW!\r\n"}; 
#define PI 3.1415926
#define POINT_NUM 60
u8 SinBuf[60]={0};
u8 *sinbuf=NULL;
u8 *buf=NULL;
extern s_DwinCurseData s_CurseData;
char const cnstr[9]={"�ⲿ����"};


/****************/
//ʹ��sine��������sineֵ����������
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

/*************main����***************/
int main(void)
{	
	Cache_Enable();                			//��L1-Cache
	MPU_Memory_Protection();        		//������ش洢����
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);  		    //����ʱ��,400Mhz 
	delay_init(400);						//��ʱ��ʼ��
	uart_init(115200);						//���ڳ�ʼ��
	RS232_Init(115200);						//RS232��ʼ��
	RS485_Init(115200);						//RS485��ʼ��
	LED_Init();								//��ʼ��LED
	SDRAM_Init();                   		//��ʼ��SDRAM
	RTC_Init();								//RTCʱ�ӳ�ʼ��
	RNG_Init();								//RNG,�����������ʼ��
    W25QXX_Init();				   		 	//��ʼ��W25Q256
 	my_mem_init(SRAMIN);		    		//��ʼ���ڲ��ڴ��
	// my_mem_init(SRAMEX);		    		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMDTCM);		    		//��ʼ��CCM�ڴ�� 
	exfuns_init();				            //Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[1],"1:",1); 		            //����SPI FLASH. 
	USB_USER_Init();
	ucPageNo=1;
    //������ʼ����
	xTaskCreate((TaskFunction_t )start_task,            //������
				(const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	//������ֵ�ź���
	RS232_BinarySemaphore=xSemaphoreCreateBinary();
	RS485_BinarySemaphore=xSemaphoreCreateBinary();
	DEBUG_BinarySemaphore=xSemaphoreCreateBinary();
	//�����¼���־��
	Dwin_EventGroupHandler=xEventGroupCreate();	
	xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ClearScreenFlag);//��ʼʱҳ���л�
    //������ҳ����
    xTaskCreate((TaskFunction_t )changepage_task,             
                (const char*    )"changepage_task",           
                (uint16_t       )CHANGEPAGE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )CHANGEPAGE_TASK_PRIO,        
                (TaskHandle_t*  )&ChangePageTask_Handler);   
    //����ҳ�����ݸ�������
    xTaskCreate((TaskFunction_t )pagedataProcess_task,     
                (const char*    )"pagedataProcess_task",   
                (uint16_t       )PAGEDATAPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )PAGEDATAPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&PageDataProcess_Handler);
    //������������
    xTaskCreate((TaskFunction_t )debugProcess_task,     
                (const char*    )"debugProcess_task",   
                (uint16_t       )DEBUGPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )DEBUGPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&DebugProcess_Handler); 	
    //������������
    xTaskCreate((TaskFunction_t )usart1Process_task,     
                (const char*    )"usart1Process_task",   
                (uint16_t       )USART1PROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART1PROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Usart1Process_Handler); 				
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//��ҳ����
//��ҳ��α�֤��������ִ�У��ҹ���ֻ�ܹ�
void changepage_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	EventBits_t EventValue;	
	u8 i,j;
	while(1)
	{	

		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//��ȡ�¼���ı�־				
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
				//�Ȱ���Ļ��ʱ����ı���Ϊ��ʼֵ
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

			err=xSemaphoreTake(RS232_BinarySemaphore,0);	//��ȡ�ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{			
				Dwin_RxDeal();
				ucDwin_RX_Cnt=0;//���ڽ��ջ���������	
			}						
		}
		Dwin_DataManager();	
		vTaskDelay(200);		
	}
}

//ҳ�����ݸ�������
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
		
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//��ȡ�¼���ı�־
		//��ӭ����
		if(ucPageNo==0)			//������ҳ�����ݼ���ʱ��ҳ�滹��Ϊ0����ӭ���棩����ҳΪ1
		{
			ucPageNo=1;
			xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);
		}
		//��ҳ
		else if(ucPageNo==1)
		{			
			usAddress=0x5001;
			for(i=0;i<20;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//��ȡ[0,100]����������
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
		//�¼���¼ҳ�����ȥ����ת��Page5
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
				ucDebug_RX_CNT=0;//���ڽ��ջ���������
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
						//������Ҫһ����Ļ�������سɹ�
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

//��������
void debugProcess_task(void *pvParameters)
{
//	EventBits_t EventValue;
//	u8 err;
	while(1)
	{
		USBH_Process(&hUSBHost);
//		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//��ȡ�¼���ı�־		
//		if(EventValue&ToDownloadFlag_2)
//		{

//			xEventGroupClearBits(Dwin_EventGroupHandler,ToDownloadFlag_2);					
//		}
        vTaskDelay(10);                          		
	}
}
//����1�շ�����
void usart1Process_task(void *pvParameters)
{
	BaseType_t err=pdFALSE;
	while(1)
	{
//		if(DEBUG_BinarySemaphore!=NULL)
//		{
//			err=xSemaphoreTake(DEBUG_BinarySemaphore,1000);	//��ȡ�ź���
//			if(err==pdTRUE)										//��ȡ�ź����ɹ�
//			{				
//				DEBUG_Send_Data(ucDebug_RX_BUF,ucDebug_RX_CNT);
//				ucDebug_RX_CNT=0;//���ڽ��ջ���������	
//			}						
//		}
		LED1_Toggle;	
		vTaskDelay(10);  
	
	
	}

}








 


