#include "dwin.h"
/*************Dwin屏相关代码***************/


/*全局变量部分*/
/* Table Of CRC Values for high-order unsigned char */
unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

/* Table of CRC values for low-order unsigned char    */
unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};
/*存储故障数据的路径*/

u8 * const ucDwin_Cycle_Text[5]=
{
	"01-04",
	"05-08", 
	"09-12", 
	"13-16",
	"17-20"
};

u8 ucDwin_Cmd[CMDMAXLEN];		//Diwn屏发送指令，通过Create来构造数组并发送
u8 ucDwin_RX_Buf[11];			//Dwin屏接收数据长度为11（此为按键返回数据）.
u8 ucPageNo=0;					//当前页码
u8 ucPageType=0;				//当前菜单，分别为实时检测、实时数据、系统设置、事件记录、产品信息


u8 ucAcquireNo=1;				//采集板数	
int8_t ucAcquirePage=0;			//采集板页码变换	
u8 ucZoomInNo=0;				//放大区域
u16 usAddress=0;					//地址，传递给ucDwin_Cmd
u8 ucData[28]={0};					//数据，同样传递给ucDwin_Cmd

u8 ucEventNo;					//存储在AT24C02中的事件数信息
u8 ucEventNoDiv;					//ucEventNo的除数
u8 ucEventNoRem;					//ucEventNo余数

u8 ucFaultRecordingNo=0;		//选中故障事件次序
u8 ucFaultPageNo=0;				//故障页码
u8 *Dwin_FaultData[8];           //这里需要动态数组去转存，如果用内部ROM会爆
u16 usFaultFlashOffset;
u8 const ucDwinPage[5][5]={{1,2,3,4,5,},{6,7,8,9,10},{20,20,20,20,20},{12,13,14,15,16},{17,17,17,17,17}};		//菜单类型和采集板对应页码数
s_DwinCurseData s_CurseData;


/****函数部分*****/

