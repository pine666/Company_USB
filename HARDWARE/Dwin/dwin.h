#ifndef __DWIN_H
#define __DWIN_H
#include "sys.h"
#include "rs232.h"
#include "usart.h"
#include "rtc.h"
#include "w25qxx.h"
#include "string.h"
#include "malloc.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
//����Dwin�������ݷ�����crcУ�����

#define Dwin_memcpy memcpy

#define MAXCURSE    	10
#define CMDMAXLEN 		100			//ָ����󳤶�


#define ADDR_UA_LINE 0x5030
#define ADDR_UB_LINE 0x5040
#define ADDR_UC_LINE 0x5050



/***���¼��궨��****/
//�¼�ͬʱ��������ӦΪ"|"����Ϊ��������
#define ChangePageFlag (1<<0)
#define ClearScreenFlag (1<<1)
#define ToDownloadFlag (1<<2)
#define ZoomInFlag (1<<3)
#define PassWordRightFlag (1<<4)
#define ToPrintFlag (1<<5)
#define QUITFLAG (1<<6)
#define ZoomPageUpFlag (1<<7)
#define ZoomPageDownFlag (1<<8)
#define FailPageChangeFlag (1<<9)

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
//����������
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
	s_DwinEventTime s_EventTime;	//����ʱ��
	u8 ucFaultType; 				//��������
	u8 DwinEventNo;					//�ڼ��η�������
}s_DwinEventList;




//�����ⲿ����
extern EventGroupHandle_t Dwin_EventGroupHandler;	//�¼���־����
extern s_DwinEventList s_dwin_event;
extern u8 ucDwin_Cmd[CMDMAXLEN];
extern u8 ucDwin_RX_Buf[11];			//Dwin���������ݳ���Ϊ11.
extern u8 ucPageNo;
extern u8 ucZoomInNo;
extern u16 usAddress;
extern u8 ucData[28];
extern s_DwinCurseData s_CurseData;
extern u8 ucZoomChange;
extern u8 ucFaultRecordingNo;



unsigned int DwinGet_CRC16(unsigned char* puchMsg,unsigned char usDataLen);
void Dwin_CmdCreate(u16 addr,u8 *data,DWIN_RW_FLAG RWflag,u16 datanum);
u8 Dwin_RxDeal(void);
void Dwin_Page_Change(u8 page);
void Dwin_DISPLAY(void);
void Dwin_Curve(s_DwinCurseData cursedata,u8 datanum);
void ClearCurse(void);
void Dwin_DrawCurse(u16 datasize,u8 *src,u8 len);
//void SetCurseData(u8 *src[8],CurseData *cursedata,u8 len);
void Dwin_DrawLine(u16 addr,DWIN_COLOR color,signed char xs,signed char ys);
void Dwin_DataSample(u8 *src,u8 *des);


#endif

