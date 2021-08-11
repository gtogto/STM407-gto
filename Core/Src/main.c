/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "dwt_stm32_delay.h"	//gto

#include "dhcp.h"				//gto
#include "w5100.h"				//gto
#include "wizchip_conf.h"		//gto
#include "socket.h"				//gto

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM7_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//gto
//char *gto_substr(int s, int e, char *str);
void Sync_out();
char *SubStr( char *pnInput, int nStart, int nLen );

#define SYNC_GEN	GPIO_PIN_0
#define DOWN_EN		GPIO_PIN_9
#define GPIO_A		GPIO_PIN_5
#define TX_EN		GPIO_PIN_1
#define RX_EN		GPIO_PIN_3

//ETHERNET (W5100) -gto
#define E_INT		GPIO_PIN_7
#define E_RST		GPIO_PIN_4
#define E_SCLK		GPIO_PIN_5
#define E_MOSI		GPIO_PIN_7
#define E_MISO		GPIO_PIN_6


//gto
/*ID*/
#define ID1			GPIO_PIN_12
#define ID2			GPIO_PIN_13
#define ID3			GPIO_PIN_14
#define ID4			GPIO_PIN_15

uint8_t start_data[17] = "START 32F407 \r\n";
uint8_t tx_data[10] = "recvD\r\n";
uint8_t rx_data5[10] = "uart5\r\n";
uint8_t rx_data4[10] = "uart4\r\n";
uint8_t data[1] = "S";
uint8_t sync_Signal = 0xff;
uint8_t test = 0x11;
uint8_t uart2_Signal = 0x00;
uint8_t uart6_Signal = 0x11;

uint8_t from105_00[5] = "10500";
uint8_t rx_data;
uint8_t uart1_key_Flag = 0;
uint8_t uart2_key_Flag = 0;

uint8_t id_1, id_2, id_3, id_4;
int deci1,deci2,deci3,deci4;
int result_bin;
int binary[20];

uint8_t recv_array[5];
uint8_t recv_cnt = 0;

char master_Name[2];


#define   START_CODE	 	'['
#define   END_CODE		 	']'
#define   START				1
#define   PAYLOAD 			2
#define   END    			3
#define   LENGTH   			35
uint8_t   rxd[40];
uint8_t   status = START;
uint8_t   rx_cnt;
uint8_t   rxdata;

#define   GTO_START_CODE	 	'('
#define   GTO_END_CODE		 	')'
#define   GTO_START				1
#define   GTO_PAYLOAD 			2
#define   GTO_END    			3
#define   GTO_LENGTH   			11
uint8_t   GTO_rxd[40];
uint8_t   GTO_status = GTO_START;
uint8_t   GTO_rx_cnt;
uint8_t   GTO_rxdata;




uint8_t   data_receive_flag = 0;

uint8_t   uart_6_flag = 0;

char 	compare_CMD[28] = "[FID=XX SLAVE ANCHOR DEVICE]";
char* 	reset_CMD;

/*##########################################################################################################*/
/* Ethernet */
void DisplayConfig(void);

#define IINCHIP_READ(ADDR)                WIZCHIP_READ(ADDR)               ///< The defined for legacy chip driver
#define GAR0           0x0001
#define SUBR0          0x0005
#define SHAR0          0x0009
#define SIPR0          0x000F

#define SOCK_ESTABLISHED    0x17
#define SOCK_CLOSE_WAIT     0x1C
#define SOCK_CLOSED			0x00

static uint16_t wizDHCPticks = 0;

#define MY_NET_MAC		{0x00, 0x08, 0xdc, 0x00, 0x00, 0x00}	// MY Mac Address : 00.08.DC.00.00.00

#define MY_NET_GWIP		{192, 168, 0, 1}	 					//Gateway     : 192.168.0.1
#define MY_SOURCEIP		{192, 168, 0, 101} 						//407 IP      : 192.168.0.101
#define MY_SUBNET		{255, 255, 255, 0}

#define MY_LISTEN_PORT  5000  									//Server Port : 5000

#define TX_RX_MAX_BUF_SIZE	1024