/**********************************************************************
**函数名称:DwinGet_CRC16
**函数功能:计算16位CRC校验码，用于迪文屏接受校验
**入口参数:puchMsg - 数据存放的数组(帧头 帧头 发送的数据长度 数据 数据 数据 数据 ... CRCH CRCL)
           usDataLen - 发送的数据长度(从指令开始到最后，包括CRCH CRCL)
**          
**出口参数:	 crc16校验码
***********************************************************************/ 
unsigned int DwinGet_CRC16(unsigned char* puchMsg,unsigned char usDataLen)
{
	u8 uchCRCHi = 0xFF ;	  //高CRC字节初始化
	u8 uchCRCLo = 0xFF ; //低CRC字节初始化
	u16  uIndex  ;	//CRC循环中的索引 
	u8 len=0;
	u8 k=3;	 //数据从数组的第四位开始，前两位是帧头，第三位是数据长度，需要计算CRC的数据从第四位开始到最后
	len = usDataLen-2; //usDataLen数值包括了CRC的值，计算CRC值的数据总数为 usDataLen-2
	while (len--)		//传输消息缓冲区 
	{
		uIndex = uchCRCHi ^ puchMsg[k++];	  //计算CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (((unsigned int)(uchCRCHi) << 8) | uchCRCLo) ;
	//puchMsg[usDataLen + 1] = uchCRCHi; // puchMsg数组的倒数第二位存放CRC高字节；usDataLen+帧头和数据长度所占的3个字节 - 2
	//puchMsg[usDataLen + 2] = uchCRCLo; // puchMsg数组的倒数第一二位存放CRC高字节；usDataLen+帧头和数据长度所占的3个字节 - 1
}
/**********************************************************************
**函数名称:Dwin_CaculatePage
**函数功能:根据页码和采集板数计算页码
**入口参数:	 无
**出口参数:	 无
***********************************************************************/ 
u8 Dwin_CaculatePage(void)
{
	u8 pagetype=ucPageType;
	u8 acquireno=(ucAcquireNo/5)?5:(ucAcquireNo%5);		//确保采集板数是5的倍数
	u8 pageno;
	pageno=ucDwinPage[pagetype-1][acquireno-1];
	return pageno;
}
/**********************************************************************
**函数名称:Dwin_CmdCreate
**函数功能:创建Dwin屏指令（232通信发送指令）
			//此函数构建指令
			//读时，data为个数，写时，为数据
			//datanum为data数据量，字节为单位，若是读，则为1字节
**入口参数:addr：地址
		   *data：指向数据的指针，读时是读取长度，写时是写入数据
		   RWflag:读写标志
		   datanum：数据长度
**出口参数:	 无
***********************************************************************/ 

void Dwin_CmdCreate(u16 addr,u8 *data,DWIN_RW_FLAG RWflag,u16 datanum)
{
	u8 i;
	u16 crc16temp;
	ucDwin_Cmd[0]=0x5A;
	ucDwin_Cmd[1]=0xA5;			//迪文帧头
	ucDwin_Cmd[3]=RWflag;		//flag为83,读；为82,写
	ucDwin_Cmd[4]=(u8)(addr>>8 & 0x00FF);
	ucDwin_Cmd[5]=(u8)(addr & 0x00FF);
	if(RWflag == DWIN_READ)		//读
	{
		ucDwin_Cmd[6]=data[0];  	//读取一个字节数据
		ucDwin_Cmd[2]=0x07;	//读一个字节指令长度为9-2；这里不做计算
		crc16temp = DwinGet_CRC16(ucDwin_Cmd,ucDwin_Cmd[2]);
		ucDwin_Cmd[7] = (u8)(crc16temp>>8 & 0x00FF);
		ucDwin_Cmd[8] = (u8)(crc16temp & 0x00FF);
	}
	else if(RWflag==DWIN_WRITE)		//写
	{
		for(i=0;i<datanum;i++)
		{
			ucDwin_Cmd[6+i]=data[i];	
		}
		ucDwin_Cmd[2]=datanum+5;						//指令长度
		crc16temp = DwinGet_CRC16(ucDwin_Cmd,ucDwin_Cmd[2]);
		ucDwin_Cmd[datanum+6] = (u8)(crc16temp>>8 & 0x00FF);
		ucDwin_Cmd[datanum+7] = (u8)(crc16temp & 0x00FF);
	}
}
/**********************************************************************
**函数名称:Dwin_Reset_Icons
**函数功能:将当前页图标复位后再去显示
**入口参数:无
**出口参数:无
***********************************************************************/ 
void Dwin_Reset_Icons(void)
{



}
/**********************************************************************
**函数名称:Dwin_RxDeal
**函数功能:处理接收到的指令，并标记各事件标志位
**入口参数:buf：指向接收到的数据，其为ucRS232_BUF                   
**出口参数:0，处理成功；1，接收数据出错
***********************************************************************/ 
u8 Dwin_RxDeal(void)
{
	EventBits_t EventValue;	
	// s_DwinSettingList s_dwin_settinglist; //用于存储设置信息
	u8 ucZoomChange=0;				//放大区域换页,0，往上(左)翻页；1，往下（右）翻页，下同
	u8 ucFaultPageChange;
	u8 ucAcquirePageChange;
	u16 crc16tem;
	u8 ucdatalen;	
	u16 usaddr;
	ucdatalen = ucRS232_Buf[2];
	usaddr = (u16)(ucRS232_Buf[4]<<8|ucRS232_Buf[5]);
	crc16tem = (u16)(ucRS232_Buf[9]<<8| ucRS232_Buf[10]);
	if(ucRS232_Buf[0]==0x5A && ucRS232_Buf[1]==0xA5)
	{		if(ucDwin_RX_Cnt==ucdatalen+3)
			{		
				if(crc16tem==DwinGet_CRC16(ucRS232_Buf,ucdatalen))//判断crc检验是否正确
				{	
					EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志		
					switch(usaddr)
					{
						/*check页码，发送查询页码指令，返回当前页码,暂时不用，若之后换页还是有问题，再使用*/
						// case 0x0014:	if(ucPageNo!=ucRS232_Buf[8])
						// 				{
						// 					ucPageNo=ucRS232_Buf[8];
						// 					if(Dwin_EventGroupHandler!=NULL)
						// 						xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ClearScreenFlag);//发生页面切换且需要刷新页面							
						// 				}
						// 				break;
						/*************以下为按键切换部分*******************/
						/*页面切换*/
						case 0x5000: 	ucPageType=ucRS232_Buf[8];		//0x5000为页面切换,获得页面类型，实际页码数要根据采集板数来计算
										if(Dwin_EventGroupHandler!=NULL)
										{
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//发生页面切换
										}
										if(ucPageType==4)
											xEventGroupSetBits(Dwin_EventGroupHandler,FaultPageRefreshFlag);
										break;

						/*采集板切换,与485协议相关*/
						case 0x5001:	ucAcquireNo=ucRS232_Buf[8];
										if(Dwin_EventGroupHandler!=NULL)
										{
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//发生页面切换
										}
										break;						
						/*采集板上下页翻页*/
						case 0x5002:    ucAcquirePageChange=ucRS232_Buf[8];
										if(ucAcquirePageChange)//0:-1,上一页;1:+1,下一页
										{	
											ucAcquirePage++;
											if(ucAcquirePage>3)ucAcquirePage=0;
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//发生页面切换
										}
										else 
										{	
											ucAcquirePage--;
											if(ucAcquirePage<0)ucAcquirePage=3;
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag);//发生页面切换
										}
										break;

						/*事件记录左右翻页，0为向左翻，1是向右翻页*/
						case 0x5003: 	ucFaultPageChange=ucRS232_Buf[8];	//获得换页标志，若为0，往左翻页，若为1，则往右翻页
										if(ucFaultPageChange)	//Down 与右相对应，需要加页数
											xEventGroupSetBits(Dwin_EventGroupHandler,FaultPageDownFlag|ChangePageFlag);								
										else 					//Up 与左对应
											xEventGroupSetBits(Dwin_EventGroupHandler,FaultPageUpFlag|ChangePageFlag);
										break;

						/*故障波形放大*/
						case 0x5004: 	ucPageNo=19;						//放大操作时先换到第19页，再确定放大区域
										ucZoomInNo=ucRS232_Buf[8];
										if(Dwin_EventGroupHandler!=NULL)
										{
											if(EventValue&CurseIsDrawingFlag)										
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomInFlag|StopDrawCurseFlag);//需要放大页面且发生页面切换										
											else
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomInFlag);
										}
										break;

						/*打印按键*/
						case 0x5005: 	if(Dwin_EventGroupHandler!=NULL)	//打印故障波形							
											xEventGroupSetBits(Dwin_EventGroupHandler,ToPrintFlag);	
										break;

						/*下载按键*/
						case 0x5006: 	if(Dwin_EventGroupHandler!=NULL)	//下载故障波形								
											xEventGroupSetBits(Dwin_EventGroupHandler,ToDownloadFlag);																				
										break;

						/*返回按键*/
						case 0x5007:	ucPageType=4;
										if(Dwin_EventGroupHandler!=NULL)
										{
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ReturnBackEventPageFlag);//发生页面切换，返回事件页，要求显示为
																															  //进入概略图前的故障页码
										}
										break;

						/*具体波形前后翻页*/
						case 0x5008:	ucZoomChange=ucRS232_Buf[8];	//获得换页标志，若为1，往上翻页，若为2，则往下翻页
										ucPageNo=19;
										if(ucZoomChange==0)//上翻
										{
											if(EventValue&CurseIsDrawingFlag)										
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomPageUpFlag|ZoomInFlag|StopDrawCurseFlag);//需要放大页面且发生页面切换										
											else
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomPageUpFlag|ZoomInFlag);
										}										
										else if(ucZoomChange==1)//下翻
										{
											if(EventValue&CurseIsDrawingFlag)										
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomPageDownFlag|ZoomInFlag|StopDrawCurseFlag);//需要放大页面且发生页面切换										
											else
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|ZoomPageDownFlag|ZoomInFlag);
										}
										break;

						/*密码按键*/
						case 0x5009:	if(((u16)ucRS232_Buf[7]<<8|ucRS232_Buf[8])==0x1A0A)			//密码验证
										{
											ucPageNo=11;
											if(Dwin_EventGroupHandler!=NULL)										
												xEventGroupSetBits(Dwin_EventGroupHandler,PassWordRightFlag|EnterCursePageFlag);//发生页面切换																					
										}
										break;

						/*事件页点击查看波形*/
						case 0x500A:	ucFaultRecordingNo=ucRS232_Buf[8];
										if(ucFaultPageNo==ucEventNoDiv+1)
										{
											if(ucFaultRecordingNo<=ucEventNoRem)
											{
												ucPageNo=18;
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|FaultPageChangeFlag);//发生页面切换											
											}
										}
										else
										{
											ucPageNo=18;
											xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|FaultPageChangeFlag);//发生页面切换											
										}										
										break;  

						/*退出具体项*/
						case 0x500B:	ucPageNo=18;		//退出放大界面,通过退出标志来重新加载概略波形图
										if(Dwin_EventGroupHandler!=NULL)
										{
											if(EventValue&CurseIsDrawingFlag)										
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|QuitFlag|StopDrawCurseFlag);//需要放大页面且发生页面切换										
											else
												xEventGroupSetBits(Dwin_EventGroupHandler,EnterCursePageFlag|QuitFlag);
										}										
										break;

						/************以下为设置参数及显示部分***************/
						//暂时不做设置部分参数
						/*设置页按键返回*/
						// case 0x5070: 	s_dwin_settinglist.ucCTratio=ucRS232_Buf[8];		//CT变比
						// 				AT24CXX_WriteOneByte(AT_CTRATIO,s_dwin_settinglist.ucCTratio);
						// 				break;
						// case 0x500C: 	s_dwin_settinglist.ucCTpercent=ucRS232_Buf[8];		//CT微调指数
						// 				AT24CXX_WriteOneByte(AT_CTPERCENT,s_dwin_settinglist.ucCTpercent);						
						// 				break;
						// case 0x500D: 	s_dwin_settinglist.ucPTratio=ucRS232_Buf[8];		//PT变比
						// 				AT24CXX_WriteOneByte(AT_PTRATIO,s_dwin_settinglist.ucPTratio);	
						// 				break;
						// case 0x500E: 	s_dwin_settinglist.ucPTpercent=ucRS232_Buf[8];		//PT微调指数
						// 				AT24CXX_WriteOneByte(AT_PTPERCENT,s_dwin_settinglist.ucPTpercent);	
						// 				break;
						// case 0x500F: 	s_dwin_settinglist.ucAddress485=ucRS232_Buf[8];		//485通信地址
						// 				AT24CXX_WriteOneByte(AT_485ADDRESS,s_dwin_settinglist.ucAddress485);	
						// 				break;
						// case 0x5010: 	s_dwin_settinglist.ucAddressIP[0]=ucRS232_Buf[8];		//IP通信地址
						// 				AT24CXX_WriteOneByte(AT_IPADDRESS,s_dwin_settinglist.ucAddressIP[0]);	
						// 				break;
						// case 0x5011: 	s_dwin_settinglist.ucAddressIP[1]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_IPADDRESS+1,s_dwin_settinglist.ucAddressIP[1]);		
						// 				break;
						// case 0x5012: 	s_dwin_settinglist.ucAddressIP[2]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_IPADDRESS+2,s_dwin_settinglist.ucAddressIP[2]);
						// 				break;
						// case 0x5013: 	s_dwin_settinglist.ucAddressIP[3]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_IPADDRESS+3,s_dwin_settinglist.ucAddressIP[3]);
						// 				break;
						// case 0x5014: 	s_dwin_settinglist.ucDwinTime[0]=ucRS232_Buf[8];		//设备时间设置
						// 				AT24CXX_WriteOneByte(AT_DWINTIME,s_dwin_settinglist.ucDwinTime[0]);
						// 				break;
						// case 0x5015: 	s_dwin_settinglist.ucDwinTime[1]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_DWINTIME+1,s_dwin_settinglist.ucDwinTime[1]);
						// 				break;
						// case 0x5016: 	s_dwin_settinglist.ucDwinTime[2]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_DWINTIME+2,s_dwin_settinglist.ucDwinTime[2]);
						// 				break;
						// case 0x5017: 	s_dwin_settinglist.ucDwinTime[3]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_DWINTIME+3,s_dwin_settinglist.ucDwinTime[3]);
						// 				break;
						// case 0x5018: 	s_dwin_settinglist.ucDwinTime[4]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_DWINTIME+4,s_dwin_settinglist.ucDwinTime[4]);
						// 				break;
						// case 0x5019: 	s_dwin_settinglist.ucDwinTime[5]=ucRS232_Buf[8];
						// 				AT24CXX_WriteOneByte(AT_DWINTIME+5,s_dwin_settinglist.ucDwinTime[5]);
						// 				break;
						default :		break;		 			
					}
					return 0;				
				}
			}				
	}
	return 1;
}
/**********************************************************************
**函数名称:Dwin_Page_Change
**函数功能:换页指令的构建（封装）
**入口参数:page：页码（ucPageNo）                 
**出口参数:	 无
***********************************************************************/ 
//换页
void Dwin_Page_Change(u8 page)
{
	usAddress=0x0084;
	ucData[0]=0x5A;
	ucData[1]=0x01;
	ucData[2]=0x00;
	ucData[3]=page;
	Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,4);
	RS232_Send_Data(ucDwin_Cmd,8+4); 
}



