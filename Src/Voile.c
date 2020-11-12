
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
#define A_ALPHA_TO_BETA (2.0/3.0) //Coefficient directeur de la partie linéaire de la fonction transformant l'allure en angle de voile
#define B_ALPHA_TO_BETA (-30.0) //Ordonnée a l'origine de cette même fonction
#define GAMMA_90 (19900) //Valeur du registre commandant la largeur de la PWM pour les voiles lachées
#define GAMMA_0 (100) //Valeur du registre commandant la largeur de la PWM pour les voiles bordées au maximum
#define A_BETA_TO_GAMMA ((GAMMA_90 - GAMMA_0)/90.0) //Coefficient directeur de la relation entre l'angle de voile et la PWM
#define B_BETA_TO_GAMMA (GAMMA_0) //Ordonnée a l'origine

int angleVoileActuel = 0; //angle de la voile. Mis à jour quans on la tend. de 0 à  45.
 
void ConfVoile(void){
	 LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	 MyTimer_Conf(Timer,ARR,PSC);
	 LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	 LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8,LL_GPIO_OUTPUT_PUSHPULL);
	 LL_TIM_OC_SetMode(Timer, channel, LL_TIM_OCMODE_PWM1);
	 Timer->CCER |= TIM_CCER_CC1E;
	 Timer->BDTR |= 0x1 << 15; 
	 Timer->CCR1 = 1500; // par defaut tendu à 1ms 
	 MyTimer_Start(Timer);
}



int RecupTension(void){
	return angleVoileActuel;
}


int CalculerTension(int alpha){
	if (abs(alpha) < 45){
		return 0;
	} else {
		float res = A_ALPHA_TO_BETA * (float)abs(alpha) + B_ALPHA_TO_BETA;
		return (int)res;
	}
}


void TendreVoile(int theta)
{
	Timer->CCR1 = (int)(A_BETA_TO_GAMMA * (float)theta + B_BETA_TO_GAMMA) ;
}
