#include "usart.h"


////////////////////////////////////////////////////////////////////////////////// 
//����1 ���ڵ��ԣ�debug��
//
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_CNT=0;       //����������

u8 ucDebug_RX_BUF[USART_REC_LEN];
u16 ucDebug_RX_CNT=0;

UART_HandleTypeDef USART_DEBUGHandler;  //USART2���(����RS232)
DMA_HandleTypeDef  DEBUGTxDMA_Handler;      //DMA���
DMA_HandleTypeDef  DEBUGRxDMA_Handler;
SemaphoreHandle_t DEBUG_BinarySemaphore;	//��ֵ�ź������,���ڽ��ճɹ���־


//ֱ���ڴ����ж����жϸ����Ƿ���ȷ

void USART1_IRQHandler(void)
{
	u32 temp;
	BaseType_t xHigherPriorityTaskWoken;
	USART_RX_CNT=0;
	HAL_UART_IRQHandler(&USART_DEBUGHandler);
	if(__HAL_UART_GET_FLAG(&USART_DEBUGHandler,UART_FLAG_IDLE)!=RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_DEBUGHandler);  //�����־λ
		SCB_DisableDCache(); //���ڴ�Cache��Ҫ�ڶ�ȡDMAǰ�ȹر�
		HAL_UART_DMAStop(&USART_DEBUGHandler); 		    //��ֹͣDMA����ͣ����    
		temp  =  __HAL_DMA_GET_COUNTER(&DEBUGRxDMA_Handler);// ��ȡDMAδ�������                     
		USART_RX_CNT =  USART_REC_LEN - temp; //������ȥδ����������õ��ѽ������ݵĸ���
		if(USART_RX_CNT>3)//�յ����������ص����ݶ���11λ����ʱ���֣�
		{			
			if(DEBUG_BinarySemaphore!=NULL)
			{
				//�ͷŶ�ֵ�ź���
				xSemaphoreGiveFromISR(DEBUG_BinarySemaphore,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
			}
			memcpy(ucDebug_RX_BUF,USART_RX_BUF,USART_RX_CNT);
			ucDebug_RX_CNT=USART_RX_CNT;
			USART_RX_CNT=0;
		}
		SCB_EnableDCache();//���´�D-cache		
		HAL_UART_Receive_DMA(&USART_DEBUGHandler,USART_RX_BUF,USART_REC_LEN);//���´�DMA���� 		
	}


}
#endif
//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOBʱ��
	__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	


    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; //PB10��PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9��10
	
	//UART ��ʼ������
	USART_DEBUGHandler.Instance=USART1;					    //USART1
	USART_DEBUGHandler.Init.BaudRate=bound;				    //������
	USART_DEBUGHandler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_DEBUGHandler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	USART_DEBUGHandler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	USART_DEBUGHandler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	USART_DEBUGHandler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&USART_DEBUGHandler);					    //HAL_UART_Init()��ʹ��UART1


    __HAL_UART_CLEAR_IT(&USART_DEBUGHandler,UART_CLEAR_TCF);

//	__HAL_UART_ENABLE_IT(&USART2_RS232Handler,UART_IT_RXNE);//���������ж�
	__HAL_UART_ENABLE_IT(&USART_DEBUGHandler,UART_IT_IDLE);//���������ж�
	__HAL_UART_CLEAR_IDLEFLAG(&USART_DEBUGHandler);//����б�־λ

	HAL_NVIC_EnableIRQ(USART1_IRQn);				        //ʹ��USART3�ж�
	HAL_NVIC_SetPriority(USART1_IRQn,5,0);			        //��ռ���ȼ�5
	//DMA��ʼ��
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,8,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn,7,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	
	
	//Tx DMA����
	DEBUGTxDMA_Handler.Instance=DMA2_Stream4;                            //������ѡ��
	DEBUGTxDMA_Handler.Init.Request=DMA_REQUEST_USART1_TX;				//USART3����DMA
	DEBUGTxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
	DEBUGTxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	DEBUGTxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	DEBUGTxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	DEBUGTxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	DEBUGTxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	DEBUGTxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	DEBUGTxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	DEBUGTxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	DEBUGTxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	DEBUGTxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&DEBUGTxDMA_Handler);   
	HAL_DMA_Init(&DEBUGTxDMA_Handler);
	__HAL_LINKDMA(&USART_DEBUGHandler,hdmatx,DEBUGTxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)
	
	//Rx DMA����
	DEBUGRxDMA_Handler.Instance=DMA2_Stream5;                            //������ѡ��
	DEBUGRxDMA_Handler.Init.Request=DMA_REQUEST_USART1_RX;				//USART3����DMA
	DEBUGRxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
	DEBUGRxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
	DEBUGRxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
	DEBUGRxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	DEBUGRxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	DEBUGRxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
	DEBUGRxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	DEBUGRxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
	DEBUGRxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
	DEBUGRxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	DEBUGRxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	HAL_DMA_DeInit(&DEBUGRxDMA_Handler);   
	HAL_DMA_Init(&DEBUGRxDMA_Handler);
	__HAL_LINKDMA(&USART_DEBUGHandler,hdmarx,DEBUGRxDMA_Handler);    //��DMA��USART3��ϵ����(����DMA)

  	HAL_UART_Receive_DMA(&USART_DEBUGHandler,USART_RX_BUF,USART_REC_LEN);	//��DMA���գ�ָ�����ջ������ͽ��մ�С

}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&DEBUGTxDMA_Handler);
}

void DMA2_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&DEBUGRxDMA_Handler);
}


//RS232����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���
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
//RS232��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void DEBUG_Receive_Data(u8 *buf,u16 len)
{
	HAL_UART_Receive_DMA(&USART_DEBUGHandler,buf,len);
} 


