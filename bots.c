#include "bots.h"
#include "fitxes.h" //per la constant MAX_N
#include <stdio.h>

int retornar_millor_fitxa(t_partida partida, t_conjunt tirades_possibles){
	//Tota la funció gira entorn a partida.taula;
	int n_extrem_dreta;
	int n_extrem_esquerra;
	int n_fitxes;
	int fitxa_a_tirar = 0; //Si no sabem quina tirar, tirem la primera fitxa
	int i;
	int n_nombres[MAX_N+1]; //Perque hi haurà MAX_n + 1 nombres, 0,1,2,3,4,5,6 per ex amb MAX_N = 6
	int max_i;

	n_fitxes = tirades_possibles.n_fitxes;
	
	if(n_fitxes == 1){ //Si nomes pots tirar 1 fitxa...
		return 0;
	} //Si hi ha més d'una fitxa per tirar.

	/*
	Primer de tot intentem tirar una fitxa de manera que els dos extrems tinguin
	el mateix nombre. D'aquesta manera, serà més difícil pel següent jugador tirar i potser
	haurà d'anar a la pila
	*/

	n_extrem_esquerra = partida.taula.fitxes[0].n1;
	n_extrem_dreta = partida.taula.fitxes[partida.taula.n_fitxes-1].n2;

	for(i=0; i < n_fitxes; i++){
		//Si podem tirar la fitxa per la esquerra
		if(tirades_possibles.fitxes[i].p == 'e'){
			//Si la cara que quedara oberta es igual al extrem oposat
			if(tirades_possibles.fitxes[i].n1 == n_extrem_dreta){
				return i;
			}
		}

		//Si podem tirar la fitxa per la dreta
		if(tirades_possibles.fitxes[i].p == 'd'){
			if(tirades_possibles.fitxes[i].n2 == n_extrem_esquerra){
				return i;
			}
		}
	}
	printf("No hi ha cap extrem igual\n");
	/*
	Si l'algoritme anterior ha fallat, que ho farà en la majoria de casos ja que no sempre
	podrem fer coincidir dos extrems, el programa contarà quin es el nombre que ha aparegut 
	més cops i intentarà tirar una fitxa amb aquell nombre, així algú altra que tingui una 
	fitxa amb elmateix nombre tindrà menys probabilitats de poder tirar-la.
	*/
	for(i=0; i <= MAX_N; i++){ //inicialitzem el vector tot a 0.
		n_nombres[i] = 0;
	}

	for(i=0; i < partida.taula.n_fitxes; i++){
		n_nombres[partida.taula.fitxes[i].n1]++;
		n_nombres[partida.taula.fitxes[i].n2]++;
	}
	//Busquem la posicio del nombre maxim
	for(i=0; i <= MAX_N; i++){
		if(i==0 || n_nombres[i] > n_nombres[max_i]);
			max_i = i;
	}

	//ara comprovem si te aquest nombre a les possibles tirades
	for(i=0; i < n_fitxes; i++){
		if(tirades_possibles.fitxes[i].n1 == max_i || tirades_possibles.fitxes[i].n2 == max_i){
			return i;
		}
	}

	//Sino, doncs tirem la primera fitxa.
	return 0;
}