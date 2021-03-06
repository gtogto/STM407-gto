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

//gto make
#include "spi_handler.h"
#include "uart.h"
#include "gpio.h"


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

//SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim7;

/*
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
//static void MX_USART2_UART_Init(void);
//static void MX_USART1_UART_Init(void);
//static void MX_UART4_Init(void);
//static void MX_UART5_Init(void);
//static void MX_USART6_UART_Init(void);
//static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM7_Init(void);
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
uint8_t dma_tx[10] = "hello gto ";
uint8_t data[1] = "S";
uint8_t sync_Signal = 0xff;
uint8_t test = 0x11;
uint8_t uart2_Signal = 0x00;
uint8_t uart6_Signal = 0x11;

uint8_t from105_00[5] = "10500";
uint8_t rx_data;


uint8_t etherNet_Flag = 0;

uint8_t id_1, id_2, id_3, id_4;
int deci1,deci2,deci3,deci4;
int result_bin;
int binary[20];

uint8_t recv_array[5];
uint8_t recv_cnt = 0;

char master_Name[2];


//uint8_t   data_receive_flag = 0;

//uint8_t   uart_6_flag = 0;

char 	compare_CMD[28] = "[FID=XX SLAVE ANCHOR DEVICE]";
char* 	reset_CMD;

/*##########################################################################################################*/
/* Ethernet */
#define PORT				60500
#define SEPARATOR            "=============================================\r\n"
#define WELCOME_MSG  		 "Welcome to STM32Nucleo Ethernet configuration\r\n"
#define NETWORK_MSG  		 "Network configuration:\r\n"
#define IP_MSG 		 		 "  IP ADDRESS:  %d.%d.%d.%d\r\n"
#define NETMASK_MSG	         "  NETMASK:     %d.%d.%d.%d\r\n"
#define GW_MSG 		 		 "  GATEWAY:     %d.%d.%d.%d\r\n"
#define MAC_MSG		 		 "  MAC ADDRESS: %x:%x:%x:%x:%x:%x\r\n"
#define GREETING_MSG 		 "Well done! Connected to the STM-407 Board. GoodBye!!\r\n"
#define CONN_ESTABLISHED_MSG "Connection established with remote IP: %d.%d.%d.%d:%d\r\n"
#define SENT_MESSAGE_MSG	 "Sent a message. Let's close the socket!\r\n"
#define WRONG_RETVAL_MSG	 "Something went wrong; return value: %d\r\n"
#define WRONG_STATUS_MSG	 "Something went wrong; STATUS: %d\r\n"
#define LISTEN_ERR_MSG		 "LISTEN Error!\r\n"
#define GTO_MSG		 		 "GTO Return MSG\r\n"

#define E_RST_HIGH()		HAL_GPIO_WritePin(GPIOC, E_RST, 1)
#define E_RST_LOW()			HAL_GPIO_WritePin(GPIOC, E_RST, 0)

#define SOCK_LISTEN         0x14
#define SOCK_INIT	        0x13
#define SOCK_ESTABLISHED    0x17
#define SOCK_CLOSE_WAIT     0x1C
#define SOCK_CLOSED			0x00

#define PRINT_STR(msg) do  {										\
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);		\
} while(0)

#define PRINT_HEADER() do  {													\
  HAL_UART_Transmit(&huart1, (uint8_t*)SEPARATOR, strlen(SEPARATOR), 100);		\
  HAL_UART_Transmit(&huart1, (uint8_t*)WELCOME_MSG, strlen(WELCOME_MSG), 100);	\
  HAL_UART_Transmit(&huart1, (uint8_t*)SEPARATOR, strlen(SEPARATOR), 100);		\
} while(0)

