/*
    timer.c
 */



#include "stdarg.h"
#include "string.h"	/* strlen */
#include "cpu.h"
#include <inttypes.h>
#include "timer.h"
#include <ecran.h>



/*Ecrit_temps prend en paramètre une chaine de caractères (ainsi que sa
taille) et l’affiche en haut à droite de l’écran : c’est cette fonction qui sera appelée par le traitant
d’interruption quand on devra mettre à jour l’affichage de l’heure*/
void ecrit_temps(const char *s, int len){
    uint32_t premLign = 0;
    // HH:MM:SS: len 8
    uint32_t col = NUM_COL - len - 2;
    for (int i = 0; i < len; i++){
		ecrit_car(premLign, col, s[i], FALSE, BLANC, NOIR);
        col++;
	}
}

// assembly
void traitant_IT_32(void);

// Traitant de l’interruption 32 qui affiche à l’écran le temps écoulé depuis le démarrage du système
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){
 
    // Chaque entrée de l’IDT occupe 2 mots consécutifs de 4 octets chacun et a le format suivant :
    // 1 mot 4*8 = 32 + 1 mot 4*8 = 32
    uint64_t (*table)[NUL_SRC] = ADR_IDT;

    // initialiser l’entrée 32 dans la table des vecteurs d’interruptions,



}

// réaliser le travail concret du traitant
void tic_PIT(void){

    //  Lorsqu’on commence à traiter une interruption, on doit le signaler à un composant matériel appelé contrôleur d’interruptions
    //  Permettre à ce contrôleur de se remettre à écouter d’autres interruptions éventuelles : 
    // elle doit donc être réalisée le plus tôt possible dans le traitant (en pratique, tout au début de la partie C du traitant)

    //  On doit envoyer la commande sur 8 bits 0x20 sur le port de commande 0x20. Comme on veut le faire en C au tout début de la partie du
    // traitant d’interruption gérant l’affichage de l’horloge: outb(0x20, 0x20).
    outb(COMMANDE_MASQ, PORT_COMMANDE_MASQ);

    // Lire la valeur actuelle du masque sur le port de données 0x21 grâce à la fonction inb
    uint8_t masq = inb(PORT_DONNEES_MASQ);

    //l’octet récupéré est en fait un tableau de booléens
    // la valeur du bit N décrit l’état de l’IRQ N : 
    // 1 si l’IRQ est masquée, 0 si elle est autorisée : 
    // il faut donc forcer la valeur du bit N à la valeur 
    // souhaitée (sans toucher les valeurs des autres bits)



    outb(masq, PORT_DONNEES_MASQ);
    //sprintf
};

void regler_freq(){
    // on envoie la commande sur 8 bits 0x34 sur le port de commande 0x43 grace à la fonction outb :
    // cette commande indique à l’horloge que l’on va lui envoyer la valeur de réglage de la fréquence
    // sous la forme de deux valeurs de 8 bits chacune qui seront émises sur le port de données ;

    outb(COMMAND_FREQ, PORT_COMMANDE_FREQ);

    // on envoie les 8 bits de poids faibles de la valeur de réglage de la fréquence sur le port de données 0x40 : 
    // cela peut se faire simplement par outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40) 
    // où QUARTZ vaut 0x1234DD et CLOCKFREQ vaut 50 ;
    outb((QUARTZ / CLOCKFREQ) & 0xFF, PORT_DONNEES_FREQ);

    // on envoie ensuite les 8 bits de poids forts de la valeur de réglage sur le même port 0x40.
     outb((QUARTZ / CLOCKFREQ) & 0xFF, PORT_DONNEES_FREQ);

}






