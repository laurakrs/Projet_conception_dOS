#include <process.h>
#include <string.h>
// #include <stdint.h>



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

// PROCESS table_proc[N_PROC];


int num_proc = 1;
PROCESS *table_proc[N_PROC];


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

void proc2(void){
    for(;;){
        
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
        /*
        printf("[proc1] idle ma donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(table_proc[1].tab_reg, table_proc[0].tab_reg);*/
}

void proc3(void){
    for(;;){
        
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
        /*
        printf("[proc1] idle ma donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(table_proc[1].tab_reg, table_proc[0].tab_reg);*/
}

void proc4(void){
    for(;;){
        
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
        /*
        printf("[proc1] idle ma donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(table_proc[1].tab_reg, table_proc[0].tab_reg);*/
}

void proc5(void){
    for(;;){
        
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
        /*
        printf("[proc1] idle ma donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(table_proc[1].tab_reg, table_proc[0].tab_reg);*/
}

void proc6(void){
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
        if (table_proc[i]->etat == ELU){
            return table_proc[i]->pid;
        }
    }
    return -1;
}


char *mon_nom(void){
    for(int i = 0; i < N_PROC; i++){
        if (table_proc[i]->etat == ELU){
            return table_proc[i]->nom;
        }
    }
    return "f";   
}


/*
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

    // if(index == 0){
    //    next = 1;
    // else{
    //    next = 0;
    // }

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
}*/


/* Prend en paramètre le code de la fonction à exécuter (ainsi que le nom du processus) et renvoie le pid du
processus créé, ou -1 en cas d’erreur (si on a essayé de créer plus de processus que le nombre
maximum). */
int32_t cree_processus(void (*code)(void), char *nom){

    if(num_proc > N_PROC){ // no more slots available
        return -1; 
    }else{

        PROCESS *new_process = (PROCESS *) malloc(sizeof(PROCESS)); 

        if(new_process == NULL){ // allocation failed
            return -1;
        }

        // Initialize process fields:
        new_process->pid = num_proc;
        strncpy(new_process->nom, nom, MAX_LENGTH_NOM - 1);
        new_process->nom[MAX_LENGTH_NOM - 1] = '\0';

        new_process->etat = ACTIVABLE;
        new_process->tab_reg[1] = (uint32_t)(new_process->stack+SIZE_S-1);
        new_process->stack[SIZE_S-1] = (int)code;



        // Add process to the table
        table_proc[num_proc] = new_process;
        num_proc++;

        return new_process->pid;
    }
}

void init_list(ListActivables *list){
    list->tete = NULL;
    list->queue = NULL;

    for(int i = 0; i < num_proc; i++){
        inserer_queue(&list,table_proc[i]);
    }
}

// Fonction d'insertion en queue
void inserer_queue(ListActivables *list, PROCESS *process){
    process->suiv = NULL;   

    if(list->queue){
        list->queue->suiv = process;
    }else{
        list->tete = process;
    }

    list->queue = process;

    process->etat = ACTIVABLE;
}

// Fonction d'extraction de la tete
PROCESS *extraire_tete(ListActivables *list){

    if(list->tete){
        PROCESS *process = list->tete;
        list->tete = process->suiv;

        if(list->tete == NULL){ // if the list is now empty
            list->queue = NULL;
        }
        // process->suiv = NULL; // necessaire ?
        process->etat = ELU;
        return process;
    }else{
        return NULL; // list was empty. there is no process to return;
    }
}


/*
void ordonnance(void){
    uint32_t pid = mon_pid();
    int index = -1;
    int next = -1;
    // Parcourir la table de processus
    for(int i = 0; i < N_PROC; i++){
        if (table_proc[i]->pid == pid){
            index = i;
            break;
        }
    }

    if(index < num_proc-1){
        next = index+1;
    }else{
        next = 0;
    }
    
    table_proc[index]->etat = ACTIVABLE;
    table_proc[next]->etat = ELU;
    ctx_sw(table_proc[index]->tab_reg, table_proc[next]->tab_reg);
}*/


void ordonnance(void){

    PROCESS *next = extraire_tete(activables);

    if(next != NULL){
        current->etat = ACTIVABLE;
        inserer_queue(activables, current);

        next->etat = ELU;

        ctx_sw(current->tab_reg, next->tab_reg);
    }


   
}



