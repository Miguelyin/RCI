//ficheiro do protocolo topologico
#include "../include/protocolo_top.h"
#include "../include/protocolo_enc.h"
#include "../include/TCP_Client.h"
#include "../include/response_handler.h"







//If the task from the interface is Join ring id, does this function

void join(struct Node *node, char *ring, char *id, char *regUDP, char *regIP, struct Node *Nodesuc, struct Node *Nodesucsuc, int *sucfd, int sockets_fds[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota) {

    char response[300];
    char message[128] = "";
    char route_message[128] = "";
    char mode[20] = "";
    char node_to_join[60] = "";
    //char linhaAtualizada[5] = "";
    


    //Cria a mensagem NODES ring
    sprintf(message, "NODES %s", ring);
    //printf("\nMensagem UDP enviada ao servidor de Nós: \"%s\"\n", message);
    

    //manda a mensagem NODES ring e recebe a resposta com a lista de nós
    send_UDP(response, message, regUDP, regIP);
    //printf("\n\nMensagem UDP recebida do servidor de Nós: \"%s\"\n\n",response);


    //Esta função analiza a resposta obtida, e retorna o mode para fazer join e o node com o qual vamos fazer join
    analize_response_for_join(response, mode, node_to_join, id);
    //printf("\n\nnovo id em char: %s", id);

    //printf("\nNode to join: %s\n", node_to_join);


    //Cria a mensagem REG ring id IP TC

    //sprintf(message, "REG %d %d %d %d", ring, node->id, node->IP, node->TCP);

    sprintf(message, "REG %s %s %s %s",ring, id , node->IP, node->TCP);
    //printf("Mensagem UDP enviada ao servidor de Nós: \"%s\"\n", message);

    //manda a mensagem REG ring id IP TCP e recebe a resposta OKREG
    send_UDP(response, message, regUDP, regIP);
    //printf("Mensagem UDP recebida do servidor de Nós: \"%s\"\n",response);

    strcpy(node->id, id);


    //Quando o anel está vazio
    if(strcmp(mode, "empty_ring")==0){
        
        
        node->predecessor = node;
        node->sucessor = node;
        node->segundo_sucessor = node;
        
        //Atualiza tabela de encaminhamento
        strcpy(rota->dest_id, id);
        add_info_table(tabela_enc, tabela_cc, tabela_exp, rota, node);

        return; //Caso o anel esteja vazio, o join está completo

    }
    else if(strcmp(mode, "ring_with_nodes")==0){    //Quando o anel está com pelo menos um nó

        //Atualiza tabela de encaminhamento com o próprio nó
        strcpy(rota->dest_id, id);
        add_info_table(tabela_enc, tabela_cc, tabela_exp, rota, node);

        sscanf(node_to_join, "%s %s %s", Nodesuc->id, Nodesuc->IP, Nodesuc->TCP);
        node->sucessor = Nodesuc;


        sprintf(message, "ENTRY %s %s %s\n", id, node->IP, node->TCP);

        *sucfd = cria_socket_Cliente(Nodesuc->IP, Nodesuc->TCP);
        for (int j = 0; j < 100; j++){
            if (sockets_fds[j] < 0){    
                sockets_fds[j] = *sucfd;
                break;                          
            }

        }

        send_and_receive_TCP(message, response, *sucfd);
        


        //Envia mensagem ROUTE
        sprintf(route_message, "ROUTE %s %s %s\n", id, id, id);   //Envia mensagem ROUTE só com o id do próprio nó
        send_route_msg(route_message, *sucfd);
        

        sscanf(response, "SUCC %s %s %s\n", Nodesucsuc->id, Nodesucsuc->IP, Nodesucsuc->TCP);
        if (strcmp(Nodesucsuc->id,Nodesuc->id)==0)
        {
            strcpy(Nodesucsuc->id, node->id);
            strcpy(Nodesucsuc->IP, node->IP);
            strcpy(Nodesucsuc->TCP, node->TCP);
            
        }
        
        node->segundo_sucessor = Nodesucsuc;

        
    }




}

void direct_join(struct Node *node, char* id, struct Node *Nodesuc, struct Node *Nodesucsuc, int *sucfd, int sockets_fds[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota){

    char response[300];
    char message[128] = "";
    char route_message[128] = "";


    strcpy(node->id, id);

    node->sucessor = Nodesuc;

    //Atualiza tabela de encaminhamento com o próprio nó
    strcpy(rota->dest_id, id);
    add_info_table(tabela_enc, tabela_cc, tabela_exp, rota, node);

    sprintf(message, "ENTRY %s %s %s\n", id, node->IP, node->TCP);

    *sucfd = cria_socket_Cliente(Nodesuc->IP, Nodesuc->TCP);

    for (int j = 0; j < 100; j++){
        if (sockets_fds[j] < 0){    
            sockets_fds[j] = *sucfd;
            break;                          
        }

    }

    send_and_receive_TCP(message, response, *sucfd);

    //Envia mensagem ROUTE
    sprintf(route_message, "ROUTE %s %s %s\n", id, id, id);   //Envia mensagem ROUTE só com o id do próprio nó
    send_route_msg(route_message, *sucfd);
    

    sscanf(response, "SUCC %s %s %s\n", Nodesucsuc->id, Nodesucsuc->IP, Nodesucsuc->TCP);
    if (strcmp(Nodesucsuc->id,Nodesuc->id)==0)
    {
        strcpy(Nodesucsuc->id, node->id);
        strcpy(Nodesucsuc->IP, node->IP);
        strcpy(Nodesucsuc->TCP, node->TCP);
        
    }
    
    node->segundo_sucessor = Nodesucsuc;

    

}



void Chord(struct Node *Nodei, char *ring, char *regUDP, char *regIP, int *chordfd, int sockets_fds[], int chords[], DataFrame *tabela_cc){


    if (*chordfd>0)
    {
        printf("\n\nNão é possível criar uma corda quando já tinha criado outra. Remova primeiro a corda já existente com o comando \"remove chord\" ou apenas \"rc\"\n\n");
        return;
    }
    

    char response[300];
    char message[128] = "";
    char mode[20] = "";
    char node_to_join[60] = "";
    char tempid[20];
    char tempIP[20];
    char tempTCP[20];
    int t_id = -1;

    //Cria a mensagem NODES ring
    sprintf(message, "NODES %s", ring);

    //manda a mensagem NODES ring e recebe a resposta com a lista de nós
    send_UDP(response, message, regUDP, regIP);

    analize_response_for_chord(Nodei,response, mode, node_to_join, chords);

    
    if (strcmp(mode, "No_Chord")==0)
    {
        printf("\n\nNão é possível criar uma corda\n\n");

    }else{


        sscanf(node_to_join, "%s %s %s", tempid, tempIP, tempTCP);

        sprintf(message, "CHORD %s\n", Nodei->id);

        *chordfd = cria_socket_Cliente(tempIP, tempTCP);

        for (int j = 0; j < 100; j++){
            if (sockets_fds[j] < 0){    
                sockets_fds[j] = *chordfd;
                break;                          
            }

        }

        t_id = atoi(tempid);

        chords[t_id] = *chordfd;

        int n2=write(*chordfd,message,strlen(message));
        if(n2==-1)exit(1);

        usleep(500); // 500000 microssegundos = 0.5 segundos

        for (int y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
            sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
            
            if(*chordfd!= -1){
            send_route_msg(message, *chordfd);
            //printf("\nMensagem ROUTE ENVIADA: %s", message);
            }
        }   
    }

}



void Chord_to(struct Node *Nodei, int *chordfd, int sockets_fds[], int chords[], char *tempid, char *tempIP, char *tempTCP){


    if (*chordfd>0)
    {
        printf("\n\nNão é possível criar uma corda quando já tinha criado outra. Remova primeiro a corda já existente com o comando \"remove chord\" ou apenas \"rc\"\n\n");
        return;
    }
    

    char message[128] = "";
    int t_id = -1;
    

    sprintf(message, "CHORD %s\n", Nodei->id);

    *chordfd = cria_socket_Cliente(tempIP, tempTCP);

    for (int j = 0; j < 100; j++){
        if (sockets_fds[j] < 0){    
            sockets_fds[j] = *chordfd;
            break;                          
        }

    }

    t_id = atoi(tempid);

    chords[t_id] = *chordfd;

    int n2=write(*chordfd,message,strlen(message));
    if(n2==-1)exit(1);

}




void remove_chord(int *chordfd, int sockets_fds[], int chords[], int *sucfd, int *prefd, struct Node *Nodei, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp){

    char message[256];
    char chord_char[4];
    int return_remove;


    if (*chordfd==-1)
    {
        printf("\n\nNão consegue remover nenhuma corda. Crie primeiro uma corda com o comando \"chord\" ou apenas \"c\"\n\n");
        return;
    }


    for (int j = 0; j < 100; j++){
        if (sockets_fds[j] == *chordfd){    
            sockets_fds[j] = -1;
            break;                          
        }
    }

    for (int i = 0; i < 100; i++){
        if (chords[i] == *chordfd){    
            chords[i] = -1;
            if (i < 10) {
                sprintf(chord_char, "0%d", i);
            } else {
                sprintf(chord_char, "%d", i);
            }
            break;                          
        }
    }

    close(*chordfd);

    *chordfd = -1;

    usleep(500);

    //Envia msg a anunciar que a remoção da adjacência aos vizinhos
    if (*sucfd != -1){   
        sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
        send_route_msg(message, *sucfd);
    }
    if (*prefd != -1){   
        sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
        send_route_msg(message, *prefd);
    }
    for ( int k = 0; k < 100; k++)
    {
        if (chords[k]!= -1)
        {
            sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
            send_route_msg(message, chords[k]);
        }
        
    }
    

    return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, chord_char, *sucfd);
    
    if(return_remove == 3){
        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->predecessor->id);
        if (*sucfd != -1)
        {
            send_route_msg(message, *sucfd);
        }
        
    }


}








