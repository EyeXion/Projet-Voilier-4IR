#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "Allure.h"
#include "Voile.h"

int drapeauTransmission = 1;

void ConfTransmission(){
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); //On enable la clock pour l'USARt	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); //enable la clock du gpio où est l'USART
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE); // Mode fonction alternative PIN USART Tx
	LL_USART_Enable(USART1); //Enable Usart
	LL_USART_EnableDirectionTx(USART1); //Enable direction Tx
	LL_USART_SetParity(USART1,LL_USART_PARITY_NONE); // disable parity bit
	LL_USART_SetStopBitsLength(USART1,LL_USART_STOPBITS_1);
	LL_USART_SetBaudRate(USART1, 72000000,9600); //Set Baud Rate à 9600 (règle de trois avec 1 pour 36 000 0000
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_11,LL_GPIO_MODE_OUTPUT); 
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_11,LL_GPIO_OUTPUT_PUSHPULL);
}

void EnvoiRegulier(char * Allure, char * tension){
	
	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	int tailleAllure = sizeof(Allure);
	int tailleTension = 8; //ATTENTION TAILLE
	int index  = 0;
	while(index < tailleAllure){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) Allure[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	index = 0;
	while(index < tailleTension){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) tension[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11);
}

void EnvoiExceptionnel(char * msgAlarme){
	
	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	int tailleMessage = sizeof(msgAlarme);
	int index  = 0;
	while(index < tailleMessage){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) msgAlarme[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	
}
