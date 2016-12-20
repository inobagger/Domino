#include <stdio.h>
#include "partida.h"
#include "fitxes.h"
#include "colores.h"

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
	printf_color(5);
	printf("Pila central: ");
	printf_color(6);
	if(partida.omnipresencia){ //si te ominiscencia
		print_conjunt(partida.pila_central);
	}else{
		print_conjunt_secret(partida.pila_central);
	}
	printf("\n");
	printf_color(-1);
	printf_color(5);
	printf("Taula: ");
	printf_color(1);
	if(partida.imprimir_2d){ //S'imprimeix la taula en 2 dimensions.
		printf("\n"); //per posar "Taula" adalt de la pila i que no quedi moguda.
		print_conjunt_2d(partida.taula);
	}else{
		print_conjunt(partida.taula); //Imprimeix la taula (fitxes del mig)
	}
	printf("\n\n");
	printf_color(-1);

	
	for(i=0; i < partida.n_jugadors; i++){ //per a cada jugador
		printf_color(5);
		printf("Pila de J%d: ", i);
		printf_color(-1);
		if(i == 0 && partida.jugador_huma){ //el jugador huma sempre estara a la posicio 0.
			printf_color(2);
			print_conjunt(partida.conjunts_jug[i]);
			printf("Huma");
			printf_color(-1);
		}else{
			printf_color(1);
			if(partida.omnipresencia){ //si te omnipresencia
				print_conjunt(partida.conjunts_jug[i]);
			}else{
				print_conjunt_secret(partida.conjunts_jug[i]);
			}
			printf("Robot");
			printf_color(-1);
		}
		printf("\n");
	}
	printf("\n");
}

int tirar_primera_fitxa(t_partida *partida){
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
		printf_color(5);
		printf("No hi ha cap doble a tirar! :O. Tirarem la primera fitxa del J0\n");
		printf_color(-1);
		//Tirem la primera fitxa del J0
		afegir_fitxa_final(&partida->taula, partida->conjunts_jug[0].fitxes[0]);
		eliminar_fitxa(&partida->conjunts_jug[0], partida->conjunts_jug[0].fitxes[0]);
		return 0;
	}else{
		max.n1++; //es suma 1 perque despres de la comprovacio li hem restat 1 a la fitxa
		max.n2++;
		afegir_fitxa_final(&partida->taula, max);
		eliminar_fitxa(&partida->conjunts_jug[jugador_tirador], max);
		return jugador_tirador;
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
	printf_color(5);
	printf("Fitxa tirada: ");
	printf_color(-1);
	printf_color(2);
	print_fitxa(f);
	printf_color(-1);
	printf("\n");
}


int no_puc_tirar(t_partida *partida, int jugador){
	//retorna 1 si tira fitxa i 0 si la pila esta buida.
	t_conjunt provar_a_tirar;
	t_conjunt tirades_possibles;
	t_fitxa fitxa_agafada;
	tirades_possibles.n_fitxes = 0;
	printf_color(5);
	printf("No pots tirar. \n");
	printf_color(-1); 
	while(partida->pila_central.n_fitxes > 0 && tirades_possibles.n_fitxes == 0){
		//metre que hi hagi fitxes a la pila central i no puguis tirar res
		//agafem una fitxa random de la pila
		fitxa_agafada = agafar_fitxa_random(&partida->conjunts_jug[jugador], &partida->pila_central);
		printf_color(5);
		printf("Agafes fitxa de la pila: ");
		printf_color(-1);
		if(partida->omnipresencia || (jugador==0 && partida->jugador_huma)){
			printf_color(2);
			print_fitxa(fitxa_agafada);
			printf_color(-1);
		}else{
			printf_color(1);
			printf("?:?|");
			printf_color(-1);
		}
		printf("\n");
		//provem a veure si es pot posar a la partida
		provar_a_tirar.n_fitxes = 1;
		provar_a_tirar.fitxes[0] = fitxa_agafada;
		tirades_possibles = retornar_tirades_possibles(provar_a_tirar, partida->taula);
		if(tirades_possibles.n_fitxes > 0){
			//si es pot la posem i trenquem el bucle amb el return 0;
			//realment la segona comprovacio del while no faria falta
			printf_color(5);
			printf("La poses a la pila, ");
			printf_color(-1);
			moure_fitxa_taula(partida, tirades_possibles.fitxes[0], jugador);
			return 0;
		}
	}
	if(partida->pila_central.n_fitxes == 0){
		//si la pila esta buida
		printf_color(5);
		printf("La pila esta buida. \n");
		printf_color(-1);
		return 1;
	}
}
