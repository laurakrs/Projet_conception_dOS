#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ecran.h>

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
   
    printf("Bonjour le monde !\n");
    uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    printf("5! = %" PRIu16 "\n",x);

    

    // tester LF
    for(int i = 0; i < 25; i++){
        printf("\n");
    }

    // tester HT
    printf("\t\tBonjour le tab !");

    // tester BS
    // printf("\b\b");

    // tester CR
    // printf("\r");

    // tester FF
    // printf("\f");



    // on ne doit jamais sortir de kernel_start

    // boucle d’attente
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }

}

