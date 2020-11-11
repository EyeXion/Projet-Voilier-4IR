
#include "Voile.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_bus.h"
#include "maths.h"  //Remplacer par maths.h quand on va tout faire marcher.
 
#define ARR (19999)
#define PSC (71)
#define Timer (TIM1)
#define channel (1)
#define coefa (1)
#define coefb (0)
#define coefc (1)
#define coefd (0)

int angleVoileActuel = 0; //angle de la voile. Mis � jour quans on la tend. de 0 �  45.
 
void ConfVoile(void){
	 LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	 MyTimer_Conf(Timer,ARR,PSC);
	 LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	 LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8,LL_GPIO_OUTPUT_PUSHPULL);
	 LL_TIM_OC_SetMode(Timer, channel, LL_TIM_OCMODE_PWM1);
	 Timer->CCER |= TIM_CCER_CC1E;
	 Timer->BDTR |= 0x1 << 15; 
	 Timer->CCR1 = 1500; // par defaut tendu � 1ms 
		MyTimer_Start(Timer);
 }



int RecupTension(void){
	return angleVoileActuel;
	}


int CalculerTension(int alpha){
	if (abs(alpha) > 45){
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
