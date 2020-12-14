#include "rs232.h"
//////////////////////////////////////////////////////////////////////////////////	 
//迪文屏通信和数据解析
//1、RS-232通信设置
//2、迪文屏crc校验
//3、迪文屏指令构建
//4、迪文屏指令的发送并接收
////////////////////////////////////////////////////////////////////////////////// 	
UART_HandleTypeDef USART_RS232Handler;  //USART2句柄(用于RS232)
DMA_HandleTypeDef  RS232TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  RS232RxDMA_Handler;
SemaphoreHandle_t RS232_BinarySemaphore;	//二值信号量句柄,用于接收成功标志


#if EN_USART2_RX   		//如果使能了接收 
//这2个参数用于232通信，外部不作为使用
//接收缓存区 	
u8 RS232_RX_BUF[BUFFER_SIZE];  	//接收缓冲,最大64个字节.
u8 RS232_RX_CNT=0;				//计数，接收到的数据个数

//Dwin屏接收数据、个数
u8 ucRS232_Buf[DWIN_RX_SIZE];		//用于复制接收，外部处理接收数据
u8 ucDwin_RX_Cnt; 					//计迪文屏接收到的数据


//直接在串口中断中判断个数是否正确

void USART3_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	RS232_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_RS232Handler);
	if(__HAL_UART_GET_FLAG(&USART_RS232Handler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_RS232Handler);  //清除标志位
		SCB_DisableDCache(); //对于带Cache需要在读取DMA前先关闭
		HAL_UART_DMAStop(&USART_RS232Handler); 		    //先停止DMA，暂停接收    
		temp  =  __HAL_DMA_GET_COUNTER(&RS232RxDMA_Handler);// 获取DMA未传输个数                     
		RS232_RX_CNT =  BUFFER_SIZE - temp; //总数减去未传输个数，得到已接收数据的个数
		if(RS232_RX_CNT>3)//收到迪文屏返回的数据都是11位（暂时发现）
		{			
			if(RS232_BinarySemaphore!=NULL)
			{
				//释放二值信号量
				xSemaphoreGiveFromISR(RS232_BinarySemaphore,&xHigherPriorityTaskWoken);	//释放二值信号量
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
			}
			RS232_memcpy(ucRS232_Buf,RS232_RX_BUF,RS232_RX_CNT);
			ucDwin_RX_Cnt=RS232_RX_CNT;
			RS232_RX_CNT=0;
		}
		SCB_EnableDCache();//重新打开D-cache		
		HAL_UART_Receive_DMA(&USART_RS232Handler,RS232_RX_BUF,BUFFER_SIZE);//重新打开DMA接收 		
	}


}
#endif

//初始化DMA
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7

void MYDMA_Config(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
	
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	
	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
	//Tx DMA配置
	RS232TxDMA_Handler.Instance=DMA2_Stream7;                            //数据流选择
	RS232TxDMA_Handler.Init.Request=DMA_REQUEST_USART3_TX;				//USART3发送DMA
	RS232TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
	RS232TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	RS232TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	RS232TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	RS232TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	RS232TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	RS232TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	RS232TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS232TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS232TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	RS232TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&RS232TxDMA_Handler);   
	HAL_DMA_Init(&RS232TxDMA_Handler);
	__HAL_LINKDMA(&USART_RS232Handler,hdmatx,RS232TxDMA_Handler);    //将DMA与USART3联系起来(发送DMA)
	
	//Rx DMA配置
	RS232RxDMA_Handler.Instance=DMA2_Stream6;                            //数据流选择
	RS232RxDMA_Handler.Init.Request=DMA_REQUEST_USART3_RX;				//USART3接收DMA
	RS232RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //外设到存储器
	RS232RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
	RS232RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
	RS232RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	RS232RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	RS232RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
	RS232RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
	RS232RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS232RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS232RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
	RS232RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
	HAL_DMA_DeInit(&RS232RxDMA_Handler);   
	HAL_DMA_Init(&RS232RxDMA_Handler);
	__HAL_LINKDMA(&USART_RS232Handler,hdmarx,RS232RxDMA_Handler);    //将DMA与USART3联系起来(接收DMA)
} 
//初始化IO 串口2
//bound:波特率
void RS232_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;	
	__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
	__HAL_RCC_USART3_CLK_ENABLE();			//使能USART2时钟

    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11; //PB10、PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA2、3
	
    //USART 初始化设置
	USART_RS232Handler.Instance=USART3;			        //USART3
	USART_RS232Handler.Init.BaudRate=bound;		        //波特率
	USART_RS232Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	USART_RS232Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	USART_RS232Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	USART_RS232Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART_RS232Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&USART_RS232Handler);			        //HAL_UART_Init()会使能USART3
    MYDMA_Config();
    __HAL_UART_CLEAR_IT(&USART_RS232Handler,UART_CLEAR_TCF);
#if EN_USART2_RX
//	__HAL_UART_ENABLE_IT(&USART2_RS232Handler,UART_IT_RXNE);//开启接收中断
	__HAL_UART_ENABLE_IT(&USART_RS232Handler,UART_IT_IDLE);//开启空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&USART_RS232Handler);//清空闲标志位

	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //使能USART3中断
	HAL_NVIC_SetPriority(USART3_IRQn,5,0);			        //抢占优先级5
#endif	

	HAL_UART_Receive_DMA(&USART_RS232Handler,RS232_RX_BUF,BUFFER_SIZE);	//打开DMA接收，指定接收缓存区和接收大小
}





void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS232TxDMA_Handler);
}

void DMA2_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS232RxDMA_Handler);
}


//RS232发送len个字节.
//buf:发送区首地址
//len:发送的字节数
void RS232_Send_Data(u8 *buf,u16 len)
{
	u32 time=0;
	u32 maxDelay=0x4FFFF;
	while(HAL_UART_Transmit_DMA(&USART_RS232Handler, buf,len)!= HAL_OK);
	while(__HAL_UART_GET_FLAG(&USART_RS232Handler,UART_FLAG_TC)==RESET)
	{
		time++;
		if(time>maxDelay)break;
	}
	__HAL_UART_CLEAR_FLAG(&USART_RS232Handler,UART_FLAG_TC);
	
}
//RS232查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS232_Receive_Data(u8 *buf,u16 len)
{
	HAL_UART_Receive_DMA(&USART_RS232Handler,buf,len);
} 