/**********************************************************************
**函数名称:Dwin_DISPLAY
**函数功能:迪文屏页面刷新，暂不用（待处理）
**入口参数:  无                   
**出口参数:	 无
***********************************************************************/ 
void Dwin_DISPLAY(void)
{	



}
/**********************************************************************
**函数名称:Dwin_Curse
**函数功能:动态曲线构造
**入口参数:cursedata：曲线结构体，8个曲线是数据值
		   datanum：每次发送的曲线数据长度
**出口参数:	 无
***********************************************************************/ 
void Dwin_Curse(s_DwinCurseData cursedata,u8 datanum)
{
	u8 channel;
	u8 *curse_cmd=0;
	u8 i=0,j;
	u8 count=2*datanum;//用于对datanum计数，一个数2字节组成，而数的范围在0~200，所以只需要奇数字节有值就行
	u16 crc16temp;
	curse_cmd=mymalloc(SRAMIN,((count+2)*8+12)*sizeof(u8));
	curse_cmd[0]=0x5A;
	curse_cmd[1]=0xA5;
	curse_cmd[2]=(count+2)*8+9;
	curse_cmd[3]=0x82;
	curse_cmd[4]=0x03;
	curse_cmd[5]=0x10;
	curse_cmd[6]=0x5A;
	curse_cmd[7]=0xA5;
	curse_cmd[8]=0x08;
	curse_cmd[9]=0x00;
	for(channel=0;channel<8;channel++)
	{
		j=0;
		curse_cmd[10+(count+2)*channel]=channel;
		curse_cmd[11+(count+2)*channel]=datanum;		//datanum:0x01~0x6E
		switch(channel)
		{
			case 0: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel0Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 1: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel1Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 2: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel2Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 3: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel3Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 4: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel4Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 5: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel5Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 6: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel6Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			case 7: 
				for(i=0;i<count;i++)
				{
					if(i%2)
						curse_cmd[12+(count+2)*channel+i]=cursedata.ucChannel7Data[j++];
					else
						curse_cmd[12+(count+2)*channel+i]=0x00;
				}
				break;
			default:break;				
		}		
	}
	crc16temp = DwinGet_CRC16(curse_cmd,(count+2)*8+9);
	curse_cmd[10+(count+2)*8] = (u8)(crc16temp>>8 & 0x00FF);
	curse_cmd[11+(count+2)*8] = (u8)(crc16temp & 0x00FF);
	RS232_Send_Data(curse_cmd,(count+2)*8+12);
	MYFREE(SRAMIN,curse_cmd);
}

