/*

 */

#define FALSE ((unsigned char) 0)
#define TRUE ((unsigned char) 1)
#include "stdarg.h"
#include "string.h"	/* strlen */
#include "cpu.h"
#include <inttypes.h>
#include "ecran.h"

/* Returns a pointer sur la case memoire corresponding to the coordinates provided */
/* Renvoie un pointeur sur la case mémoire correspondant aux coordonnées fournies */
uint16_t *ptr_mem(uint32_t lig, uint32_t col){
	
	// l’adresse de début est 0xB8000
	
	uint16_t adr; // address to be calculated and returned
	adr = 0xB000 + 2 * (lig * 80 + col);
	
	return adr; 
}


/* Writes character c in the coordinates provided
 * Écrit le caractère c aux coordonnées spécifiées
 * (Ajouter des paramètres pour permettre de préciser
 * la couleur du caractère, celle du fond ou le bit de clignotement)
 */
void ecrit_car(uint32_t lig, uint32_t col, char c){

	// On doit ecrire dans le mot de 2 octets (uint16_t en C) dont l'adresse en memoire peut etre calcule a partir de la fonction
	
	// retrouver le valeur ASCII
	uint16_t *mot_c = ptr_mem(lig, col);

	// retrouver le valeur ASCII
	uint8_t asciiVal = c;
	
	uint8_t format;
	
	
	mot_c = (format << 8) | asciiVal: 
	
	
	

}

&

