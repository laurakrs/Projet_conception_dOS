#include <inttypes.h>
#include <stdio.h>
#include <tinyalloc.h>   



#define N_REG 5

#define SIZE_S 512

#define N_PROC 8

#define MAX_LENGTH_NOM 50


// etat du processus : 3 états possibles : élu, activable, ou endormi -> énumération de constantes
enum ETAT {ELU, ACTIVABLE, ENDORMI, ZOMBIE};

typedef struct PROCESS {
    // le pid du processus, sous la forme d’un entier (signe -1 en cas derreur)
    uint32_t pid;
    // un chaine de caractères avec une taille maximum fixée ;
    char nom[MAX_LENGTH_NOM];
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

   // champ suiv défini comme un pointeur vers une structure de processus : 
   // ce champ va permettre de chainer les processus les uns aux autres ;
   struct PROCESS *suiv;

   uint32_t heure_reveil;

} PROCESS;

void idle(void);


void proc1(void);

void proc2(void);

void proc3(void);

void proc4(void);

void proc5(void);

void proc6(void);

void proc7(void);

// assembly
// Elle prend 2 paramètres de types pointeurs sur des entiers : il s’agit en fait des adresses des zones de sauvegarde des registres des contextes de l’ancien processus et du nouveau.
void ctx_sw(uint32_t* adr_anc, uint32_t* adr_nouv);

void ordonnance(void);

void ordonnance_endormi(void);

int32_t mon_pid(void);

char *mon_nom(void);

int32_t cree_processus(void (*code)(void), char *nom);


// table globale
// table globale
// extern PROCESS table_proc[N_PROC];
extern PROCESS* table_proc[N_PROC];


extern PROCESS *current;

/* définir une liste des activables : il est sûrement plus efficace de conserver deux pointeurs, un sur
la tête et un autre sur la queue de la liste, pour garantir l’insertion en queue à coût constant.*/


// LIST
typedef struct ListProc {
    PROCESS *tete;
    PROCESS *queue;
} ListProc;

void init_list_proc(ListProc *list);

void init_list(ListProc *list);

// Fonction d'insertion en queue
void inserer_queue(ListProc *list, PROCESS *process);

// Fonction d'insertion en la list des endormis
void inserer_endormi(ListProc *list, PROCESS *process);

void dors(uint32_t nbr_secs);

// Fonction d'extraction de la tete
PROCESS *extraire_tete(ListProc *list);

// Fonction pour reveiller tous les processus dont l’heure de réveil est dépassée
void reveiller_procs(ListProc *list);

// Désactiver le processus actif (puisque c’est forcément lui qui l’appelle)
// et passer la main au prochain processus activable.
void fin_processus();

void affiche_etats(void);

extern ListProc activables; 

extern ListProc endormis; 

extern ListProc zombies; 