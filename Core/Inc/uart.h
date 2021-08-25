/*
 * uart.h
 *
 *  Created on: Aug 25, 2021
 *      Author: KETI_GTO
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart3;		// current GPIO check for SPI_DMA
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;

//uint8_t uart1_key_Flag = 0;
//uint8_t uart2_key_Flag = 0;
uint8_t rx_data;

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART6_UART_Init(void);

#endif /* INC_UART_H_ */
