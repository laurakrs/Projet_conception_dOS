#include <process.h>
#include <string.h>
// #include <stdint.h>
#include <cpu.h>
#include <timer.h>
#include <ecran.h>



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
ListProc activables; 
ListProc endormis;
ListProc zombies;
PROCESS *current;

const char *etat_noms[] = {
    "ELU",
    "ACTIVABLE",
    "ENDORMI",
    "ZOMBIE"
};

void idle(void){
    for (;;) {
        sti();
        hlt();
        cli();
    }
}

void proc1(void){
    for (int32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(2);
    }   
    fin_processus();
}

void proc2(void){
    for (int32_t i = 0; i < 4; i++) {
        uint32_t rev = (uint32_t)(nbr_secondes() + 3);
        printf("[temps = %u] processus %s pid = %i . [Rev = %u]\n", nbr_secondes(), mon_nom(), mon_pid(), rev);
        dors(3);
    }
    fin_processus();
   
}

void proc3(void){
    for (int32_t i = 0; i < 8; i++) {
        uint32_t rev = (uint32_t)(nbr_secondes() + 5);
        printf("[temps = %u] processus %s pid = %i . [Rev = %u]\n", nbr_secondes(), mon_nom(), mon_pid(), rev);
        dors(5);
    }
    fin_processus();
}

