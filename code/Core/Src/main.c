/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include <string.h>
#include <stdio.h>
#include "lcd.h"
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
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// lcd相关
char bt[1024]={0};
// 状态。ab：有无连接
char sta[1024]={0};
char cmd[1024]={0};
// l：显示的内容
char l[12][1024]={0};
char receiver[1024]={0};
char sender[1024]={0};
int re=0;

unsigned char outd[1024]={0};
unsigned char outd2[1024]={0};
unsigned char outl;
unsigned char outl2;
int icnt=0;
unsigned char snum[1024]={0};
unsigned char snumLength=0;


// 蓝牙相关
char rcv_buffer_user[1024]={0};
char rcv_buffer_bluetooth[1024]={0};
char helloWorld[40]="Hello world!\r\n";
char version[40] = "AT+VERSION?\r\n";
char cmode1[15] = "AT+CMODE=1\r\n";
char cmode0[15] = "AT+CMODE=0\r\n";
char modeat[15] = "enter at\r\n";
char at[10] = "AT\r\n";
char BindAddress[30]="AT+BIND=2113,21,000C10\r\n";
char EnBluetooth[18]="Bluetooth_Open\r\n";
char DisBluetooth[19]="Bluetooth_close\r\n";

int timer;
int key_interval = 300;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// 1 电脑
	if(huart->Instance==USART1)
	{
		//HAL_UART_Transmit(&huart2, "DO", 2, 0xffff);
		static unsigned char uRx_Data[1024] = {0};
		static unsigned char uLength = 0;
		if(rcv_buffer_user[0] == '\r')
		{
			uRx_Data[uLength] = rcv_buffer_user[0];
			uRx_Data[uLength+1] = '\n';
			uRx_Data[uLength+2] = '\n';
			for (int i=0;i<uLength;i++)
			{
				outd[i]=uRx_Data[i];
			}
			for (int i=uLength;i<1024;i++)
			{
				outd[i]=0;
			}
			outl = uLength;
			//uLength = 0;
			int q = outl / 13;
				if(outl % 13 != 0)
				{
					q = q + 1;
				}
				if(icnt+q<13)
				{
					if(q==1)
					{
						for(int k=0;k<28-outl;k++)
						{
							l[icnt][k]=' ';
						}
						for(int k=28-outl;k<28;k++)
						{
							l[icnt][k]=(char) outd[k-28+outl];
						}
					}
					else
					{
						for (int j=0;j<q;j++)
						{
							l[icnt+j][0]=' ';
							l[icnt+j][1]=' ';
							l[icnt+j][2]=' ';
							l[icnt+j][3]=' ';
							l[icnt+j][4]=' ';
							l[icnt+j][5]=' ';
							l[icnt+j][6]=' ';
							l[icnt+j][7]=' ';
							l[icnt+j][8]=' ';
							l[icnt+j][9]=' ';
							l[icnt+j][10]=' ';
							l[icnt+j][11]=' ';
							l[icnt+j][12]=' ';
							l[icnt+j][13]=' ';
							l[icnt+j][14]=' ';

							for (int k=0;k<13;k++)
							{
								if(j*13+k<outl)
								{
									l[icnt+j][k+15]=(char) outd[j*13+k];
								}
								else
								{
									l[icnt+j][k+15]=' ';
								}
							}
						}
					}
					icnt+=q;
				}
				else
				{
					int re=icnt+q-12;
					for(int j=0;j<12-re;j++)
					{
						for(int k=0;k<28;k++)
						{
							l[j][k]=l[j+re][k];
						}
					}
					if(q==1)
					{
						for(int k=0;k<28-outl;k++)
						{
							l[12-q][k]=' ';
						}
						for(int k=28-outl;k<28;k++)
						{
							l[12-q][k]=(char) outd[k-28+outl];
						}
						//HAL_UART_Transmit(&huart1, outd, strlen(outd), 0xffff);
						//HAL_UART_Transmit(&huart1, outd2, strlen(outd2), 0xffff);
					}
					else
					{

						for(int j=0;j<q;j++)
						{
							l[j+12-q][0]=' ';
							l[j+12-q][1]=' ';
							l[j+12-q][2]=' ';
							l[j+12-q][3]=' ';
							l[j+12-q][4]=' ';
							l[j+12-q][5]=' ';
							l[j+12-q][6]=' ';
							l[j+12-q][7]=' ';
							l[j+12-q][8]=' ';
							l[j+12-q][9]=' ';
							l[j+12-q][10]=' ';
							l[j+12-q][11]=' ';
							l[j+12-q][12]=' ';
							l[j+12-q][13]=' ';
							l[j+12-q][14]=' ';
							 for(int k=0;k<13;k++)
							 {
								 if(j*13+k<outl)
								 {
									 l[j+12-q][k+15]=outd[j*13+k];
								 }
								 else
								 {
									 l[j+12-q][k+15]=' ';
								 }
							 }
						}
					}
					icnt=12;
				}
				snum[snumLength]=q;
				snumLength++;
			// 电脑
//			HAL_UART_Transmit(&huart1, uRx_Data, uLength, 0xffff);
			// 蓝牙
			HAL_UART_Transmit(&huart2, uRx_Data, uLength + 3, 0xffff);
			uLength=0;
			// 接下来把数据写到显示的array里

			//re=0;
		}
		else
		{
			uRx_Data[uLength] = rcv_buffer_user[0];
			uLength++;
		}
	}
	// 2 蓝牙
	if(huart->Instance==USART2)
	{
		static unsigned char uRx_Data2[1024] = {0};
		static unsigned char uLength2 = 0;
		/*if(re==0){
		write_to_array_right();
		re=1;
		}*/
		if(rcv_buffer_bluetooth[0] == '\r')
		{
			uRx_Data2[uLength2] = rcv_buffer_bluetooth[0];
			// 电脑
			if(strlen(uRx_Data2[0])==0){
				for (int i=0;i<uLength2;i++){
					uRx_Data2[i]=uRx_Data2[i+1];
				}
				uLength2--;
			}


			for (int i=0;i<uLength2;i++)
			{
				outd2[i] = uRx_Data2[i];
			}
			for (int i=uLength2;i<1024;i++)
			{
				outd2[i] = 0;
			}
			outl2=uLength2;
			//HAL_UART_Transmit(&huart1, outd2, uLength2 + 1, 0xffff);
			int q = outl2 / 13;
				if(outl2 % 13 != 0){
					q=q+1;
				}
				if(icnt+q<13)
				{
					for (int j=0;j<q;j++)
					{
						for (int k=0;k<13;k++)
						{
							if(j*13 + k < outl2)
							{
								l[icnt+j][k]=(char) outd2[j*13+k];
							}
							else
							{
								l[icnt+j][k]=' ';
							}
						}for (int k=13;k<28;k++){
							l[icnt+j][k]=' ';
						}
					}
					icnt+=q;
					//HAL_UART_Transmit(&huart1, outd, strlen(outd), 0xffff);
					//HAL_UART_Transmit(&huart1, outd2, strlen(outd2), 0xffff);
				}
				else
				{
					int re=icnt+q-12;
					for(int j=0;j<12-re;j++)
					{
						for(int k=0;k<28;k++)
						{
							l[j][k]=l[j+re][k];
						}
					}
					for(int j=0;j<q;j++)
					{
						 for(int k=0;k<13;k++)
						 {
							 if(j*13+k<outl2)
							 {
								 l[j+12-q][k]=outd2[j*13+k];
							 }
							 else
							 {
								 l[j+12-q][k]=' ';
							 }
						 }
						 for (int k=13;k<28;k++){
							 l[j+12-q][k]=' ';
						 }
					}
					icnt=12;
				}
				//HAL_UART_Transmit(&huart1, outd2,strlen(outd2), 0xffff);
				//HAL_UART_Transmit(&huart1, l[3],strlen(l[3]), 0xffff);
				snum[snumLength]=q;
				snumLength++;
			uLength2 = 0;
		}
		else
		{
			uRx_Data2[uLength2] = rcv_buffer_bluetooth[0];
			uLength2++;
		}
	}
}

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rcv_buffer_user, 1);
  /* USER CODE END USART1_IRQn 1 */
}
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  HAL_UART_Receive_IT(&huart2, (uint8_t *)rcv_buffer_bluetooth, 1);
  /* USER CODE END USART2_IRQn 1 */
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GetTick() - timer < key_interval)
	{
		return;
	}
	else
	{
		timer = HAL_GetTick();
		switch(GPIO_Pin)
		{
			case KEY0_Pin://建立默认连接
			{
				HAL_UART_Transmit(&huart2, (uint8_t*)cmode0, strlen(cmode0), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2, (uint8_t*)BindAddress, strlen(BindAddress), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart1, (uint8_t*)cmode0, strlen(cmode0), HAL_MAX_DELAY);
				break;
			}
			case KEY1_Pin://任意连接
			{
				HAL_UART_Transmit(&huart2, (uint8_t*)cmode1, strlen(cmode1), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart1, (uint8_t*)cmode1, strlen(cmode1), HAL_MAX_DELAY);
				break;
			}
			case Enable_Pin://打开或关闭蓝牙模
			{
				HAL_GPIO_TogglePin(EN_GPIO_Port, EN_Pin);
				int IfEn=HAL_GPIO_ReadPin(EN_GPIO_Port,EN_Pin);
				if (IfEn==0)
				{
					HAL_UART_Transmit(&huart1, (uint8_t*)DisBluetooth, strlen(DisBluetooth), HAL_MAX_DELAY);
				}
				else
				{
					HAL_UART_Transmit(&huart1, (uint8_t*)EnBluetooth, strlen(EnBluetooth), HAL_MAX_DELAY);
				}
				return;
			}
		}

	}
}


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
  LCD_Init();
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rcv_buffer_user, 1);
  HAL_UART_Receive_IT(&huart2, (uint8_t *)rcv_buffer_bluetooth, 1);
  HAL_TIM_Base_Start_IT(&htim3);

  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

  timer = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
	  HAL_Delay(1000);
	  /*HAL_UART_Transmit(&huart1, "new line0\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[0], strlen(l[0]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line1\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[1], strlen(l[1]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line2\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[2], strlen(l[2]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line3\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[3], strlen(l[3]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line4\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[4], strlen(l[4]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line5\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[5], strlen(l[5]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line6\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[6], strlen(l[6]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line7\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[7], strlen(l[7]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line8\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[8], strlen(l[8]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line9\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[9], strlen(l[9]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line10\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[10], strlen(l[10]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line11\n", 11, 0xffff);
	  HAL_UART_Transmit(&huart1, l[11], strlen(l[11]), 0xffff);
	  HAL_UART_Transmit(&huart1, "new line12\n", 11, 0xffff);*/
      /* USER CODE END WHILE */
  	  char msg[20];
  	  char btmsg[20];
  	  char cmdmsg[20];
  	  sprintf(btmsg, "BlueTooth:%s", bt);
  	  sprintf(msg, "status:%s", sta);
  	  sprintf(cmdmsg, "msg:%s", cmd);
  	  POINT_COLOR = BLACK;
  	  LCD_DrawRectangle(5, 5, 235, 65);
  	  LCD_ShowString(6, 6, 228, 16, 16, (uint8_t*) btmsg);
  	  LCD_ShowString(6, 26, 228, 16, 16, (uint8_t*) msg);
  	  LCD_ShowString(6, 46, 228, 16, 16, (uint8_t*) cmdmsg);
  	  LCD_DrawRectangle(5, 65, 235, 315);
