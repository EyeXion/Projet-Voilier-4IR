#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "Allure.h"
#include "Voile.h"



void EnvoiRegulier(char * Allure, char * tension);

void EnvoiExceptionnel(char * msgAlarme);

void ConfTransmission();

