#ifndef FITXES_H
#define FITXES_H

#define N 28
#define MAX_N 6

typedef struct { //estructura d'una fitxa
	int n1; //a la posicio default i horitzontal es l'esquerra
	int n2; //a la posicio default i horitzontal es la dreta // [n1/n2]
    char p; //char per a la posicio, per posarla a la dreta o a l'esquerra. 
} t_fitxa;

typedef struct { //estructura d'un cojunt o grup de fitxes
	int n_fitxes;
	t_fitxa fitxes[N];
} t_conjunt;

void print_fitxa(t_fitxa f);
void print_conjunt(t_conjunt c);
void print_conjunt_secret(t_conjunt c);
void print_conjunt_tirades(t_conjunt c);
t_conjunt generar_fitxes();
void afegir_fitxa_final(t_conjunt *c, t_fitxa f);
void afegir_fitxa_principi(t_conjunt *c, t_fitxa f);
int eliminar_fitxa(t_conjunt *c, t_fitxa f);
t_fitxa agafar_fitxa_random(t_conjunt *c_que_agafa, t_conjunt *c_a_qui_agafa);
t_conjunt retornar_tirades_possibles(t_conjunt c, t_conjunt taula);

#endif