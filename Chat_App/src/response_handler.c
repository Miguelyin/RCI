#include "../include/response_handler.h"


void analize_response_for_join(char* response, char* mode, char* node_to_join, char *id){
    char message_copy[strlen(response) + 1];
    char tempid[20];
    int flag= 0;
    int novo_id= -1;
    int t_id = -1;

    srand(time(NULL));

    strcpy(message_copy, response);


    // Skip the "NODESLIST" part
    char *token = strtok(message_copy, "\n");
    if (token == NULL) {
        fprintf(stderr, "Error: Invalid message format.\n");
        return;
    }

    // Tokenize the rest of the message and count the nodes
    int num_nodes = 0;
    while ((token = strtok(NULL, "\n")) != NULL) {
        num_nodes++;

        sscanf(token, "%s", tempid);

        if (strcmp(tempid, id)==0)
        {
            flag = 1;
        }
        
    }

    if (flag == 1)
    {
        
        while (flag ==1)
        {
            strcpy(message_copy, response);


            // Skip the "NODESLIST" part
            char *token = strtok(message_copy, "\n");
            if (token == NULL) {
                fprintf(stderr, "Error: Invalid message format.\n");
                return;
            }

            flag = 0;

            novo_id = rand() % 100;
            while ((token = strtok(NULL, "\n")) != NULL) {

                sscanf(token, "%s", tempid);

                t_id = atoi(tempid);

                if (t_id==novo_id)
                {
                    flag = 1;
                }
                
            }

        }


        if (novo_id < 10) {
            sprintf(id, "0%d", novo_id);
        } else {
            sprintf(id, "%d", novo_id);
        }
        
        printf("\n\nO id escolhido já existe. Novo id: %s", id);
        fflush(stdout);
    }
    


    if (num_nodes == 0) {
        strcpy(mode, "empty_ring");
        return;
    }else if(num_nodes > 0){
        strcpy(mode, "ring_with_nodes");
    }

    // Select a random node
    int random_index = rand() % num_nodes + 1;

    // Reset the tokenizer to find the start position of the random node
    strcpy(message_copy, response);
    strtok(message_copy, "\n");

    while (random_index > 0)
    {
        token = strtok(NULL, "\n");
        random_index--;
    }
    
    strcpy(node_to_join, token);
 
}

void analize_response_for_chord(struct Node *Nodei, char* response, char* mode, char* node_to_join, int chords[]){

    char message_copy[strlen(response) + 1];
    char tempid[20];
    char tempIP[20];
    char tempTCP[20];
    int no_join=0;
    int chords_to_join=0;
    int t_id = -1;



    strcpy(message_copy, response);


    // Skip the "NODESLIST" part
    char *token = strtok(message_copy, "\n");
    if (token == NULL) {
        fprintf(stderr, "Error: Invalid message format.\n");
        return;
    }

    // Tokenize the rest of the message and count the nodes
    int num_nodes = 0;
    while ((token = strtok(NULL, "\n")) != NULL) {
        num_nodes++;

        sscanf(token, "%s", tempid);
        t_id = atoi(tempid);

        if((strcmp(tempid, Nodei->id)==0)||(strcmp(tempid, Nodei->sucessor->id)==0)||(strcmp(tempid, Nodei->predecessor->id)==0)||(chords[t_id]>0)){
            no_join++;
        }


        //Falta verificar se o id já existe
    }

    //printf("\n\nNúmero de nós: %d\n\n", num_nodes);

    chords_to_join = num_nodes - no_join;

    if (chords_to_join > 0){
        // Select a random node
        srand(time(NULL));
        int random_index = rand() % num_nodes + 1;

        // Reset the tokenizer to find the start position of the random node
        strcpy(message_copy, response);
        strtok(message_copy, "\n");

        while (random_index > 0)
        {
            token = strtok(NULL, "\n");
            random_index--;
        }
        
        strcpy(node_to_join, token);

        //printf("\n\nNode to join 1: %s\n\n", node_to_join);

        sscanf(node_to_join, "%s %s %s", tempid, tempIP, tempTCP);
        t_id = atoi(tempid);


        while ((strcmp(Nodei->id, tempid)==0)||(strcmp(Nodei->predecessor->id, tempid)==0) || (strcmp(Nodei->sucessor->id, tempid)==0)||(chords[t_id]>0))
        {
            //printf("\n\nPrimeiro nó para fazer corda invalido\n\n");

            random_index = rand() % num_nodes + 1;

            //printf("\n\nNovo random number : %d\n\n", random_index);

            strcpy(message_copy, response);

            //printf("\n\nReconstrução da mensagem : %s\n\n", message_copy);
            
            strtok(message_copy, "\n");
            while (random_index > 0)
            {
                token = strtok(NULL, "\n");
                random_index--;
            }
            strcpy(node_to_join, token);
            //printf("\n\nNovo nó para criar corda : %s\n\n", node_to_join);

            sscanf(node_to_join, "%s %s %s", tempid, tempIP, tempTCP);
            t_id = atoi(tempid);
            
        }
    }else
    {
        strcpy(mode, "No_Chord");
    }
    
    


 
}