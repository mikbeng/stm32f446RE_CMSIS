/*
 * main.c
 *
 *  Created on: 28 dec. 2018
 *      Author: mikae
 */

#include "stm32f446xx.h"

#define LED_GPIO_PIN 5
#define LED_GPIO_PORT GPIOA

void GPIO_LED_Init(void)
{
	/* Enable GPIOA clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Set direction */
	LED_GPIO_PORT-> MODER |= GPIO_MODER_MODER5_0;

    /* Speed mode configuration */
	LED_GPIO_PORT->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;			//0x03 = high speed

    /* Output mode configuration*/
	LED_GPIO_PORT->OTYPER &= ~(GPIO_OTYPER_OT_5);

    /* Pull-up Pull down resistor configuration*/
	LED_GPIO_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR5);	//Disable pull up/pull down
}

void delay (int a)
{
    volatile uint32_t i;
    for (i=0 ; i < a ; i++){
    }
    return;
}

static void SystemClock_Config(void)
{
	const uint32_t PLL_M=8;
	const uint32_t PLL_N=192;
	const uint32_t PLL_P=8;
	const uint32_t PLL_Q=8;


	/******************************************************************************/
	/*            PLL (clocked by HSI) used as System clock source                */
	/******************************************************************************/

  /* At this stage the HSI is already enabled and used as System clock source */

    /* Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

  /* HCLK = SYSCLK / 1*/
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

  /* PCLK2 = HCLK / 1*/
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

  /* PCLK1 = HCLK / 1*/
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

  /* Configure the main PLL */
  RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                 (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24);

  /* Enable the main PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till the main PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0)
  {
  }

  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_1WS;

  /* Select the main PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  /* Wait till the main PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {
  }
}

int main()
{
	SystemClock_Config();
	SystemCoreClockUpdate();

	GPIO_LED_Init();

	while (1){
		LED_GPIO_PORT->BSRR = GPIO_BSRR_BR_5;
		delay(1600000);
		LED_GPIO_PORT->BSRR = GPIO_BSRR_BS_5;
		delay(1600000);
	}
	return 0;
}
