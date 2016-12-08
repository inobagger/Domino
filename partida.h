#ifndef PARTIDA_H
#define PARTIDA_H
#include "fitxes.h"

#define MAX_JUGADORS 4
#define FITXES_JUGADOR 7

typedef struct{
	int n_jugadors;
	int jugador_huma;
	int omnipresencia;
    int imprimir_2d;
    int acabada; //0 si la partida no ha acabat, 1 si ha acabat
	t_conjunt pila_central; //pila de fitxes central
    t_conjunt taula; //pila on es juguen les fitxes
	t_conjunt conjunts_jug[MAX_JUGADORS]; //piles dels jugadors
} t_partida;

void repartir_fitxes(t_partida *partida);
void imprimir_partida(t_partida partida);
void tirar_primera_fitxa(t_partida *partida); //tira la primera fitxa a la taula
void moure_fitxa_taula(t_partida *partida, t_fitxa f, int jugador); //posa la fitxa a la taula, 'e' o 'd'.
int no_puc_tirar(t_partida *partida, int jugador); //retorna 0 si agafes fitxa o 1 si la pila esta buida

#endif