#define SOCK_TCPS			0
#define MY_NET_MEMALLOC	0x55									// MY iinchip memory allocation

unsigned char m_SokStatus1 = 2;


#define setSHAR(shar) \
		WIZCHIP_WRITE_BUF(SHAR, shar, 6)

#define getGAR(gar) \
		WIZCHIP_READ_BUF(GAR,gar,4)

#define setSUBR(subr) \
      WIZCHIP_WRITE_BUF(SUBR,subr,4)


#define setSIPR(sipr) \
		WIZCHIP_WRITE_BUF(SIPR, sipr, 4)

#define setMR(mr) \
	WIZCHIP_WRITE(MR,mr)


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
/*Send information (strings) to the console (PC) */

/*printf for uart1*/	//gto
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, 500);
	return len;
}

/*Function to write directly to UART*/	//gto
void debugPrint(UART_HandleTypeDef *huart1, char _out[]){
 HAL_UART_Transmit(huart1, (uint8_t *) _out, strlen(_out), 10);
}


/*Function to write to UART and new line termination*/ //gto
void debugPrintln(UART_HandleTypeDef *huart, char _out[]){
 HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
 char newline[2] = "\r\n";
 HAL_UART_Transmit(huart, (uint8_t *) newline, 2, 10);
}

/*##########################################################################################################*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART6_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM7_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  //gto
  HAL_UART_Receive_IT(&huart1, &rx_data, 1);
  HAL_UART_Receive_IT(&huart2, (uint8_t *) &data, 1); // interrupt uart 2
  HAL_UART_Receive_IT(&huart6, (uint8_t *) &GTO_rxdata, 1); // interrupt uart 6
  //HAL_UART_Transmit(&huart1, start_data, 17, 10);
  //debugPrintln(&huart1, "\n Start STM32F407");

  /*##########################################################################################################*/
  /*START DEBUGGING MESSAGE*/

  printf("\r\n Start STM32F407 - 20210811 \r\n");

  NetInit();
  DisplayConfig();

  /*END DEBUGGING MESSAGE*/
  /*##########################################################################################################*/

  /*
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1); // GPIO PC1 OUTPUT HIGH
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0); // GPIO PB9 OUTPUT LOW
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0); // GPIO PC3 OUTPUT LOW
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0); // GPIO PE0 OUTPUT LOW*/

  //HAL_TIM_Base_Start_IT(&htim7); // timer test
  //HAL_UART_Receive_IT(&huart6, (uint8_t *) &data, 1); // interrupt usart6
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1); // GPIO PC1 OUTPUT HIGH -> tx enable
  HAL_GPIO_WritePin(GPIOB, DOWN_EN, 0); 	// GPIO PB9 OUTPUT LOW -> Down enable
  HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 0); 	// SYNC_GEN LOW

  HAL_GPIO_WritePin(GPIOC, TX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> tx enable
  HAL_GPIO_WritePin(GPIOC, RX_EN, 0);		// GPIO PC1 OUTPUT LOW -> rx enable

  //HAL_GPIO_WritePin(GPIOC, RX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> rx enable

  HAL_GPIO_WritePin(GPIOC, E_RST, 1);
  //HAL_GPIO_WritePin(GPIOC, E_RST, 0);

  while (1)
  {
	  //HAL_SPI_Transmit(&hspi1, (uint8_t *) &test, 1, 10);
	  //HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *) &test, 1);
	  //HAL_Delay(1);
	  //DWT_Delay_us(1000);											// 10 microsecond

	  /*
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 1);	// GPIO PE0 OUTPUT High -> SYNC_GEN
	  HAL_UART_Transmit(&huart6, (uint8_t *) &test, 1, 10);

	  DWT_Delay_us(100);	// 10 microsecond
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0);
	  HAL_Delay(1000);	// 1 second
	  */
	  /*
   	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 1);						// GPIO PE0 OUTPUT High -> SYNC_GEN
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);						// GPIO PC1 OUTPUT HIGH -> tx enable
	  HAL_UART_Transmit(&huart6, (uint8_t *) &sync_Signal, 1, 10);	// send data(0xff)
	  DWT_Delay_us(100);											// 10 microsecond
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);						// GPIO PC1 OUTPUT LOW -> tx enable
	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0);						// GPIO PE0 OUTPUT LOW -> SYNC_GEN

	  HAL_Delay(1000);	// 1 second*/

	  //HAL_UART_Transmit(&huart6, (uint8_t *) &test, 1, 10);		//4mbps test

	  ProcessTcpSever();	// Ethernet test

	  if (uart1_key_Flag){
		  uart1_key_Flag = 0;
		  switch(rx_data){

			  case 's':
				  Sync_out();

				  break;

			  case 'a':
				  debugPrint(&huart1, "debugPrint test ");
				  debugPrintln(&huart1, "debugPrintln test");
				  printf("printf test\r\n");
				  break;

			  case '2':
				  HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 0);
				  HAL_GPIO_WritePin(GPIOB, GPIO_A, 0);								// GPIO PB5 OUTPUT LOW  -> USART2 enable

				  break;

			  case '6':
				  HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 0);
				  HAL_GPIO_WritePin(GPIOB, GPIO_A, 1);								// GPIO PB5 OUTPUT HIGH -> USART6 enable

				  break;


			  case 'r':
				  //HAL_GPIO_WritePin(GPIOC, TX_EN, 1);								// GPIO PC1 OUTPUT HIGH -> tx enable

				  HAL_UART_Transmit(&huart2, (uint8_t *) data, 1, 10);				// send data(0x00)

				  //HAL_UART_Transmit(&huart6, (uint8_t *) &uart2_Signal, 1, 10);	// send data(0x00)

				  break;

			  case 't':
				  //HAL_UART_Transmit(&huart2, (uint8_t *) &uart2_Signal, 1, 10);	// send data(0x00)

				  HAL_UART_Transmit(&huart6, (uint8_t *) &uart6_Signal, 1, 10);		// send data(0x00)

				  break;

			  case 'z':
				  //HAL_UART_Transmit(&huart2, (uint8_t *) &data, 1, 10);
				  //HAL_GPIO_WritePin(GPIOC, TX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> tx enable
				  //HAL_GPIO_WritePin(GPIOC, RX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> rx enable

				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=00 407 TO SLAVE ANCHOR DEVICE]", 35, 100);

				  //HAL_GPIO_WritePin(GPIOC, TX_EN, 0);		// GPIO PC1 OUTPUT LOW -> tx enable
				  //HAL_GPIO_WritePin(GPIOC, RX_EN, 0);		// GPIO PC1 OUTPUT LOW -> rx enable

				  /*if(uart2_key_Flag){
					  printf("PIC Received\r\n");
				  }*/


				  break;

			  case 'x':
				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=01 407 TO SLAVE ANCHOR DEVICE]", 35, 100);
				  break;

			  case 'c':
				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=08 407 TO SLAVE ANCHOR DEVICE]", 35, 100);

				  break;

		  }
	  }


	  if(uart2_key_Flag) {
		  //HAL_UART_Transmit(&huart6, (uint8_t *) &uart2_Signal, 1, 10);				// send data(0x00)
		  uart2_key_Flag = 0;
		  printf("uart2 flag on \r\n");
		  //HAL_UART_Transmit(&huart1, "OK", 1, 10);
		  //HAL_UART_Transmit(&huart6, (uint8_t *) &uart2_Signal, 1, 10);				// send data(0x00)
		  //HAL_UART_Receive_IT(&huart2, &data, 1); // interrupt uart 2

		  reset_CMD = SubStr(rxd, 0, 35);
		  printf("substring %s\r\n", reset_CMD);

		  //HAL_Delay(1);
		  for (int i = 0; i < LENGTH; i++) {
			  HAL_UART_Transmit(&huart1, (uint8_t *) &rxd[i], 1, 10);
		  }

	  }

	  if(uart_6_flag) {
		  uart_6_flag = 0;

		  printf("Received 105 Data \r\n");

		  //HAL_Delay(1);

		  for (int i = 0; i < GTO_LENGTH; i++) {
			  HAL_UART_Transmit(&huart1, (uint8_t *) &GTO_rxd[i], 1, 10);
		  }
		  //printf("\r\n\r\n");

	  }


	  id_1 = HAL_GPIO_ReadPin(GPIOB, ID1);
	  id_2 = HAL_GPIO_ReadPin(GPIOB, ID2);
	  id_3 = HAL_GPIO_ReadPin(GPIOB, ID3);
	  id_4 = HAL_GPIO_ReadPin(GPIOB, ID4);
	  /*
	  if (id_1){
		  printf("ID 1 High \r\n");
		  deci1 = 1;
	  }
	  else {
		  printf("ID 1 Low \r\n");
		  deci1 = 0;
	  }

	  if (id_2){
		  printf("ID 2 High \r\n");
		  deci2 = 2;
	  }
	  else {
		  printf("ID 2 Low \r\n");
		  deci2 = 0;
	  }

	  if (id_3){
		  printf("ID 3 High \r\n");
		  deci3 = 4;
	  }
	  else {
		  printf("ID 3 Low \r\n");
		  deci3 = 0;
	  }

	  if (id_4){
		  printf("ID 4 High \r\n");
		  deci4 = 8;
	  }
	  else {
		  printf("ID 4 Low \r\n");
		  deci4 = 0;
	  }

	  result_bin = deci1+deci2+deci3+deci4;

	  int i;

	  sprintf(master_Name,"%02X", result_bin);
	  printf("----------> %s\r\n", master_Name);

	  printf("ID is -> ");

	  if (result_bin != 0){
		  for (i = 0; result_bin >= 1; i++) {
			  binary[i] = result_bin%2;
			  result_bin = result_bin/2;
		  }

		  for (int j = i-1; j >= 0; j--) {
			  printf("%d", binary[j]);    // 1011
		  }
	  }
	  else printf("0000");

	  printf("\r\n");


	  HAL_Delay(1000);

	  result_bin = 0;*/

  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART6_IRQn);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
  /*Configure GPIO pins : PC1 PC3 PC4 */
  /**SPI2 GPIO Configuration
	PB4     ------> SPI1_SCK
	PB6     ------> SPI1_MISO
	PB7     ------> SPI1_MOSI
  */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 41;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
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

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
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

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
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

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1 PC3 PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PE7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*##########################################################################################################*/
/*gto*/
/* Synchronization Function */

