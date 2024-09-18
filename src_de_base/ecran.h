/*
 * ecran.h
 *
 * 
 */

#ifndef __ECRAN_H_INCLUDED__
#define __ECRAN_H_INCLUDED__


#define FALSE ((unsigned char) 0)
#define TRUE ((unsigned char) 1)
#define ADR_DEBUT 0xB8000
#define NUM_COL 80
#define NUM_LIN 25

// Caracteres
#define SPACE 0x20
#define BS 0x08
#define HT 0x09
#define LF 0x0A
#define FF 0x0C
#define CR 0x0D


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



/* Returns a pointer sur la case memoire corresponding to the coordinates provided */
uint16_t *ptr_mem(
	uint32_t lig, /* line */
	uint32_t col); /* column */

/* Writes character c in the coordinates provided
 *
 * (vous pouvez aussi ajouter des paramètres pour permettre de préciser
 * la couleur du caractère, celle du fond ou le bit de clignotement)
 */
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t b_clignote, uint8_t couleur_c, uint8_t couleur_fond);


/* Clears the screen
 *
 * Parcourir les lignes et les colonnes de l’écran
 * pour écrire dans chaque case un espace en blanc sur fond noir (afin d’initialiser les formats dans
 * la mémoire) ;
 */
void efface_ecran(void);

/* Placer le curseur à la position donnée */
void place_curseur(uint32_t lig, uint32_t col);

/* Recuperer la position du curseur */
uint16_t getCursorPosition();

/* Taiter un caractère donné (l’afficher si c’est un caractère normal ou qui implante l’effet voulu si c’est   un caractère de contrôle) */
void traite_car(char c);

/* Faire remonter d’une ligne l’affichage à l’écran (il pourra être judicieux d’utiliser memmove définie dans string.h pour cela) ; */
void defilement(void);


/* Afficher une chaine de caractères à la position courante du curseur */
void console_putbytes(const char *s, int len);



#endif /* __ECRAN_H_INCLUDED__ */
