#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <timer.h>


// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

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

void kernel_start(void)
{

    // TIMER
    // initialisations
    //...



    // On pourra donc initialiser la table des vecteurs d’interruption en appelant simplement 
    init_traitant_IT(32, traitant_IT_32);


    // démasquage des interruptions externes
    sti();


    ecrit_temps("HH:MM:SS",8); 

    printf("Bonjour le monde !\n");
    uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    printf("5! = %" PRIu16 "\n",x);
    // on ne doit jamais sortir de kernel_start

    // boucle d’attente
    while (1) {
        // cette fonction arrete le processeur


        // : il est  essentiel que les interruptions soient démasquees avant d’appeler cette fonction.
        hlt();
    }

}

