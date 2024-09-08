/*

 */

#define FALSE ((unsigned char) 0)
#define TRUE ((unsigned char) 1)
#define ADR_DEBUT 0xB000
#define NUM_COL 80
#define NUM_LIN 25

// Caracteres
#define SPACE 32
#define BS 8
#define HT 9
#define LF 10
#define FF 12
#define CR 13


// Couleurs
#define NOIR 0
#define BLEU 1
#define VERT 2
#define CYAN 3
#define ROUGE 4
#define MAGENTA 5
#define MARRON 6
#define GRIS 7

// seulement pour le texte
#define GRIS_FONCE 8
#define BLEU_CLAIR 9
#define VERT_CLAIR 10
#define CYAN_CLAIR 11
#define ROUGE_CLAIR 12
#define MAGENTA_CLAIR 13
#define JAUNE 14
#define BLANC 15

// Port de commande gérant la position du curseur: 0x3D4 
// Port de données associé: 0x3D5. 
#define PORT_COMMANDE 0x3D4
#define PORT_DONNEES 0x3D5


#include "stdarg.h"
#include "string.h"	/* strlen */
#include "cpu.h"
#include <inttypes.h>
#include "ecran.h"

/* Returns a pointer sur la case memoire corresponding to the coordinates provided */
/* Renvoie un pointeur sur la case mémoire correspondant aux coordonnées fournies */
uint16_t *ptr_mem(uint32_t lig, uint32_t col){

	if (lig >= NUM_LIN || col >= NUM_COL){
		return NULL; 
	}
	
	// l’adresse de début est 0xB8000
	
	uint16_t *adr; // address to be calculated and returned
	adr = ADR_DEBUT + 2 * (lig * NUM_COL + col);
	
	return adr; 
}


/* Writes character c in the coordinates provided
 * Écrit le caractère c aux coordonnées spécifiées
 * (Ajouter des paramètres pour permettre de préciser
 * la couleur du caractère, celle du fond ou le bit de clignotement)
 */
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t b_clignote, uint8_t couleur_c, uint8_t couleur_fond){

	// On doit ecrire dans le mot de 2 octets (uint16_t en C) dont l'adresse en memoire peut etre calcule a partir de la fonction
	
	// retrouver le valeur ASCII
	uint16_t *pos = ptr_mem(lig, col);

	if (pos == NULL){
		return;
	}

	uint8_t format = b_clignote << 7  | couleur_fond << 4  | couleur_c;
	
	*pos = (format << 8) | c;

}

/* Clears the screen
 *
 * Parcourir les lignes et les colonnes de l’écran
 * pour écrire dans chaque case un espace en blanc sur fond noir (afin d’initialiser les formats dans
 * la mémoire) ;
 */
void efface_ecran() {

	for(uint32_t lig = 0; lig <  NUM_LIN ; lig++) {
		for(uint32_t col = 0; col < NUM_COL; col++){
			ecrit_car(lig, col, SPACE, FALSE, BLANC, NOIR);
		}
	}

}

/*

La position du curseur est un entier sur 16 bits
calculé via la formule : pos = col + lig × 80. 

Cette position doit être envoyée en deux temps à la carte vidéo 

*/
/* Placer le curseur à la position donnée */
void place_curseur(uint32_t lig, uint32_t col){

	uint16_t pos = col + lig * NUM_COL;

	uint8_t posHaute = pos >> 8;
    uint8_t posBasse = pos;
	
	// 1. envoyer la commande 0x0F sur le port de commande pour indiquer à la carte que l’on va envoyer
	// la partie basse de la position du curseur
	outb(0x0F, PORT_COMMANDE); 

	// 2. envoyer cette partie basse sur le port de données ;
	outb(posBasse, PORT_DONNEES);

	// 3. envoyer la commande 0x0E sur le port de commande pour signaler qu’on envoie maintenant la
    // partie haute ;
	outb(0x0E, PORT_COMMANDE);

	// 4. envoyer la partie haute de la position sur le port de données.
	outb(posHaute, PORT_DONNEES);
 
}

/* Get current cursor position*/
uint16_t getCursorPosition(){
	uint16_t pos;



	return pos;
}

/*On considère dans ce TP les caractères de la table ASCII (man ascii), qui sont numérotés de 0 à 127
inclus. Les caractères dont le code est supérieur à 127 (accents, ...) seront ignorés.
Les caractères de code ASCII 32 à 126 doivent être affichés 
en les plaçant à la position actuelle du curseur clignotant 
et en déplaçant ce curseur sur la position suivante : à droite, ou au début de la ligne suivante
si le curseur était sur la dernière colonne.
Les caractères de 0 à 31, ainsi que le caractère 127 sont des caractères de contrôle.*
*/

/* Taiter un caractère donné (l’afficher si c’est un caractère normal ou qui implante l’effet voulu si c’est un caractère de contrôle) */
void traite_car(char c){

	if(c < 127){
		return;
	}

	uint16_t pos = getCursorPosition();

	// get line and column
	uint32_t lig = pos / NUM_LIN;
	uint32_t col = pos % NUM_COL;


	if(c > 31){
		ecrit_car(lig, col, c, FALSE, BLANC, NOIR);
		col++;
		if (col >= NUM_COL){
			col = 0;
			lig++;
		}
	} else {
		switch(c){
			case BS: 
				// Recule le curseur d’une colonne (si colonne ̸= 0)
				if(col > 0){
					col--;
				}
				break;
			case HT: 
				// Avance a la prochaine tabulation (colonnes 0,8,16, ...,72,79)
				place_curseur(lig, col*8);
				break;
			case LF:
				// Deplace le curseur sur la ligne suivante, colonne 0
				if(lig < NUM_LIN-1){
					lig++;
					col = 0;
				}
				break;
			case FF:
				// Efface l'ecran et place le curseur sur la collone 0 de la ligne 0
				efface_ecran();
				lig = 0;
				col = 0; 
				break;
			case CR:
				// Deplace le curseur sur la ligne actuelle, colonne 0
				col = 0;
				break;
			// else: ignore
			default:
				break;
		}
	}

	if(lig >= NUM_LIN){
		defilement();
		lig = NUM_LIN - 1;
	}

	place_curseur(lig, col);
}

/* Faire remonter d’une ligne l’affichage à l’écran (il pourra être judicieux d’utiliser memmove définie dans string.h pour cela) ; */
void defilement(void){

	void *p = 5997;
	void *p1 = 5997;
	size_t size = sizeof(*p);

	// void *memmove(void *dest, const void *src, size_t n);

	*memmove(p, p1, size);

}


/* Afficher une chaine de caractères à la position courante du curseur */
void console_putbytes(const char *s, int len){
	for (int i = 0; i < len; i++){
		traite_car(s[i]);
	}
}


