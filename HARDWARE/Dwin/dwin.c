#include "dwin.h"
#include "string.h"
/*************Dwin����ش���***************/


/*ȫ�ֱ�������*/
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
/*�洢�������ݵ�·��*/
u8 const* Fault_PATH={"/FAULTVALUE/1.bin"}


u8 ucDwin_Cmd[CMDMAXLEN];		//Diwn������ָ�ͨ��Create���������鲢����
u8 ucDwin_RX_Buf[11];			//Dwin���������ݳ���Ϊ11.
u8 ucPageNo=0;					//��ǰҳ��
u8 ucZoomInNo=0;					//�Ŵ�����
u8 ucZoomChange=0;				//�Ŵ�����ҳ
u16 usAddress=0;					//��ַ�����ݸ�ucDwin_Cmd
u8 ucData[28]={0};					//���ݣ�ͬ�����ݸ�ucDwin_Cmd
u8 ucFaultRecordingNo=0;		//ѡ�й����¼�����
s_DwinSettingList s_dwin_settinglist;
s_DwinCurseData s_CurseData;
s_DwinEventList s_dwin_event;
/****��������*****/

/**********************************************************************
**��������:DwinGet_CRC16
**��������:����16λCRCУ���룬���ڵ���������У��
**��ڲ���:puchMsg - ���ݴ�ŵ�����(֡ͷ ֡ͷ ���͵����ݳ��� ���� ���� ���� ���� ... CRCH CRCL)
           usDataLen - ���͵����ݳ���(��ָ�ʼ����󣬰���CRCH CRCL)
**          
**���ڲ���:	 crc16У����
***********************************************************************/ 
unsigned int DwinGet_CRC16(unsigned char* puchMsg,unsigned char usDataLen)
{
	u8 uchCRCHi = 0xFF ;	  //��CRC�ֽڳ�ʼ��
	u8 uchCRCLo = 0xFF ; //��CRC�ֽڳ�ʼ��
	u16  uIndex  ;	//CRCѭ���е����� 
	u8 len=0;
	u8 k=3;	 //���ݴ�����ĵ���λ��ʼ��ǰ��λ��֡ͷ������λ�����ݳ��ȣ���Ҫ����CRC�����ݴӵ���λ��ʼ�����
	len = usDataLen-2; //usDataLen��ֵ������CRC��ֵ������CRCֵ����������Ϊ usDataLen-2
	while (len--)		//������Ϣ������ 
	{
		uIndex = uchCRCHi ^ puchMsg[k++];	  //����CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (((unsigned int)(uchCRCHi) << 8) | uchCRCLo) ;
	//puchMsg[usDataLen + 1] = uchCRCHi; // puchMsg����ĵ����ڶ�λ���CRC���ֽڣ�usDataLen+֡ͷ�����ݳ�����ռ��3���ֽ� - 2
	//puchMsg[usDataLen + 2] = uchCRCLo; // puchMsg����ĵ�����һ��λ���CRC���ֽڣ�usDataLen+֡ͷ�����ݳ�����ռ��3���ֽ� - 1
}

/**********************************************************************
**��������:Dwin_CmdCreate
**��������:����Dwin��ָ�232ͨ�ŷ���ָ�
**��ڲ���:addr����ַ
		   *data��ָ�����ݵ�ָ�룬��ʱ�Ƕ�ȡ���ȣ�дʱ��д������
		   RWflag:��д��־
		   datanum�����ݳ���
**���ڲ���:	 ��
***********************************************************************/ 
//�˺�������ָ��
//��ʱ��dataΪ������дʱ��Ϊ����
//datanumΪdata���������ֽ�Ϊ��λ�����Ƕ�����Ϊ1�ֽ�
void Dwin_CmdCreate(u16 addr,u8 *data,DWIN_RW_FLAG RWflag,u16 datanum)
{
	u8 i;
	u16 crc16temp;
	ucDwin_Cmd[0]=0x5A;
	ucDwin_Cmd[1]=0xA5;			//����֡ͷ
	ucDwin_Cmd[3]=RWflag;		//flagΪ83,����Ϊ82,д
	ucDwin_Cmd[4]=(u8)(addr>>8 & 0x00FF);
	ucDwin_Cmd[5]=(u8)(addr & 0x00FF);
	if(RWflag == DWIN_READ)		//��
	{
		ucDwin_Cmd[6]=data[0];  	//��ȡһ���ֽ�����
		ucDwin_Cmd[2]=0x07;	//��һ���ֽ�ָ���Ϊ9-2�����ﲻ������
		crc16temp = DwinGet_CRC16(ucDwin_Cmd,ucDwin_Cmd[2]);
		ucDwin_Cmd[7] = (u8)(crc16temp>>8 & 0x00FF);
		ucDwin_Cmd[8] = (u8)(crc16temp & 0x00FF);
	}
	else if(RWflag==DWIN_WRITE)		//д
	{
		for(i=0;i<datanum;i++)
		{
			ucDwin_Cmd[6+i]=data[i];	
		}
		ucDwin_Cmd[2]=datanum+5;						//ָ���
		crc16temp = DwinGet_CRC16(ucDwin_Cmd,ucDwin_Cmd[2]);
		ucDwin_Cmd[datanum+6] = (u8)(crc16temp>>8 & 0x00FF);
		ucDwin_Cmd[datanum+7] = (u8)(crc16temp & 0x00FF);
	}
}

/**********************************************************************
**��������:Dwin_RxDeal
**��������:������յ���ָ�����Ǹ��¼���־λ
**��ڲ���:buf��ָ����յ������ݣ���ΪucRS232_BUF                   
**���ڲ���:0������ɹ���1������
***********************************************************************/ 
u8 Dwin_RxDeal(void)
{
	u16 crc16tem;
	u8 ucdatalen;	
	u16 usaddr;
	ucdatalen = ucRS232_Buf[2];
	usaddr = (u16)(ucRS232_Buf[4]<<8|ucRS232_Buf[5]);
	crc16tem = (u16)(ucRS232_Buf[9]<<8| ucRS232_Buf[10]);
	if(ucRS232_Buf[0]==0x5A && ucRS232_Buf[1]==0xA5)
	{		if(ucDwin_RX_Cnt==ucdatalen+3)
			{		
				if(crc16tem==DwinGet_CRC16(ucRS232_Buf,ucdatalen))//�ж�crc�����Ƿ���ȷ
				{				
					switch(usaddr)
					{
						/*ҳ���л�*/
						case 0x5000: 	ucPageNo=ucRS232_Buf[8];		//0x5000Ϊҳ���л�
										if(Dwin_EventGroupHandler!=NULL)
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ClearScreenFlag);//����ҳ���л�
										break;
						/*����ҳ��������*/
						case 0x500B: 	s_dwin_settinglist.ucCTratio=ucRS232_Buf[8];		//CT���
										break;
						case 0x500C: 	s_dwin_settinglist.ucCTpercent=ucRS232_Buf[8];		//CT΢��ָ��						
										break;
						case 0x500D: 	s_dwin_settinglist.ucPTratio=ucRS232_Buf[8];		//PT���
										break;
						case 0x500E: 	s_dwin_settinglist.ucPTpercent=ucRS232_Buf[8];		//PT΢��ָ��
										break;
						case 0x500F: 	s_dwin_settinglist.ucAddress485=ucRS232_Buf[8];		//485ͨ�ŵ�ַ
										break;
						case 0x5010: 	s_dwin_settinglist.ucAddressIP[0]=ucRS232_Buf[8];		//IPͨ�ŵ�ַ
										break;
						case 0x5011: 	s_dwin_settinglist.ucAddressIP[1]=ucRS232_Buf[8];		
										break;
						case 0x5012: 	s_dwin_settinglist.ucAddressIP[2]=ucRS232_Buf[8];
										break;
						case 0x5013: 	s_dwin_settinglist.ucAddressIP[3]=ucRS232_Buf[8];
										break;
						case 0x5014: 	s_dwin_settinglist.ucDwinTime[0]=ucRS232_Buf[8];		//�豸ʱ������
										break;
						case 0x5015: 	s_dwin_settinglist.ucDwinTime[1]=ucRS232_Buf[8];
										break;
						case 0x5016: 	s_dwin_settinglist.ucDwinTime[2]=ucRS232_Buf[8];
										break;
						case 0x5017: 	s_dwin_settinglist.ucDwinTime[3]=ucRS232_Buf[8];
										break;
						case 0x5018: 	s_dwin_settinglist.ucDwinTime[4]=ucRS232_Buf[8];
										break;
						case 0x5019: 	s_dwin_settinglist.ucDwinTime[5]=ucRS232_Buf[8];
										break;
						/*ҳ��3��5��6���¼���¼�Ȱ���*/				
						case 0x501A:    ucPageNo=6;			//�Ŵ����ʱ�Ȼ�����5ҳ����ȷ���Ŵ�����
										ucZoomInNo=ucRS232_Buf[8];
										if(Dwin_EventGroupHandler!=NULL)										
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ZoomInFlag|ClearScreenFlag);//��Ҫ�Ŵ�ҳ���ҷ���ҳ���л�										
										break;
						case 0x501D:	ucZoomChange=ucRS232_Buf[8];	//��û�ҳ��־����Ϊ1�����Ϸ�ҳ����Ϊ2�������·�ҳ
										if(ucZoomChange==0x01)
											xEventGroupSetBits(Dwin_EventGroupHandler,ZoomPageUpFlag);									
										else if(ucZoomChange==0x02)
											xEventGroupSetBits(Dwin_EventGroupHandler,ZoomPageDownFlag);
										break;		
						case 0x501E:	ucPageNo=5;			//�˳��Ŵ����,ͨ���˳���־�����¼��ظ��Բ���ͼ
										if(Dwin_EventGroupHandler!=NULL)										
											xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|ClearScreenFlag|QUITFLAG);//����ҳ���л�										
										break;
						case 0x501F:	ucFaultRecordingNo=ucRS232_Buf[8];
										ucPageNo=5;
										xEventGroupSetBits(Dwin_EventGroupHandler,ChangePageFlag|FaultPageChangeFlag);//����ҳ���л�
										break;				
						case 0x501B: 	if(Dwin_EventGroupHandler!=NULL)	//��ӡ���ϲ���							
											xEventGroupSetBits(Dwin_EventGroupHandler,ToPrintFlag);	
										break;
						case 0x501C:	if(Dwin_EventGroupHandler!=NULL)	//���ع��ϲ���								
											xEventGroupSetBits(Dwin_EventGroupHandler,ToDownloadFlag);																				
										break;
						/*������֤*/
						case 0x519A:	if(((u16)ucRS232_Buf[7]<<8|ucRS232_Buf[8])==0x1A0A)			//������֤
										{
											ucPageNo=2;
											if(Dwin_EventGroupHandler!=NULL)										
												xEventGroupSetBits(Dwin_EventGroupHandler,PassWordRightFlag|ChangePageFlag);//����ҳ���л�																					
										}
										break;
						default :		break;		 			
					}
					return 0;				
				}
			}				
	}
	return 1;
}
/**********************************************************************
**��������:Dwin_Page_Change
**��������:��ҳָ��Ĺ�������װ��
**��ڲ���:page��ҳ�루ucPageNo��                 
**���ڲ���:	 ��
***********************************************************************/ 
//��ҳ
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
**��������:Dwin_DISPLAY
**��������:������ҳ��ˢ�£�������
**��ڲ���:  ��                   
**���ڲ���:	 ��
***********************************************************************/ 
void Dwin_DISPLAY(void)
{
	u8 data[24];
	switch(ucPageNo)
	{
		case 0x01:
			usAddress=0x5002;
			ucData[0]=0xAA;
			ucData[1]=0xBB;
			ucData[2]=0xCC;
			ucData[3]=0xDD;
			Dwin_CmdCreate(usAddress,data,DWIN_WRITE,4);
			RS232_Send_Data(ucDwin_Cmd,8+4);
			break;
		default :break;
	}
	
	
}
/**********************************************************************
**��������:Dwin_Curve
**��������:��̬���߹���
**��ڲ���:cursedata�����߽ṹ�壬8������������ֵ
		   datanum��ÿ�η��͵��������ݳ���
**���ڲ���:	 ��
***********************************************************************/ 
void Dwin_Curve(s_DwinCurseData cursedata,u8 datanum)
{
	u8 channel;
	u8 *curse_cmd=0;
	u8 i=0,j;
	u8 count=2*datanum;//���ڶ�datanum������һ����2�ֽ���ɣ������ķ�Χ��0~200������ֻ��Ҫ�����ֽ���ֵ����
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
**��������:ClearCurse
**��������:��ҳǰ����������ݣ�ֻ�轫0300~030F�ĵ��֣���0301/0303...��д0x0000����ȡ����ʾ��
**��ڲ���:                     
**���ڲ���:	 
***********************************************************************/ 
//��ҳǰ����������ݣ���Ϊ0���һ��ֱ�ߣ�
//������ʵֻҪ����ʾ���߾���
//ֻ�轫0300~030F�ĵ��֣���0301/0303...��д0x0000����ȡ����ʾ��
//�����ٷ�������ָ���
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
**��������: Dwin_DrawCurse
**��������: ���ڴ������ݵ����ߴ���
**��ڲ���: datasize:���ݴ�С   src:����Դ    len:ÿ�η��ͳ��ȣ��ɹ̶�Ϊ10
**���ڲ���: ��	 
***********************************************************************/
void Dwin_DrawCurse(u16 datasize,u8 *src,u8 len)
{
	u16 i;
	for(i=0;i<datasize/len;i++)
	{
		mymemcpy(s_CurseData.ucChannel0Data,src,len);
		mymemcpy(s_CurseData.ucChannel1Data,src,len);
		mymemcpy(s_CurseData.ucChannel2Data,src,len);
		mymemcpy(s_CurseData.ucChannel3Data,src,len);
		mymemcpy(s_CurseData.ucChannel4Data,src,len);
		mymemcpy(s_CurseData.ucChannel5Data,src,len);
		mymemcpy(s_CurseData.ucChannel6Data,src,len);
		mymemcpy(s_CurseData.ucChannel7Data,src,len);
		Dwin_Curve(s_CurseData,len);
		vTaskDelay(40);
		if(i==datasize/len-1)break;
		else src+=len;
	}
}
/**********************************************************************
**��������: Dwin_DrawLine
**��������: �����ڻ�ͼ���ϻ��ߣ�ͬ���ͻ��л��㡢���Ρ�Բ������
			�����ɫ��Ϊ�βΣ��Լ����Լ���ͬʱ���ƶ���ֱ��
**��ڲ���: xyƫ���������Ϊ150��                  
**���ڲ���: ��	 
***********************************************************************/ 
void Dwin_DrawLine(u16 addr,DWIN_COLOR color,signed char xs,signed char ys)
{
	usAddress=addr;
	ucData[0]=0x00;
	ucData[1]=0x02;//����ָ��
	ucData[2]=0x00;
	ucData[3]=0x01;//һ����
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
**��������: Dwin_EventPageShow
**��������: �����¼����棬ÿ���¼�ʱ��͹������͵���Ϣ����ʾ�������л�ҳ����
**��ڲ���: ��             
**���ڲ���: ��	 
***********************************************************************/ 
void Dwin_DataManager()
{							    
	FIL * ffdwin=0;
	u8 rval=0;
	u8 *fpath=0;      
	ffdwin=(FIL*)mymalloc(SRAMIN,sizeof(FIL));	//�����ڴ�	
	if(ffdwin==NULL)rval=1;
	fpath=mymalloc(SRAMIN,40);
	strcpy((char *)fpath,(char *)src);
	strcat((char *)fpath,(char *));
	if(tempbuf==NULL)rval=1;
 	res=f_open(ffdwin,(const TCHAR*)fxpath,FA_READ); 
 	if(res)rval=2;//���ļ�ʧ��  
	


}
void Dwin_EventDeal(void)
{
	//��ѯ�¼���¼���棬���ϼ�¼�Ƿ����������ˣ��������ҳ��ʾ�����´ӵ�һ����ʾ�����������������Ӧλ����ʾ��
	static int EventCount=0;
	BaseType_t err;
	u8 i;
	if(DEBUG_BinarySemaphore!=NULL)
	{
		err=xSemaphoreTake(DEBUG_BinarySemaphore,1000);	//��ȡ�ź���,�й������ݽ��յ�����ʱ�ж�
		if(err==pdTRUE)										//��ȡ�ź����ɹ�
		{	
			mymemcpy(s_dwin_event.Dwin_FaultData,ucDebug_RX_BUF,DWIN_BUFFER_LEN);
			s_dwin_event.DwinEventNo=++EventCount;
			if(EventCount>20)EventCount=0;
			HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BCD);
			HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BCD);
			s_dwin_event.s_EventTime.EventTime_Year=RTC_DateStruct.Year;
			s_dwin_event.s_EventTime.EventTime_Month=RTC_DateStruct.Month;
			s_dwin_event.s_EventTime.EventTime_Day=RTC_DateStruct.Date;
			s_dwin_event.s_EventTime.EventTime_Hour=RTC_TimeStruct.Hours;
			s_dwin_event.s_EventTime.EventTime_Minute=RTC_TimeStruct.Minutes;
			s_dwin_event.s_EventTime.EventTime_Second=RTC_TimeStruct.Seconds;
			// s_dwin_event.ucFaultType=
			//�ⲿ�ַ����������ͣ�������ʾ����Ļ��
			usAddress=0x5020;
			ucData[0]=0x20;
			ucData[1]=s_dwin_event.s_EventTime.EventTime_Year;
			ucData[2]=s_dwin_event.s_EventTime.EventTime_Month;
			ucData[3]=s_dwin_event.s_EventTime.EventTime_Day;
			ucData[4]=s_dwin_event.s_EventTime.EventTime_Hour;
			ucData[5]=s_dwin_event.s_EventTime.EventTime_Minute;
			ucData[6]=s_dwin_event.s_EventTime.EventTime_Second;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,7);
			RS232_Send_Data(ucDwin_Cmd,8+7);
			vTaskDelay(40);
			usAddress=0x5050;
			ucData[0]=EventCount;
			Dwin_CmdCreate(usAddress,ucData,DWIN_WRITE,1);
			RS232_Send_Data(ucDwin_Cmd,8+1);
			vTaskDelay(40);		
		}
	}
}


