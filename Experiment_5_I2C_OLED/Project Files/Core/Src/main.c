/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include <string.h>

/* Private function prototypes -----------------------------------------------*/
void Error_Handler(void);
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void OLED_SetCursor(uint8_t page, uint8_t col);
void OLED_Command(uint8_t cmd);
void OLED_Data(uint8_t *data, uint16_t size);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define OLED_ADDR (0x3C << 1)

// ---------- LOW LEVEL FUNCTIONS ----------

void OLED_Command(uint8_t cmd)
{
  uint8_t data[2] = {0x00, cmd};
  HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, data, 2, HAL_MAX_DELAY);
}

void OLED_Data(uint8_t *data, uint16_t size)
{
  uint8_t temp[size + 1];
  temp[0] = 0x40;
  memcpy(&temp[1], data, size);
  HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, temp, size + 1, HAL_MAX_DELAY);
}

void OLED_Init()
{
  HAL_Delay(200);

  OLED_Command(0xAE);
  OLED_Command(0x20);
  OLED_Command(0x00);
  OLED_Command(0xB0);
  OLED_Command(0xC8);
  OLED_Command(0x00);
  OLED_Command(0x10);
  OLED_Command(0x40);
  OLED_Command(0x81);
  OLED_Command(0xFF);
  OLED_Command(0xA1);
  OLED_Command(0xA6);
  OLED_Command(0xA8);
  OLED_Command(0x3F);
  OLED_Command(0xA4);
  OLED_Command(0xD3);
  OLED_Command(0x00);
  OLED_Command(0xD5);
  OLED_Command(0xF0);
  OLED_Command(0xD9);
  OLED_Command(0x22);
  OLED_Command(0xDA);
  OLED_Command(0x12);
  OLED_Command(0xDB);
  OLED_Command(0x20);
  OLED_Command(0x8D);
  OLED_Command(0x14);
  OLED_Command(0xAF);
}

void OLED_Clear()
{
  uint8_t buffer[128];
  memset(buffer, 0, 128);

  for (int i = 0; i < 8; i++)
  {
    OLED_Command(0xB0 + i);
    OLED_Command(0x00);
    OLED_Command(0x10);
    OLED_Data(buffer, 128);
  }
}
void OLED_DrawBigChar(uint8_t *ch, uint8_t page, uint8_t col)
{
  for(int i = 0; i < 5; i++)
  {
    uint8_t line = ch[i];

    for(int j = 0; j < 8; j++)
    {
      if(line & (1 << j))
      {
        // draw 2x2 pixels
        OLED_SetCursor(page + (j/4), col + i*2);
        uint8_t block[2] = {0xFF, 0xFF};
        OLED_Data(block, 2);
      }
    }
  }
}
void OLED_DrawBigChar3x(uint8_t *ch, uint8_t page, uint8_t col)
{
  for(int i = 0; i < 5; i++)   // columns
  {
    uint8_t line = ch[i];

    for(int j = 0; j < 8; j++) // rows
    {
      if(line & (1 << j))
      {
        // draw 3x3 block
        for(int dx = 0; dx < 3; dx++)
        {
          for(int dy = 0; dy < 3; dy++)
          {
            OLED_SetCursor(page + (j/8)*3 + dy, col + i*3 + dx);

            uint8_t pixel = 0xFF;
            OLED_Data(&pixel, 1);
          }
        }
      }
    }
  }
}
// ---------- FONT ----------
// Additional fonts
// Existing fonts (YOU FORGOT THESE)
uint8_t font_H[5] = {0x7F,0x08,0x08,0x08,0x7F};
uint8_t font_E[5] = {0x7F,0x49,0x49,0x49,0x41};
uint8_t font_L[5] = {0x7F,0x40,0x40,0x40,0x40};
uint8_t font_O[5] = {0x3E,0x41,0x41,0x41,0x3E};

// New fonts
uint8_t font_A[5] = {0x7E,0x11,0x11,0x11,0x7E};
uint8_t font_P[5] = {0x7F,0x09,0x09,0x09,0x06};
uint8_t font_Y[5] = {0x07,0x08,0x70,0x08,0x07};
uint8_t font_J[5] = {0x20,0x40,0x41,0x3F,0x01};
uint8_t font_U[5] = {0x3F,0x40,0x40,0x40,0x3F};
uint8_t font_R[5] = {0x7F,0x09,0x19,0x29,0x46};
uint8_t font_N[5] = {0x7F,0x02,0x04,0x08,0x7F};
uint8_t font_B[5] = {0x7F,0x49,0x49,0x49,0x36};
uint8_t font_space[5] = {0x00,0x00,0x00,0x00,0x00};

uint8_t font_S[5] = {0x46,0x49,0x49,0x49,0x31};
uint8_t font_K[5] = {0x7F,0x08,0x14,0x22,0x41};

// ---------- HIGH LEVEL ----------

void OLED_SetCursor(uint8_t page, uint8_t col)
{
  OLED_Command(0xB0 + page);
  OLED_Command(0x00 + (col & 0x0F));
  OLED_Command(0x10 + (col >> 4));
}

void OLED_PrintText()
{
  OLED_Clear();
  OLED_SetCursor(0, 0);

  uint8_t *msg[] = {
    font_H, font_E, font_L, font_L, font_O,
    font_space,
    font_S, font_A, font_S, font_K, font_E, font_N
  };

  for(int i = 0; i < 12; i++)
  {
    OLED_Data(msg[i], 5);

    uint8_t space = 0x00;   // spacing column
    OLED_Data(&space, 1);
  }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  HAL_Delay(200);
  OLED_Init();
  OLED_Clear();
  OLED_PrintText();
  /* USER CODE END 2 */

  while (1)
  {
  }
}

/* System Clock Configuration (keep your existing one) */
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

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}
