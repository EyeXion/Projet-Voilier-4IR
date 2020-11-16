#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "Allure.h"
#include "Voile.h"
#include "string.h"

#define TAILLE_MESSAGE_MAX 100
#define USARTX USART1

// Structure pour gérer la transmission du message
struct t_transmission {
	char message[TAILLE_MESSAGE_MAX];// Le message lui même
	int position;// La position du caractere a envoyer
	int taille_message; //On peut envoyer des messages de tailles variables, il faut donc spécifier la taille
	char envoyer;// Booléen utilisé comme flag -> vrai on envoie, faux on fait rien
	char exceptionnel; //Booléen utilisé comme flag -> vrai on envoie un message excep donc, on ne fait pas l'envoi regulier
};

/**
	* @brief  Initialise une structure t_transmission.
  * @note   Structure initialisée :  (message -> 00:00:00:, end_of_message -> 0x0D, position -> 0, first_appel -> 1)
	* @param  @ de la structure a initialiser
  * @retval None
  */
void init_t_transmission(struct t_transmission * transmission) {
	int i;
	for (i=0; i<TAILLE_MESSAGE_MAX; i++) {
		transmission->message[i] = '0';
	}
	transmission->message[TAILLE_MESSAGE_MAX - 1] = '\0';
	transmission->position = 0;
	transmission->taille_message = 0;
	transmission->envoyer = 0;
	transmission->exceptionnel = 0;
}

static struct t_transmission transmission;

void ConfTransmission(){
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); //On enable la clock pour l'USARt	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); //enable la clock du gpio où est l'USART
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE); // Mode fonction alternative PIN USART Tx
	
	LL_USART_InitTypeDef USART_InitStruct;
	LL_USART_StructInit(&USART_InitStruct);
	USART_InitStruct.BaudRate = 9600; //Set Baud rate -> 19200Bd
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B; //Set data width -> 8bits
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;// Disable Parity
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;// Set stop bit -> 1
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;//Set sens -> TX Only
	LL_USART_Init(USARTX, &USART_InitStruct);// Applique les modifs
	LL_USART_Enable(USARTX);// Rend l'USART enable A FAIRE EN DERNIER
	
	
	/*LL_USART_EnableDirectionTx(USARTX); //Enable direction Tx
	LL_USART_SetParity(USARTX,LL_USART_PARITY_NONE); // disable parity bit
	LL_USART_SetStopBitsLength(USARTX,LL_USART_STOPBITS_1);
	LL_USART_SetBaudRate(USARTX, 72000000,9600); //Set Baud Rate à 9600 (règle de trois avec 1 pour 36 000 0000
	LL_USART_Enable(USARTX); //Enable Usart*/
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_11,LL_GPIO_MODE_OUTPUT); 
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_11,LL_GPIO_OUTPUT_PUSHPULL);
	
	init_t_transmission(&transmission);
}



void EnvoiRegulier(char * Allure, char * tension){
	
	if (!transmission.exceptionnel) {
		/* Message de la forme suivante :
		_____________________________
		Allure actuelle : xxxxxxxxxxx
		Tension des voiles : xxxxxxxx
		_____________________________*/
		
		static char promptligne1[] = "Allure actuelle : ";
		static int promptligne1size = 19;
		static char promptligne2[] = "Tension des voiles : ";
		static int promptligne2size = 22;
		static char rc[] = "\n";
		static int rcsize = 2;
		
		transmission.message[0] = '\0';
		int current_size = 1;
		
		current_size = Concatenate(promptligne1, transmission.message, promptligne1size, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(Allure, transmission.message, size_of(Allure), current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(promptligne2, transmission.message, promptligne2size, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(tension, transmission.message, size_of(tension), current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		
		transmission.envoyer = 1;
		transmission.position = 0;
		transmission.taille_message = current_size;
	}
}
	
/*LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
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
}*/



void EnvoiExceptionnel(char * msgAlarme){
	
		/* Message de la forme suivante :
		_____________________________
		Allure actuelle : xxxxxxxxxxx
		Tension des voiles : xxxxxxxx
		_____________________________*/
		
		static char prompt[] = "WARNING : ";
		static int promptsize = 11;
		static char rc[] = "\n";
		static int rcsize = 2;
		
		transmission.message[0] = '\0';
		int current_size = 1;
		
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(prompt, transmission.message, promptsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(msgAlarme, transmission.message, size_of(msgAlarme), current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		current_size = Concatenate(rc, transmission.message, rcsize, current_size, TAILLE_MESSAGE_MAX);
		
		transmission.envoyer = 1;
		transmission.position = 0;
		transmission.taille_message = current_size;
		transmission.exceptionnel = 1;
	
}
	
/*LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	int tailleMessage = sizeof(msgAlarme);
	int index  = 0;
	while(index < tailleMessage){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) msgAlarme[index]); //On envoie le message (8 bits)
			index++;
		}
	} 	
	LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11);
}*/

void EnvoyerCaractere(void) {
	if (transmission.envoyer) {
		LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
		if (LL_USART_IsActiveFlag_TXE(USARTX)) {//On regarde si le flag de transmission terminée est actif
		  if (transmission.position < transmission.taille_message) {
				LL_USART_TransmitData8(USARTX, (uint8_t)(transmission.message[transmission.position]));
				transmission.position += 1;
			} else {
				transmission.envoyer = 0;
				transmission.exceptionnel = 0;
			}
		}
		LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11);
	}	
}
