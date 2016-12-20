#include <stdio.h>
#include "fitxes.h"
#include "azar.h"
#include "colores.h"

t_conjunt generar_fitxes(){ /*Funció que genera les 28 fitxes del domino*/
	int i, j, nf;
	t_conjunt conjunt;
	conjunt.n_fitxes = N;
	for (i=0,nf=0; i<=MAX_N; i++){
		for (j=i; j<=MAX_N; j++,nf++){
			conjunt.fitxes[nf].n1 = i;
			conjunt.fitxes[nf].n2 = j;
		}
	}
	return conjunt;
}
void print_conjunt(t_conjunt c){ 
	//Imprimeix un conjunt de fitxes
	int i = 0;
	for(i=0; i < c.n_fitxes; i++){
		print_fitxa(c.fitxes[i]);
	}
}

void print_conjunt_secret(t_conjunt c){ 
	//Imprimeix un conjunt sense mostrar les fitxes
	int i = 0;
	for(i=0; i < c.n_fitxes; i++){
		printf("?:?|");
	}
}

void print_conjunt_2d(t_conjunt c){
	int n_caracters;
	int caracters_totals;
	char linees[3][MAX_CHAR];
	int i, j;
	for(j=0; j < 3; j++){ //Per a cada una de les 3 linees
		n_caracters = 0; //el n de caracters de cada linea comença a 0
		for(i=0; i < c.n_fitxes; i++){ //Per a cada fitxa
			if(c.fitxes[i].n1 == c.fitxes[i].n2){ //Si es un doble
				if(j%2 == 0){ //linea 0 o 2
					linees[j][n_caracters++] = c.fitxes[i].n1 + '0';
					linees[j][n_caracters++] = '|';
				}else{ //linea 1
					linees[j][n_caracters++] = '-';
					linees[j][n_caracters++] = '|';
				}
			}else{ //Si no es cap doble
				if(j == 0){ //linea 0, afegim la cadena "___ " que es posa sobre la fitxa
					linees[j][n_caracters++] = '_';
					linees[j][n_caracters++] = '_';
					linees[j][n_caracters++] = '_';
					linees[j][n_caracters++] = ' ';
				}else if(j == 1){ //linea 1, afegim la fitxa en si
					linees[j][n_caracters++] = c.fitxes[i].n1 + '0';
					linees[j][n_caracters++] = ':';
					linees[j][n_caracters++] = c.fitxes[i].n2 + '0';
					linees[j][n_caracters++] = '|';
				}else{ //linea 2, afegim nomes espais
					linees[j][n_caracters++] = ' ';
					linees[j][n_caracters++] = ' ';
					linees[j][n_caracters++] = ' ';
					linees[j][n_caracters++] = ' ';
				}
			}
		}
		if(j == 1){ //Els caracters totals seran els que hi ha a la segona linea
			caracters_totals = n_caracters;
		}
	}

	//Imprimim la matriu generada
	for(i=0; i < 3; i++){
		for(j=0; j < caracters_totals; j++){
			printf("%c", linees[i][j]);
		}
		printf("\n");
	}
}

void print_fitxa(t_fitxa f) { //Imprimeix una fitxa
	printf("%d:%d|", f.n1, f.n2);
}

void afegir_fitxa_final(t_conjunt *c, t_fitxa f){
	 //afegeix una fitxa al final d'un conjunt de fitxes
	(*c).n_fitxes++;
	(*c).fitxes[(*c).n_fitxes - 1] = f;
}

void afegir_fitxa_principi(t_conjunt *c, t_fitxa f){ 
	//afegeix una fitxa al principi d'un conjunt de fitxes
	t_conjunt c2;
	int i;
	c2.n_fitxes = (*c).n_fitxes; //Creem un conjunt identic al conjunt a modificar
	for(i=0; i < c2.n_fitxes; i++){ 
		c2.fitxes[i] = (*c).fitxes[i];
	}
	(*c).n_fitxes++; //Afegim 1 fitxa al n. total de fitxes
	(*c).fitxes[0] = f; //La posicio 0 del vector de fitxes sera la fitxa a afegir
	for(i=1; i < (*c).n_fitxes; i++){ //a partir d'aqui afegim la copia que haviem fet abans
		(*c).fitxes[i] = c2.fitxes[i-1];
	}

}

int eliminar_fitxa(t_conjunt *c, t_fitxa f){
	//elimina una fitxa d'un conjunt
	t_conjunt c2;
	int i;
	int eliminada = 0;
	//creem un nou conjunt amb la fitxa eliminada);
	c2.n_fitxes = (*c).n_fitxes;
	for(i=0; i < c2.n_fitxes; i++){
		if((*c).fitxes[i].n1 == f.n1 && (*c).fitxes[i].n2 == f.n2){
			eliminada = 1;
		}else{
			//si s'elimina les posicions seran i - 1
			c2.fitxes[i-eliminada] = (*c).fitxes[i];
		}		
	}
	if(eliminada == 0){
		return 0;
	}
	//volquem les dades del segon conjunt al conjunt original 
	c2.n_fitxes--;
	(*c).n_fitxes--;
	for(i=0; i < c2.n_fitxes; i++){
		(*c).fitxes[i] = c2.fitxes[i];
	}
	return 1;
}

t_fitxa agafar_fitxa_random(t_conjunt *c_que_agafa, t_conjunt *c_a_qui_agafa){
	int r;
	t_fitxa fitxa;
	r = numero_al_azar(c_a_qui_agafa->n_fitxes); //es genera un nombre random de 0 al n_fitxes que quedin al conjunt a repatir -1.
	fitxa = c_a_qui_agafa->fitxes[r];
	afegir_fitxa_final(c_que_agafa, fitxa);
	eliminar_fitxa(c_a_qui_agafa, fitxa); //la fitxa que li ha tocat al jugador s'elimina del conjunt a repartir+
	return fitxa;
}

t_conjunt retornar_tirades_possibles(t_conjunt c, t_conjunt taula){
	t_fitxa esquerra, dreta;
	t_conjunt posibles;
	posibles.n_fitxes = 0;
	int i;
	esquerra = taula.fitxes[0];
	dreta = taula.fitxes[taula.n_fitxes-1];
	for(i=0; i < c.n_fitxes; i++){
		if(c.fitxes[i].n1 == esquerra.n1 || c.fitxes[i].n2 == esquerra.n1){
			c.fitxes[i].p = 'e';
			posibles.fitxes[posibles.n_fitxes] = c.fitxes[i];
			posibles.n_fitxes++;
		} //No es posa else if perque una fitxa es pot tirar per la dreta o per l'esquerra
		if(c.fitxes[i].n1 == dreta.n2 || c.fitxes[i].n2 == dreta.n2){
			c.fitxes[i].p = 'd';
			posibles.fitxes[posibles.n_fitxes] = c.fitxes[i];
			posibles.n_fitxes++;
		}
		//printf("\n");

	}
	return posibles;
}

void print_conjunt_tirades(t_conjunt c){
	int i = 0;
	printf_color(5);
	printf("Tirades possibles: ");
	printf_color(2);
	for(i=0; i < c.n_fitxes; i++){
		printf("%d(%c", i, c.fitxes[i].p);
		print_fitxa(c.fitxes[i]);
		printf(") ");
	}
	printf_color(-1);
	printf("\n");
}

