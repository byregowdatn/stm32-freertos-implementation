#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  char prompt[] = "Enter Password:\r\n";
  char correct[] = "Password Correct\r\n";
  char wrong[] = "Wrong Password\r\n";

  char rx_buffer[20];
  char correct_pass[] = "1234";

  while (1)
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)prompt, strlen(prompt), HAL_MAX_DELAY);

    memset(rx_buffer, 0, sizeof(rx_buffer));

    HAL_UART_Receive(&huart2, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);

    if (strncmp(rx_buffer, correct_pass, strlen(correct_pass)) == 0)
    {
      HAL_UART_Transmit(&huart2, (uint8_t*)correct, strlen(correct), HAL_MAX_DELAY);
    }
    else
    {
      HAL_UART_Transmit(&huart2, (uint8_t*)wrong, strlen(wrong), HAL_MAX_DELAY);
    }

    HAL_Delay(1000);
  }
}

/* ================= CLOCK CONFIG ================= */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* ================= ERROR HANDLER ================= */

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