void Sync_out(){
	// sync signal
	HAL_GPIO_WritePin(GPIOB, GPIO_A, 1);							// GPIO PB5 OUTPUT HIGH -> USART6 enable
	HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 1);							// GPIO PE0 OUTPUT High -> SYNC_GEN
	HAL_GPIO_WritePin(GPIOC, TX_EN, 1);								// GPIO PC1 OUTPUT HIGH -> tx enable
	HAL_UART_Transmit(&huart6, (uint8_t *) &sync_Signal, 1, 10);	// send data(0xff)
	DWT_Delay_us(100);												// 10 microsecond
	HAL_GPIO_WritePin(GPIOC, TX_EN, 0);								// GPIO PC1 OUTPUT LOW -> tx enable
	HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 0);							// GPIO PE0 OUTPUT LOW -> SYNC_GEN
}
/*##########################################################################################################*/

/*##########################################################################################################*/
/*gto*/
/* make a Substring function */

char *SubStr( char *pnInput, int nStart, int nLen )
{
    int nLoop ;
    int nLength ;
    char *pszOutPut ;

    if( pnInput == NULL ){
        return NULL ;
    }
    pszOutPut = (char *)malloc( sizeof(char) * nLen + 1 ) ;
    nLength = strlen( pnInput ) ;
    if( nLength > nStart + nLen ){
        nLength = nStart + nLen ;
    }
    for( nLoop = nStart ; nLoop < nLength ; nLoop++ ){
        pszOutPut[nLoop-nStart] = pnInput[nLoop] ;
    }
    pszOutPut[nLoop - nStart] = '\0' ;
    return pszOutPut ;
}
/*##########################################################################################################*/

