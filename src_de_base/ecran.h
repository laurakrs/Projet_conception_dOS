/*
 * ecran.h
 *
 * 
 */

#ifndef __ECRAN_H_INCLUDED__
#define __ECRAN_H_INCLUDED__



/* Returns a pointer sur la case memoire corresponding to the coordinates provided */
uint16_t *ptr_mem(
	uint32_t lig, /* line */
	uint32_t col); /* column */

/* Writes character c in the coordinates provided
 *
 * (vous pouvez aussi ajouter des paramètres pour permettre de préciser
 * la couleur du caractère, celle du fond ou le bit de clignotement)
 */
void ecrit_car(uint32_t lig, uint32_t col, chqr c);


/* Clears the screen
 *
 * Parcourir les lignes et les colonnes de l’écran
 * pour écrire dans chaque case un espace en blanc sur fond noir (afin d’initialiser les formats dans
 * la mémoire) ;
 */
void efface_ecran(void);

/* Placer le curseur à la position donnée */
void place_curseur(uint32_t lig; uint32_t col);


/* Taiter un caractère donné (l’afficher si c’est un caractère normal ou qui implante l’effet voulu si c’est   un caractère de contrôle) */
void traite_car(char c);

/* Faire remonter d’une ligne l’affichage à l’écran (il pourra être judicieux d’utiliser memmove définie dans string.h pour cela) ; */
void defilement(void);


/* Afficher une chaine de caractères à la position courante du curseur */
void console_putbytes(const char *s, int len);



#endif /* __ECRAN_H_INCLUDED__ */
