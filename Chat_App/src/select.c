#include "../include/select.h"
#include "../include/protocolo_top.h"
#include "../include/protocolo_enc.h"
#include "../include/TCP_Server.h"
#include <sys/select.h>
#include "../include/TCP_Client.h"
#include "../include/interface.h"
#include <arpa/inet.h> // for sockaddr_in and inet_ntop
#include <sys/socket.h>
#define MAX_CONNECTIONS 100



int func_select(struct Node *Nodei, char *ring, char *id, char *regUDP, char *regIP, struct Node *Nodepre, struct Node *Nodesuc, struct Node *Nodesucsuc, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota)
{
    char in_str[128];
    char command[128];
    char command2[128];
    char command3[128];
    char command4[128];

    fd_set inputs, testfds;
    struct timeval timeout;

    int i,out_fds, var, y;
    int flag = 0;
    char *token2;
    char *token3;
    

    char prt_str[400];
    int sockets[MAX_CONNECTIONS];
    int chords[MAX_CONNECTIONS];


// socket variables
    int tcp_fd, new_fd;
    int prefd = -1;
    int sucfd = -1;

    int chordfd = -1;
    char chord_char[4];


    int return_remove = 0;

    char chord[20];
    int t_id;

    char temp[90];
    int n =-1;
    int n3= -1;

    ssize_t n2;
    char temp_id[20];
    char temp_id2[20];
    char temp_IP[25];
    char temp_TCP[20];
    char message[256] = "";
    char ipstr[INET6_ADDRSTRLEN];
    char port_emissor[6];
    int port_em;

    char inicio[5], destino[5], vizinho[5];
    char buffer[256];


    for (i = 0; i < MAX_CONNECTIONS; i++){
        sockets[i] = -1;
    }
    for (i = 0; i < MAX_CONNECTIONS; i++){
        chords[i] = -1;
    }

// TCP server section
    tcp_fd = cria_socket_principal(Nodei->TCP);
    if (tcp_fd==-1) exit(1); //error
    
     // O primeiro fd é o deste nó, útil para quando queremos identificar novas conexões
    sockets[0] = tcp_fd;


    
    while(1)
    {   
        FD_ZERO(&inputs); // Clear input mask
        FD_ZERO(&testfds);
        FD_SET(0,&inputs); // Set standard input channel on


        memset(prt_str, 0, sizeof prt_str);
        memset((void *)&timeout,0,sizeof(timeout));
        timeout.tv_sec=30;

        //Verificar bem este ciclo for, alterar se necessário
        for(i = 0; i < MAX_CONNECTIONS; i++){
            if(sockets[i] >= 0)
                FD_SET(sockets[i], &inputs);   //FD das conexões TCP
        }

        testfds=inputs; // Reload mask


        out_fds=select(FD_SETSIZE,&testfds,(fd_set *)NULL,(fd_set *)NULL,NULL);
        
        switch(out_fds){
            case 0:
                break;
            case -1:
                perror("select");
                printf("error");
                exit(1);
            default:
                if(FD_ISSET(0,&testfds))    //Input do teclado
                {
                    fgets(in_str, 50, stdin);
                    printf("---Input at keyboard: %s\n",in_str);
                    
                    sscanf(in_str, "%s", command);
                    
                    
                    var = menu_opt(in_str, sockets, Nodei, ring, id, regUDP, regIP, Nodesuc, Nodesucsuc, &sucfd, &prefd, &chordfd, chords, tabela_enc, tabela_cc, tabela_exp, rota);

                    if(var == 0){   //exit
                        return 0;
                    }
                    

                }
                
                if (FD_ISSET(tcp_fd, &testfds))   //quando novo nó quer-se conectar
                {
                    

                    struct sockaddr_in new_addr;
                    socklen_t addrlen = sizeof(new_addr);
                    new_fd = accept(tcp_fd, (struct sockaddr *)&new_addr, &addrlen);
                    
                    if (new_fd >= 0){
                        
                        for (i = 0; i < MAX_CONNECTIONS; i++){
                            if (sockets[i] < 0){    
                                sockets[i] = new_fd;        
                                break;                          
                            }
                        }
                    }
                }
                
                    
                for (i = 1; i < MAX_CONNECTIONS; i++){
                    if ((sockets[i] > 0) && (FD_ISSET(sockets[i], &testfds)))  //mudar pra socket tcp de conexões já estabelecidas
                    { 

                        
                        strcpy(temp_id, "");
                        strcpy(temp_id2,"");
                        strcpy(temp_IP, "");
                        strcpy(temp_TCP, "");
                        strcpy(message, "");
                        strcpy(ipstr, "");
                        strcpy(port_emissor, "");

                        new_fd = sockets[i];

                        ssize_t n1 = recv(sockets[i], prt_str, sizeof(prt_str), 0);
                        if (n1 == -1) {
                            perror("recv");
                            exit(1);
                        }
                        else if (n1 == 0) {  // Se recv retornar 0, a conexão foi fechada pelo outro lado
                            /*printf("Conexão fechada pelo outro lado\n");
                            printf("\n\n i= %d", i);
                            printf("\n\n sockets[i]= %d", sockets[i]);
                            printf("\n\n sucfd= %d", sucfd);
                            printf("\n\n prefd= %d", prefd);
                            */

                            if (sucfd==sockets[i]) //A conexão com o sucessor fechou
                            {
                                if (strcmp(Nodei->id, Nodei->segundo_sucessor->id)==0)  //Caso fiquemos sozinhos no anel
                                {
                                    //Função que atualiza a tabela de encaminhamento, remove linha e remove coluna
                                    return_remove = removeColumn(Nodei, tabela_enc,tabela_cc, tabela_exp, Nodei->sucessor->id, -99); //Alterar para não mandar msg a ninguém neste caso

                                    Nodei->sucessor=Nodei;
                                    Nodei->predecessor=Nodei;
                                    Nodei->segundo_sucessor=Nodei;
                                    //printf("\n\nSaiu um nó e fiquei sozinho no anel :/\n\n");
                                    FD_CLR(sucfd,&testfds);
                                    close(sucfd);
                                    sucfd = -1;
                                    sockets[i]=-1;

                                    for (int j = 0; j < MAX_CONNECTIONS; j++){
                                        if (sockets[j] == prefd){    
                                            sockets[j] = -1;
                                            break;                          
                                        }
                                    }

                                    FD_CLR(prefd,&testfds);
                                    close(prefd);
                                    prefd = -1;
                                    
                                    
                                }else   //Caso não fiquemos sozinhos no anel
                                {
                                    close(sucfd);
                                    sucfd = -1;

                                    if (chords[atoi(Nodei->segundo_sucessor->id)]>0)    //Caso tenhamos uma corda com o 2º sucessor como ele passa a sucessor, fechamos a corda
                                    {
                                        for (i = 0; i < 100; i++)
                                        {
                                            if (sockets[i] == chords[atoi(Nodei->segundo_sucessor->id)])
                                            {
                                                close(sockets[i]);
                                                sockets[i]= -1;
                                                break;
                                            }   
                                        }

                                        chords[atoi(Nodei->segundo_sucessor->id)] = -1;  

                                         usleep(500);

                                        //Envia msg a anunciar que a remoção da adjacência aos vizinhos
                                        if (sucfd != -1){   
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->segundo_sucessor->id);
                                            
                                            send_route_msg(message, sucfd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        if (prefd != -1){   
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->segundo_sucessor->id);
                                            
                                            send_route_msg(message, prefd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        for ( int k = 0; k < 100; k++)
                                        {
                                            if (chords[k]!= -1)
                                            {
                                                sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->segundo_sucessor->id);
                                                
                                                send_route_msg(message, chords[k]);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                            
                                        }
                                        

                                        return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, Nodei->segundo_sucessor->id, -99);
                                        
                                        if(return_remove == 3){
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->predecessor->id);
                                            
                                            if (sucfd != -1)
                                            {
                                                send_route_msg(message, sucfd);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                            
                                        } 

                                        if(chords[atoi(Nodei->segundo_sucessor->id)] == chordfd){
                                            chordfd = -1;
                                        }                                     
                                    }
                                    
                                    
                                    //Envia msg a anunciar que a remoção da adjacência aos vizinhos
                                    if (prefd != -1){
                                        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->sucessor->id);
                                        
                                        send_route_msg(message, prefd);
                                        //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                    }
                                    usleep(5000); // 500000 microssegundos = 0.5 segundos

                                    //Função para remover coluna na tabela de encaminhamento
                                    return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, Nodei->sucessor->id, prefd);

                                    if(return_remove == 3){
                                        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->sucessor->id);
                                        
                                        if (prefd != -1)
                                        {
                                            send_route_msg(message, prefd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        
                                    }

                                    usleep(5000);

                                    //Função para encontrar o caminho mais curto até o nó que saiu

                                    sucfd = cria_socket_Cliente(Nodei->segundo_sucessor->IP, Nodei->segundo_sucessor->TCP);
                                    //printf("\n\nCriei uma conexão com o antigo segundo sucessor; sucfd = %d\n\n", sucfd);
                                    sockets[i] = sucfd;

                                    strcpy(Nodesuc->id,Nodei->segundo_sucessor->id);
                                    strcpy(Nodesuc->IP,Nodei->segundo_sucessor->IP);
                                    strcpy(Nodesuc->TCP,Nodei->segundo_sucessor->TCP);

                                    Nodei->sucessor = Nodesuc;

                                    sprintf(message, "PRED %s\n", Nodei->id);
                                    n2 = write(sucfd,message,strlen(message));
                                    //printf("\n\nEnviei mensagem PRED\n\n");

                                    sprintf(message, "SUCC %s %s %s\n", Nodei->sucessor->id, Nodei->sucessor->IP, Nodei->sucessor->TCP);
                                    n1=write(prefd,message,strlen(message));
                                    //printf("\n\nEnviei mensagem SUUC\n\n");

                                    usleep(500); // 500000 microssegundos = 0.5 segundos

                                    for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                        sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                        
                                        if(sucfd!= -1){
                                        send_route_msg(message, sucfd);
                                        //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                    }   


                                    
                                }
                                
                                
                            }else if (prefd==sockets[i])    //Quando o predecessor sai
                            {
                                if (strcmp(Nodei->id, Nodei->segundo_sucessor->id)!=0)  //Se não ficar sozinho
                                {                          


                                    //Envia msg a anunciar que a remoção da adjacência aos vizinhos
                                    if (sucfd != -1){   
                                        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->predecessor->id);
                                        
                                        send_route_msg(message, sucfd);
                                        //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                    }

                                    return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, Nodei->predecessor->id, sucfd);
                                    
                                    if(return_remove == 3){
                                        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->predecessor->id);
                                        
                                        if (sucfd != -1)
                                        {
                                            send_route_msg(message, sucfd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        
                                    }

                                    flag = 1;
                                    FD_CLR(prefd,&testfds);
                                    close(prefd);

                                    sockets[i]= -1;
                                    prefd = -1;
                                    
                                }
                                else
                                {
                                    return_remove = removeColumn(Nodei, tabela_enc,tabela_cc, tabela_exp, Nodei->sucessor->id, -99);
                                    FD_CLR(prefd,&testfds);
                                    close(prefd);

                                    sockets[i]= -1;
                                }
                                
                            }else //Quando uma corda se desconecta
                            {

                                for (int j = 0; j < 100; j++)
                                {
                                    if (chords[j]==sockets[i])
                                    {
                                        if (j < 10) {
                                            sprintf(chord_char, "0%d", j);
                                        } else {
                                            sprintf(chord_char, "%d", j);
                                        }

                                        if(chords[j]==chordfd){

                                            chordfd = -1;
                                        }
                                        chords[j] = -1;

                                        //Envia msg a anunciar que a remoção da adjacência aos vizinhos
                                        if (sucfd != -1){   
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
                                            send_route_msg(message, sucfd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        if (prefd != -1){   
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
                                            
                                            send_route_msg(message, prefd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        for ( int k = 0; k < 100; k++)
                                        {
                                            if (chords[k]!= -1)
                                            {
                                                sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
                                                
                                                send_route_msg(message, chords[k]);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                            
                                        }
                                        

                                        return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, chord_char, sucfd);
                                        
                                        if(return_remove == 3){
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, chord_char);
                                            
                                            if (sucfd != -1)
                                            {
                                                send_route_msg(message, sucfd);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                            
                                        }

                                        FD_CLR(sockets[i],&testfds);
                                        close(sockets[i]);
                                        sockets[i]= -1;

                                        //("\n\n Conexão fechada com a corda %d\n\n", j);
                                        break;
                                    }
                                    
                                }
                                
                            }
                            
                            
                            
                            continue; // Continue com o próximo descritor de arquivo
                        }


                        struct sockaddr_storage addr;
                        socklen_t addr_size = sizeof(addr);
                        if (getpeername(sockets[i], (struct sockaddr *)&addr, &addr_size) == -1) {
                            perror("getpeername");
                        } else {

                            if (addr.ss_family == AF_INET) {
                                struct sockaddr_in *s = (struct sockaddr_in *)&addr;
                                port_em = ntohs(s->sin_port);
                                inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
                            } else { // AF_INET6
                                struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
                                port_em = ntohs(s->sin6_port);
                                inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
                            }
                            snprintf(port_emissor, sizeof(port_emissor), "%d", port_em);

                            //printf("Connection from %s:%s\n", ipstr, port_emissor);
                        }

                            
                        //printf("\n\nMensagem TCP Recebida: %s\n\n", prt_str);  

                        token3 = prt_str;

                       //sscanf(prt_str, "%s", command);
                        while (sscanf(token3, "%s %s %s %s\n", command, command2, command3, command4) >= 2){

                        


                            //Caso o programa receba uma mensagem ENTRY
                            if(strcmp(command, "ENTRY")==0){

                                sscanf(token3, "ENTRY %s %s %s\n", temp_id, temp_IP, temp_TCP);


                                /*
                                printf("\n\nMENSAGEM RECEBIDA DO FD: %d", sockets[i]);
                                printf("\n\nSUCFD: %d", sucfd);
                                */

                                if (sucfd == sockets[i])    //Quando recebe ENTRY do nó sucessor
                                {
                                    close(sucfd);

                                    if (strcmp(Nodei->sucessor->id, Nodei->predecessor->id)!=0) //Se houver mais do que 2 nós no anel
                                    {
                                        
                                        sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->sucessor->id);
                                        
                                        if(prefd!= -1){
                                            send_route_msg(message, prefd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                        return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, Nodei->sucessor->id, prefd); //?
                                        
                                        if(return_remove == 3){
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->sucessor->id);
                                            
                                            if (prefd != -1)
                                            {
                                                send_route_msg(message, prefd);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                        
                                        }
                                        
                                    }
                                    

                                    sucfd = cria_socket_Cliente(temp_IP, temp_TCP);
                                    sockets[i] = sucfd;

                                    //printf("\n\nsucfd= %d", sucfd);
                                    sprintf(message, "PRED %s\n", Nodei->id);
                                    n2 = write(sucfd, message, strlen(message)); //envia PRED pra o nó que acabou de entrar (novo sucessor)

                                    //printf("\n\nMensagem PRED: %s\n\n", message);

                                    usleep(500); // 500000 microssegundos = 0.5 segundos

                                    for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                        sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                        

                                        if (sucfd != -1){
                                            send_route_msg(message, sucfd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                    }
                                    
                                    //atualiza o antigo sucessor para segundo sucessor
                                    strcpy(Nodesucsuc->id, Nodesuc->id);
                                    strcpy(Nodesucsuc->IP, Nodesuc->IP);
                                    strcpy(Nodesucsuc->TCP, Nodesuc->TCP);

                                    Nodei->segundo_sucessor = Nodesucsuc;

                                    //atualiza novo nó que entrou para sucessor
                                    strcpy(Nodesuc->id, temp_id);
                                    strcpy(Nodesuc->IP, temp_IP);
                                    strcpy(Nodesuc->TCP, temp_TCP);

                                    Nodei->sucessor = Nodesuc;

                                    //envia SUCC pra o PRED
                                    sprintf(message,"SUCC %s %s %s\n", Nodei->sucessor->id, Nodei->sucessor->IP, Nodei->sucessor->TCP);
                                    //printf("\n\nMensagem %s enviada no predfd %d\n\n", message, prefd);
                                    n2=write(prefd,message,strlen(message));
                                        if(n2==-1)exit(1);

                                    //Enviar msg ROUTE para o predecessor

                                    
                                }else{  //Quando a mensagem ENTRY não é vinda no sucessor, mas de um novo nó

                                    
                                    //Envia-se a mensagem SUCC para definir 
                                    sprintf(message, "SUCC %s %s %s\n", Nodei->sucessor->id, Nodei->sucessor->IP, Nodei->sucessor->TCP);
                                    n1=write(sockets[i],message,strlen(message));
                                    if(n1==-1) exit(1);

                                    //printf("\n\nMensagem TCP Enviada: %s\n\n", message); 

                                    usleep(500); // 500000 microssegundos = 0.5 segundos

                                    for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                        sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                        
                                        if(sockets[i]!= -1){
                                        send_route_msg(message, sockets[i]);
                                        //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                    }   

                                    //Caso o nó não seja o seu proprio predessedor, envia o entry para o seu predessedor.
                                    if (Nodei->predecessor->id!=Nodei->id)
                                    {
                                        //Reencaminha o ENTRY pra o seu predecessor
                                        //printf("\n\nMensagem entry enviada no prefd %d\n\n", prefd);
                                        n2=write(prefd,token3,strlen(token3));
                                        if(n2==-1)exit(1);

                                        if (strcmp(Nodei->predecessor->id, Nodei->sucessor->id)!=0) //Se houver mais do que 2 nós no anel
                                        {
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->sucessor->id);
                                            
                                            if(sucfd != -1){
                                                send_route_msg(message, sucfd);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                            }
                                            return_remove = removeColumn(Nodei, tabela_enc, tabela_cc, tabela_exp, Nodei->predecessor->id, sucfd);   //?

                                            if(return_remove == 3){
                                                sprintf(message, "ROUTE %s %s\n", Nodei->id, Nodei->predecessor->id);
                                                
                                                if (sucfd != -1)
                                                {
                                                    send_route_msg(message, sucfd);
                                                    //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                                }
                                            
                                            }

                                            
                                        }

                                        //atualiza novo nó como predecessor
                                        strcpy(Nodepre->id, temp_id);
                                        strcpy(Nodepre->IP, temp_IP);
                                        strcpy(Nodepre->TCP, temp_TCP);

                                        for (int j = 0; j < MAX_CONNECTIONS; j++){
                                            if (sockets[j] == prefd){    
                                                sockets[j] = -1;
                                                break;                          
                                            }
                                        }
                                        close(prefd);
                                        

                                        prefd = sockets[i];
                                        //printf("\n\nNovo prefd: %d\n\n", prefd);

                                        Nodei->predecessor = Nodepre;

                                        for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                                sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                                
                                                send_route_msg(message, prefd);
                                                //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                


                                    }else{  //Quando só há um nó no anel

                                        sucfd = cria_socket_Cliente(temp_IP, temp_TCP);
                                        //printf("\n\nsucfd= %d", sucfd);
                                        for (int j = 0; j < 100; j++){
                                            if (sockets[j] < 0){    
                                                sockets[j] = sucfd;
                                                break;                          
                                            }

                                        }
                                        //printf("\n\nNOVO SUCFD == %d", sucfd);

                                        //printf("\n\nsucfd= %d", sucfd);
                                        sprintf(message, "PRED %s\n", Nodei->id);
                                        n2 = write(sucfd,message,strlen(message));

                                        //printf("\n\nMensagem PRED: %s\n\n", message);

                                        usleep(500); // 500000 microssegundos = 0.5 segundos

                                        for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                                sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                                
                                                if (sucfd != -1){
                                                    send_route_msg(message, sucfd);
                                                    //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                                }
                                        }

                                        
                                        
                                        prefd = sockets[i];
                                        //printf("\n\nNovo prefd: %d\n\n", prefd);

                                        strcpy(Nodepre->id, temp_id);
                                        strcpy(Nodepre->IP, temp_IP);
                                        strcpy(Nodepre->TCP, temp_TCP);

                                        strcpy(Nodesuc->id, temp_id);
                                        strcpy(Nodesuc->IP, temp_IP);
                                        strcpy(Nodesuc->TCP, temp_TCP);

                                        Nodei->predecessor = Nodepre;
                                        Nodei->sucessor = Nodesuc;
                                        
                                    }

                                }
                                
                            }
                            //Quando recebemos PRED de um nó
                            else if ((strcmp(command, "PRED")==0)&&(flag == 0))
                            {
                                //printf("\n\nRecebi mensagem pred; Pred = %d\n\n", sockets[i]);
                                prefd = sockets[i];
                                //printf("\n\nNovo prefd: %d\n\n", prefd);

                                for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                        sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                        
                                        if (prefd != -1){
                                            send_route_msg(message, prefd);
                                            //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                        }
                                    }

                                sscanf(token3, "PRED %s\n", temp_id2);

                                
                                strcpy(Nodepre->id, temp_id2);
                                strcpy(Nodepre->IP, ipstr);
                                strcpy(Nodepre->TCP, port_emissor);

                                Nodei->predecessor = Nodepre;
                                
                                
                            }
                            else if((strcmp(command, "PRED")==0)&&(flag == 1)){

                                //printf("\n\nRecebi mensagem pred; Pred = %d\n\n", sockets[i]);
                                prefd = sockets[i];
                                //printf("\n\nNovo prefd: %d\n\n", prefd);

                                sscanf(token3, "PRED %s\n", temp_id2);

                                
                                strcpy(Nodepre->id, temp_id2);
                                strcpy(Nodepre->IP, ipstr);
                                strcpy(Nodepre->TCP, port_emissor);

                                Nodei->predecessor = Nodepre;

                                sprintf(message, "SUCC %s %s %s\n", Nodei->sucessor->id, Nodei->sucessor->IP, Nodei->sucessor->TCP);
                                n1=write(prefd,message,strlen(message));

                                flag = 0;


                            }else if (strcmp(command, "SUCC")==0)   //Quando recebemos SUCC de um nó
                            {
                                sscanf(token3, "SUCC %s %s %s\n", Nodesucsuc->id, Nodesucsuc->IP, Nodesucsuc->TCP);
                                if (strcmp(Nodesucsuc->id,Nodesuc->id)==0)
                                {
                                    strcpy(Nodesucsuc->id, Nodei->id);
                                    strcpy(Nodesucsuc->IP, Nodei->IP);
                                    strcpy(Nodesucsuc->TCP, Nodei->TCP);
                                    
                                }
                                Nodei->segundo_sucessor = Nodesucsuc;

                            }else if (strcmp(command, "CHORD")==0){

                                sscanf(token3, "CHORD %s\n", chord);

                                //printf("\n\n Recebemos Corda de %s", chord);

                                t_id = atoi(chord);

                                chords[t_id] = sockets[i];

                                usleep(500); // 500000 microssegundos = 0.5 segundos

                                for (y = 0; y < tabela_cc->altura && strlen(tabela_cc->col[0].row[y].data) != 0; y++){
                                    sprintf(message, "ROUTE %s %s %s\n", Nodei->id, tabela_cc->col[0].row[y].data, tabela_cc->col[1].row[y].data);
                                    
                                    if(sockets[i]!= -1){
                                    send_route_msg(message, sockets[i]);
                                    //printf("\nMensagem ROUTE ENVIADA: %s", message);
                                    }
                                }   


                            }
                            
                            
                            else if (strcmp(command, "ROUTE")==0){
                                n =-1;
                                n3= -1;

                                token2 = token3;


                                while (sscanf(token2, "ROUTE %s %s %s\n", rota->orig_id, rota->dest_id, rota->route) >= 2) {    //Verifica que pelo menos 2 parâmetros são preenchidos

                                    if(strlen(rota->route) == 0){   //Se recebermos algo do tipo ROUTE 20 30\n
                                        //Houve uma remoção de adjacência
                                        update_enc_removal(Nodei, tabela_enc, tabela_cc, tabela_exp, rota->orig_id, rota->dest_id, -99);    //!!!! Verificar se está bem ou não, e que não manda msg route aos vizinhos
                                        n3 = 1;
                                    }
                                    else{
                                        //printf("\n\nMensagem ROUTE a ser analizada: %s\n\n", rota->route);
                                        n3 = verify_route(rota, Nodei->id);
                                    }

                                    //printf("\n\n Resultado do verify route: %d\n\n", n3);

                                    if (n3==2){
                                        update_enc_removal(Nodei, tabela_enc, tabela_cc, tabela_exp, rota->orig_id, rota->dest_id, -99);
                                    }

                                    if(n3!=1){

                                        strcpy(temp, rota->route);


                                        sprintf(rota->route, "%s-%s", Nodei->id, temp); //Faz uma rota com o nó que acabou de entrar
                                        //printf("\n\nMensagem ROUTE depois de ser analizada ser analizada: %s\n\n", rota->route);
                                        adj_node(rota);


                                        //Se o nó vizinho na rota for igual a uma corda
                                        t_id = -1;
                                        t_id = atoi(rota->adj_id);

                                        if (chords[t_id] != -1)
                                        {
                                            //printf("Caminho é: %s\n", rota->route);
                                            n = add_info_table(tabela_enc, tabela_cc, tabela_exp, rota, Nodei);
                                            //printf("\n\nretorno do add_info_table: %d\n\n", n);
                                        }                                           
                                        

                                        
                                        //Se o nó vizinho na rota for igual ao nosso sucessor ou se for igual ao nosso predecessor
                                        if (strcmp(rota->adj_id,Nodei->sucessor->id)==0 || strcmp(rota->adj_id,Nodei->predecessor->id)==0)
                                        {
                                            //printf("Caminho é: %s\n", rota->route);
                                            n = add_info_table(tabela_enc, tabela_cc, tabela_exp, rota, Nodei);
                                            //printf("\n\nretorno do add_info_table: %d\n\n", n);
                                        }

                                        if (n == 3)
                                        {
                                            usleep(500); // 500000 microssegundos = 0.5 segundos
                                            sprintf(message, "ROUTE %s %s\n", Nodei->id, rota->dest_id);
                                            if (prefd!= -1)
                                            {
                                                send_route_msg(message, prefd);
                                                //printf("\n\nMensagem ROUTE enviada ao prefd: %s", message);
                                            }
                                            if (sucfd!= -1)
                                            {
                                                send_route_msg(message, sucfd);
                                                //printf("\n\nMensagem ROUTE enviada ao sucfd: %s", message);
                                            }
                                            for(i = 0; i<100; i++){
                                                if(chords[i]>0){
                                                    send_route_msg(message, chords[i]);
                                                    //printf("\n\nMensagem ROUTE enviada a corda: %s", message);
                                                }
                                            }
                                        }
                                        

                                        if(n==1){

                                            usleep(500); // 500000 microssegundos = 0.5 segundos
                                            sprintf(message, "ROUTE %s %s %s\n", Nodei->id, rota->dest_id, rota->route);
                                            if (prefd!= -1)
                                            {
                                                send_route_msg(message, prefd);
                                                //printf("\n\nMensagem ROUTE enviada ao prefd: %s", message);
                                            }
                                            if (sucfd!= -1)
                                            {
                                                send_route_msg(message, sucfd);
                                                //printf("\n\nMensagem ROUTE enviada ao sucfd: %s", message);
                                            }
                                            for(i = 0; i<100; i++){
                                                if(chords[i]>0){
                                                    send_route_msg(message, chords[i]);
                                                    //printf("\n\nMensagem ROUTE enviada a corda: %s", message);
                                                }
                                            }


                                        }

                                    }
                                                                            
                                
                                    
                                    //Avança para a próxima linha
                                    token2 += strcspn(token2, "\n") + 1;
                                    strcpy(rota->route,"");   //Reset da rota->route para o próximo ciclo
                                
                                
                                }

                                                                    
                                    //Necessário enviar ROUTE para os vizinhos
                                    
                                
                                //Necessário fazer o raciocínio de enviar e reencaminhar as msgs aos nós vizinhos

                            }else if (strcmp(command, "CHAT")==0){

                                strcpy(inicio,""); 
                                strcpy(destino,""); 
                                strcpy(vizinho,"");
                                strcpy(buffer,"");
                                t_id = -1;


                                sscanf(token3, "CHAT %s %s %[^\n]\n", inicio, destino, buffer);
                                
                                /*
                                printf("\n\nDestino: %s", destino);
                                printf("\n\nId: %s", Nodei->id);
                                fflush(stdout);
                                */

                                if (strcmp(destino, Nodei->id) == 0)
                                {
                                    //printf("\n\n\n\nO nó %s enviou-nos a seguinte mensagem: \"%s\"", inicio, buffer);
                                    //fflush(stdout);

                                }else
                                {
                                    Get_tabela_expedicao(tabela_exp, vizinho, destino);

                                    t_id = atoi(vizinho);

                                    if (strcmp(vizinho, Nodei->predecessor->id) == 0)
                                    {
                                        n2=write(prefd, token3, strlen(token3));
                                        if(n2==-1)exit(1);
                                        
                                    }else if (strcmp(vizinho, Nodei->sucessor->id) == 0)
                                    {
                                        n2=write(sucfd, token3, strlen(token3));
                                        if(n2==-1)exit(1);

                                    }else{
                                        if (chords[t_id] > 0)
                                        {
                                            n2=write(chords[t_id], token3, strlen(token3));
                                            if(n2==-1)exit(1);
                                        }
                                    }
                                }
                                
                                


                            }   

                            token3 += strcspn(token3, "\n") + 1;
                        }              




                        //falta meter o resto.
                        FD_CLR(new_fd, &testfds);
                        FD_CLR(sockets[i], &testfds);
                        //close(new_fd);
                        out_fds--;
                        //sockets[i] = -1;
                    }
                    
                }
        }
    }

    close(tcp_fd);
}

