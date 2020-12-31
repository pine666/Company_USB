#ifndef __DWIN_H
#define __DWIN_H
#include "sys.h"
#include "rs232.h"
#include "usart.h"
#include "rtc.h"
#include "w25qxx.h"
#include "24cxx.h"
#include "string.h"
#include "ff.h"
#include "malloc.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
//����Dwin�������ݷ�����crcУ�����

#define Dwin_memcpy mymemcpy
/*(32-25)*1024=7168-6156=1012-----����ֻ��Ҫ240������˷�������512��Ϊ��������4096��ƫ�ƣ���������ʹ�ڴ�ȣ���֮���ͨ����忨Ѱ������ʵ��*/
#define DWININFOADDR 	32256*1024  			//spiflash��25M����FAFTS���ڴ����
												//��Ȼ�ⲿ�ֿ���ֱ��ʹ�ã���������Ҫһ���ļ�����
												//�������¼��Ļ�����Ϣֻ�ǽṹ�壬����С��������Ҫ�ļ�����
												//��˰�������ʹ�õĵ�ַ���ڴ洢�����¼�

#define MAXCURSE    	10			//����һ����෢��10*8��ͨ���������ݣ�������ʾ�����ȽϺ���
#define CMDMAXLEN 		100			//ָ����󳤶�
#define DWIN_BUFFER_LEN 6000		//ÿ�ι���¼������������֮����Ϊ800��

#ifdef USE_Vector_Diagram
	#define ADDR_UA_LINE 0x5030			
	#define ADDR_UB_LINE 0x5040
	#define ADDR_UC_LINE 0x5050
#endif


/***���¼��궨��****/
//���º궨�彫�����¼���־������ʹ�ã���ʹ��2���¼���������Ϊÿ���¼�����ֻ��ʹ��24���¼���־λ��Զ������
//�¼�ͬʱ��������ӦΪ"|"����Ϊ��������
//Event
#define ChangePageFlag 			(1<<0)	//��ҳ
#define EnterCursePageFlag 		(1<<1)	//������ҳ
#define ToDownloadFlag 			(1<<2)	//���ذ���
#define ZoomInFlag 				(1<<3)	//�Ŵ󰴼�
#define PassWordRightFlag 		(1<<4)	//������֤��ȷ��־
#define ToPrintFlag 			(1<<5)	//��ӡ����
#define QuitFlag 				(1<<6)	//�˳��Ŵ�ͼ����
#define FaultPageChangeFlag 	(1<<7)	//����ҳ��ҳ��־
#define StopDrawCurseFlag 		(1<<8)	//ֹͣ�����ߣ�ֱ�Ӵ�ѭ��������������ҳ����ת�ͷŴ���Сʱ
#define CurseIsDrawingFlag		(1<<9)	//��ǰ���ڻ����߱�־�������жϻ�ҳʱ����Ҫֹͣ������

#define ReturnBackEventPageFlag	(1<<16)	//�����¼�ҳ
#define FaultPageRefreshFlag		(1<<17)	//�¼�ҳˢ��

//��ҳ�¼���
#define AcquirePageUpFlag 		(1<<10)	//�ɼ������Ϸ�ҳ
#define AcquirePageDownFlag	 	(1<<11)	//�ɼ������·�ҳ
#define ZoomPageUpFlag 			(1<<12)	//�Ŵ�ҳ���Ϸ�ҳ
#define ZoomPageDownFlag 		(1<<13)	//�Ŵ�ҳ���·�ҳ
#define FaultPageUpFlag 		(1<<14)	//����ҳ��ǰ��ҳ
#define FaultPageDownFlag 		(1<<15)	//����ҳ���ҳ
//DWIN��дenum
typedef enum
{	
	DWIN_WRITE = 0x82,
	DWIN_READ = 0x83
}DWIN_RW_FLAG;

