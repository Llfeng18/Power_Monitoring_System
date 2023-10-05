/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "rx_data_queue.h"
#include "bsp_485.h"
#include "bsp_AdvanceTim.h" 

extern uint32_t TimeDisplay;
extern uint32_t time_clock;
//uint8_t USART_ReceiveString[100];													//接收PC端发送过来的字符
uint8_t Receive_Flag = 0;															//接收消息标志位
extern volatile uint32_t distance_time;
//uint16_t Receive_sum = 0;															//数组下标
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

// 串口中断服务函数


// 串口中断服务函数
// 把接收到的数据写入缓冲区，在main函数中轮询缓冲区输出数据
void HPLC_USART_IRQHandler(void)
{	
	uint8_t ucCh;
	QUEUE_DATA_TYPE *data_p; 
	
	if(USART_GetITStatus(HPLC_USART1,USART_IT_RXNE)!=RESET)
	{	
		ucCh  = USART_ReceiveData( HPLC_USART1 );
//		USART_SendData(HPLC_USART1,ucCh);  
						/*获取写缓冲区指针，准备写入新数据*/
		data_p = cbWrite(&rx_queue); 
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		

			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->head + data_p->len) = ucCh;
				
			if( ++data_p->len >= QUEUE_NODE_DATA_LEN)
			{
				cbWriteFinish(&rx_queue);
			}
		}else return;	
	}
	
	if ( USART_GetITStatus( HPLC_USART1, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
			/*写入缓冲区完毕*/
			cbWriteFinish(&rx_queue);
		  ucCh = USART_ReceiveData( HPLC_USART1 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)

	}
}

void RS485_IRQHandler(void)
{
    bsp_RS485_IRQHandler();

}


// /*串口中断服务函数*/
//void DEBUG_USART_IRQHandler(void)
//{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USART3,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(DEBUG_USART3);
//    USART_SendData(DEBUG_USART3,ucTemp);    
//	}	 
//}



void DEBUG_USART_IRQHandler(void)
{	
	uint8_t ucCh;
	QUEUE_DATA_TYPE *data_p; 
	
	if(USART_GetITStatus(DEBUG_USART3,USART_IT_RXNE)!=RESET)
	{	
		ucCh  = USART_ReceiveData( DEBUG_USART3 );
//		USART_SendData(DEBUG_USART3,ucCh); 
		USART_SendData(DEBUG_USART3,ucCh);  
						/*获取写缓冲区指针，准备写入新数据*/
		data_p = cbWrite(&rx_queue_debug); 
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		

			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->head + data_p->len) = ucCh;
				
			if( ++data_p->len >= QUEUE_NODE_DATA_LEN)
			{
				cbWriteFinish(&rx_queue_debug);
			}
		}else return;	
	}
//	if((ucCh== '\r') && (ucCh == '\n') ){
//	if(ucCh == '\n'){
//		Receive_Flag = 1;
//	}
	
	if ( USART_GetITStatus( DEBUG_USART3, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
			/*写入缓冲区完毕*/
			cbWriteFinish(&rx_queue_debug);
			ucCh = USART_ReceiveData( DEBUG_USART3 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)

	}
}
//void USART1_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == 1)							//USART_FLAG_RXNE判断数据，== 1则有数据
//	{
//		if(Receive_sum > 100)													//数组能存放50个字节的数据				
//		{
//			USART_ReceiveString[100] = '\0';										//数据字节超过50位时，将最后一位设置为\0	
//			Receive_Flag = 1;													//接收标志位置1，停止接收数据
//			Receive_sum = 0;													//数组下标置0
//		}
//		
//		if(Receive_Flag == 0)													//接收标志位等于0，开始接收数据
//		{
//			USART_ReceiveString[Receive_sum] = USART_ReceiveData(USART1);		//通过USART1串口接收字符
//			Receive_sum++;														//数组下标++
//		}
//		
//		if(Receive_sum >= 2)													//数组下标大于2
//		{
//			if(USART_ReceiveString[Receive_sum-2] == '\r' && USART_ReceiveString[Receive_sum-1] == '\n' )
//			{
//				USART_ReceiveString[Receive_sum-1] = '\0';						
//				USART_ReceiveString[Receive_sum-2] = '\0';
//				Receive_Flag = 1;												//接收标志位置1，停止接收数据
//				Receive_sum = 0;												//数组下标置0
//				printf("%s\r\n",USART_ReceiveString);					

////				if(strcmp(USART_ReceiveString,"hello") == 0)
////				{
////					PFout(9) = !PFout(9);
////				}
////				if(strcmp(USART_ReceiveString,"world") == 0)
////				{
////					PFout(10) = !PFout(10);
////				}
////				if(strcmp(USART_ReceiveString,"jiajia") == 0)
////				{
////					PEout(13) = !PEout(13);
////				}
////				if(strcmp(USART_ReceiveString,"haha") == 0)
////				{
////					PEout(14) = !PEout(14);
////				}
//			}		
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);							//接收后先清空标志位
//	}
//}

void Pi_USART_IRQHandler(void)
{
	
	uint8_t ucCh;
	QUEUE_DATA_TYPE *data_p; 
	
	if(USART_GetITStatus(Pi_USART4,USART_IT_RXNE)!=RESET)
	{	
		ucCh  = USART_ReceiveData( Pi_USART4 );
//		USART_SendData(DEBUG_USART3,ucCh); 
//		USART_SendData(Pi_USART4,ucCh);  
						/*获取写缓冲区指针，准备写入新数据*/
		data_p = cbWrite(&rx_queue_pi); 
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		

			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->head + data_p->len) = ucCh;
				
			if( ++data_p->len >= QUEUE_NODE_DATA_LEN)
			{
				cbWriteFinish(&rx_queue_pi);
			}
		}else return;	
	}
//	if((ucCh== '\r') && (ucCh == '\n') ){
//	if(ucCh == '\n'){
//		Receive_Flag = 1;
//	}
	
	if ( USART_GetITStatus( Pi_USART4, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
			/*写入缓冲区完毕*/
			cbWriteFinish(&rx_queue_pi);
			ucCh = USART_ReceiveData( Pi_USART4 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)

	}
//  uint8_t ucTemp;
//	if(USART_GetITStatus(Pi_USART4,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(Pi_USART4);
//    USART_SendData(Pi_USART4,ucTemp);    
//	}	 
}


void RTC_IRQHandler(void)
{
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
	
	    /* Enable time update */
	    TimeDisplay = 1;
			time_clock=time_clock+1;
	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
}


void  ADVANCE_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( ADVANCE_TIM, TIM_IT_Update) != RESET ) 
	{	
		distance_time++;
		TIM_ClearITPendingBit(ADVANCE_TIM , TIM_FLAG_Update);  		 
	}		 	
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