/**********************************************************************
**函数名称:ClearCurse
**函数功能:	 换页前清空曲线数据，只需将0300~030F的低字（即0301/0303...）写0x0000即可取消显示，
			这里其实只要不显示曲线就行
			只需将0300~030F的低字（即0301/0303...）写0x0000即可取消显示，
			后续再发送曲线指令即可
**入口参数:                     
**出口参数:	 	
***********************************************************************/ 
void ClearCurse(void)
{
	u8 i;
	usAddress=0x0301;
	ucData[0]=0x00;
	ucData[1]=0x00;
	for(i=0;i<8;i++)
	{					
		Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,2);
		RS232_Send_Data(ucDwin_Cmd,8+2);	
		usAddress=usAddress+2;
	}
}
/**********************************************************************
**函数名称: Dwin_DrawRealTimeCurse
**函数功能: 用于实时曲线的绘制，函数较事件曲线绘制不需要中断画曲线标志
**入口参数: datasize:数据大小   src:数据源    
**出口参数: 无	 
***********************************************************************/
void Dwin_DrawRealTimeCurse(u16 datasize,u8 *src)
{
	u16 i;
	for(i=0;i<datasize/10;i++)
	{
		mymemcpy(s_CurseData.ucChannel0Data,src,10);
		mymemcpy(s_CurseData.ucChannel1Data,src,10);
		mymemcpy(s_CurseData.ucChannel2Data,src,10);
		mymemcpy(s_CurseData.ucChannel3Data,src,10);
		mymemcpy(s_CurseData.ucChannel4Data,src,10);
		mymemcpy(s_CurseData.ucChannel5Data,src,10);
		mymemcpy(s_CurseData.ucChannel6Data,src,10);
		mymemcpy(s_CurseData.ucChannel7Data,src,10);
		Dwin_Curse(s_CurseData,10);		
		xEventGroupSetBits(Dwin_EventGroupHandler,CurseIsDrawingFlag);									
		vTaskDelay(40);
		if(i==datasize/10-1)
			break;					//画完了退出
		else src+=10;
	}
}
/**********************************************************************
**函数名称: Dwin_DrawEventCurse
**函数功能: 用于大量数据的曲线传输，使用了延时，会进入其他任务中，注意其造成的不利影响
			若画曲线的过程中发生换页和退出，则需要停止画线并清空去曲线
			之后还要想办法将不同的数据复制，感觉还是用固定数组更好一点，这个在确定协议之后再想吧
**入口参数: datasize:数据大小   src:数据源    len:每次发送长度，固定为10	mode:0,40ms,1,100ms
**出口参数: 无	 
***********************************************************************/
void Dwin_DrawEventCurse(u16 datasize,u8 *src,u8 mode)
{
	EventBits_t EventValue;
	u16 i;
	for(i=0;i<datasize/10;i++)
	{
		EventValue=xEventGroupGetBits(Dwin_EventGroupHandler);	//获取事件组的标志				
		if(EventValue&StopDrawCurseFlag)
		{
			xEventGroupClearBits(Dwin_EventGroupHandler,StopDrawCurseFlag);				
			break;
		}
		mymemcpy(s_CurseData.ucChannel0Data,src,10);
		mymemcpy(s_CurseData.ucChannel1Data,src,10);
		mymemcpy(s_CurseData.ucChannel2Data,src,10);
		mymemcpy(s_CurseData.ucChannel3Data,src,10);
		mymemcpy(s_CurseData.ucChannel4Data,src,10);
		mymemcpy(s_CurseData.ucChannel5Data,src,10);
		mymemcpy(s_CurseData.ucChannel6Data,src,10);
		mymemcpy(s_CurseData.ucChannel7Data,src,10);
		Dwin_Curse(s_CurseData,10);		
		xEventGroupSetBits(Dwin_EventGroupHandler,CurseIsDrawingFlag);
		if(mode)
			vTaskDelay(100);											
		else
			vTaskDelay(40);
		if(i==datasize/10-1)
		{
			xEventGroupClearBits(Dwin_EventGroupHandler,CurseIsDrawingFlag);		
			break;					//画完了退出
		}
		else src+=10;

	}

}
/**********************************************************************
**函数名称: Dwin_DrawLine
**函数功能: 用于在绘图板上画线，同类型还有画点、矩形、圆，后续
			会把颜色作为形参，以及尝试加入同时绘制多条直线（弃用）
**入口参数: xy偏移量（最大为150）                  
**出口参数: 无	 
***********************************************************************/ 
void Dwin_DrawLine(u16 addr,DWIN_COLOR color,signed char xs,signed char ys)
{
	usAddress=addr;
	ucData[0]=0x00;
	ucData[1]=0x02;//画线指令
	ucData[2]=0x00;
	ucData[3]=0x01;//一条线
	ucData[4]=(u8)(color>>8&0x00FF);
	ucData[5]=(u8)(color&0x00FF);
	ucData[6]=0x04;
	ucData[7]=0x28;
	ucData[8]=0x00;
	ucData[9]=0xB2;
	ucData[10]=(u8)((0x0428+xs)>>8&0x00FF);
	ucData[11]=(u8)((0x0428+xs)&0x00FF);
	ucData[12]=(u8)((0x00B2-ys)>>8&0x00FF);
	ucData[13]=(u8)((0x00B2-ys)&0x00FF);
	Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,14);
	RS232_Send_Data(ucDwin_Cmd,8+14); 
}