#define PRINT_NETINFO(netInfo) do { 																					\
  HAL_UART_Transmit(&huart1, (uint8_t*)NETWORK_MSG, strlen(NETWORK_MSG), 100);											\
  sprintf(msg, MAC_MSG, netInfo.mac[0], netInfo.mac[1], netInfo.mac[2], netInfo.mac[3], netInfo.mac[4], netInfo.mac[5]);\
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);															\
  sprintf(msg, IP_MSG, netInfo.ip[0], netInfo.ip[1], netInfo.ip[2], netInfo.ip[3]);										\
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);															\
  sprintf(msg, NETMASK_MSG, netInfo.sn[0], netInfo.sn[1], netInfo.sn[2], netInfo.sn[3]);								\
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);															\
  sprintf(msg, GW_MSG, netInfo.gw[0], netInfo.gw[1], netInfo.gw[2], netInfo.gw[3]);										\
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);															\
} while(0)

void loopback_tcpc(SOCKET s, uint16_t port);

char msg[60];

void cs_sel() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //CS LOW
}

void cs_desel() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //CS HIGH
}

uint8_t spi_rb(void) {
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi1, &rbuf, 1, 0xFFFFFFFFU);
	return rbuf;
}

void spi_wb(uint8_t b) {
	HAL_SPI_Transmit(&hspi1, &b, 1, 0xFFFFFFFFU);
}



uint8_t retVal, sockStatus;
uint8_t bufSize[] = {2, 2, 2, 2};

#define SOCK_TCPS			0
#define TX_RX_MAX_BUF_SIZE	2048

int32_t loopback_tcps(uint8_t, uint8_t*, uint16_t);		// Loopback TCP server

//static uint8_t buff_size[] = { 2, 2, 2, 2 };
#define Sn_IMR(N)          (_W5100_IO_BASE_ + (0x002C << 8) + (WIZCHIP_SREG_BLOCK(N) << 3))
#define setSn_IMR(sn, imr) \
		WIZCHIP_WRITE(Sn_IMR(sn), (imr & 0x1F))1

uint32_t	ret;
uint16_t	size = 0, sentsize = 0;

uint8_t c_buffer[2048];		//client sends 8 bytes
uint8_t test_100[100] = "gtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogtogto1234567890";


uint8_t ah, as;
static uint16_t wizDHCPticks = 0;

#define DMA_BUF_SIZE   2048
uint8_t TX_Buffer[5];

DESTINATION_SETUP Destination_Setup;
uint8_t gDATABUF[DATA_BUF_SIZE];
uint8_t TX_BUF[DATA_BUF_SIZE]; // TX Buffer for applications
uint8_t ch_status[MAX_SOCK_NUM] = { 0, };	// 0:close, 1:ready, 2:connected
uint16_t sys_tick_cnt;
wiz_NetInfo gWIZNETINFO;
/*##########################################################################################################*/
//for SPI DMA

DMA_InitTypeDef  DMA_InitStructure;



