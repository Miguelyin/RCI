//Ficheiro do protocolo chat

#include "../include/protocolo_chat.h"
#include "../include/protocolo_enc.h"

void chat(char* buffer, char* dest, struct Node *Nodei, int *sucfd, int *prefd, int chords [], DataFrame *tabela_exp){

    char message [256];
    char vizinho [5];
    int n2;
    int tempid;

    sprintf(message, "CHAT %s %s %s\n", Nodei->id, dest, buffer);
    //printf("\n\nMensagem Chat para enviar a um certo vizinho\n\n");

    Get_tabela_expedicao(tabela_exp, vizinho, dest);

    tempid = atoi(vizinho);

    if (strcmp(vizinho, Nodei->predecessor->id) == 0)
    {
        n2=write(*prefd, message, strlen(message));
        if(n2==-1)exit(1);
        
    }else if (strcmp(vizinho, Nodei->sucessor->id) == 0)
    {
        n2=write(*sucfd, message, strlen(message));
        if(n2==-1)exit(1);

    }else{
        if (chords[tempid] > 0)
        {
            n2=write(chords[tempid], message, strlen(message));
            if(n2==-1)exit(1);
        }
    }
    
    

    
    
}