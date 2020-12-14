#include "notepad.h"
//////////////////////////////////////////////////////////////////////////////////	 
u8 Text[]={"THIS IS TEST VALUE!!"};							 
////////////////////////////////////////////////////////////////////////////////// 	   
//ͨ��ʱ���ȡ�ļ���
//��U���б���,��֧��flash disk����
//��ϳ�:����"3:TEXT/TEXT20120321210633.txt"/"2:TEXT/TEXT20120321210633.txt"���ļ��� 
USBH_HandleTypeDef hUSBHost;
static void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;
        case HOST_USER_DISCONNECTION:
            f_mount(0,"3:",1); 	//ж��U��
            break;
        case HOST_USER_CLASS_ACTIVE:
            f_mount(fs[3],"3:",1); 	//���¹���U��
			LED0_Toggle;			//LED��˵��USB���سɹ�
			break;
        case HOST_USER_CONNECTION:
            break;
        default:
            break;
    }
}

void USB_USER_Init(void)
{
	USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
    USBH_Start(&hUSBHost);
    HAL_PWREx_EnableUSBVoltageDetector();


}
//���±�����
//�����½��ı��ļ�,��������SD����TEXT�ļ�����
//ÿ��д6000�����ݣ��ӿ�ͷд�����λ��Ϊ0
//���أ�0���ɹ���1��ʧ��
extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
u8 noteWrite(u8 *buf,u16 len)
{
	FIL* f_txt=0;					//�ļ�
	u32 total,free;	
 	u8 *pname=0; 
	u8 res;
	u8 rval=0;						//��Ǹ�API����ֵ��Ϊ0ʱ��APIִ������
	u16 br;
	
	//��ѡ��ģʽ 
	f_txt=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
  	if(f_txt==NULL)		rval=1;
	else				//f_txt����OK
	{	  	 
   		//�½��ı��ļ�
		f_mkdir("3:TEXT");		//ǿ�ƴ����ļ���,�����±���
		pname=mymalloc(SRAMIN,40);//����40���ֽ��ڴ�,����"0:TEXT/TEXT20120321210633.txt"
		if(pname==NULL)rval=1;
		pname[0]='\0';//��ӽ�����
        HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
		sprintf((char*)pname,"3:TEXT/20%02d%02d%02d%02d%02d%02d.txt",
							RTC_DateStruct.Year,
							RTC_DateStruct.Month,
							RTC_DateStruct.Date,
							RTC_TimeStruct.Hours,
							RTC_TimeStruct.Minutes,
							RTC_TimeStruct.Seconds);	//������U��	
		f_txt->obj.objsize=0;
		if(rval==0)	//�ļ��������ɹ�
		{
			res=exf_getfree("3:",&total,&free);
			if(res==0)
			{
				if(((f_txt->obj.objsize+NOTEPAD_EDIT_LEN)/1024)>free)//�趨���ɵ�text�ļ������2KB��ʣ���ڴ������ʱ�ᱨ��
				{	
					res=1;
					return res;
				}
				else
				{
					res=f_open(f_txt,(const TCHAR*)pname,FA_READ|FA_WRITE|FA_CREATE_NEW);//�����µ��ļ�		
					if(res==0)//�򿪳ɹ�
					{
						f_lseek(f_txt,0);		//ָ���ļ��Ŀ�ʼ��ַ,�ӵ�0���ֽڿ�ʼд,
						f_write(f_txt,buf,len,(UINT*)&br);	//�����ļ�
						while(br<len);
						f_close(f_txt);
					}
				}
			}
		}
		MYFREE(SRAMIN,pname);  
	}
	MYFREE(SRAMIN,f_txt);


	return rval;

}