/*##########################################################################################################*/
//External Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("EXTERNAL INTERRUPT CALLBACK EVENT !! \r\n");
	etherNet_Flag = 1;
	//setSn_IR(0, 0xff);
	//TCP_Ethernet_Server();
}
/*##########################################################################################################*/


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

  MX_SPI1_Init();

  MX_DMA_Init();

  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_UART5_Init();
  MX_UART4_Init();
  MX_USART6_UART_Init();

  MX_TIM1_Init();
  MX_TIM7_Init();

  MX_RTC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /*##########################################################################################################*/
  /*START DEBUGGING MESSAGE*/

  printf("\r\n * Start STM32F407 for Master Anchor - 20210825 GTO * \r\n\n");
  printf(" =============== 1. UART and 4Mbps test Okay (USART 1, 2, 3, 4, 5, 6) \r\n");
  printf(" =============== 2. GPIO test Okay \r\n");
  printf(" =============== 3. SPI and EXTI test Okay (for Ethernet) \r\n");
  printf(" =============== 4. ETHERNET test Okay (Handler Callback) \r\n");
  printf("\n");

  /*END DEBUGGING MESSAGE*/
  /*##########################################################################################################*/

  /*##########################################################################################################*/
  /*INTERRUPT SET*/
  //gto
  HAL_UART_Receive_IT(&huart1, &rx_data, 1);
  HAL_UART_Receive_IT(&huart2, (uint8_t *) &data, 1); // interrupt uart 2
  HAL_UART_Receive_IT(&huart6, (uint8_t *) &GTO_rxdata, 1); // interrupt uart 6

  /*PIN SET*/
  HAL_GPIO_WritePin(GPIOB, DOWN_EN, 0); 	// GPIO PB9 OUTPUT LOW -> Down enable
  HAL_GPIO_WritePin(GPIOE, SYNC_GEN, 0); 	// SYNC_GEN LOW

  HAL_GPIO_WritePin(GPIOC, TX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> tx enable
  HAL_GPIO_WritePin(GPIOC, RX_EN, 0);		// GPIO PC1 OUTPUT LOW -> rx enable

  //HAL_GPIO_WritePin(GPIOC, RX_EN, 1);		// GPIO PC1 OUTPUT HIGH -> rx enable

  /*##########################################################################################################*/

  /*##########################################################################################################*/
  /*ETHERNET SET*/
  //gto
  //printf("\r\n EHTERNET RUNNING .... 20210813 \r\n");
  E_RST_HIGH();								// Ethernet Enable
  //HAL_GPIO_WritePin(GPIOC, E_RST, 1);		// Ethernet Enable

  //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 1);	// Ethernet Extra Interrupt Enable

  reg_wizchip_cs_cbfunc(cs_sel, cs_desel);

  reg_wizchip_spi_cbfunc(spi_rb, spi_wb);


  wizchip_init(bufSize, bufSize);
  /*
  wiz_NetInfo netInfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
		  	  	  	  	  .ip 	= {192, 168, 2, 192},					// IP address
						  .sn 	= {255, 255, 255, 0},					// Subnet mask
						  .gw 	= {192, 168, 2, 1}};					// Gateway address
  */


  wiz_NetInfo netInfo = { .mac 	= {0x59, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
 		  	  	  	  	  .ip 	= {10, 0, 7, 211},						// IP address
 						  .sn 	= {255, 255, 248, 0},					// Subnet mask
 						  .gw 	= {10, 0, 0, 1}};						// Gateway address

  wizchip_setnetinfo(&netInfo);
  wizchip_getnetinfo(&netInfo);
  PRINT_NETINFO(netInfo);

  // target server ip set
  Destination_Setup.destip[0] = 10; Destination_Setup.destip[1] = 0; Destination_Setup.destip[2] = 0; Destination_Setup.destip[3] = 87;
  // target server port set
  Destination_Setup.port = 60500;

  //HAL_SPI_Transmit_DMA(&hspi1, DMA_BUF_SIZE, 1); //Transmit DMA Enable
  //HAL_SPI_Receive_DMA(&hspi1, DMA_BUF_SIZE, 1);  //Receive DMA Enable


  /*##########################################################################################################*/


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1); // GPIO PC1 OUTPUT HIGH -> tx enable
  //HAL_GPIO_WritePin(GPIOC, E_RST, 0);

  uint8_t tmp;
  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

  /* WIZCHIP SOCKET Buffer initialize */
  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
  {
	  printf("WIZCHIP Initialized fail.\r\n");
	  DWT_Delay_us(10);
	  while(1);
  }

  /* PHY link status check */

  do
  {
	  if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1){
		  DWT_Delay_us(10);
		  printf("Unknown PHY Link stauts.\r\n");
	  }

  }while(tmp == PHY_LINK_OFF);


  while (1)
  {

	  //DWT_Delay_us(1000);											// 10 microsecond
	  //HAL_UART_Transmit(&huart6, (uint8_t *) &test, 1, 10);		//4mbps test

	  //loopback_tcps(SOCK_TCPS, 20, 60500);
	  loopback_tcpc(1, 60500);
	  DWT_Delay_us(1000);

	  //setIMR(0x01);
	  //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.

	  if (uart1_key_Flag){
		  uart1_key_Flag = 0;
		  switch(rx_data){
			  case 'a':
				  debugPrint(&huart1, "debugPrint test ");
				  debugPrintln(&huart1, "debugPrintln test");
				  printf("printf test\r\n");
				  break;

			  case 's':
				  Sync_out();
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
				  HAL_UART_Transmit(&huart2, (uint8_t *) data, 1, 10);				// send data(0x00)
				  break;

			  case 't':
				  HAL_UART_Transmit(&huart6, (uint8_t *) &uart6_Signal, 1, 10);		// send data(0x00)
				  break;

			  case 'z':
				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=00 407 TO SLAVE ANCHOR DEVICE]", 35, 100);
				  break;

			  case 'x':
				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=01 407 TO SLAVE ANCHOR DEVICE]", 35, 100);
				  break;

			  case 'c':
				  HAL_UART_Transmit(&huart2, (uint8_t *) "[RID=08 407 TO SLAVE ANCHOR DEVICE]", 35, 100);
				  break;

			  case 'e':
				  //etherNet_Flag = 1;
				  //setIMR(0x01);
				  //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.

				  //HAL_SPI_Transmit(&hspi1, (uint8_t *)"GTO", 3, 100);
				  /*
				  if (etherNet_Flag){
					  etherNet_Flag = 0;
					  //setSn_IMR(0, 0xff);	//Interrupt occurs only in the recv interrupt of socket '0'.
					  if((retVal = send(SOCK_TCPS, GREETING_MSG, strlen(GREETING_MSG))) == (int16_t)strlen(GREETING_MSG)) {
						  printf("######### 407 board transmits data to server !! \r\n");
					  }
				  }*/
				  /*
				  for(int j = 0; j <100; j++ ){

					  for (int i = 0; i < 100; i++){
						  //send(SOCK_TCPS, test_100, 100);
						  while(!(USART1->SR & 0x80)){}
						  USART1->DR=test_100[i];
					  }
					  printf("\r\n");
				  }

				  printf("----------------------------- for loop end \r\n");
				  */
				  send(0, test_100, 100);


				  break;
		  }
	  }

	  /*
	  if (etherNet_Flag){
		  etherNet_Flag = 0;
		  if(getSn_SR(SOCK_TCPS) == SOCK_ESTABLISHED){
			  //setIMR(0x01);
			  //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.
			  ret = recv(SOCK_TCPS, c_buffer, sizeof(c_buffer));
			  ret = send(SOCK_TCPS, c_buffer, strlen(c_buffer));
			  //printf("recv data from client %c \r\n", c_buffer);
			  for (int i = 0; i < 4; i++) {
				  printf("recv data from client %c \r\n", c_buffer[i]);
			  }
		  }
	  }*/

	  //setIMR(0x01);
	  //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.


	  if(uart2_key_Flag) {
		  uart2_key_Flag = 0;
		  printf("uart2 flag on \r\n");
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


/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */



/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */


/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}


