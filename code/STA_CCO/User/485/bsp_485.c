/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   485驱动
  ******************************************************************************
  */ 
#include "bsp_485.h"
#include <stdarg.h>


static void Delay(__IO u32 nCount); 


/// 配置USART接收中断优先级
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RS485_INT_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
}
/*
 * 函数名：RS485_Config
 * 描述  ：USART GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void RS485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART clock */
	RCC_APB2PeriphClockCmd(RS485_USART_RX_GPIO_CLK|RS485_USART_TX_GPIO_CLK|RS485_RE_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RS485_USART_CLK, ENABLE); 	
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = RS485_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_USART_TX_GPIO_PORT, &GPIO_InitStructure);
  	
	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = RS485_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RS485_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
  
  /* 485收发控制管脚 */
	GPIO_InitStructure.GPIO_Pin = RS485_RE_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  GPIO_Init(RS485_RE_GPIO_PORT, &GPIO_InitStructure);
	  
	/* USART 模式配置*/
	USART_InitStructure.USART_BaudRate = RS485_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(RS485_USART, &USART_InitStructure); 
	/*使能USART*/
  USART_Cmd(RS485_USART, ENABLE);
	
	/*配置中断优先级*/
	NVIC_Configuration();
	/* 使能串口接收中断 */
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);
	
	/*控制485芯片进入接收模式*/
	GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);
	
	printf("\r                RS485_Config\n");
}



/***************** 发送一个字符  **********************/
//使用单字节数据发送前要使能发送引脚，发送后要使能接收引脚。
void RS485_SendByte(  uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(RS485_USART,ch);
//	SysTick_Delay_Us(20);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET);	
//	Delay(0xFF);
//	SysTick_Delay_Us(5);
}
/*****************  发送指定长度的字符串 **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;

	RS485_TX_EN()	;//	使能发送数据	
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
		
	/*加短暂延时，保证485发送数据完毕*/
	Delay(0xFFFF);
		
	RS485_RX_EN()	;//	使能接收数据
}


/*****************  发送字符串 **********************/
void RS485_SendString(  uint8_t *str)
{
	unsigned int k=0;
	
	RS485_TX_EN()	;//	使能发送数据
	
    do 
    {
        RS485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
	/*加短暂延时，保证485发送数据完毕*/
	Delay(0xFFFF);
		
	RS485_RX_EN()	;//	使能接收数据
}






//中断缓存串口数据
#define UART_BUFF_SIZE      1024
volatile    uint16_t uart_p = 0;
uint8_t     uart_buff[UART_BUFF_SIZE];

void bsp_RS485_IRQHandler(void)
{
    if(uart_p<UART_BUFF_SIZE)
    {
        if(USART_GetITStatus(RS485_USART, USART_IT_RXNE) != RESET)
        {
            uart_buff[uart_p] = USART_ReceiveData(RS485_USART);
            uart_p++;
						
						USART_ClearITPendingBit(RS485_USART, USART_IT_RXNE);
        }
    }
		else
		{
			USART_ClearITPendingBit(RS485_USART, USART_IT_RXNE);
//			clean_rebuff();       
		}
}



//获取接收到的数据和长度
char *get_rebuff(uint16_t *len) 
{
    *len = uart_p;
    return (char *)&uart_buff;
}

//清空缓冲区
void clean_rebuff(void) 
{

    uint16_t i=UART_BUFF_SIZE+1;
    uart_p = 0;
	while(i)
		uart_buff[--i]=0;

}



static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


void RS485_Modified_baud_rate(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* config USART clock */
	USART_Cmd(RS485_USART, DISABLE);
	
	/* USART 模式配置*/
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(RS485_USART, &USART_InitStructure); 
	/*使能USART*/
  USART_Cmd(RS485_USART, ENABLE);
	
	/*控制485芯片进入接收模式*/
	GPIO_ResetBits(RS485_RE_GPIO_PORT,RS485_RE_PIN);
	
//	printf("\r\n 修改485波特率baud为%d \r\n",baud);
}