/**********************************************************************
**函数名称: Dwin_FlashDataSave
**函数功能: 用于数据在flash中的存储，使用FATFS文件管理系统，存储格式为.bin文件
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/ 
u8 Dwin_FlashDataSave(u8 *buf,u8 Type,u8 No)
{							    
	FIL *ffdwin=0;
	u8 rval=0;
	u8 *fpath=0; 
	u8 res;
	u16 br;    

	ffdwin=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	fpath=mymalloc(SRAMIN,100);
	if(ffdwin==NULL||fpath==NULL)rval=1;
	else
	{ 
		sprintf((char *)fpath,"1:FAULTVALUE/%02dNO.%02d.bin",Type,No);
 		res=f_open(ffdwin,(const TCHAR*)fpath,FA_READ| FA_WRITE| FA_CREATE_ALWAYS); 
 		if(res)rval=2;//打开文件失败  
		else 
		{
			f_lseek(ffdwin,0);
			f_write(ffdwin,buf,DWIN_BUFFER_LEN,(UINT*)&br);
			while(br<DWIN_BUFFER_LEN);
			f_close(ffdwin);
		}
	}
	MYFREE(SRAMIN,ffdwin);
	MYFREE(SRAMIN,fpath);

	return rval;
}
/**********************************************************************
**函数名称: Dwin_FlashDataRead
**函数功能: 用于事件界面，每个事件时间和故障类型等信息的显示，并配有换页功能
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/ 
u8 Dwin_FlashDataRead(u8 *buf,u8 Type,u8 No)
{							    
	FIL *ffdwin=0;
	u8 rval=0;
	u8 *fpath=0; 
	u8 res;
	u16 br;   	
	ffdwin=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	fpath=mymalloc(SRAMIN,100);
	if(ffdwin==NULL||fpath==NULL)rval=1;
	else
	{
		sprintf((char *)fpath,"1:FAULTVALUE/%02dNO.%02d.bin",Type,No);
		res=f_open(ffdwin,(const TCHAR*)fpath,FA_READ); 
		if(res)rval=2;//打开文件失败  
		else 
		{
			f_read(ffdwin,buf,DWIN_BUFFER_LEN,(UINT*)&br);
			while(br<DWIN_BUFFER_LEN);
			f_close(ffdwin);
		}
	}
	MYFREE(SRAMIN,ffdwin);
	MYFREE(SRAMIN,fpath);

	return rval;
}
/**********************************************************************
**函数名称: Dwin_FlashDataErase
**函数功能: 当文件以及接近60个（超过50），在确保文件以及存储在U盘的情况下删除前40个数据，
		   并且把页码更新
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/ 
void Dwin_FlashDataErase()
{



}
/**********************************************************************
**函数名称: Dwin_DataManager
**函数功能: 数据管理，用于数据的串口接收后的复制，写入flash，页码先设定1~60（60页）
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/ 
void Dwin_DataManager(void)
{
	s_DwinEventList s_dwin_event;
	u32 offset=0;
	u8 flasherr;
	BaseType_t err;
	u8 i;	
	if(DEBUG_BinarySemaphore!=NULL)
	{		
		err=xSemaphoreTake(DEBUG_BinarySemaphore,0);	//获取信号量,有故障数据接收到，此时判断
		if(err==pdTRUE)										//获取信号量成功
		{	
			u8 ucEventCount;				//事件计数，这个在系统关机或复位时会被初始化，为了避免计数错误，需要将计数值存在24c02中，每次读取			

			ucEventCount=AT24CXX_ReadOneByte(AT_EVENTCOUNT);
			if(ucEventCount==0)	ucEventCount=1;			//至今无故障发生或者当前容纳的故障数满，计数被清零
			else if(ucEventCount==240)
			{
				ucEventCount=1;			//最大计数为240，当到240时，复位计数，并给予标志（4个轮回）
				//计数复位标志
			}
			else ucEventCount+=1;		//计数加1
			AT24CXX_WriteOneByte(AT_EVENTCOUNT,ucEventCount);
			
			/**********事件序号为0~59***********/
			ucEventNo=AT24CXX_ReadOneByte(AT_EVENTNO);		//读出上次事件序号（0~59）
			ucEventNo++;
			if(ucEventNo>59)ucEventNo=0;

			
			//本次事件序号					
			s_dwin_event.DwinEventNo=ucEventNo;
			//事件时间信息
			HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BCD);
			HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BCD);
			//时钟信息
			s_dwin_event.s_EventTime.EventTime_Year=RTC_DateStruct.Year;
			s_dwin_event.s_EventTime.EventTime_Month=RTC_DateStruct.Month;
			s_dwin_event.s_EventTime.EventTime_Day=RTC_DateStruct.Date;
			s_dwin_event.s_EventTime.EventTime_Hour=RTC_TimeStruct.Hours;
			s_dwin_event.s_EventTime.EventTime_Minute=RTC_TimeStruct.Minutes;
			s_dwin_event.s_EventTime.EventTime_Second=RTC_TimeStruct.Seconds;
			//事件故障类似


			//以下存储数据，分配内存
			for(i=0; i<8;i++)
				Dwin_FaultData[i]=mymalloc(SRAMIN,DWIN_BUFFER_LEN);			//这个指针似乎在其他位置还没用上
			//判断一下是什么数据，电压、电流、还是零序，这里先直接赋值了
			for(i=0;i<8;i++)
			{
				mymemcpy(Dwin_FaultData[i],ucDebug_RX_BUF,DWIN_BUFFER_LEN);	
				flasherr=Dwin_FlashDataSave(Dwin_FaultData[i],i,ucEventNo);
				if(flasherr!=0)
				{	
					//这里给个写入失败标志	

					break;
				}
			}		
			//查询事件记录界面，故障记录是否已满，满了，则清掉此页显示，重新从第一个显示，若不满，则放在相应位置显示。

			// s_dwin_event.ucFaultType=
			//这部分分析故障类型，将其显示在屏幕内


			//将结构体内容存在spi-falsh中
			usFaultFlashOffset=ucEventNo*4096;
			W25QXX_Write((u8 *)&s_dwin_event,DWININFOADDR+usFaultFlashOffset,sizeof(s_DwinEventList));
			AT24CXX_WriteOneByte(AT_EVENTNO,ucEventNo);
			//页码通过计数来显示来得出
			ucDebug_RX_CNT=0;//串口接收缓冲区清零	
			memset(ucDebug_RX_BUF,0,sizeof(ucDebug_RX_BUF));
			for(i=0; i<8;i++)
				MYFREE(SRAMIN,Dwin_FaultData[i]);
		}
	}
}
/**********************************************************************
**函数名称:  Dwin_FaultEventDisplay
**函数功能: 用于事件信息显示，包括故障时间、故障类型的显示，此部分之后会加上翻页键，以将全部信息可以显示出来
			page:0~9/10~19/20~29/30~39/40~49/50~59
			有6页，每页10条，是从1~6
**入口参数: page：事件页码             
**出口参数: 暂无	 
***********************************************************************/
void Dwin_FaultEventDisplay(void)
{
	int8_t i,j;
	u8 index;
	u8 temp;
	// EventBits_t EventValue;
	s_DwinEventList dwineventlist[10];

	//若无故障数据，则清屏显示
	if(AT24CXX_ReadOneByte(AT_EVENTCOUNT)==0)
	{
		usAddress=0x5100;
		for(i=0;i<10;i++)
		{
			for(j=0;j<7;j++)
					ucData[j]=0x00;					
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
			RS232_Send_Data(ucDwin_Cmd,	8+7);	
			usAddress+=0x0010;
		}
	}
	else
	{	
		if(ucFaultPageNo<=ucEventNoDiv+1)
		{
			
			if(ucFaultPageNo==ucEventNoDiv+1)				//处在最新事件页
			{
				index=0;
				for(i=ucEventNoRem;i>=0;i--)	
				{
					usFaultFlashOffset=(ucEventNoDiv*10+i)*4096;			//取得
					W25QXX_Read((u8 *)&dwineventlist[index],DWININFOADDR+usFaultFlashOffset,sizeof(s_DwinEventList));
					index++;
				}
				for(i=ucEventNoRem;i>=0;i--)
				{
					switch(i)
					{
						case 0: usAddress=0x5100;break;
						case 1: usAddress=0x5110;break;
						case 2: usAddress=0x5120;break;
						case 3: usAddress=0x5130;break;
						case 4: usAddress=0x5140;break;
						case 5: usAddress=0x5150;break;
						case 6: usAddress=0x5160;break;
						case 7: usAddress=0x5170;break;
						case 8: usAddress=0x5180;break;
						case 9: usAddress=0x5190;break;
						default: break;
					}
					ucData[0]=0x20;
					ucData[1]=dwineventlist[i].s_EventTime.EventTime_Year;
					ucData[2]=dwineventlist[i].s_EventTime.EventTime_Month;
					ucData[3]=dwineventlist[i].s_EventTime.EventTime_Day;
					ucData[4]=dwineventlist[i].s_EventTime.EventTime_Hour;
					ucData[5]=dwineventlist[i].s_EventTime.EventTime_Minute;
					ucData[6]=dwineventlist[i].s_EventTime.EventTime_Second;
					Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
					RS232_Send_Data(ucDwin_Cmd,8+7);
				}
				for(i=9;i>ucEventNoRem;i--)
				{
					switch(i)
					{
						case 0: usAddress=0x5100;break;
						case 1: usAddress=0x5110;break;
						case 2: usAddress=0x5120;break;
						case 3: usAddress=0x5130;break;
						case 4: usAddress=0x5140;break;
						case 5: usAddress=0x5150;break;
						case 6: usAddress=0x5160;break;
						case 7: usAddress=0x5170;break;
						case 8: usAddress=0x5180;break;
						case 9: usAddress=0x5190;break;
						default: break;
					}	
						for(j=0;j<7;j++)
							ucData[j]=0x00;					
						Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
						RS232_Send_Data(ucDwin_Cmd,	8+7);	
				}
			}
			else if(ucFaultPageNo<ucEventNoDiv+1)
			{
				for(i=9;i>=0;i--)	
				{
					usFaultFlashOffset=((ucFaultPageNo-1)*10+i)*4096;
					W25QXX_Read((u8 *)&dwineventlist[i],DWININFOADDR+usFaultFlashOffset,sizeof(s_DwinEventList));
				}
				for(i=9;i>=0;i--)
				{
					switch(i)
					{
						case 0: usAddress=0x5100;break;
						case 1: usAddress=0x5110;break;
						case 2: usAddress=0x5120;break;
						case 3: usAddress=0x5130;break;
						case 4: usAddress=0x5140;break;
						case 5: usAddress=0x5150;break;
						case 6: usAddress=0x5160;break;
						case 7: usAddress=0x5170;break;
						case 8: usAddress=0x5180;break;
						case 9: usAddress=0x5190;break;
						default: break;
					}
					ucData[0]=0x20;
					ucData[1]=dwineventlist[i].s_EventTime.EventTime_Year;
					ucData[2]=dwineventlist[i].s_EventTime.EventTime_Month;
					ucData[3]=dwineventlist[i].s_EventTime.EventTime_Day;
					ucData[4]=dwineventlist[i].s_EventTime.EventTime_Hour;
					ucData[5]=dwineventlist[i].s_EventTime.EventTime_Minute;
					ucData[6]=dwineventlist[i].s_EventTime.EventTime_Second;
					Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
					RS232_Send_Data(ucDwin_Cmd,8+7);
				}
			}
		}
		else if(ucFaultPageNo>ucEventNoDiv+1)		//若大于故障事件总数，则清零
		{
			usAddress=0x5100;
			for(i=0;i<10;i++)
			{
				for(j=0;j<7;j++)
						ucData[j]=0x00;					
				Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
				RS232_Send_Data(ucDwin_Cmd,	8+7);	
				usAddress+=0x0010;
			}
		}
	}
	usAddress=0x504F;
	ucData[0]=0x00;
	ucData[1]=ucFaultPageNo;
	Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,2);
	RS232_Send_Data(ucDwin_Cmd,8+2);	
}

