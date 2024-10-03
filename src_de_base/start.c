#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ecran.h>
#include <timer.h>
#include <process.h>





// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// table globale
PROCESS table_proc[N_PROC];

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void idle(void){
    printf("[idle] je tente de passer la main a proc1...\n");

    // Elle prend 2 paramètres de types pointeurs sur des entiers : il s’agit en fait des adresses des zones de sauvegarde des registres des contextes de l’ancien processus et du nouveau.

    ctx_sw(table_proc[0].tab_reg, table_proc[1].tab_reg);

}
void proc1(void){
    printf("[proc1] idle ma donne la main\n");
    printf("[proc1] jarrete le systeme\n");
    hlt();
}

void kernel_start(void)
{

    
    // initialisation des structures de processus

    // il faut définir la table des processus dont la taille est là encore une constante prédéfinie dans le système. constante sera fixée à 2.


    /*Lors du démarrage du système, il faut initialiser les structures de processus de idle et proc1 avec des
    valeurs pertinentes, c’est à dire :
    — pour les deux processus, son pid, son nom et son état : par défaut, c’est le processus idle qui est
    élu le premier ;
 */

    // PROCESS idle_p = {0, "idle", ELU};

    // IDLE
    table_proc[0].pid = 0;
    table_proc[0].etat = ELU;
    strcpy(table_proc[0].nom,"idle");

    // PROC 1 
    table_proc[1].pid = 1;
    table_proc[1].etat = ACTIVABLE;
    strcpy(table_proc[1].nom,"proc1");

    // — pour proc1 la case de la zone de sauvegarde des registres correspondant à %esp doit pointer sur le sommet de pile, 
    // pas le début de la zone mémoire allouée, mais la fin de cette zone (lorsqu’on empile des valeurs, on descend vers les adresses décroissantes)
    table_proc[1].tab_reg[1] = (uint32_t)(table_proc[1].stack+SIZE_S-1);

    // la case en sommet de pile doit contenir l’adresse de la fonction proc1
    table_proc[1].stack[SIZE_S-1] = (uint32_t)proc1;
   


    // demarrage du processus par defaut
    idle();

    // initialisations timer
    // On pourra donc initialiser la table des vecteurs d’interruption en appelant simplement 
    // regler_freq();
    // init_traitant_IT(32, traitant_IT_32);
    //masque_IRQ(0,0);


    // efface_ecran();


    // démasquage des interruptions externes
    // sti(); Enlever pour traiter lordonnance 


    // ecrit_temps("HH:MM:SS",9); 

    // printf("Bonjour le monde !\n");
    // uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    // (void)x;
    //printf("5! = %" PRIu16 "\n",x);



    /*

    // tester LF
    for(int i = 0; i < 25; i++){
        printf("\n");
    }

   // tester HT
   printf("\t\tBonjour le tab !");

    // tester BS
   printf("\b\b");

   // tester CR
   printf("\r");

   // tester FF
   printf("\f");

   */

    // on ne doit jamais sortir de kernel_start

    // boucle d’attente
    // while (1) {
        // cette fonction arrete le processeur


        // : il est  essentiel que les interruptions soient démasquees avant d’appeler cette fonction.
       // hlt();
    //}

}

