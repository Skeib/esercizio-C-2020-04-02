#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h> // errno
#include <signal.h> // signal

#define NUM_CHARS 80

//il programma legge caratteri da stdin.
//Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array.
//Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
//che fa delle analisi che scrive su stdout (analisi sotto riportate).
//
//Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:
//
//fatta l'analisi e scritto i risultati , il processo figlio termina.


typedef		//struttura definita per il calcolo dell'occorrenza dei termini
		struct {
			char carattere;
			unsigned int occorrenze;
			} char_int;

unsigned int occourence(char c, char * array){

	int posto=0;

	for(int i=0; i<NUM_CHARS; i++){

		if(array[i] == c){
			posto = i;
		}
	}
	return posto;
}

char minimum(char * array){
	//carattere minimo (mostrare il primo minimo)

	unsigned int c;
	c = (unsigned int)array[0];
	char retr;

	for(int i=0; i<NUM_CHARS; i++){
		unsigned int d = (unsigned int)array[i];

		if(d < c){
			c = d;
			retr = array[i];
		}
	}
	return retr;
}


char maximum(char * array){
	//carattere massimo (mostrare il primo massimo)

	unsigned int c;
	c = (unsigned int)array[0];
	char retr;

	for(int i=0; i<NUM_CHARS; i++){
		unsigned int d = array[i];

		if(d>c){
			c = d;
			retr = array[i];
		}
	}
	return retr;
}

int cercatore(char * array, char trova){

	int prima_occorrenza = 0;
	int i=0;

	while(prima_occorrenza == 0){

		if(array[i] == trova){
			prima_occorrenza = i;
		}else
			i++;
	}
	return prima_occorrenza;
}


char_int occurence(char * char_occur, int num){

	char_int char_arr[NUM_CHARS] = { 0 };

	int count = 0;

	//char_arr[0].carattere = char_occur[0];
	//char_arr[0].occorrenze = 1;

	for(int i=0; i<NUM_CHARS; i++){

		count = 0;

		while(count < NUM_CHARS){
			if(char_arr[i].carattere == char_occur[count]){

				char_arr[i].occorrenze++;
				count++;
			}
		}
	}

	char_int carattere = char_arr[0];

	if(num==1){	//cerco il massimo
	//carattere con più occorrenze (il primo, a parità di occorrenze)

		for(int i=1; i<count; i++){

			if(carattere.occorrenze < char_arr[i].occorrenze)
				carattere = char_arr[i];

		}
	}
	if(num ==0){//cerco il minimo
//carattere con meno occorrenze (il primo, a parità di occorrenze)

		for(int i=1; i<count; i++){

			if(carattere.occorrenze > char_arr[i].occorrenze)
				carattere = char_arr[i];

		}
	}
	return carattere;
}

int main(){

	char * char_array;
	char c;

	char_array = calloc(NUM_CHARS, sizeof(char));

	unsigned int i=0;
	unsigned int conta_figli =0;

	while((c = getchar()) != EOF){

		char_array[i] = c;

		i++;
		char min, max;
		char_int occur_min;
		char_int occur_max;



		if(i == NUM_CHARS-1){

			switch(fork()){

			case 0:

				min = minimum(char_array);
				max = maximum(char_array);

				occur_min = occurence(char_array, 0);

				occur_max = occurence(char_array, 1);

				printf("Il carattere minimo è %c ed occorre alla posizione: %u\n", min, cercatore(char_array, min));

				printf("Il carattere massimo è %c ed occorre alla posizione: %u\n", max, cercatore(char_array, max));

				printf("Il carattere che occorre più spesso è %c ed occorre alla posizione: %u\n",occur_max.carattere,occur_max.occorrenze);

				printf("Il carattere che occorre meno spesso è %c ed occorre alla posizione: %u\n",occur_min.carattere, occur_min.occorrenze);

				conta_figli++;
				exit(0);
				break;
			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");

				exit(1);
				break;
			default :
				free(char_array);
				char_array = calloc(NUM_CHARS, sizeof(char));
				i=0;
				break;
			}

		}
	}

//Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.

	printf("il processo padre ha lanciato: %u processi figli", conta_figli);

	return 0;

}
