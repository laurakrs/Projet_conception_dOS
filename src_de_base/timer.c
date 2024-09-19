/*
    timer.c
 */



#include "stdarg.h"
#include "string.h"	/* strlen */
#include "cpu.h"
#include <inttypes.h>
#include "timer.h"

/*Ecrit_temps prend en paramètre une chaine de caractères (ainsi que sa
taille) et l’affiche en haut à droite de l’écran : c’est cette fonction qui sera appelée par le traitant
d’interruption quand on devra mettre à jour l’affichage de l’heure*/
void ecrit_temps(const char *s, int len){
    uint32_t premLign = 0;
    // HH:MM:SS: len 8
    uint32_t col = NUM_COL - len - 2;
    for (int i = 0; i < len; i++){
		ecrit_car(0, col+1, s[i], FALSE, BLANC, NOIR);
	}
    
}

void tic_PIT(void){
    //sprintf
};


void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));



