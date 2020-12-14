#include "rs232.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ͨ�ź����ݽ���
//1��RS-232ͨ������
//2��������crcУ��
//3��������ָ���
//4��������ָ��ķ��Ͳ�����
////////////////////////////////////////////////////////////////////////////////// 	
UART_HandleTypeDef USART_RS232Handler;  //USART2���(����RS232)
DMA_HandleTypeDef  RS232TxDMA_Handler;      //DMA���
DMA_HandleTypeDef  RS232RxDMA_Handler;
SemaphoreHandle_t RS232_BinarySemaphore;	//��ֵ�ź������,���ڽ��ճɹ���־


#if EN_USART2_RX   		//���ʹ���˽��� 
//��2����������232ͨ�ţ��ⲿ����Ϊʹ��
//���ջ����� 	
u8 RS232_RX_BUF[BUFFER_SIZE];  	//���ջ���,���64���ֽ�.
u8 RS232_RX_CNT=0;				//���������յ������ݸ���

//Dwin���������ݡ�����
u8 ucRS232_Buf[DWIN_RX_SIZE];		//���ڸ��ƽ��գ��ⲿ�����������
u8 ucDwin_RX_Cnt; 					//�Ƶ��������յ�������


//ֱ���ڴ����ж����жϸ����Ƿ���ȷ

void USART3_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	RS232_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_RS232Handler);
	if(__HAL_UART_GET_FLAG(&USART_RS232Handler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_RS232Handler);  //�����־λ
		SCB_DisableDCache(); //���ڴ�Cache��Ҫ�ڶ�ȡDMAǰ�ȹر�
		HAL_UART_DMAStop(&USART_RS232Handler); 		    //��ֹͣDMA����ͣ����    
		temp  =  __HAL_DMA_GET_COUNTER(&RS232RxDMA_Handler);// ��ȡDMAδ�������                     
		RS232_RX_CNT =  BUFFER_SIZE - temp; //������ȥδ����������õ��ѽ������ݵĸ���
		if(RS232_RX_CNT>3)//�յ����������ص����ݶ���11λ����ʱ���֣�
		{			
			if(RS232_BinarySemaphore!=NULL)
			{
				//�ͷŶ�ֵ�ź���
				xSemaphoreGiveFromISR(RS232_BinarySemaphore,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
			}
			RS232_memcpy(ucRS232_Buf,RS232_RX_BUF,RS232_RX_CNT);
			ucDwin_RX_Cnt=RS232_RX_CNT;
			RS232_RX_CNT=0;
		}
		SCB_EnableDCache();//���´�D-cache		
		HAL_UART_Receive_DMA(&USART_RS232Handler,RS232_RX_BUF,BUFFER_SIZE);//���´�DMA���� 		
	}


}
#endif

//��ʼ��DMA
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7

void MYDMA_Config(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
	
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	
	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
	//Tx DMA����
	RS232TxDMA_Handler.Instance=DMA2_Stream7;                            //������ѡ��
	RS232TxDMA_Handler.Init.Request=DMA_REQUEST_USART3_TX;				//USART3����DMA
	RS232TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
	RS232TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	RS232TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	RS232TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	RS232TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	RS232TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	RS232TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	RS232TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS232TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS232TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	RS232TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&RS232TxDMA_Handler);   
	HAL_DMA_Init(&RS232TxDMA_Handler);
	__HAL_LINKDMA(&USART_RS232Handler,hdmatx,RS232TxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)
	
	//Rx DMA����
	RS232RxDMA_Handler.Instance=DMA2_Stream6;                            //������ѡ��
	RS232RxDMA_Handler.Init.Request=DMA_REQUEST_USART3_RX;				//USART3����DMA
	RS232RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
	RS232RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	RS232RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	RS232RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	RS232RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	RS232RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	RS232RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	RS232RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS232RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS232RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	RS232RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&RS232RxDMA_Handler);   
	HAL_DMA_Init(&RS232RxDMA_Handler);
	__HAL_LINKDMA(&USART_RS232Handler,hdmarx,RS232RxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)
} 
//��ʼ��IO ����2
//bound:������
void RS232_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;	
	__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
	__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART2ʱ��

    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11; //PB10��PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART3
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PA2��3
	
    //USART ��ʼ������
	USART_RS232Handler.Instance=USART3;			        //USART3
	USART_RS232Handler.Init.BaudRate=bound;		        //������
	USART_RS232Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_RS232Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART_RS232Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	USART_RS232Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART_RS232Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&USART_RS232Handler);			        //HAL_UART_Init()��ʹ��USART3
    MYDMA_Config();
    __HAL_UART_CLEAR_IT(&USART_RS232Handler,UART_CLEAR_TCF);
#if EN_USART2_RX
//	__HAL_UART_ENABLE_IT(&USART2_RS232Handler,UART_IT_RXNE);//���������ж�
	__HAL_UART_ENABLE_IT(&USART_RS232Handler,UART_IT_IDLE);//���������ж�
	__HAL_UART_CLEAR_IDLEFLAG(&USART_RS232Handler);//����б�־λ

	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //ʹ��USART3�ж�
	HAL_NVIC_SetPriority(USART3_IRQn,5,0);			        //��ռ���ȼ�5
#endif	

	HAL_UART_Receive_DMA(&USART_RS232Handler,RS232_RX_BUF,BUFFER_SIZE);	//��DMA���գ�ָ�����ջ������ͽ��մ�С
}





void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS232TxDMA_Handler);
}

void DMA2_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&RS232RxDMA_Handler);
}


//RS232����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���
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
//RS232��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS232_Receive_Data(u8 *buf,u16 len)
{
	HAL_UART_Receive_DMA(&USART_RS232Handler,buf,len);
} 



