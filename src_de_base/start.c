#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ecran.h>
#include <timer.h>
#include <process.h>

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

    
    // initialisation des structures de processus

    // il faut définir la table des processus dont la taille est là encore une constante prédéfinie dans le système. constante sera fixée à 2.


    /*Lors du démarrage du système, il faut initialiser les structures de processus de idle et proc1 avec des
    valeurs pertinentes, c’est à dire :
    — pour les deux processus, son pid, son nom et son état : par défaut, c’est le processus idle qui est
    élu le premier ;
 */

    for (int i = 0; i < N_PROC; i++) {
        table_proc[i] = NULL;
    }

    PROCESS *idle_p = (PROCESS *) malloc(sizeof(PROCESS)); 

    current = (PROCESS *) malloc(sizeof(PROCESS)); 

    idle_p->pid = 0;
    strncpy(idle_p->nom, "idle", MAX_LENGTH_NOM - 1);
    idle_p->nom[MAX_LENGTH_NOM - 1] = '\0';
    idle_p->etat = ELU;
    idle_p->suiv = NULL;


    // Add process to the table
    table_proc[0] = idle_p;

    //int32_t pid0 = cree_processus(idle, "idle");
    int32_t pid1 = cree_processus(proc1, "proc1");
    int32_t pid2 = cree_processus(proc2, "proc2");
    int32_t pid3 = cree_processus(proc3, "proc3");
    int32_t pid4 = cree_processus(proc4, "proc4");
    int32_t pid5 = cree_processus(proc5, "proc5");
    int32_t pid6 = cree_processus(proc6, "proc6");
    int32_t pid7 = cree_processus(proc7, "proc7");

    // Check PIDs for errors
    if (pid1 < 0 || pid2 < 0 || pid3 < 0 || pid4 < 0 || pid5 < 0 || pid6 < 0 || pid7 < 0) {
        printf("Error: Unable to create all processes.\n");
    }

    // Processes successfully created and added to the activables list
    printf("Processes created successfully.\n");

    init_list(&activables);

    current = extraire_tete(&activables);

    // demarrage du processus par defaut
    idle();

}