/* USER CODE BEGIN 4 */
/*##########################################################################################################*/
/*TCP Client*/
#ifdef __TCP_CLIENT__
void loopback_tcpc(SOCKET s, uint16_t port)
{
   uint16_t RSR_len;
   uint16_t received_len;
   uint16_t any_port = 60500;
   uint8_t * data_buf = TX_BUF;
   //printf("socket client -> %d \r\n", s);

   switch (getSn_SR(s))
   {
   	   	//printf("socket client -> %d \r\n", s);
		case SOCK_ESTABLISHED:                 /* if connection is established */
				  if(ch_status[s]==1)
				  {
					 printf("%d : Server IP : %d.%d.%d.%d , Port : %d  Connect \r\n", s,
							 Destination_Setup.destip[0],Destination_Setup.destip[1],Destination_Setup.destip[2],Destination_Setup.destip[3],Destination_Setup.port);
					 //printf("\r\n%d : Server IP : %d.%d.%d.%d  , Port : %d  Connected",s, 10, 0, 0 , 87, 60500);
					 ch_status[s] = 2;
				  }

				  if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
				  {
					 if (RSR_len > DATA_BUF_SIZE) RSR_len = DATA_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																						/* the data size to read is MAX_BUF_SIZE. */
					 received_len = recv(s, data_buf, RSR_len);         /* read the received data */
					 send(s, data_buf, received_len);         /* sent the received data */

					 printf("RX : %s \r\n",data_buf);
				  }

				  break;

		case SOCK_CLOSE_WAIT:                                 /* If the client request to close */
				  printf("\r\n%d : close ! \r\n", s);
				  if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
				  {
					 if (RSR_len > DATA_BUF_SIZE) RSR_len = DATA_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																								/* the data size to read is MAX_BUF_SIZE. */
					 received_len = recv(s, data_buf, RSR_len);         /* read the received data */
				  }
				  disconnect(s);
				  ch_status[s] = 0;
				  break;

		case SOCK_CLOSED:                                               /* if a socket is closed */
				  if(!ch_status[s])
				  {
					 printf("\r\n%d : Loop-Back TCP Client Started. port: %d\r\n", s, port);
					 ch_status[s] = 1;
				  }
				  if(socket(s, Sn_MR_TCP, any_port++, 0x00) == 0)    /* reinitialize the socket */
				  {
					 printf("\a%d : Fail to create socket.\r\n",s);
					 ch_status[s] = 0;
				  }
				  break;

		case SOCK_INIT:     /* if a socket is initiated */
				  if(sys_tick_cnt == 0)
				  {
					  connect(s, Destination_Setup.destip, Destination_Setup.port); /* Try to connect to TCP server(Socket, DestIP, DestPort) */
					  sys_tick_cnt = 3000;
				  }
				  break;
		default:
				  break;
	}
}
#endif