//Dwin����ɫ
typedef enum
{
	BLACK=0x0000,
	WHITE=0xFFFF,
	RED=0xF800,
	BLUE=0x001F,
	YELLOW=0xFFE0,
	PURPLE=0xF81F,
	GREEN=0x07E0,
	GRAY=0xC618,
	BROWN=0x8200,
	ORANGE=0xFC00
}DWIN_COLOR;

//ÿ��ͨ������������
//ÿ�θ�������������ֻ�ܵ�14����Ϊָ��������Ϊ255��
//14�ᵼ��ͨ��7�޷����ܵ����ݣ���������̶���10.
typedef struct
{
	u8 ucChannel0Data[MAXCURSE];
	u8 ucChannel1Data[MAXCURSE];
	u8 ucChannel2Data[MAXCURSE];
	u8 ucChannel3Data[MAXCURSE];
	u8 ucChannel4Data[MAXCURSE];
	u8 ucChannel5Data[MAXCURSE];
	u8 ucChannel6Data[MAXCURSE];
	u8 ucChannel7Data[MAXCURSE];
}s_DwinCurseData;
//����������,������
typedef struct
{	
	u8 ucCTratio;
	u8 ucPTratio;
	u8 ucCTpercent;
	u8 ucPTpercent;
	u8 ucAddress485;
	u8 ucAddressIP[4];
	u8 ucDwinTime[6];
}s_DwinSettingList;
//�¼�����ʱ��
typedef struct 
{
	u8 EventTime_Year;
	u8 EventTime_Month;
	u8 EventTime_Day;
	u8 EventTime_Hour;
	u8 EventTime_Minute;
	u8 EventTime_Second;
}s_DwinEventTime;

//�����¼���������
typedef struct
{
	u8 DwinEventNo;					//�ڼ��η�������
	s_DwinEventTime s_EventTime;	//����ʱ��
	u8 ucFaultType; 				//��������	
}s_DwinEventList;


//�����ⲿ����ȫ�ֱ���
extern EventGroupHandle_t Dwin_EventGroupHandler;	//�¼���־����
extern s_DwinEventList s_dwin_event;
extern u8 ucDwin_Cmd[CMDMAXLEN];
extern u8 ucDwin_RX_Buf[11];			//Dwin���������ݳ���Ϊ11.
extern u8 ucPageNo;
extern u8 ucPageType;
extern u8 ucAcquireNo;
extern int8_t ucAcquirePage;

extern u8 ucZoomInNo;
extern u16 usAddress;
extern u8 ucData[28];
extern s_DwinCurseData s_CurseData;
extern u8 ucZoomChange;
extern u8 ucFaultRecordingNo;
extern u8 ucFaultPageNo;
extern u8 *Dwin_FaultData[8]; //�������ݱ���

extern u8 ucEventNo;					//�洢��AT24C02�е��¼�����Ϣ
extern u8 ucEventNoDiv;					//ucEventNod�ĳ���
extern u8 ucEventNoRem;					//ucEventNod������


unsigned int DwinGet_CRC16(unsigned char* puchMsg,unsigned char usDataLen);
u8 Dwin_CaculatePage(void);
void Dwin_CmdCreate(u16 addr,u8 *data,DWIN_RW_FLAG RWflag,u16 datanum);
u8 Dwin_RxDeal(void);
void Dwin_Page_Change(u8 page);
void Dwin_CheckPage(void);
void Dwin_DISPLAY(void);
void Dwin_Curse(s_DwinCurseData cursedata,u8 datanum);
void ClearCurse(void);
void Dwin_DrawRealTimeCurse(u16 datasize,u8 *src);
void Dwin_DrawEventCurse(u16 datasize,u8 *src,u8 mode);
//void SetCurseData(u8 *src[8],CurseData *cursedata,u8 len);
void Dwin_DataSample(u8 *src,u8 *des);
u8 Dwin_FlashDataRead(u8 *buf,u8 Type,u8 No);
u8 Dwin_FlashDataSave(u8 *buf,u8 Type,u8 No);
void Dwin_DataManager(void);


void Dwin_FaultEventDisplay(void);
void Dwin_Page5_Display(void);
void Dwin_Page6_Display(void);

#endif



