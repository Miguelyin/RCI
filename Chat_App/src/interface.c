#include "../include/interface.h"
#include "../include/protocolo_enc.h"
#include "../include/protocolo_chat.h"


int menu_opt(char in_str[], int sockets[], struct Node *Nodei, char *ring, char *id, char *regUDP, char *regIP, struct Node *Nodesuc, struct Node *Nodesucsuc, int *sucfd, int *prefd, int *chordfd, int chords[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota){
    char command_1[128];
    char command_2[128];
    char tempid[20];
    char tempIP[40];
    char tempTCP[20];
    char dest[20];
    char buffer[256];
    int n=0;

    sscanf(in_str, "%s %s", command_1, command_2);
    
    if(strcmp(command_1, "join")==0){       //Importante

        sscanf(in_str, "join %s %s", ring, id);
        join(Nodei, ring, id, regUDP, regIP, Nodesuc, Nodesucsuc, sucfd, sockets, tabela_enc, tabela_cc, tabela_exp, rota);

    }else if (strcmp(command_1, "j")==0){ 

        sscanf(in_str, "j %s %s", ring, id);
        join(Nodei, ring, id, regUDP, regIP, Nodesuc, Nodesucsuc, sucfd, sockets, tabela_enc, tabela_cc, tabela_exp, rota);

    }
    else if (strcmp(command_1, "direct")==0 && strcmp(command_2, "join")==0){ 

        sscanf(in_str, "direct join %s %s %s %s", id ,Nodesuc->id, Nodesuc->IP, Nodesuc->TCP);
        direct_join(Nodei, id, Nodesuc, Nodesucsuc, sucfd, sockets, tabela_enc, tabela_cc, tabela_exp, rota);

    }
    else if (strcmp(command_1, "dj")==0){ 

        sscanf(in_str, "dj %s %s %s %s", id ,Nodesuc->id, Nodesuc->IP, Nodesuc->TCP);
        direct_join(Nodei, id, Nodesuc, Nodesucsuc, sucfd, sockets, tabela_enc, tabela_cc, tabela_exp, rota);

    }
    else if (strcmp(command_1, "chord")==0 && strcmp(command_2, "to")==0){

        sscanf(in_str, "chord to %s %s %s",tempid, tempIP, tempTCP);

        Chord_to(Nodei, chordfd, sockets, chords, tempid, tempIP, tempTCP);

    }
    else if (strcmp(command_1, "ct")==0){

        sscanf(in_str, "chord to %s %s %s",tempid, tempIP, tempTCP);

        Chord_to(Nodei, chordfd, sockets, chords, tempid, tempIP, tempTCP);

    }
    else if (strcmp(command_1, "chord")==0){

        Chord(Nodei, ring, regUDP, regIP, chordfd, sockets, chords, tabela_cc);

    }
    else if (strcmp(command_1, "c")==0){

        Chord(Nodei, ring, regUDP, regIP, chordfd, sockets, chords, tabela_cc);

    }
    else if (strcmp(command_1, "remove")==0 && strcmp(command_2, "chord")==0){

        remove_chord(chordfd,sockets, chords, sucfd, prefd, Nodei, tabela_enc, tabela_cc, tabela_exp);

    }
    else if (strcmp(command_1, "rc")==0){

        remove_chord(chordfd,sockets, chords, sucfd, prefd, Nodei, tabela_enc, tabela_cc, tabela_exp);

    }
    else if (strcmp(command_1, "show")==0 && strcmp(command_2, "routing")==0){
        sscanf(in_str, "show routing %s", dest);
        n=show_routing(tabela_enc, dest);
        if(n == 0){
            printf("\nNó %s não existe na tabela\n", dest);
        }

    }
    else if (strcmp(command_1, "sr")==0){
        sscanf(in_str, "sr %s", dest);
        n=show_routing(tabela_enc, dest);
        if(n == 0){
            printf("\nNó %s não existe na tabela\n", dest);
        }

    }
    else if (strcmp(command_1, "show")==0 && strcmp(command_2, "path")==0){
        sscanf(in_str, "show path %s", dest);
        show_path(tabela_cc, dest);

    }
    else if (strcmp(command_1, "sp")==0){
        sscanf(in_str, "sp %s", dest);
        show_path(tabela_cc, dest);

    }
    else if (strcmp(command_1, "show")==0 && strcmp(command_2, "forwarding")==0){
        printf("\nTabela de Expedição:\n");
        printDataFrame(tabela_exp);

    }
    else if (strcmp(command_1, "sf")==0){
        printf("\nTabela de Expedição:\n");
        printDataFrame(tabela_exp);

    }
    else if (strcmp(command_1, "show")==0 && strcmp(command_2, "topology")==0){ 
        show_topology(Nodei, chords);
    }
    else if (strcmp(command_1, "st")==0){ //Importante
        
        show_topology(Nodei, chords);
    }
    else if (strcmp(command_1, "show")==0 && strcmp(command_2, "tables")==0){
        show_tables(tabela_enc, tabela_cc, tabela_exp);
    }
    else if (strcmp(command_1, "stab")==0){
        show_tables(tabela_enc, tabela_cc, tabela_exp);
    }
    else if (strcmp(command_1, "message")==0){

        sscanf(in_str, "message %s %[^\n]", tempid, buffer);
        chat(buffer, tempid, Nodei, sucfd, prefd, chords, tabela_exp);

    }
    else if (strcmp(command_1, "m")==0){

        sscanf(in_str, "m %s %[^\n]", tempid, buffer);
        chat(buffer, tempid, Nodei, sucfd, prefd, chords, tabela_exp);

    }
    else if (strcmp(command_1, "leave")==0){
        leave(Nodei, ring, regUDP, regIP, sockets, chords, sucfd, prefd, tabela_enc, tabela_cc, tabela_exp);
    }
    else if (strcmp(command_1, "l")==0){
        leave(Nodei, ring, regUDP, regIP, sockets, chords, sucfd, prefd, tabela_enc, tabela_cc, tabela_exp);
    }
    else if (strcmp(command_1, "exit")==0){
        return 0;
    }
    else if (strcmp(command_1, "x")==0){
        return 0;
    }
    else if (strcmp(command_1, "help")==0){
        printf("\n1. join (j) ring id");
        printf("\n2. direct join (dj) id succid succIP succTCP");
        printf("\n3. chord (c)");
        printf("\n3. chord to (ct) succid succIP succTCP");
        printf("\n4. remove chord (rc)");
        printf("\n5. show topology (st)");
        printf("\n6. show routing (sr) dest");
        printf("\n7. show path (sp) dest");
        printf("\n8. show forwarding (sf)");
        printf("\n7. show tables");
        printf("\n9. message (m) dest message");
        printf("\n10. leave (l)");
        printf("\n11. exit (x)");
        printf("\n12. help (h)");
    }
    else{
        printf("\nFormatação Incorreta: Esse comando não existe. Use o comando help (h) para saber os comandos disponíveis.\n");
        return 1;
    }

    return 1;
}
