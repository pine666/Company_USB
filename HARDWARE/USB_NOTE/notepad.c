#include "notepad.h"
//////////////////////////////////////////////////////////////////////////////////	 
u8 Text[]={"THIS IS TEST VALUE!!"};							 
////////////////////////////////////////////////////////////////////////////////// 	   
//通过时间获取文件名
//在U盘中保存,不支持flash disk保存
//组合成:形如"3:TEXT/TEXT20120321210633.txt"/"2:TEXT/TEXT20120321210633.txt"的文件名 
USBH_HandleTypeDef hUSBHost;
static void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;
        case HOST_USER_DISCONNECTION:
            f_mount(0,"3:",1); 	//卸载U盘
            break;
        case HOST_USER_CLASS_ACTIVE:
            f_mount(fs[3],"3:",1); 	//重新挂载U盘
			LED0_Toggle;			//LED亮说明USB加载成功
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
//记事本功能
//所有新建文本文件,都保存在SD卡的TEXT文件夹内
//每次写6000个数据，从开头写，因此位置为0
//返回：0，成功；1，失败
extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
u8 noteWrite(u8 *buf,u16 len)
{
	FIL* f_txt=0;					//文件
	u32 total,free;	
 	u8 *pname=0; 
	u8 res;
	u8 rval=0;						//标记个API返回值，为0时，API执行正常
	u16 br;
	
	//先选择模式 
	f_txt=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区域 
  	if(f_txt==NULL)		rval=1;
	else				//f_txt创建OK
	{	  	 
   		//新建文本文件
		f_mkdir("3:TEXT");		//强制创建文件夹,给记事本用
		pname=mymalloc(SRAMIN,40);//申请40个字节内存,类似"0:TEXT/TEXT20120321210633.txt"
		if(pname==NULL)rval=1;
		pname[0]='\0';//添加结束符
        HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
		sprintf((char*)pname,"3:TEXT/20%02d%02d%02d%02d%02d%02d.txt",
							RTC_DateStruct.Year,
							RTC_DateStruct.Month,
							RTC_DateStruct.Date,
							RTC_TimeStruct.Hours,
							RTC_TimeStruct.Minutes,
							RTC_TimeStruct.Seconds);	//保存在U盘	
		f_txt->obj.objsize=0;
		if(rval==0)	//文件名创建成功
		{
			res=exf_getfree("3:",&total,&free);
			if(res==0)
			{
				if(((f_txt->obj.objsize+NOTEPAD_EDIT_LEN)/1024)>free)//设定生成的text文件最大是2KB，剩余内存大于它时会报错
				{	
					res=1;
					return res;
				}
				else
				{
					res=f_open(f_txt,(const TCHAR*)pname,FA_READ|FA_WRITE|FA_CREATE_NEW);//创建新的文件		
					if(res==0)//打开成功
					{
						f_lseek(f_txt,0);		//指向文件的开始地址,从第0个字节开始写,
						f_write(f_txt,buf,len,(UINT*)&br);	//保存文件
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








