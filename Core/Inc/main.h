/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
// Enter interrupt
#define __TCP_CLIENT__		//Client mode
//#define __TCP_SERVER__	//server mode
#define __DEF_IINCHIP_BUS__ __DEF_IINCHIP_SPI_MODE__ /*Enable SPI_mode*/
#define enable()  __asm__ __volatile__ ("sei" ::)
#define disable() __asm__ __volatile__ ("cli" ::)

void cs_sel();
void cs_desel();
uint8_t spi_rb(void);
void spi_wb(uint8_t b);

typedef struct _DESTINATION_SETUP
{
	uint16_t port;
	uint8_t destip[4];
}DESTINATION_SETUP;

#define DATA_BUF_SIZE   2048
#define MAX_SOCK_NUM		8	/**< Maxmium number of socket  */
#define tick_second 1

// uart
uint8_t uart1_key_Flag;
uint8_t uart2_key_Flag;
uint8_t uart_6_flag;

// state machine
#define   START_CODE	 	'['
#define   END_CODE		 	']'
#define   START				1
#define   PAYLOAD 			2
#define   END    			3
#define   LENGTH   			35
uint8_t   rxd[40];
uint8_t   status;
uint8_t   rx_cnt;
uint8_t   rxdata;

#define   GTO_START_CODE	 	'('
#define   GTO_END_CODE		 	')'
#define   GTO_START				1
#define   GTO_PAYLOAD 			2
#define   GTO_END    			3
#define   GTO_LENGTH   			11
uint8_t   GTO_rxd[40];
uint8_t   GTO_status;
uint8_t   GTO_rx_cnt;
uint8_t   GTO_rxdata;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
