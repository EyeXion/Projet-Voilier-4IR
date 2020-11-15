#include "Voile.h"
#include "stm32f1xx_ll_gpio.h"
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

//Valeurs de CCR1 pour la PWM output
#define POSITION_00 1000
#define POSITION_10 1100
#define POSITION_20 1200
#define POSITION_30 1300
#define POSITION_40 1400
#define POSITION_50 1500
#define POSITION_60 1600
#define POSITION_70 1700
#define POSITION_80 1800
#define POSITION_90 1900


int angleVoileActuel = 0; //angle de la voile. Mis à jour quans on la tend. de 0 à  45.
 
 

void ConfVoile(void){
	 LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); //Enable l'horloge du GPIOA
	 LL_TIM_SetAutoReload(Timer, ARR); //On règle l'ARR
	 LL_TIM_SetPrescaler(Timer, PSC); //On règle le PSC
	 LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE); //Pin en mode output alternate
	 LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8,LL_GPIO_OUTPUT_PUSHPULL); //Pin en mode output pushpull
	 LL_TIM_OC_SetMode(Timer, channel, LL_TIM_OCMODE_PWM1); //On met le timer en mode PWM1 sur le channel choisi
	 Timer->CCER |= TIM_CCER_CC1E; //On enable le comptage dans le timer en output
	 Timer->BDTR |= 0x1 << 15; //Comme on utilise le Timer1, on met le bit MOE à 1
	 Timer->CCR1 = 1500; // par defaut tendu à 1ms 
	 LL_TIM_EnableCounter(Timer); //On commence le comptage
}


int RecupTensionVoile(void){
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
	if (theta < 5){
		Timer->CCR1 = POSITION_00;
		angleVoileActuel = 0;
	}
	if (theta < 10){
		Timer->CCR1 = POSITION_10;
		angleVoileActuel = 10;
	}
	else if(theta < 20){
		Timer->CCR1 = POSITION_20;
		angleVoileActuel = 20;
		}
	else if(theta < 30){
		Timer->CCR1 = POSITION_30;
		angleVoileActuel = 30;
		}
	else if(theta < 40){
		Timer->CCR1 = POSITION_40;
		angleVoileActuel = 40;
		}
	else if(theta < 50){
		Timer->CCR1 = POSITION_50;
		angleVoileActuel = 50;
		}
	else if(theta < 60){
		Timer->CCR1 = POSITION_60;
		angleVoileActuel = 60;
		}
	else if(theta < 70){
		Timer->CCR1 = POSITION_70;
		angleVoileActuel = 70;
		}
	else if(theta < 80){
		Timer->CCR1 = POSITION_80;
		angleVoileActuel = 80;
		}
	else{
		Timer->CCR1 = POSITION_90;
		angleVoileActuel = 90;
	}
	
	
	//Timer->CCR1 = (int)(A_BETA_TO_GAMMA * (float)theta + B_BETA_TO_GAMMA) ;
}

char * TensionVoileToString(int theta) {
	return "";
}