/**********************************************************************
**函数名称: Dwin_CheckPage
**函数功能: 用于查询当前页面号,返回5A A5 06 83 00 14 01 00 01
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/
void Dwin_CheckPage(void)
{
	usAddress=0x0014;
	ucData[0]=0x01;
	Dwin_CmdCreate(usAddress,ucData,DWIN_READ,1);
	RS232_Send_Data(ucDwin_Cmd,8+1);
}

/**********************************************************************
**函数名称: Dwin_Pag5_Display
**函数功能: 页面5的包装程序，有诸多限制。
			1、页面5和6进入的条件是发生了故障，在页面7如果未发生故障的事件条是无法进入的（需要加上限制条件）
			2、需要根据页码和条例来确定是哪个数据
			3、能够在切换时先停止加载曲线——清空——切换
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/
void Dwin_Page5_Display(void)
{
	u8 *buf;
	u8 *Usart1Buf;
	u8 index;
	u16 i,k=0;
	index=ucFaultRecordingNo+(ucFaultPageNo-1)*10;
	buf=mymalloc(SRAMIN,DWIN_BUFFER_LEN*sizeof(u8));
	Usart1Buf=mymalloc(SRAMIN,800*sizeof(u8));
	if(buf!=NULL&&Usart1Buf!=NULL)
	{
		Dwin_FlashDataRead(buf,index);
		k=0;
		for(i=0;i<800;i++)
		{
			Usart1Buf[i]=buf[k];
			if(i%2)k=k+7;
			else k=k+8;
		}
		Dwin_DrawEventCurse(800,Usart1Buf,1);
	}
		MYFREE(SRAMIN,Usart1Buf);
		MYFREE(SRAMIN,buf);
}


/**********************************************************************
**函数名称: Dwin_Pag6_Display
**函数功能: 页面6的包装程序，仅需要
**入口参数: 无             
**出口参数: 无	 
***********************************************************************/
void Dwin_Page6_Display(void)
{
	u8 *buf;
	u8 *BigBuf;
	u8 index;
	u16 i,inc,k;
	index=ucFaultRecordingNo+(ucFaultPageNo-1)*10;
	buf=mymalloc(SRAMIN,DWIN_BUFFER_LEN*sizeof(u8));
	BigBuf=mymalloc(SRAMIN,1200*sizeof(u8));
	if(BigBuf!=NULL&&BigBuf!=NULL)
	{

		Dwin_FlashDataRead(buf,index);
		k=0;
		for(i=0;i<1200;i++)
		{
			switch(ucZoomInNo)
			{
				case 1:	inc=0;break;
				case 2:	inc=1200;break;
				case 3:	inc=2400;break;
				case 4:	inc=3600;break;
				case 5:	inc=4800;break;
				default:break;
			}
			BigBuf[i]=buf[inc+(k++)];
		}
			usAddress=0x5048;
			for(i=0;i<6;i++)
				ucData[i]=ucDwin_Cycle_Text[ucZoomInNo-1][i];
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,6);
			RS232_Send_Data(ucDwin_Cmd,8+6); 

			Dwin_DrawEventCurse(1200,BigBuf,0);
	}

	MYFREE(SRAMIN,BigBuf);		
	MYFREE(SRAMIN,buf);

}



 