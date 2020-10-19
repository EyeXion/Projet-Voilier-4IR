#include "maths.h"

void ConfAllure() {
	
}

int RecupAllure() {
	return 0;
}

char * ToString(int alpha) {
	int alpha_abs = abs(alpha);
	if (alpha_abs < 45) {
		return "Vent debout";
	} else if (alpha < 55) {
		return "Près";
	} else if (alpha < 65) {
		return "Bon plein";
	} else if (alpha < 80) {
		return "Petit largue";
	} else if (alpha < 100) {
		return "Travers";
	} else if (alpha < 115) {
		return "Largue";
	} else if (alpha < 155) {
		return "Grand largue";
	} else if (alpha < 180) {
		return "Vent arrière";
	} else {
		return "ERROR";
	}		
}
