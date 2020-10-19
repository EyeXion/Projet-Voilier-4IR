#include "Allure.h"
#include "AntiChavirement.h"
#include "Batterie.h"
#include "Gouvernail.h"
#include "Heure.h"
#include "Transmission.h"
#include "Voile.h"


/**
	* @brief configure tous les periphs qu'on va utiliser en appelant les 
					 fonctions de config de chaque "module"
  * @note   
	* @param None
  * @retval None
  */
void Setup(){
	ConfAllure();
	ConfVoile();
	ConfGouvernail();
	ConfAntiChavirement();
	
}


//on utilise EnvoiRegulier de Transmission 
//TODO : r�cuperer les  valeurs des params de EnvoiRegulier
//Interruption toute les 3s via un Timer qui d�clenche cette fonction qui permet d'envoyer ce qu'il faut

/**
	* @brief  
  * @note   
	* @param  
  * @retval 
  */
void Envoi3s(){
	
}

/**
	* @brief  tache qui va fonctionner en fond qui gere l'orientation
						des voiles, le gouvernail et aussi le systeme anti-chavirement
  * @note  relacher les voiles <=> tendre les voiles avec un angle � 90� 
	* @param None
  * @retval None
  */
void Background(){
	//Voiles
	int allure = RecupAllure(); //on recupere l'allulre via la girouette
	int tensionVoile = CalculerTension(allure); //grace � l'allure on peut calculer la tension � appliquer sur la voile
	TendreVoile(tensionVoile); //on tend la voile gr�ce � la tension obtenue
	
	//Gouvernail 
	int valeurTelecommande = LireTelecommande(); //on lit la valeur renvoy�e par la t�l�commande
	int vitesse = CalculerVitesse(valeurTelecommande);//gr�ace � la valeur de la telecommande on calcule la vitesse souhait�e
	CommanderMoteur(vitesse);//On commande le moteur pour aller � la vitesse voulue
	
	//Anti-Chavirement
	int rouli = RecupRouli();
	int danger = CalculerDanger(rouli);//renvoi boolean : int � 0 si faux et 1 si vrai
	if(danger){
		TendreVoile(90); //si il y a danger on relache les voiles (relacher les voiles = les mettre � 90)
	}
	
}

int main(){
	Setup();
	while(1){
		Background();
	}
	return 0;
}