/*int Keep_ring(struct Node *Nodei, struct Node *Nodesuc, struct Node *Nodesucsuc, struct Node *Nodepre, int sockets[], int i, int prefd2, char *prt_str, char *ipstr, char *port_emissor, int sucfd2){

}*/












void show_topology(struct Node *node, int chords[]){

    int flag = 0;
    printf("\n----------------------------------------------------------------------------------------");
    fflush(stdout);
    printf("\n(i) Próprio Nó: ID=%s IP=%s Porta=%s", node->id, node->IP, node->TCP);
    fflush(stdout);
    printf("\n(ii) Sucessor: ID=%s IP=%s Porta=%s", node->sucessor->id, node->sucessor->IP, node->sucessor->TCP);
    fflush(stdout);
    printf("\n(iii) Segundo-Sucessor: ID=%s IP=%s Porta=%s", node->segundo_sucessor->id, node->segundo_sucessor->IP, node->segundo_sucessor->TCP);
    fflush(stdout);
    printf("\n(iv) Predecessor: ID=%s", node->predecessor->id);
    fflush(stdout);   
    printf("\n(v) Vizinhos da Corda:");
    fflush(stdout);
    for (int i = 0; i < 100; i++)
    {
        if (chords[i]>0)
        {
            if (i<10)
            {
                if (flag == 0)
                {
                    printf(" ID=0%d", i);
                    fflush(stdout);
                }else
                {
                    printf(", ID=0%d", i);
                    fflush(stdout);
                }
                
            }else
            {
                if (flag == 0)
                {
                    printf(" ID=%d", i);
                    fflush(stdout);
                }else
                {
                    printf(", ID=%d", i);
                    fflush(stdout);
                }
            }
            
            flag=1;
        }
        
    }
    if (flag == 0)
    {
        printf(" Não há cordas");
        fflush(stdout);
    }
    printf("\n----------------------------------------------------------------------------------------\n\n");
    fflush(stdout);
    
}



void leave(struct Node *node, char *ring, char *regUDP, char *regIP, int sockets[], int chords[], int *sucfd, int *prefd, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp){

    char response[128] = "";
    char message[128] = "";


    sprintf(message, "UNREG %s %s", ring, node->id);
    send_UDP(response, message, regUDP, regIP);

    node->predecessor = node;
    node->sucessor = node;
    node->segundo_sucessor = node;
    
    //Fazer a lógica para sairmos do anel
    close_TCP(sockets);
    for (int i = 0; i < 100; i++) {    //Não vamos fechamos o socket do TCP Server
        if (chords[i] != -1) {
            chords[i] = -1;
        }
    }
    
    limparTabela(tabela_enc);
    limparTabela(tabela_cc);
    limparTabela(tabela_exp);

    *prefd = -1;
    *sucfd = -1;
    
    limparTabela(tabela_enc);
    limparTabela(tabela_cc);
    limparTabela(tabela_exp);

}