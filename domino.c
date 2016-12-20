#include <stdio.h>
#include "fitxes.h"
#include "azar.h"
#include "preguntas.h"
#include "partida.h"
#include "bots.h"
#include "duerme.h"
#include "colores.h"

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
	
	//es reparteixen les fitxes i es tira la primera fitxa
	repartir_fitxes(&partida_1);
	primer_jugador = tirar_primera_fitxa(&partida_1); //ens retorna qui ha tirat la primera fitxa
	jugador_actual = (primer_jugador + 1) % partida_1.n_jugadors; //el jugador 4 passaria a ser el 0
	printf("\n\nEl J%d ha tirat el primer doble i li toca tirar a J%d\n", primer_jugador, jugador_actual);
	
	//comencem a jugar
	while(!partida_1.acabada){
		duerme_un_rato();
		printf_color(3);
		printf("\t\t\t(Ronda %d)\n", ronda);
		printf_color(-1);
		ronda++;
		bloqueig = 0;
		//La primera iteracio comencem per el jugador de despres del que ha tirat el primer doble
		for(jug = jugador_actual; (jug < partida_1.n_jugadors) && (!partida_1.acabada); jug++){ //Per cada jugador de la ronda i mentre no s'hagia acabat la partida
		
			printf_color(3);
			printf("\t\t\t(Turn del jugador J%d)\n", jug);
			printf_color(-1);
			imprimir_partida(partida_1);
			
			//Busquem les tirades possibles que pot fer el jugador
			tirades_possibles = retornar_tirades_possibles(partida_1.conjunts_jug[jug], partida_1.taula);
			if(tirades_possibles.n_fitxes > 0){ //Si pot tirar
				if(jug == 0 && partida_1.jugador_huma){
					//Si el J0 es huma
					print_conjunt_tirades(tirades_possibles);
					fitxa_a_tirar = preguntar_n_en_rango("Fitxa a tirar: ", 0, tirades_possibles.n_fitxes-1);
				}else{ 
					//Pels bots
					fitxa_a_tirar = retornar_millor_fitxa(partida_1, tirades_possibles);
				}
				moure_fitxa_taula(&partida_1, tirades_possibles.fitxes[fitxa_a_tirar], jug); //Es mou la fitxa a la taula
			}else{	//Si no pot tirar
				bloqueig = bloqueig + no_puc_tirar(&partida_1, jug); //Si la pila és buida se li suma 1 a la variable bloqueig
			}
			
			duerme_un_rato();
			//Aqui comprovem si la partida s'ha acabat: un jugador ha arribat a tenir 0 fitxes o la partida s'ha bloquejat (ningú pot tirar)
			if(partida_1.conjunts_jug[jug].n_fitxes == 0 || bloqueig == partida_1.n_jugadors){
				partida_1.acabada = 1;
			}
			
			printf("-------------------------------------\n\n");
			jugador_actual = 0; //Ara sempre començarem pel jugador 0
		}
	}
	printf_color(5);
	printf("PARTIDA ACABADA!\n");
	printf_color(-1);
	if(bloqueig == partida_1.n_jugadors){
		printf_color(5);
		printf("La partida s'ha bloquejat.\n"); //a la fase d'extras posarem que conti quin jugador te menys fitxes per saber el guanyador "parcial"
		printf_color(-1);

	}else{
		printf_color(5);
		printf("\nHa guanyat: J%d\n", jug-1); //perque al for a la comprovacio sumara 1
		printf_color(-1);
	}
}