/*##########################################################################################################*/
/*TCP Server*/
#ifdef __TCP_SERVER__
int32_t loopback_tcps(uint8_t sn, uint8_t * buf, uint16_t port)
{
	//setIMR(0x01);
	//setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.

   switch(getSn_SR(sn))
   {
   	   case SOCK_LISTEN :
   		   asm("nop");
   		   break;

   	   case SOCK_ESTABLISHED :
   		   //setIMR(0x01);
   		   //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.

   		   if(getSn_IR(sn) & Sn_IR_CON)
   		   {
   			   printf(" >>>>>>>> Socket Num. %d : Connected \r\n\n", sn);
   			   setSn_IR(sn, Sn_IR_CON);
   		   }
   		   if((size = getSn_RX_RSR(sn)) > 0)
		   {
			   ret = recv(SOCK_TCPS, c_buffer, sizeof(c_buffer));
			   //USART1->DR = c_buffer[0] ;
			   printf(" ######### Data received from the CLient : %s \r\n", c_buffer);
			   ret = send(SOCK_TCPS, c_buffer+sentsize, size-sentsize);
			   //HAL_SPI_Transmit_DMA(&hspi1, dma_tx, 10);
			   //HAL_SPI_Transmit_DMA(&hspi1, c_buffer, sizeof(c_buffer));

		   }

   		   while(size != sentsize)
   		   {
   			 if(ret < 0)
   			 {
   				 close(sn);
   				 return ret;
   			 }
   			 sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
   		   }

   		   /*
   		   if((size = getSn_RX_RSR(sn)) > 0)
  		   {
   			   //setIMR(0x01);
			   //setSn_IR(0, Sn_IR_RECV);	//Interrupt occurs only in the recv interrupt of socket '0'.
   			   if(size > TX_RX_MAX_BUF_SIZE) size = TX_RX_MAX_BUF_SIZE;
   			   ret = recv(sn, buf, size);
   			   if(ret <= 0) return ret;
   			   sentsize = 0;

   			   ret = send(sn, buf, strlen(buf));
			   printf(" ######### Data received from the server : %s \r\n", buf);

   			   while(size != sentsize)
   			   {
   				   //ret = send(sn, buf, strlen(buf));
   				   //ret = send(sn, GTO_MSG, strlen(GTO_MSG));
   				   //printf(" ######### Data received from the server : %s \r\n", buf);

   				   //ret = send(SOCK_TCPS, c_buffer, strlen(c_buffer));
   				   //printf(" ######### Data received from the server : %s \r\n", c_buffer);

   				   if(ret < 0)
   				   {
   					   close(sn);
   					   return ret;
   				   }
   				   sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
   			   }
   		   }*/
   		   break;

   	   case SOCK_CLOSE_WAIT :
   		   printf(" ******** Socket Num. %d:CloseWait \r\n",sn);
   		   if((ret=disconnect(sn)) != SOCK_OK) return ret;
   		   printf("%d:Closed \r\n", sn);
   		   break;

   	   case SOCK_INIT :
   		   printf(" ******** Socket Num. %d : Listen, port [%d]\r\n",sn, port);
   		   if( (ret = listen(sn)) != SOCK_OK) return ret;
   		   break;

   	   case SOCK_CLOSED:
   		   printf(" ******** Socket Num. %d : LBTStart \r\n", sn);
   		   if((ret=socket(sn, Sn_MR_TCP, port, 0x00)) != sn)
   			   return ret;
   		   printf(" ******** Socket Num. %d : Opened \r\n", sn);
   		   break;

   	   default:
   		   break;
   }
   return 1;
}
#endif
/*##########################################################################################################*/
void TCP_Ethernet_Server(void){
	printf("\r\n EHTERNET RUNNING .... 20210817 \r\n");

	/* Open socket 0 as TCP_SOCKET with port 60500 */
	if((retVal = socket(0, Sn_MR_TCP, PORT, 0)) == 0) {
		/* Put socket in LISTEN mode. This means we are creating a TCP server */
		if((retVal = listen(0)) == SOCK_OK) {
			printf("######### SOCKET READY !! \r\n");
			/* While socket is in LISTEN mode we wait for a remote connection */
			while((sockStatus = getSn_SR(0)) == SOCK_LISTEN)
				//printf("SOCK LISTENNING ... !! \r\n");
				HAL_Delay(100);
			/* OK. Got a remote peer. Let's send a message to it */
				while(1) {
	  			  /* If connection is ESTABLISHED with remote peer */
	  			  if((sockStatus = getSn_SR(0)) == SOCK_ESTABLISHED) {
	  				  printf("######### SOCK_ESTABLISHED !! \r\n");
	  				  uint8_t remoteIP[4];
	  				  uint16_t remotePort;

	  				  /* Retrieving remote peer IP and port number */
	  				  getsockopt(0, SO_DESTIP, remoteIP);
	  				  getsockopt(0, SO_DESTPORT, (uint8_t*)&remotePort);
	  				  sprintf(msg, CONN_ESTABLISHED_MSG, remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3], remotePort);
	  				  printf("====================================================== \r\n");
	  				  printf(msg, CONN_ESTABLISHED_MSG, remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3], remotePort);
	  				  printf("====================================================== \r\n");
	  				  PRINT_STR(msg);

	  				  /* Let's send a welcome message and closing socket */
	  				  if((retVal = send(0, GREETING_MSG, strlen(GREETING_MSG))) == (int16_t)strlen(GREETING_MSG)) {
	  					  printf("######### WELCOME MESSAGE !! \r\n");
	  					  PRINT_STR(SENT_MESSAGE_MSG);
	  					  //etherNet_Flag = 0;
	  				  }

	  				  else { /* Ops: something went wrong during data transfer */
	  					  sprintf(msg, WRONG_RETVAL_MSG, retVal);
	  					  printf(" Wrong MESSAGE !! \r\n");
	  					  PRINT_STR(msg);
	  				  }
	  				  break;
	  			  }
	  			  else { /* Something went wrong with remote peer, maybe the connection was closed unexpectedly */
	  				  sprintf(msg, WRONG_STATUS_MSG, sockStatus);
	  				  PRINT_STR(msg);
	  				  break;
	  			  }
	  		  }

	  	  } else /* Ops: socket not in LISTEN mode. Something went wrong */
	  		  PRINT_STR(LISTEN_ERR_MSG);
	  } else { /* Can't open the socket. This means something is wrong with W5100 configuration: maybe SPI issue? */
	  	  sprintf(msg, WRONG_RETVAL_MSG, retVal);
	  	  PRINT_STR(msg);
	  }

	  /* We close the socket and start a connection again */
	  disconnect(0);
	  close(0);
}
/*##########################################################################################################*/
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
  	  //printf("TIM6 callback ==== %d \r\n", wizDHCPticks);
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
