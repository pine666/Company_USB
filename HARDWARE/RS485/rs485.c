#include "rs485.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//RS485��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

UART_HandleTypeDef USART_RS485Handler;  	//USART2���(����RS485)
DMA_HandleTypeDef  RS485TxDMA_Handler;      //DMA���
DMA_HandleTypeDef  RS485RxDMA_Handler;
SemaphoreHandle_t  RS485_BinarySemaphore;	//��ֵ�ź������,���ڽ��ճɹ���־
  	  
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
u8 RS485_RX_CNT=0;  	//���յ������ݳ���
//Data Acquisition(DAQ)�ɼ��ź�
//�ɼ����ݽ��ա�����
u8 ucRS485_Buf[DAQ_RX_SIZE];		//���ڸ��ƽ��գ��ⲿ�����������
u8 ucDAQ_RX_Cnt; 					//�Ʋɼ����յ�������


void USART2_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	RS485_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_RS485Handler);
	if(__HAL_UART_GET_FLAG(&USART_RS485Handler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_RS485Handler);  //�����־λ
		SCB_DisableDCache(); //���ڴ�Cache��Ҫ�ڶ�ȡDMAǰ�ȹر�
		HAL_UART_DMAStop(&USART_RS485Handler); 		    //��ֹͣDMA����ͣ����    
		temp  =  __HAL_DMA_GET_COUNTER(&RS485RxDMA_Handler);// ��ȡDMAδ�������                     
		RS485_RX_CNT =  RS485_BUFFERSIZE - temp; //������ȥδ����������õ��ѽ������ݵĸ���
		if(RS485_RX_CNT>3)//�յ����������ص����ݶ���11λ����ʱ���֣�
		{			
			if(RS485_BinarySemaphore!=NULL)
			{
				//�ͷŶ�ֵ�ź���
				xSemaphoreGiveFromISR(RS485_BinarySemaphore,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
			}
			RS485_memcpy(ucRS485_Buf,RS485_RX_BUF,RS485_RX_CNT);
			ucDAQ_RX_Cnt=RS485_RX_CNT;
			RS485_RX_CNT=0;
		}
		SCB_EnableDCache();//���´�D-cache
		RS485_TX_Set(0);			//����Ϊ����ģʽ			
		HAL_UART_Receive_DMA(&USART_RS485Handler,RS485_RX_BUF,RS485_BUFFERSIZE);//���´�DMA���� 		
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
//��ʼ��IO ����2
//bound:������
void RS485_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;	
    PCF8574_Init();                         //��ʼ��PCF8574�����ڿ���RE��	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	__HAL_RCC_DMA1_CLK_ENABLE();//DMA2ʱ��ʹ��
	
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PA2,3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2,3
    
    //USART ��ʼ������
	USART_RS485Handler.Instance=USART2;			        //USART2
	USART_RS485Handler.Init.BaudRate=bound;		        //������
	USART_RS485Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_RS485Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART_RS485Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	USART_RS485Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART_RS485Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&USART_RS485Handler);			        //HAL_UART_Init()��ʹ��USART2
    
    __HAL_UART_CLEAR_IT(&USART_RS485Handler,UART_CLEAR_TCF);
	__HAL_UART_ENABLE_IT(&USART_RS485Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //ʹ��USART1�ж�
	HAL_NVIC_SetPriority(USART2_IRQn,5,0);			        //��ռ���ȼ�3�������ȼ�3
	
	//DMA��ʼ��	
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	//Tx DMA����
	RS485TxDMA_Handler.Instance=DMA1_Stream4;                            //������ѡ��
	RS485TxDMA_Handler.Init.Request=DMA_REQUEST_USART2_TX;				//USART3����DMA
	RS485TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
	RS485TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	RS485TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	RS485TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	RS485TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	RS485TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	RS485TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	RS485TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS485TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS485TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	RS485TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&RS485TxDMA_Handler);   
	HAL_DMA_Init(&RS485TxDMA_Handler);
	__HAL_LINKDMA(&USART_RS485Handler,hdmatx,RS485TxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)
	
	//Rx DMA����
	RS485RxDMA_Handler.Instance=DMA1_Stream5;                            //������ѡ��
	RS485RxDMA_Handler.Init.Request=DMA_REQUEST_USART2_RX;				//USART3����DMA
	RS485RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
	RS485RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	RS485RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	RS485RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	RS485RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	RS485RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	RS485RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	RS485RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	RS485RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	RS485RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	RS485RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&RS485RxDMA_Handler);   
	HAL_DMA_Init(&RS485RxDMA_Handler);
	__HAL_LINKDMA(&USART_RS485Handler,hdmarx,RS485RxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)
    RS485_TX_Set(0);                                        //����Ϊ����ģʽ	
	HAL_UART_Receive_DMA(&USART_RS485Handler,RS485_RX_BUF,RS485_BUFFERSIZE);	//��DMA���գ�ָ�����ջ������ͽ��մ�С

}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u32 time=0;
	u32 maxDelay=0x1FFFF;
	RS485_TX_Set(1);			//����Ϊ����ģʽ
	while(HAL_UART_Transmit_DMA(&USART_RS485Handler, buf,len)!= HAL_OK);
	while(__HAL_UART_GET_FLAG(&USART_RS485Handler,UART_FLAG_TC)==RESET)
	{
		time++;
		if(time>maxDelay)break;
	}
	__HAL_UART_CLEAR_FLAG(&USART_RS485Handler,UART_FLAG_TC); 
	RS485_TX_Set(0);			//����Ϊ����ģʽ	
}

//RS485ģʽ����.
//en:0,����;1,����.
void RS485_TX_Set(u8 en)
{
	PCF8574_WriteBit(RS485_RE_IO,en);
}
