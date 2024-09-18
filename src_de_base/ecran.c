/*

 */



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
	return (uint16_t *) (ADR_DEBUT + 2 * (lig * NUM_COL + col));
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

	uint8_t format = (b_clignote << 7) | (couleur_fond << 4)  | (couleur_c);
	
	*pos = (format << 8) | (uint8_t) c;

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
	
	// 1. envoyer la commande 0x0F sur le port de commande pour indiquer à la carte que l’on va envoyer
	// la partie basse de la position du curseur
	outb(0x0F, PORT_COMMANDE); 

	// 2. envoyer cette partie basse sur le port de données ;
	outb((uint8_t)(pos & 0xFF), PORT_DONNEES);

	// 3. envoyer la commande 0x0E sur le port de commande pour signaler qu’on envoie maintenant la
    // partie haute ;
	outb(0x0E, PORT_COMMANDE);

	// 4. envoyer la partie haute de la position sur le port de données.
	outb((uint8_t)(pos >> 8) & 0xFF, PORT_DONNEES);
 
}

/* Get current cursor position*/
uint16_t getCursorPosition(){
	uint16_t pos = 0;

	// partie basse
	outb(0x0F,PORT_COMMANDE);
	pos |= inb(PORT_DONNEES);
	
	// partie haute
	outb(0x0E,PORT_COMMANDE);
	pos |= (inb(PORT_DONNEES) << 8);

	return pos;
}

/*On considère dans ce TP les caractères de la table ASCII (man ascii), qui sont numéros de 0 à 127
inclus. Les caractères dont le code est supérieur à 127 (accents, ...) seront ignorés.
Les caractères de code ASCII 32 à 126 doivent être affichés 
en les plaçant à la position actuelle du curseur clignotant 
et en déplaçant ce curseur sur la position suivante : à droite, ou au début de la ligne suivante
si le curseur était sur la dernière colonne.
Les caractères de 0 à 31, ainsi que le caractère 127 sont des caractères de contrôle.*
*/

/* Taiter un caractère donné (l’afficher si c’est un caractère normal ou qui implante l’effet voulu si c’est un caractère de contrôle) */
void traite_car(char c){

	if(c < 0 || c > 127){
		return;
	}

	uint16_t pos = getCursorPosition();

	// get line and column
	uint32_t lig = pos / NUM_COL;
	uint32_t col = pos % NUM_COL;


	if(c > 31){
		ecrit_car(lig, col, c, FALSE, BLANC, NOIR);
		col++;
		if (col >= NUM_COL){
			col = 0;
			lig++;
			if(lig >= NUM_LIN){
				defilement();
				lig = NUM_LIN -1;
			}
		}
	} else {
		// Control
		switch(c){
			case BS: // Backspace
				// Recule le curseur d’une colonne (si colonne ̸= 0)
				if(col > 0){
					col--;
				}else if (lig > 0){
					lig--;
					col = NUM_COL - 1;
				}
				break;
			case HT: // Horizontal tab
				// Avance a la prochaine tabulation (colonnes 0,8,16, ...,72,79)
				col = ((col / 8) + 1) * 8;
				if (col >= NUM_COL){
					col = NUM_COL - 1;
				}
				break;
			case LF: // Line Feed
				// Deplace le curseur sur la ligne suivante, colonne 0
				if(lig < NUM_LIN - 1){
					lig++;
				} else { // Derniere ligne
					defilement();
					lig = NUM_LIN - 1;
				}
				col = 0;
				break;
			case FF: // Form Feed
				// Efface l'ecran et place le curseur sur la collone 0 de la ligne 0
				efface_ecran();
				lig = 0;
				col = 0; 
				break;
			case CR: // Carriage Return
				// Deplace le curseur sur la ligne actuelle, colonne 0
				col = 0;
				break;
			// else: ignore
			default:
				break;
		}
	}

	place_curseur(lig, col);
}

/* Faire remonter d’une ligne l’affichage à l’écran (il pourra être judicieux d’utiliser memmove définie dans string.h pour cela) ; */
void defilement(void){
	memmove((void *)ADR_DEBUT, (void *)(ADR_DEBUT + 2*NUM_COL),2*NUM_COL*(NUM_LIN-1));
	for (uint32_t col = 0; col < NUM_COL; col++){
		ecrit_car(NUM_LIN-1, col, SPACE, FALSE, BLANC, NOIR);
	}
}


/* Afficher une chaine de caractères à la position courante du curseur */
void console_putbytes(const char *s, int len){
	for (int i = 0; i < len; i++){
		traite_car(s[i]);
	}
}


