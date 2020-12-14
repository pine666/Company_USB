#include "usart.h"


////////////////////////////////////////////////////////////////////////////////// 
//串口1 用于调试（debug）
//
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_CNT=0;       //接收数据量

u8 ucDebug_RX_BUF[USART_REC_LEN];
u16 ucDebug_RX_CNT=0;

UART_HandleTypeDef USART_DEBUGHandler;  //USART2句柄(用于RS232)
DMA_HandleTypeDef  DEBUGTxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  DEBUGRxDMA_Handler;
SemaphoreHandle_t DEBUG_BinarySemaphore;	//二值信号量句柄,用于接收成功标志


//直接在串口中断中判断个数是否正确

void USART1_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	USART_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_DEBUGHandler);
	if(__HAL_UART_GET_FLAG(&USART_DEBUGHandler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_DEBUGHandler);  //清除标志位
		SCB_DisableDCache(); //对于带Cache需要在读取DMA前先关闭
		HAL_UART_DMAStop(&USART_DEBUGHandler); 		    //先停止DMA，暂停接收    
		temp  =  __HAL_DMA_GET_COUNTER(&DEBUGRxDMA_Handler);// 获取DMA未传输个数                     
		USART_RX_CNT =  USART_REC_LEN - temp; //总数减去未传输个数，得到已接收数据的个数
		if(USART_RX_CNT>3)//收到迪文屏返回的数据都是11位（暂时发现）
		{			
			if(DEBUG_BinarySemaphore!=NULL)
			{
				//释放二值信号量
				xSemaphoreGiveFromISR(DEBUG_BinarySemaphore,&xHigherPriorityTaskWoken);	//释放二值信号量
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
			}
			memcpy(ucDebug_RX_BUF,USART_RX_BUF,USART_RX_CNT);
			ucDebug_RX_CNT=USART_RX_CNT;
			USART_RX_CNT=0;
		}
		SCB_EnableDCache();//重新打开D-cache		
		HAL_UART_Receive_DMA(&USART_DEBUGHandler,USART_RX_BUF,USART_REC_LEN);//重新打开DMA接收 		
	}


}
#endif
//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{	
	    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOB时钟
	__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	


    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; //PB10、PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9、10
	
	//UART 初始化设置
	USART_DEBUGHandler.Instance=USART1;					    //USART1
	USART_DEBUGHandler.Init.BaudRate=bound;				    //波特率
	USART_DEBUGHandler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	USART_DEBUGHandler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	USART_DEBUGHandler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	USART_DEBUGHandler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	USART_DEBUGHandler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&USART_DEBUGHandler);					    //HAL_UART_Init()会使能UART1


    __HAL_UART_CLEAR_IT(&USART_DEBUGHandler,UART_CLEAR_TCF);

//	__HAL_UART_ENABLE_IT(&USART2_RS232Handler,UART_IT_RXNE);//开启接收中断
	__HAL_UART_ENABLE_IT(&USART_DEBUGHandler,UART_IT_IDLE);//开启空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&USART_DEBUGHandler);//清空闲标志位

	HAL_NVIC_EnableIRQ(USART1_IRQn);				        //使能USART3中断
	HAL_NVIC_SetPriority(USART1_IRQn,5,0);			        //抢占优先级5
	//DMA初始化
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	
	
	//Tx DMA配置
	DEBUGTxDMA_Handler.Instance=DMA2_Stream4;                            //数据流选择
	DEBUGTxDMA_Handler.Init.Request=DMA_REQUEST_USART1_TX;				//USART3发送DMA
	DEBUGTxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
	DEBUGTxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	DEBUGTxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	DEBUGTxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	DEBUGTxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	DEBUGTxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	DEBUGTxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	DEBUGTxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	DEBUGTxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	DEBUGTxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	DEBUGTxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&DEBUGTxDMA_Handler);   
	HAL_DMA_Init(&DEBUGTxDMA_Handler);
	__HAL_LINKDMA(&USART_DEBUGHandler,hdmatx,DEBUGTxDMA_Handler);    //将DMA与USART3联系起来(发送DMA)
	
	//Rx DMA配置
	DEBUGRxDMA_Handler.Instance=DMA2_Stream5;                            //数据流选择
	DEBUGRxDMA_Handler.Init.Request=DMA_REQUEST_USART1_RX;				//USART3接收DMA
	DEBUGRxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //外设到存储器
	DEBUGRxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	DEBUGRxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	DEBUGRxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	DEBUGRxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	DEBUGRxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	DEBUGRxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	DEBUGRxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	DEBUGRxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	DEBUGRxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	DEBUGRxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&DEBUGRxDMA_Handler);   
	HAL_DMA_Init(&DEBUGRxDMA_Handler);
	__HAL_LINKDMA(&USART_DEBUGHandler,hdmarx,DEBUGRxDMA_Handler);    //将DMA与USART3联系起来(接收DMA)

  	HAL_UART_Receive_DMA(&USART_DEBUGHandler,USART_RX_BUF,USART_REC_LEN);	//打开DMA接收，指定接收缓存区和接收大小

}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&DEBUGTxDMA_Handler);
}

void DMA2_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&DEBUGRxDMA_Handler);
}


//RS232发送len个字节.
//buf:发送区首地址
//len:发送的字节数
void DEBUG_Send_Data(u8 *buf,u16 len)
{
	u32 time=0;
	u32 maxDelay=0x1FFFF;
	while(HAL_UART_Transmit_DMA(&USART_DEBUGHandler, buf,len)!= HAL_OK);
	while(__HAL_UART_GET_FLAG(&USART_DEBUGHandler,UART_FLAG_TC)==RESET)
	{
		time++;
		if(time>maxDelay)break;
	}
	__HAL_UART_CLEAR_FLAG(&USART_DEBUGHandler,UART_FLAG_TC);
	
}
//RS232查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void DEBUG_Receive_Data(u8 *buf,u16 len)
{
	HAL_UART_Receive_DMA(&USART_DEBUGHandler,buf,len);
} 


