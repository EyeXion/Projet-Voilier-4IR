
#include "Voile.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_tim.h" 
#include "stdlib.h"  //Remplacer par maths.h quand on va tout faire marcher.
 
#define ARR (59999)
#define PSC (23)
#define Timer (TIM1)
#define channel (1)
#define coefa (2)
#define coefb (1)
#define coefc (2)
#define coefd (1)

int angleVoileActuel = 0; //angle de la voile. Mis à jour quans on la tend. de 0 à  45.
 
 void ConfVoile(){
	 MyTimer_Conf(Timer,ARR,PSC);
	 LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	 LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8,LL_GPIO_OUTPUT_PUSHPULL);
	 LL_TIM_OC_SetMode(Timer, channel, LL_TIM_OCMODE_PWM1);
	 Timer->CCR1 = (ARR) / 20 ; // par defaut tendu à 1ms 
		MyTimer_Start(Timer);
 }



int RecupTension(void){
	return angleVoileActuel;
	}


int CalculerTension(int alpha){
	if (0 < abs(alpha) && abs(alpha) > 45){
		return 0;
	}
	else {
		int res = alpha * coefa + coefb;
		return res;
	}
}


void TendreVoile(int theta)
{
	Timer->CCR1 = coefc * theta + coefd ;
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}

int main(){
	
	
	ConfVoile();
	
	while (1){}
	
}