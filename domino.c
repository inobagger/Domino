#include <stdio.h>
#include "fitxes.h"
#include "azar.h"
#include "preguntas.h"
#include "partida.h"
#include "bots.h"
#include "duerme.h"

void main(){
	t_partida partida_1;
	t_conjunt tirades_possibles;
	int fitxa_a_tirar, n_fitxes_a_tirar;
	int ronda = 1;
	int jug;
	int bloqueig = 0;
	int primer_jugador;
	int jugador_actual;

	//Inicialitzem la partida
	inicializar_azar();
	partida_1.pila_central = generar_fitxes();
	partida_1.taula.n_fitxes = 0;
	partida_1.n_jugadors = preguntar_n_en_rango("Nombre de jugadors?", 2, 4);
	partida_1.jugador_huma = preguntar_si_o_no("Un jugador huma?");
	partida_1.omnipresencia = preguntar_si_o_no("Vols omnipresencia?");
	partida_1.imprimir_2d = preguntar_si_o_no("Vols mostrar el tauler en 2D?");
	partida_1.acabada = 0;
	repartir_fitxes(&partida_1);
	primer_jugador = tirar_primera_fitxa(&partida_1);
	jugador_actual = (primer_jugador + 1) % partida_1.n_jugadors; //el jugador 4 passaria a ser el 0
	printf("\n\nEl J%d ha tirat el primer doble i li toca tirar a J%d\n", primer_jugador, jugador_actual);
	//comencem a jugar
	while(!partida_1.acabada){
		duerme_un_rato();
		printf("\t\t\t(Ronda %d)\n", ronda);
		ronda++;
		bloqueig = 0;
		for(jug = jugador_actual; (jug < partida_1.n_jugadors) && (!partida_1.acabada); jug++){
			printf("\t\t\t(Turn del jugador J%d)\n", jug);
			imprimir_partida(partida_1);
			tirades_possibles = retornar_tirades_possibles(partida_1.conjunts_jug[jug], partida_1.taula);
			if(tirades_possibles.n_fitxes > 0){
				if(jug == 0 && partida_1.jugador_huma){
					print_conjunt_tirades(tirades_possibles);
					fitxa_a_tirar = preguntar_n_en_rango("Fitxa a tirar: ", 0, tirades_possibles.n_fitxes-1);
				}else{ 
					//Funcio a bots.c, "inteligencia artificial"
					print_conjunt_tirades(tirades_possibles);
					fitxa_a_tirar = retornar_millor_fitxa(partida_1, tirades_possibles);
				}
				moure_fitxa_taula(&partida_1, tirades_possibles.fitxes[fitxa_a_tirar], jug);
			}else{
				bloqueig = bloqueig + no_puc_tirar(&partida_1, jug);
			}
			duerme_un_rato();
			if(partida_1.conjunts_jug[jug].n_fitxes == 0 || bloqueig == partida_1.n_jugadors){
				partida_1.acabada = 1;
			}
			printf("-------------------------------------\n\n");
			jugador_actual = 0; //Ara sempre començarem pel jugador 0
		}
	}
	printf("PARTIDA ACABADA!\n");
	if(bloqueig == partida_1.n_jugadors){
		printf("La partida s'ha bloquejat\n");
	}else{   
		printf("\nHa guanyat: J%d\n", jug-1); //perque al for a la comprovacio sumara 1
	}
}

