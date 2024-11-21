/*
 * timer.h
 *
 * 
 */

#ifndef __TIMER_H_INCLUDED__
#define __TIMER_H_INCLUDED__

#include <inttypes.h>
#include <stdbool.h>
#include "segment.h"

#define NOIR 0
#define BLANC 15
#define FALSE ((unsigned char) 0)
#define NUM_COL 80
#define NUM_LIN 25

// Adresse de la table des vecteurs dinterruption
#define ADR_IDT 0x1000
#define NUL_SRC 256

// Constante 0x8E00 - execution du traitant se fait interruptions masquees : un traitant dinterruption ne peut donc pas etre lui-meme interrompu
#define AUX_CONST 0x8E00

#define COMMANDE_MASQ 0x20
#define PORT_COMMANDE_MASQ 0x20
#define PORT_DONNEES_MASQ 0x21

#define PORT_COMMANDE_FREQ 0x43
#define COMMAND_FREQ 0x34
#define PORT_DONNEES_FREQ 0x40

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

#define LEN_TEMPS 9


/*Ecrit_temps prend en paramètre une chaine de caractères (ainsi que sa
taille) et l’affiche en haut à droite de l’écran : c’est cette fonction qui sera appelée par le traitant
d’interruption quand on devra mettre à jour l’affichage de l’heure ;*/
void ecrit_temps(const char *s, int len);

// traitant de l’interruption 32 qui affiche à l’écran le temps écoulé depuis le démarrage du système
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));

// Travail concret du traitant
// l’acquittement de l’interruption et la partie gérant l’affichage
void tic_PIT(void);

// assembly
void traitant_IT_32(void);

// frequence
void regler_freq();

// 
void masque_IRQ(uint32_t num_IRQ, bool masque);

// renvoie le nombre de secondes écoulées depuis le démarrage du système
uint32_t nbr_secondes();








#endif /* __TIMER_H_INCLUDED__ */