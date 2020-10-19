
#include "Voile.h"
#include "stm32f1xx_ll_gpio.h"
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


int main(){
	
	
	ConfVoile();
	
	while (1){}
	
}