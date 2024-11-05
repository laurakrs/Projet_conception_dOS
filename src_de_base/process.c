#include <process.h>


/* struct Process {
    // le pid du processus, sous la forme d’un entier (signe -1 en cas derreur)
    uint32_t pid;
    // un chaine de caractères avec une taille maximum fixée ;
    char nom[50];
    // dans ce premier exemple, il n’y a que 2 états possibles : élu ou activable, 
    enum ETAT etat;

    la zone de sauvegarde des registres du processeur : il s’agira tout simplement d’un tableau d’entiers,
    puisqu’on manipule des registres 32 bits, et vous avez besoin de 5 cases car il n’y a que 5 registres
    importants à sauvegarder sur x86 (voir plus bas l’explication du changement de contexte) ;

   uint32_t tableau_reg[N_REG]; 

    — la pile d’exécution du processus, qui sera définie comme un tableau d’entiers d’une taille fixée par
    une constante, par exemple 512.

   int pile[TAILLE_P];
};

 */

PROCESS *table_proc[N_PROC];
int num_proc = 0;

void idle(void){

        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            ordonnance();
        }
    /*
    for (int i = 0; i < 3; i++) {
        printf("[idle] je tente de passer la main a proc1...\n");
        ctx_sw(table_proc[0].tab_reg, table_proc[1].tab_reg);
        printf("[idle] proc1 ma redonne la main\n");
    }
    printf("[idle] je bloque le systeme\n");
    hlt();*/
}

void proc1(void){
    for(;;){
        
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
        /*
        printf("[proc1] idle ma donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(table_proc[1].tab_reg, table_proc[0].tab_reg);*/
}

int32_t mon_pid(void){
    for(int i = 0; i < N_PROC; i++){
        if (*table_proc[i] != NULL)
        
        ->etat == ELU){
            return *table_proc[i].pid;
        }
    }
    return -1;
}

char *mon_nom(void){
    for(int i = 0; i < N_PROC; i++){
        if (*table_proc[i].etat == ELU){
            return table_proc[i].nom;
        }
    }
    return "f";   
}

void ordonnance(void){
    uint32_t pid = mon_pid();
    int index = -1;
    int next = -1;
    for(int i = 0; i < N_PROC; i++){
        if (table_proc[i].pid == pid){
            index = i;
            break;
        }
    }

    /*
    if(index == 0){
        next = 1;
    }else{
        next = 0;
    }*/
    if(index < N_PROC){
        next = index+1;
    }else{
        next = 0;
    }
    
    table_proc[index].etat = ACTIVABLE;
    table_proc[next].etat = ELU;
    ctx_sw(table_proc[index].tab_reg, table_proc[next].tab_reg);
    // ctx_sw(table_proc[0].tab_reg, table_proc[1].tab_reg);
    // ctx_sw(table_proc[index].tab_reg, table_proc[next].tab_reg);
}

int32_t cree_processus(void (*code)(void), char *nom){



    PROCESS *new = (PROCESS *) malloc(sizeof(PROCESS)); // WRONG!!!

    if(num_proc > N_PROC){
        return -1;
    }else{
        table_proc[num_proc] = new;
        num_proc++;

        return table_proc[num_proc-1].pid;
    }

    

    // error
    return -1;
}


