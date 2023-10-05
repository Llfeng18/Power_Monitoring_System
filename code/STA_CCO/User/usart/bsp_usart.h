#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
	* 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
	* 2-�޸�GPIO�ĺ�
  */
	
// ����1_HPLC_USART1
#define  HPLC_USART1                   USART1
#define  HPLC_USART_CLK                RCC_APB2Periph_USART1
#define  HPLC_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  HPLC_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  HPLC_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  HPLC_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  HPLC_USART_TX_GPIO_PORT       GPIOA   
#define  HPLC_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  HPLC_USART_RX_GPIO_PORT       GPIOA
#define  HPLC_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  HPLC_USART_IRQ                USART1_IRQn
#define  HPLC_USART_IRQHandler         USART1_IRQHandler


//// ����2_RS485_USART2
//#define  RS485_USART2                   USART2
//#define  RS485_USART_CLK                RCC_APB1Periph_USART2
//#define  RS485_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  RS485_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  RS485_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  RS485_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  RS485_USART_TX_GPIO_PORT       GPIOA   
//#define  RS485_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  RS485_USART_RX_GPIO_PORT       GPIOA
//#define  RS485_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  RS485_USART_IRQ                USART2_IRQn
//#define  RS485_USART_IRQHandler         USART2_IRQHandler

// ����3-USART3
#define  DEBUG_USART3                   USART3
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOB   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART_IRQ                USART3_IRQn
#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

// ����4-UART4
#define  Pi_USART4                      UART4
#define  Pi_USART_CLK                   RCC_APB1Periph_UART4
#define  Pi_USART_APBxClkCmd            RCC_APB1PeriphClockCmd
#define  Pi_USART_BAUDRATE              115200

// USART GPIO ���ź궨��
#define  Pi_USART_GPIO_CLK              (RCC_APB2Periph_GPIOC)
#define  Pi_USART_GPIO_APBxClkCmd       RCC_APB2PeriphClockCmd
      
#define  Pi_USART_TX_GPIO_PORT          GPIOC   
#define  Pi_USART_TX_GPIO_PIN           GPIO_Pin_10
#define  Pi_USART_RX_GPIO_PORT          GPIOC
#define  Pi_USART_RX_GPIO_PIN           GPIO_Pin_11

#define  Pi_USART_IRQ                   UART4_IRQn
#define  Pi_USART_IRQHandler            UART4_IRQHandler


// ����5-UART5
//#define  DEBUG_USARTx                   UART5
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  DEBUG_USART_IRQ                UART5_IRQn
//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void HPLC_USART_Config(void);
void Pi_USART_Config(void);
void DEBUG_USART_Config(void);
void HPlC_SendByte(uint8_t ch);
#endif /* __USART_H */