/*##########################################################################################################*/

void EthernetTest(unsigned char *pRcvBuffer, unsigned int len)
{
	unsigned int i;

	printf("Read Data[%d]\r\n", len);

	for(i=0;i<len;i++)
	{
		//receive data
		printf("%c ", pRcvBuffer[i]);
	}

	//receive data control
	if(pRcvBuffer[0] == '1')
	{
		printf("zero\r\n");
	}
	else if(pRcvBuffer[0] == '0')
	{
		printf("one\r\n");
	}
}


//TCP-Server process
void ProcessTcpSever(void)
{
	//printf("process TCP Server function .. \r\n");
	int len;
	unsigned char data_buf[TX_RX_MAX_BUF_SIZE];

	unsigned int port = MY_LISTEN_PORT;

	switch (getSn_SR(SOCK_TCPS))
	{
	case SOCK_ESTABLISHED:
		// check receive data
		if((len = getSn_RX_RSR(SOCK_TCPS)) > 0)
		{
			//if Rx data size is lager than TX_RX_MAX_BUF_SIZE
			if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE;

			// data receive
			len = recv(SOCK_TCPS, data_buf, len);

			// send the received data
			//send(SOCK_MYTEST, data_buf, len);

			EthernetTest(data_buf, len);
		}
		break;

	case SOCK_CLOSE_WAIT:
		//If the client request to close
		disconnect(SOCK_TCPS);

		m_SokStatus1 = 0;
		break;

	case SOCK_CLOSED:
		if(!m_SokStatus1)
		{
			m_SokStatus1 = 1;
		}

		//reinitialize the socket
		if(socket(SOCK_TCPS,Sn_MR_TCP, port,0x00) == 0)
		{
			printf("Fail to create socket.");
			m_SokStatus1 = 0;
		}
		else
		{
			listen(SOCK_TCPS);
		}

		break;
	}
}

