#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_CHARS 80

//il programma svolge le attività sotto descritte fino a quando incontra EOF su stdin.
//
//1: il programma legge caratteri da stdin.
//Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.
//
//Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array.
//(introdurre nel codice un #define NUM_CHARS 80).
//
//Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
//che fa delle analisi che scrive su stdout (analisi sotto riportate).
//
//Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:
//
//
//*analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
//carattere minimo (mostrare il primo minimo, si applica a chi risolve dal 3/4)
//carattere massimo (mostrare il primo massimo, si applica a chi risolve dal 3/4)
//carattere con più occorrenze (il primo, a parità di occorrenze, si applica a chi risolve dal 3/4)
//carattere con meno occorrenze (il primo, a parità di occorrenze, si applica a chi risolve dal 3/4)
//
//fatta l'analisi e scritto i risultati , il processo figlio termina.

char char_array[NUM_CHARS];

void fork_creator();
void analisi_figlio();
unsigned int child_counter = 0;

int main(int argc, char *argv[]){


	int c;

	char * ptr;
	char * end;

	ptr = char_array;

	end = &char_array[NUM_CHARS-1];

	while((c=getchar())!=EOF){

		*ptr = c;

		if(ptr == end){
			fork_creator();

			child_counter++;
			ptr = char_array;

			memset(char_array, 0, NUM_CHARS*sizeof(char));
		}
		else{
			ptr++;
		}
	}

	printf("Il processo padre ha lanciato %u processi figli", child_counter);

	return 0;
}

void fork_creator(){

	switch(fork()){

	case 0:
		analisi_figlio();
		break;
	case -1:
		perror("fork()");
		exit(0);
		break;
	default:
		//
		break;

	}
}

void analisi_figlio(){

//*analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
//carattere minimo (mostrare il primo minimo)
//carattere massimo (mostrare il primo massimo)
//carattere con più occorrenze (il primo, a parità di occorrenze)
//carattere con meno occorrenze (il primo, a parità di occorrenze)


	int minimo = 256;
	int massimo = -1;

	int car;

	int pos_min;
	int pos_max;

	char char_occurrence[256] = { 0 };

	for(int i =0; i<NUM_CHARS; i++){

		car = (int)char_array[i] & 0xFF;

		if(minimo > car){
			minimo = car;
			pos_min = i;
		}
		if(massimo < car){
			massimo=car;
			pos_max = i;
		}

		char_occurrence[car]++;

	}

	int freq_max = -1;
	int freq_min = 81;

	char car_fr_max;
	char car_fr_min;

	for(int i=0; i< 256; i++){

		if(freq_max < char_occurrence[i]){

			freq_max = char_occurrence[i];
			car_fr_max = i;

		}

		if(freq_min > char_occurrence[i] && char_occurrence[i]>0){

			freq_min = char_occurrence[i];
			car_fr_min = i;

		}
	}
	freq_max = 80;
	freq_min = 80;

	for(int i =0; i<NUM_CHARS; i++){

		if(car_fr_max == char_array[i] && freq_max>i){
			freq_max = i;
		}

		if(car_fr_min == char_array[i] && freq_min>i)
			freq_min = i;

	}


	printf("Il carattere minimo è %c, ed occorre per la prima volta alla posizione: %d\n", minimo, pos_min);

	printf("Il carattere massimo è %c, ed occorre per la prima volta alla posizione: %d\n", massimo, pos_max);

	printf("Il carattere che compare con maggior frequenza è %c, ed occorre per la prima volta alla posizione: %d\n", car_fr_max, freq_max);

	printf("Il carattere che compare con minor frequenza è %c, ed occorre per la prima volta alla posizione: %d\n", car_fr_min, freq_min);


	exit(0);
}

