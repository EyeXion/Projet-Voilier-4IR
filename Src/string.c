int Concatenate(char * chaine1, char * chaine2, int sizechaine1, int sizechaine2, int sizemax) {
	if ((sizechaine1 + sizechaine2 - 1) <= sizemax) {
		int j;
		for (j=0; j < sizechaine1; j++) {
			chaine2[(sizechaine2 - 1 )+ j] = chaine1[j];
		}
		return sizechaine1 + sizechaine2 - 1;
	} else {
		return 0;
	}
}

int size_of(char * chaine) {
	int i = 0;
	while (chaine[i] != '\0') {
		i++;
	}
	
	return i+1;
}