//  	 HAL_UART_Transmit(&huart1, "First char\r\n", 20, 0xffff);
  	  /*for (int i=0;i<12;i++){
//  		  HAL_UART_Transmit(&huart1, l[i][0], 3, 0xffff);
  		  if(l[i][0]==' '){
  			  POINT_COLOR=BLUE;
  		  }else{
  			  POINT_COLOR=RED;
  		  }
			  LCD_ShowString(6,66+20*i,228,16,16,l[i]);
  	  }*/
  	  	  	  if(l[0][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 66, 228, 16, 16, (uint8_t*) l[0]);
  		  	  if(l[1][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 86, 228, 16, 16, (uint8_t*) l[1]);
  		  	  if(l[2][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 106, 228, 16, 16, (uint8_t*) l[2]);
  		  	  if(l[3][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 126, 228, 16, 16, (uint8_t*) l[3]);
  		  	  if(l[4][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 146, 228, 16, 16, (uint8_t*) l[4]);
  		  	  if(l[5][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 166, 228, 16, 16, (uint8_t*) l[5]);
  		  	  if(l[6][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 186, 228, 16, 16, (uint8_t*) l[6]);
  		  	  if(l[7][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 206, 228, 16, 16, (uint8_t*) l[7]);
  		  	  if(l[8][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 226, 228, 16, 16, (uint8_t*) l[8]);
  		  	  if(l[9][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 246, 228, 16, 16, (uint8_t*) l[9]);
  		  	  if(l[10][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 266, 228, 16, 16, (uint8_t*) l[10]);
  		  	  if(l[11][0]==' '){
  		  		POINT_COLOR = BLUE;
  		  	  }else{
  		  		POINT_COLOR = RED;
  		  	  }
  		  	LCD_ShowString(6, 286, 228, 16, 16, (uint8_t*) l[11]);
  	//LCD_ShowString(6,66+20*3,228,16,16,l[3]);

  	  	POINT_COLOR = GRAY;
  	  	if(icnt<12){
  	  		int nl=0;
  	  		for(int i=0;i<snumLength;i++){
  	  			nl+=snum[i];
  	  			LCD_DrawLine(6,65+20*nl,234,65+20*nl);
  	  		}
  	  	}else{
  	  		int nl=0;
  	  		/*for(int i=0;i<snumLength;i++){
  	  			LCD_DrawLine(6,305-20*nl,234,305-20*nl);
  	  			nl+=snum[snumLength-1-i];
  	  			if(nl>12){
  	  				break;
  	  			}
  	  		}*/
  	  		int j=0;
  	  		for(int i=0;i<12;i++){
  	  			if(i==nl){
  	  				POINT_COLOR=GRAY;
  	  				LCD_DrawLine(6,305-20*i,234,305-20*i);
  	  				nl+=snum[snumLength-1-j];
  	  				j++;
  	  			}else{
  	  				POINT_COLOR=WHITE;
  	  				LCD_DrawLine(6,305-20*i,234,305-20*i);
  	  			}
  	  		}
  	  	}
      /* USER CODE BEGIN 3 */
    }
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2499;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  huart1.Init.BaudRate = 38400;
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
  huart2.Init.BaudRate = 38400;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Enable_Pin */
  GPIO_InitStruct.Pin = Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Enable_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STATE_Pin */
  GPIO_InitStruct.Pin = STATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY0_Pin */
  GPIO_InitStruct.Pin = KEY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY1_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EN_Pin */
  GPIO_InitStruct.Pin = EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