void DisplayConfig(void)
{
	u_char addr[6];
	u_char i = 0;
	//u_long iaddr;
	printf("\r\n================================================\r\n");
	printf("       Net Config Information\r\n");
	printf("================================================\r\n");


	for(i=0; i<6;i++)addr[i] = IINCHIP_READ(SHAR0+i);
	printf("MAC ADDRESS      : 0x%02X.0x%02X.0x%02X.0x%02X.0x%02X.0x%02X\r\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);


	printf("SUBNET MASK      : ");
	for(i=0; i < 4; i++)
	{
		printf("%d.", (char)IINCHIP_READ(SUBR0+i));
	}


	printf("\r\nG/W IP ADDRESS   : ");
	for(i=0; i < 4; i++)
	{
		printf("%d.", (char)IINCHIP_READ(GAR0+i));
	}

	printf("\r\nLOCAL IP ADDRESS : ");
	for(i=0; i < 4; i++)
	{
		printf("%d.", (char)IINCHIP_READ(SIPR0+i));
	}

	printf("\r\n================================================\r\n");

}


/*##########################################################################################################*/

/*##########################################################################################################*/

void NetInit(void)
{
	unsigned char mac[6] = MY_NET_MAC;
	unsigned char sm[4]	= MY_SUBNET;
	unsigned char gwip[4]	= MY_NET_GWIP;
	unsigned char m_sip[4]	= MY_SOURCEIP;

	//W5100 Chip Init
	iinchip_init();

	wizchip_init(2, 2);

	//Set MAC Address
	setSHAR(mac);

	//Set Gateway
	setGAR(gwip);

	//Set Subnet Mask
	setSUBR(sm);

	//Set My IP
	setSIPR(m_sip);

#ifdef __DEF_IINCHIP_INT__
	setIMR(0xEF);
#endif

	//sysinit(MY_NET_MEMALLOC, MY_NET_MEMALLOC);
}

/**
@brief	This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/
void iinchip_init(void)
{
	setMR( MR_RST );
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	setMR( MR_IND | MR_AI );
#ifdef __DEF_IINCHIP_DBG__
	printf("MR value is %d \r\n",IINCHIP_READ(MR));
#endif
#endif
}

/*##########################################################################################################*/

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim->Instance == TIM6) {
	  wizDHCPticks++;
	  if(wizDHCPticks >= 1000)
	  {
		  wizDHCPticks = 0;
		  DHCP_time_handler();
	  }
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
