#include "rs485.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//RS485驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

UART_HandleTypeDef USART_RS485Handler;  	//USART2句柄(用于RS485)
DMA_HandleTypeDef  RS485TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  RS485RxDMA_Handler;
SemaphoreHandle_t  RS485_BinarySemaphore;	//二值信号量句柄,用于接收成功标志
  	  
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
u8 RS485_RX_CNT=0;  	//接收到的数据长度
//Data Acquisition(DAQ)采集信号
//采集数据接收、个数
u8 ucRS485_Buf[DAQ_RX_SIZE];		//用于复制接收，外部处理接收数据
u8 ucDAQ_RX_Cnt; 					//计采集接收到的数据


void USART2_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	RS485_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_RS485Handler);
	if(__HAL_UART_GET_FLAG(&USART_RS485Handler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_RS485Handler);  //清除标志位
		SCB_DisableDCache(); //对于带Cache需要在读取DMA前先关闭
		HAL_UART_DMAStop(&USART_RS485Handler); 		    //先停止DMA，暂停接收    
		temp  =  __HAL_DMA_GET_COUNTER(&RS485RxDMA_Handler);// 获取DMA未传输个数                     
		RS485_RX_CNT =  RS485_BUFFERSIZE - temp; //总数减去未传输个数，得到已接收数据的个数
		if(RS485_RX_CNT>3)//收到迪文屏返回的数据都是11位（暂时发现）
		{			
			if(RS485_BinarySemaphore!=NULL)
			{
				//释放二值信号量
				xSemaphoreGiveFromISR(RS485_BinarySemaphore,&xHigherPriorityTaskWoken);	//释放二值信号量
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
			}
			RS485_memcpy(ucRS485_Buf,RS485_RX_BUF,RS485_RX_CNT);
			ucDAQ_RX_Cnt=RS485_RX_CNT;
			RS485_RX_CNT=0;
		}
		SCB_EnableDCache();//重新打开D-cache
		RS485_TX_Set(0);			//设置为接收模式			
		HAL_UART_Receive_DMA(&USART_RS485Handler,RS485_RX_BUF,RS485_BUFFERSIZE);//重新打开DMA接收 		
	}
}    
void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS485TxDMA_Handler);
}

void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS485RxDMA_Handler);
}
//初始化IO 串口2
//bound:波特率
void RS485_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;	
    PCF8574_Init();                         //初始化PCF8574，用于控制RE脚	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	__HAL_RCC_DMA1_CLK_ENABLE();//DMA2时钟使能
	
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PA2,3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2,3
    
    //USART 初始化设置
	USART_RS485Handler.Instance=USART2;			        //USART2
	USART_RS485Handler.Init.BaudRate=bound;		        //波特率
	USART_RS485Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	USART_RS485Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	USART_RS485Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	USART_RS485Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART_RS485Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&USART_RS485Handler);			        //HAL_UART_Init()会使能USART2
    
    __HAL_UART_CLEAR_IT(&USART_RS485Handler,UART_CLEAR_TCF);
	__HAL_UART_ENABLE_IT(&USART_RS485Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //使能USART1中断
	HAL_NVIC_SetPriority(USART2_IRQn,5,0);			        //抢占优先级3，子优先级3
	
	//DMA初始化	
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	//Tx DMA配置
	RS485TxDMA_Handler.Instance=DMA1_Stream4;                            //数据流选择
	RS485TxDMA_Handler.Init.Request=DMA_REQUEST_USART2_TX;				//USART3发送DMA
	RS485TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
	RS485TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	RS485TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	RS485TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	RS485TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	RS485TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	RS485TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	RS485TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS485TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS485TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	RS485TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&RS485TxDMA_Handler);   
	HAL_DMA_Init(&RS485TxDMA_Handler);
	__HAL_LINKDMA(&USART_RS485Handler,hdmatx,RS485TxDMA_Handler);    //将DMA与USART3联系起来(发送DMA)
	
	//Rx DMA配置
	RS485RxDMA_Handler.Instance=DMA1_Stream5;                            //数据流选择
	RS485RxDMA_Handler.Init.Request=DMA_REQUEST_USART2_RX;				//USART3接收DMA
	RS485RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //外设到存储器
	RS485RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	RS485RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	RS485RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	RS485RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	RS485RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	RS485RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	RS485RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS485RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS485RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	RS485RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&RS485RxDMA_Handler);   
	HAL_DMA_Init(&RS485RxDMA_Handler);
	__HAL_LINKDMA(&USART_RS485Handler,hdmarx,RS485RxDMA_Handler);    //将DMA与USART3联系起来(接收DMA)
    RS485_TX_Set(0);                                        //设置为接收模式	
	HAL_UART_Receive_DMA(&USART_RS485Handler,RS485_RX_BUF,RS485_BUFFERSIZE);	//打开DMA接收，指定接收缓存区和接收大小

}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u32 time=0;
	u32 maxDelay=0x1FFFF;
	RS485_TX_Set(1);			//设置为发送模式
	while(HAL_UART_Transmit_DMA(&USART_RS485Handler, buf,len)!= HAL_OK);
	while(__HAL_UART_GET_FLAG(&USART_RS485Handler,UART_FLAG_TC)==RESET)
	{
		time++;
		if(time>maxDelay)break;
	}
	__HAL_UART_CLEAR_FLAG(&USART_RS485Handler,UART_FLAG_TC); 
	RS485_TX_Set(0);			//设置为接收模式	
}

//RS485模式控制.
//en:0,接收;1,发送.
void RS485_TX_Set(u8 en)
{
	PCF8574_WriteBit(RS485_RE_IO,en);
}
