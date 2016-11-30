#include <stdio.h>
#include "fitxes.h"
#include "azar.h"
#include "preguntas.h"
#include "partida.h"

void main(){
    t_partida partida_1;
    /* 
    Aqui podem definir les t_Partides que volguem. Les variables següents es poden reutilitzar,
    es a dir, que no depenen de la partida.
    Sino, es podria crear un altra struct.
    */
    t_conjunt tirades_possibles;
    int fitxa_a_tirar, n_fitxes_a_tirar;
    int ronda = 1;
    int jug;
    int bloqueig = 0;
    
    //Inicialitzem la partida
    inicializar_azar();
    partida_1.pila_central = generar_fitxes();
    partida_1.taula.n_fitxes = 0;
    partida_1.n_jugadors = preguntar_n_en_rango("Nombre de jugadors?", 2, 4);
    partida_1.jugador_huma = preguntar_si_o_no("Un jugador huma?");
    partida_1.omnipresencia = preguntar_si_o_no("Vols omnipresencia?");
    partida_1.acabada = 0;
    repartir_fitxes(&partida_1);
    tirar_primera_fitxa(&partida_1);

    //comencem a jugar
    while(!partida_1.acabada){  
        printf("\n\t\t\t(Ronda %d)\n", ronda);
        ronda++;
        bloqueig = 0; //si arriba a 4 (o n_jugadors) es bloqueja la partida.
        for(jug = 0; (jug < partida_1.n_jugadors) && (!partida_1.acabada); jug++){
            printf("\t\t\t(Turn del jugador J%d)\n", jug);
            imprimir_partida(partida_1);
            tirades_possibles = retornar_tirades_possibles(partida_1.conjunts_jug[jug], partida_1.taula);
            if(tirades_possibles.n_fitxes > 0){
                if(jug == 0 && partida_1.jugador_huma){
                    print_conjunt_tirades(tirades_possibles);
                    fitxa_a_tirar = preguntar_n_en_rango("Fitxa a tirar: ", 0, tirades_possibles.n_fitxes-1);
                }else{ //aqui es on va la inteligencia artificial dels bots
                    fitxa_a_tirar = 0;
                }
                moure_fitxa_taula(&partida_1, tirades_possibles.fitxes[fitxa_a_tirar], jug);
            }else{
                bloqueig = bloqueig + no_puc_tirar(&partida_1, jug);
            }
            if(partida_1.conjunts_jug[jug].n_fitxes == 0 || bloqueig == partida_1.n_jugadors){
                partida_1.acabada = 1;
            }
            printf("-------------------------------------\n");
        }
    }
    printf("PARTIDA ACABADA!\n");
    if(bloqueig == partida_1.n_jugadors){
        printf("La partida s'ha bloquejat!\n");
    }else{   
        printf("\nHa guanyat: J%d\n", jug-1); //perque al for a la comprovacio sumara 1
    }
}