void proc4(void){
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc5(void){
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc6(void){
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc7(void){
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
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
        table_proc[num_proc-1]->suiv = new_process;


        new_process->suiv = NULL;
  
        num_proc++;

        return new_process->pid;
    }
}

void init_list_proc(ListProc *list){
    list->tete = NULL;
    list->queue = NULL;

    for(int i = 0; i < num_proc; i++){
        inserer_queue(list,table_proc[i]);
    }
}

void init_list(ListProc *list){
    list->tete = NULL;
    list->queue = NULL;
}

// Fonction d'insertion en queue
void inserer_queue(ListProc *list, PROCESS *process){
    process->suiv = NULL;   

    if(list->queue){
        list->queue->suiv = process;
    }else{
        list->tete = process;
    }

    list->queue = process;

    process->etat = ACTIVABLE;
}

void inserer_queue_zombie(ListProc *list, PROCESS *process){
    process->suiv = NULL;   

    if(list->queue){
        list->queue->suiv = process;
    }else{
        list->tete = process;
    }

    list->queue = process;
    
    process->etat = ZOMBIE;
}

// Fonction d'extraction de la tete
PROCESS *extraire_tete(ListProc *list){

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


void ordonnance(void){

    // C’est la fonction d’ordonnancement qui devra réveiller tous les processus dont l’heure de réveil est dépassée.
    reveiller_procs(&endormis);
    
    if((current != NULL)){
    // & (current != next)){
        current->etat = ACTIVABLE;
        inserer_queue(&activables, current);
    }

    // Extraire la tete des activables
    PROCESS *next = extraire_tete(&activables);

    next->etat = ELU;

    PROCESS *prev = current;
    current = next;
    affiche_etats();
    ctx_sw(prev->tab_reg, next->tab_reg);
    
}

void ordonnance_endormi(void){

    // C’est la fonction d’ordonnancement qui devra réveiller tous les processus dont l’heure de réveil est dépassée.
    reveiller_procs(&endormis);

    // Extraire la tete des activables
    PROCESS *next = extraire_tete(&activables);

    if(next != NULL){
        next->etat = ELU;

        PROCESS *prev = current;
        current = next;
        affiche_etats();
        ctx_sw(prev->tab_reg, next->tab_reg);
    }
}

void reveiller_procs(ListProc *list){

    // Trouver les seconds passes
    uint32_t temps = nbr_secondes();

    // Trouver la tete de la liste dendormis
    PROCESS* curr = list->tete;

    // il n'y a aucun processus a reveiller
    if(curr == NULL){
        return;
    }

    while(curr != NULL){
        if(curr->heure_reveil <= temps){
            
            // remove from the list of endormis
            PROCESS* proc = extraire_tete(list);

            // insert in the list of activables
            inserer_queue(&activables, proc);

            // mettre a jour curr avec la nouvelle tete
            curr = list->tete;
        }else{
            // il ne faut pas reveiller dautres processus
            return;
        }
    }
    return;
}

// void dors(uint32_t nbr_secs) qui prend en paramètre le nombre de secondes pendant lequel le processus doit dormir
void dors(uint32_t nbr_secs){
    /*gérer une liste des processus endormis :
    lorsqu’un processus appelle la procédure d’endormissement*/
    
    // on l’enlève de la file des activables 
    // PROCESS* proc = extraire_tete(&activables);

    // heure_reveil = current time + nbr_secs dans lequel le processus doit dormir
    current->heure_reveil = (uint32_t)(nbr_secondes() + nbr_secs);
    
    
    // et on l’ajoute dans cette liste des endormis, et vice-versa quand il se réveille.
    inserer_endormi(&endormis, current);
    
    ordonnance_endormi();

}

// Fonction d'insertion en la liste des endormis
void inserer_endormi(ListProc *list, PROCESS *process){

    // les processus devant se réveiller en premier seront en tête de liste 

    PROCESS* curr = list->tete;
    PROCESS* prev = NULL;

    // if the list is empty, insert the process as tete
    if (curr == NULL) {
        list->tete = process;
        list->queue = process;
        process->etat = ENDORMI;
        return;
    }

    // if process should be inserted at the end
    if(list->queue->heure_reveil < process->heure_reveil){
        list->queue->suiv = process;
        list->queue = process;
        process->suiv = NULL;
        process->etat = ENDORMI;
        return;
    }

    // if the process in tete should be awaken after the new process
    if(curr->heure_reveil > process->heure_reveil){   
        // the new process should occupy the tete
        process->suiv = curr;
        list->tete = process;

    }else{

        // should find the correct place
        while(curr != NULL && (curr->heure_reveil <= process->heure_reveil)){
            prev = curr;
            curr = curr->suiv;
        }

        // either found a process with a later heure-reveil or reached the end of the list

        prev->suiv = process;

        if(curr != NULL){
            // insert process in the correct place
            process->suiv = curr;
        }else{ // reached the end of the list
            list->queue = process;
            process->suiv = NULL;
        }       
    }

    process->etat = ENDORMI;
}

void fin_processus(){
    // Désactiver le processus actif (puisque c’est forcément lui qui l’appelle)
    inserer_queue_zombie(&zombies, current);

    // Passer la main au prochain processus activable.
    // Extraire la tete des activables
    PROCESS *next = extraire_tete(&activables);

    next->etat = ELU;

    PROCESS *prev = current;
    current = next;
    affiche_etats();
    ctx_sw(prev->tab_reg, next->tab_reg);

}

void affiche_etats(void) {

    // Get line and column
    uint32_t lig = 0;
    uint32_t col = 0;

    const char *entete = "PID  | Etat     ";
    for (int i = 0; entete[i] != '\0'; i++) {
        ecrit_car(lig, col + i, entete[i], false, BLANC, NOIR);
    }

    lig++;

    // Parcourir les processus
    for (int i = 0; i < num_proc; i++) {

        
        // Ecrire pid
        char pid_str[10]; 
        sprintf(pid_str, "%d", table_proc[i]->pid);

        int offset = 0; 
        for (int j = 0; pid_str[j] != '\0'; j++) {
            ecrit_car(lig, col + offset + j, pid_str[j], FALSE, BLANC, NOIR);
        }

        offset += strlen(pid_str); 

        const char *separator = " | ";
        for (int j = 0; separator[j] != '\0'; j++) {
            ecrit_car(lig, col + offset + j, separator[j], FALSE, BLANC, NOIR);
        }

        offset += strlen(separator); 

        // Ecrire Etat
        enum ETAT etat = table_proc[i]->etat;
        const char *etat_str = etat_noms[etat];

        for (int j = 0; etat_str[j] != '\0'; j++) {
            ecrit_car(lig, col + offset + j, etat_str[j], FALSE, BLANC, NOIR);
        }

        lig++; // Prochain processus
    }
}




