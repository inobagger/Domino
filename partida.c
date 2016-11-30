#include <stdio.h>
#include "partida.h"
#include "fitxes.h"

void repartir_fitxes(t_partida *partida){
	//funcio que donada una partida, reparteix les fitxes de la pila central. FITXES_JUGADOR = 7 per a cada jugador
	int i, j, r;
	for(i=0; i < partida->n_jugadors; i++){ //per a cada jugador
		partida->conjunts_jug[i].n_fitxes = 0;
		for(j=0; j < FITXES_JUGADOR; j++){ //per a cada fitxa que li toca al jugador
		   	agafar_fitxa_random(&partida->conjunts_jug[i], &partida->pila_central);
		}
	}
}

void imprimir_partida(t_partida partida){
	//funció per imprimir la partida  del domino (pila central, taula i piles dels jugadors)
	int i;
	printf("Pila central: ");
	if(partida.omnipresencia){ //si te ominiscencia
		print_conjunt(partida.pila_central);
	}else{
		print_conjunt_secret(partida.pila_central);
	}
	printf("\n");
	printf("Taula: ");
	print_conjunt(partida.taula); //Imprimeix la taula (fitxes del mig)
	printf("\n\n");

	
	for(i=0; i < partida.n_jugadors; i++){ //per a cada jugador
		printf("Pila de J%d: ", i);
		if(i == 0 && partida.jugador_huma){ //el jugador huma sempre estara a la posicio 0.
			print_conjunt(partida.conjunts_jug[i]);
			printf("Huma");
		}else{
			if(partida.omnipresencia){ //si te omnipresencia
				print_conjunt(partida.conjunts_jug[i]);
			}else{
				print_conjunt_secret(partida.conjunts_jug[i]);
			}
			printf("Robot");
		}
		printf("\n");
	}
	printf("\n");
}

void tirar_primera_fitxa(t_partida *partida){
	//busca el doble mes alt dels jugadors i el tira
	int i, j;
	int jugador_tirador = -1;
	t_fitxa max = {MAX_N, MAX_N}; // estableix el mes alt com el (6,6)
	t_fitxa actual;
	do{
		for(i=0; i < partida->n_jugadors; i++){
			//per cada jugador de la partida
			for(j=0; j < partida->conjunts_jug[i].n_fitxes; j++){
				//per cada fitxa
				if(max.n1 == partida->conjunts_jug[i].fitxes[j].n1 && 
					max.n2 == partida->conjunts_jug[i].fitxes[j].n2){
					jugador_tirador = i;
					//si algu te la fitxa el (6,6) (en la primera iteracio)
				}
			}
		}
		max.n1--; // si ningu te el (6,6), passa a ser el (5,5),
		max.n2--;
	}while( (jugador_tirador == -1) && (max.n1 != -1 && max.n2 != -1)); 
	//El bucle es trenca quan ja sha trobat un jugador amb un maxim o quan ningu te dobles
	
	if(max.n1 == -1 && max.n2 == -1){ //si ningu te dobles
		printf("No hi ha cap doble a tirar! :O\n");
		scanf("*c");
		//???
	}else{
		max.n1++; //es suma 1 perque despres de la comprovacio li hem restat 1 a la fitxa
		max.n2++;
		afegir_fitxa_final(&partida->taula, max);
		eliminar_fitxa(&partida->conjunts_jug[jugador_tirador], max);
	}
	
}

void moure_fitxa_taula(t_partida *partida, t_fitxa f, int jugador){
	// suposem que la fitxa passada es pugui posar i evitem comprovar-ho
	int n3; //per girar la fitxa si fa falta
	t_fitxa colindant; //fitxa que esta a l'extrem on posarem la fitxa
	eliminar_fitxa(&partida->conjunts_jug[jugador], f); //eliminem la fitxa del conjunt del jugador
	if(f.p == 'e'){ //si es posa a l'esquerra
		colindant = partida->taula.fitxes[0];
		if(colindant.n1 != f.n2){ //si les cares no coincideixen invertim la fitxa
			n3 = f.n2;
			f.n2 = f.n1;
			f.n1 = n3;
		}
		afegir_fitxa_principi(&partida->taula, f);
	}else if(f.p == 'd'){ //si es posa a la dreta
		colindant = partida->taula.fitxes[partida->taula.n_fitxes-1]; //la ultima fitxa
		if(colindant.n2 != f.n1){
			n3 = f.n2;
			f.n2 = f.n1;
			f.n1 = n3;
		}
		afegir_fitxa_final(&partida->taula, f);
	}
	printf("Fitxa tirada: ");
	print_fitxa(f);
	printf("\n");
}


int no_puc_tirar(t_partida *partida, int jugador){
	//retorna 1 si tira fitxa i 0 si la pila esta buida.
	t_conjunt provar_a_tirar;
	t_conjunt tirades_possibles;
	t_fitxa fitxa_agafada;
	tirades_possibles.n_fitxes = 0;
	printf("No pots tirar. \n");      
	while(partida->pila_central.n_fitxes > 0 && tirades_possibles.n_fitxes == 0){
		//metre que hi hagi fitxes a la pila central i no puguis tirar res
		//agafem una fitxa random de la pila
		fitxa_agafada = agafar_fitxa_random(&partida->conjunts_jug[jugador], &partida->pila_central);
		printf("Agafes fitxa de la pila: ");
		if(partida->omnipresencia || (jugador==0 && partida->jugador_huma))
			print_fitxa(fitxa_agafada);
		else
			printf("?:?|");
		printf("\n");
		//provem a veure si es pot posar a la partida
		provar_a_tirar.n_fitxes = 1;
		provar_a_tirar.fitxes[0] = fitxa_agafada;
		tirades_possibles = retornar_tirades_possibles(provar_a_tirar, partida->taula);
		if(tirades_possibles.n_fitxes > 0){
			//si es pot la posem i trenquem el bucle amb el return 0;
			//realment la segona comprovacio del while no faria falta
			printf("La poses a la pila, ");
			moure_fitxa_taula(partida, tirades_possibles.fitxes[0], jugador);
			return 0;
		}
	}
	if(partida->pila_central.n_fitxes == 0){
		//si la pila esta buida
		printf("La pila esta buida. \n");
		return 1;
	}
}