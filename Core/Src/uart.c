/*
 * uart.c
 *
 *  Created on: Aug 25, 2021
 *      Author: KETI_GTO
 */

#include "uart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;



void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */
#if 0
  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
#endif
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 4000000;	//4mbps //115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_8;	//UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END USART6_Init 2 */

}

/*##########################################################################################################*/

/*uart interrupt test*/ //gto
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
			//debugPrintln(&huart1, "uart1 interrupt! ");
	        // When one data is received, an interrupt is generated.

			HAL_UART_Receive_IT(&huart1, &rx_data, 1);

	        // Send the received data.

			HAL_UART_Transmit(&huart1, &rx_data, 1, 10);

			uart1_key_Flag = 1;

	}

	if (huart->Instance == USART2) {
		//uart2_key_Flag = 1;
		//debugPrintln(&huart1, "uart2 interrupt! ");
		HAL_UART_Receive_IT(&huart2, (uint8_t *) &rxdata, 1);

		switch(status){

		    case START:
		        if( rxdata == START_CODE ) {
		            rxd[0] = START_CODE ;
		            rx_cnt = 1 ;
		            status = PAYLOAD ;
		            uart2_key_Flag = 0 ;
		        }
		        break ;

		    case PAYLOAD :
		        if( rxdata == START_CODE ) {
		            rxd[0] = START_CODE ;
		            rx_cnt = 1 ;
		            status = PAYLOAD ;
		        }
		        else if( rxdata == END_CODE ) {
		            if( rx_cnt == (LENGTH-1) )  {
		                rxd[rx_cnt++] = rxdata ;
		                uart2_key_Flag = 1 ;
		            }
		            status = START ;
		        }
		        else{
		            if( rx_cnt < (LENGTH+2) )  rxd[rx_cnt++] = rxdata ;
		            else {
		                status = START ;
		                rx_cnt = 0 ;
		                uart2_key_Flag = 0 ;
		            }
		        }
		        break ;
		}
	}

	if (huart->Instance == USART6) {

		//uart_6_flag = 1;

		HAL_UART_Receive_IT(&huart6, (uint8_t *) &GTO_rxdata, 1);

		switch(GTO_status){

				case GTO_START:
					if( GTO_rxdata == GTO_START_CODE ) {
						GTO_rxd[0] = GTO_START_CODE ;
						GTO_rx_cnt = 1 ;
						GTO_status = GTO_PAYLOAD ;
						uart_6_flag = 0 ;
					}
					break ;

				case GTO_PAYLOAD :
					if( GTO_rxdata == GTO_START_CODE ) {
						GTO_rxd[0] = GTO_START_CODE ;
						GTO_rx_cnt = 1 ;
						GTO_status = GTO_PAYLOAD ;
					}
					else if( GTO_rxdata == GTO_END_CODE ) {
						if( GTO_rx_cnt == (GTO_LENGTH-1) )  {
							GTO_rxd[GTO_rx_cnt++] = GTO_rxdata ;
							uart_6_flag = 1 ;
						}
						GTO_status = GTO_START ;
					}
					else{
						if( GTO_rx_cnt < (GTO_LENGTH+2) )  GTO_rxd[GTO_rx_cnt++] = GTO_rxdata ;
						else {
							GTO_status = GTO_START ;
							GTO_rx_cnt = 0 ;
							uart_6_flag = 0 ;
						}
					}
					break ;
		}

	}
}
/*##########################################################################################################*/
