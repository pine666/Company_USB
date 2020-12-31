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

/************���Ź�����***************/

/************ҳ��鿴����***************/

//��ֵ�ź������
EventGroupHandle_t Dwin_EventGroupHandler;	//�¼���־����
/***********������Ҫ�趨��ȫ�ֱ����ͺ궨�壬�����һ��ͷ�ļ���*****************/

#define PI 3.1415926
#define POINT_NUM 60
u8 SinBuf[60]={0};
u8 *sinbuf=NULL;
u8 *buf=NULL;
extern s_DwinCurseData s_CurseData;

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

	//ϵͳ��ʼ������һЩȫ�ֱ�����ʼ����ԭ���ϣ�ȫ�ֱ���ֻ��һ���ط����Ը��ģ�����λ�ý����ã���ֹ�ڴ�������⣩��
	ucPageType=1;
	ucAcquireNo=1;
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
	xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//��ʼʱҳ���л�
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

//��ҳ���̲������������κ������л�������ʹ�����񲻻��������̬��
//����������
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
			
			err=xSemaphoreTake(RS232_BinarySemaphore,0);	//��ȡ�ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{	
				Dwin_RxDeal();				
				ucDwin_RX_Cnt=0;//���ڽ��ջ���������	
			}						
		}
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//��ȡ�¼���ı�־
		/***************�˵��л�******************/		
		if(EventValue&ChangePageFlag)
		{ 
			ucPageNo=Dwin_CaculatePage();	//ͨ���˵�ȥ��ҳ�����
			ClearCurse();							//��ҳ������
			if(ucPageType==1||ucPageType==2||ucPageType==4)
			{
				//�ȸ��ɼ������
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
						xEventGroupSetBits(Dwin_EventGroupHandler,StopDrawCurseFlag);//��Ҫ�Ŵ�ҳ���ҷ���ҳ���л�	
				}	
				if(ucPageType==4)
				{	
					//��õ�ǰ���µ�ҳ����
					tempValue=AT24CXX_ReadOneByte(AT_EVENTNO);
					if(ucEventNo!=tempValue)ucEventNo=tempValue;
					ucEventNoDiv=ucEventNo/10;
					ucEventNoRem=ucEventNo%10;

					if(EventValue&CurseIsDrawingFlag)						
						xEventGroupSetBits(Dwin_EventGroupHandler,StopDrawCurseFlag);//��Ҫ�Ŵ�ҳ���ҷ���ҳ���л�	
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
					//�Ȱ���Ļ��ʱ����ı���Ϊ��ʼֵ				

					//�ٰ��¼���־��ͼ�꣩����λ
					//
				}	
			}
			
			//��ҳ			
			usAddress=0x0084;
			ucData[0]=0x5A;
			ucData[1]=0x01;
			ucData[2]=0x00;
			ucData[3]=ucPageNo;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,4);
			RS232_Send_Data(ucDwin_Cmd,8+4); 
			xEventGroupClearBits(Dwin_EventGroupHandler,ChangePageFlag);
		}

		/*****************���벨��ҳ��**************/
		if(EventValue&EnterCursePageFlag)
		{

			ucPageType=0;
			ClearCurse();
			//��ҳ			
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

//ҳ�����ݸ�������
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
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//��ȡ�¼���ı�־
		/***************�˵�ҳ****************/
		//ʵʱ���
		if(ucPageType==1)
		{	
			if(EventValue&StopDrawCurseFlag)		//��������ͼ������ʾѭ������Ҫ���־λ
				xEventGroupClearBits(Dwin_EventGroupHandler,StopDrawCurseFlag);	
			usAddress=0x5010;
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
			Dwin_DrawRealTimeCurse(60,sinbuf);
			if(flag>10)flag=1;
		}
		//ʵʱ����,32�����ݣ���Ϊ7/7/9/9ȥ���䣨���Է��֣�ÿ�δ���ı���������14������10�����У��ͻ�����ֵ�޷���ʾ��
		else if(ucPageType==2)
		{
			usAddress=0x5050;
			for(i=0;i<14;i++)
			{
				random=(u8)RNG_Get_RandomRange(0,100);//��ȡ[0,100]����������	
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
				random=(u8)RNG_Get_RandomRange(0,100);//��ȡ[0,100]����������	
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
				random=(u8)RNG_Get_RandomRange(0,100);//��ȡ[0,100]����������	
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
				random=(u8)RNG_Get_RandomRange(0,100);//��ȡ[0,100]����������	
				if(i%2)
					ucData[i]=random;
				else
					ucData[i]=0x00;
			}
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,18);
			RS232_Send_Data(ucDwin_Cmd,8+18);
			vTaskDelay(400);
		}
		//ϵͳ����
		else if(ucPageType==3)
		{
			//�洢������Ϣ��24C64��
		}
		//�¼���¼ҳ�����ȥ����ת��Page18���¼�ucEventNo��0~59����ҳ��6��Ϊ����+1
		//�¼���¼��2����ʾ״̬��һ���ǵ��¼���¼ͼ�꣬��ʾ����һҳ���ݣ���һ����ѡ��һҳ���и���ͼ�鿴�󷵻غ󣬵�֮ǰ��ʾ��ҳ��
		else if(ucPageType==4)
		{	
			
			if(EventValue&StopDrawCurseFlag)		//��������ͼ������ʾѭ������Ҫ���־λ
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
			else if(ucFaultPageNo<1)ucFaultPageNo=6;			//���Ʒ�Χ

			Dwin_FaultEventDisplay(); 							//Ĭ������ʾ���µ��¼�	

			vTaskDelay(500);			
							
		}
		/*************�ǲ˵�ҳ***************/
		//���θ���ͼ
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
			//���ξ���ͼ
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
	// BaseType_t err=pdFALSE;
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
		vTaskDelay(1000);  
	
	
	}

}








 


