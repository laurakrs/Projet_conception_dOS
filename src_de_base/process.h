#include <inttypes.h>
#include <stdio.h>       


#define N_REG 5

#define SIZE_S 512

#define N_PROC 2


// etat du processus : dans ce premier exemple, il n’y a que 2 états possibles : élu ou activable, vous
// pouvez toutefois déjà définir proprement les états comme une énumération de constantes
enum ETAT {ELU, ACTIVABLE};

typedef struct  {
    // le pid du processus, sous la forme d’un entier (signe -1 en cas derreur)
    uint32_t pid;
    // un chaine de caractères avec une taille maximum fixée ;
    char nom[50];
    // dans ce premier exemple, il n’y a que 2 états possibles : élu ou activable, 
    enum ETAT etat;

    /* la zone de sauvegarde des registres du processeur : il s’agira tout simplement d’un tableau d’entiers,
    puisqu’on manipule des registres 32 bits, et vous avez besoin de 5 cases car il n’y a que 5 registres
    importants à sauvegarder sur x86 (voir plus bas l’explication du changement de contexte) ;
    */
   uint32_t tab_reg[N_REG]; 

    /* — la pile d’exécution du processus, qui sera définie comme un tableau d’entiers d’une taille fixée par
    une constante, par exemple 512.
    */
   int stack[SIZE_S];
} PROCESS;

void idle(void);


void proc1(void);

// assembly
// Elle prend 2 paramètres de types pointeurs sur des entiers : il s’agit en fait des adresses des zones de sauvegarde des registres des contextes de l’ancien processus et du nouveau.
void ctx_sw(uint32_t* adr_anc, uint32_t* adr_nouv);

void ordonnance(void);

int32_t mon_pid(void);

char *mon_nom(void);


// table globale
extern PROCESS table_proc[N_PROC];




