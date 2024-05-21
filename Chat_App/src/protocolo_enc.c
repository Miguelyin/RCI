//ficheiro do protocolo encaminhamento
#include "../include/protocolo_enc.h"
#include "../include/TCP_Client.h"

static bool linha1Usada = false; // Variável estática para manter o estado da linha 1

//Função de ler o vizinho adjacente na mensagem ROUTE
void adj_node(Routes *rota){
    int count_token = 0;
    char text[128];
    strcpy(text, rota->route);
    char* token = strtok(text, "-");
    
    while (token != NULL) { //Enquanto houver "-" na string
        count_token++;
        switch(count_token){
            case 1:
                strcpy(rota->orig_id, token);
                break;
            case 2:
                strcpy(rota->adj_id, token);
                break;
            default:

                break;
        }
        token = strtok(NULL, "-");
    }
}


//Função para atualizar a tabela de expedição
void atualizar_tab_exp(DataFrame *df2, DataFrame *df3, char linhaAtualizada[]){
    int count_token = 0;
    char text[128];

    int linhaAtualizadaEncontrada = 0;
     char* token;
    

    for (int y = 0; y < df2->altura && !linhaAtualizadaEncontrada; y++){

        if(strcmp(df2->col[0].row[y].data, linhaAtualizada) == 0){
            linhaAtualizadaEncontrada = 1;
            strcpy(text, df2->col[1].row[y].data);

            token = strtok(text, "-");
    
            while (token != NULL) { //Enquanto houver "-" na string
                count_token++;
                switch(count_token){
                    
                    case 2:
                        strcpy(df3->col[0].row[y].data, df2->col[0].row[y].data);
                        strcpy(df3->col[1].row[y].data, token);
                        break;
                    default:

                        break;
                }
                token = strtok(NULL, "-");
            }

        }

    }
    
    
}



// Função para adicionar uma célula ao dataframe
void addCell(DataFrame *df, int x, int y, const char *data) {
    if (x < 0 || x >= df->largura || y < 0 || y >= df->altura) {
        printf("Índices da célula fora dos limites!\n");
        return;
    }

    strcpy(df->col[x].row[y].data, data);
    df->col[x].row[y].len = strlen(data);
}


// Função para imprimir o Dataframe
void printDataFrame(DataFrame *df) {
    for (int y = 0; y < df->altura; y++) {
        for (int x = 0; x < df->largura; x++) {
            printf("%-*s", df->col[x].row[y].len, df->col[x].row[y].data);
            if (x < df->largura - 1) {
                printf(" | ");
            }
        }
        printf("\n");

        if (y < df->largura - 1) {
            for (int i = 0; i < df->largura; i++) {
                for (int j = 0; j < df->col[i].row[y].len; j++) {
                    printf("-");
                }
                if (i < df->largura - 1) {
                    printf("-+-");
                }
            }
            printf("\n");
        }
    }
}


// Função para remover uma linha do dataframe
void verify_removeRow(DataFrame *df, DataFrame *df2, DataFrame *df3, char linha[]) {
    int x, y;
    int flag = 0;
    int flag_cc = 0;
    int flag_exp = 0;
    int count_cell = 0;

    for (y = 2; y < df->altura - 1; y++) {
        if ((strcmp(df->col[0].row[y].data, linha) == 0) || flag == 1){
            flag = 1;
            //printf("\n Encontrou a linha pra remover: %s\n", df->col[x].row[0].data);

            for (x = 1; x < df->largura; x++) {
                if(df->col[x].row[y].len == 0){
                    count_cell++;
                }

            }

            if(count_cell == 17 || flag_cc == 1){   //Se linha com destino estiver vazia
                //printf("\nAPAGOU A linha: %s", linha);
                flag_cc = 1;

                for (x = 0; x < df->largura; x++) {
                    strcpy(df->col[x].row[y].data, df->col[x].row[y+1].data);
                    df->col[x].row[y].len = df->col[x].row[y + 1].len;
                }
            }  
        }

    }

    if(flag_cc == 1){   //Foi apagada uma linha da tabela de encaminhamento e atualiza tabela de cc e exp
        for (y = 1; y < df2->altura; y++) {
            if(strcmp(df2->col[0].row[y].data, linha) == 0 || flag_exp == 1){   //Entre se encontrou a linha ou se já foi encontrada
                flag_exp = 1;

                strcpy(df2->col[x].row[y].data, df2->col[x].row[y+1].data); //Atualiza tabela de caminhos mais curtos
                df2->col[x].row[y].len = df2->col[x].row[y + 1].len;

                strcpy(df3->col[x].row[y].data, df3->col[x].row[y+1].data); //Atualiza tabela d expedição
                df3->col[x].row[y].len = df3->col[x].row[y + 1].len;
            }
        }
        
    }



}

//Função para limpar uma linha inteira
void limpar_linha(DataFrame *tabela, char linha[]){
    int x, y;

    for (y = 1; y < tabela->altura; y++) {
        if ((strcmp(tabela->col[0].row[y].data, linha) == 0)){    //Encontra a linha que quer
            for (x = 0; x < tabela->largura; x++) {
                strcpy(tabela->col[x].row[y].data, "");
                tabela->col[x].row[y].len = 0;
            }
        
            break;
        }
    }
}


//Função para atualizar a tabela de encaminhamento quando recebemos mensagem de remoção de adjacência
void update_enc_removal(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char coluna[], char linha[], int socket){    //col=rota->orig_id e linha=rota->dest_id
    int x, y;

    for (y = 1; y < df->altura; y++) {
        if ((strcmp(df->col[0].row[y].data, linha) == 0)){    //Encontra a linha que quer
            for (x = 1; x < df->largura; x++) {
                if ((strcmp(df->col[x].row[0].data, coluna) == 0)){     //Encontra a coluna que quer
                    strcpy(df->col[x].row[y].data, "");
                    df->col[x].row[y].len = 0;
                    break;
                }
            }
        
            break;
        }
    }
    
    find_cc(Nodei, df, df2, df3, coluna, socket);
}

void Get_tabela_expedicao(DataFrame *df3, char vizinho[], char *destino){

    int y;

    for (y = 0; y < df3->altura; y++)
    {
        if (strcmp(df3->col[0].row[y].data, destino) == 0)
        {
            strcpy(vizinho, df3->col[1].row[y].data);
        }
        
    }
    

}


// Função para remover uma coluna do dataframe
int removeColumn(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char col[], int socket) {
    int x, y;
    int flag = 0;
    int n =-1;
    char temp1[128];
    char temp2[128];

    for (x = 1; x < df->largura - 1; x++) {
        if ((strcmp(df->col[x].row[0].data, col) == 0) || flag == 1){
            flag = 1;
            //printf("\n Encontrou a coluna pra remover: %s\n", df->col[x].row[0].data);

            for (y = 0; y < df->altura; y++) {
                if(x + 1 < df->largura){
                    strcpy(df->col[x].row[y].data, df->col[x + 1].row[y].data);
                    df->col[x].row[y].len = df->col[x + 1].row[y].len;
                }
            }
        }    
    }

    for (y = 1; y < df2->altura ; y++)
    {
        sscanf(df2->col[1].row[y].data, "%s-%s", temp1, temp2);
        if (strcmp(col,temp2) == 0 || strcmp(col,df2->col[0].row[y].data) == 0)
        {
            n = find_cc(Nodei, df, df2, df3, df2->col[0].row[y].data, socket);
        }
        
        
    }

    //verify_removeRow(df, df2, df3, col);

    return n;

}


void limparTabela(DataFrame *tabela) {
    for (int y = 0; y < tabela->altura; y++) {
        for (int x = 0; x < tabela->largura; x++) {
            strcpy(tabela->col[x].row[y].data, "");
            tabela->col[x].row[y].len = 0;
        }
    }
}


// Função revisada para encontrar o caminho mais curto na tabela de encaminhamento e atualizar a tabela de caminho mais curto
int find_cc(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char linhaAtualizada[], int socket) {
    int count_token[18] = {0}; // Ajustado para iniciar todos os elementos com 0
    char text[128];
    char* token;
    char message[256];
    int menor = INT_MAX; // Inicializa com o maior inteiro possível para a comparação funcionar corretamente
    int x, y, local_count;
    int linhaAtualizadaEncontrada = 0;  // Variável de controle para verificar se a linha foi encontrada
    int cam_curto = -1; // Inicializado como -1 para indicar que nenhum caminho foi selecionado ainda

    /*if(strcmp(linhaAtualizada, id) == 0){  //Verifica se é o próprio nó
                
                addCell(df2, 1, 0, df->col[0].row[1].data); // Atualiza a rota mais curta até o nó destino
                addCell(df2, 0, 0, df->col[0].row[1].data); // Atualiza o índice de nó destino

                addCell(df3, 0, 0, df->col[0].row[1].data); // Atualiza o índice de nó destino

                return;
    }
    */

    for (y = 1; y < df->altura && !linhaAtualizadaEncontrada; y++) {
        if (strcmp(df->col[0].row[y].data, linhaAtualizada) == 0) {  // Verifica se a linha foi atualizada
            linhaAtualizadaEncontrada = 1;

            for (x = 1; x < df->largura; x++) {  // Percorre a linha
                strcpy(text, df->col[x].row[y].data);
                local_count = 0; // Contador local para tokens nesta célula
                token = strtok(text, "-");
    
                while (token != NULL) {
                    local_count++;
                    token = strtok(NULL, "-");
                }
                count_token[x] = local_count; // Armazena a contagem de tokens para esta coluna
            }
        }
    }

    if (linhaAtualizadaEncontrada) {
        // Encontra a menor rota e guarda o índice
        for (int i = 1; i < df->largura; i++) {
            if (count_token[i] < menor && count_token[i] > 0) { // Assegura que não consideramos colunas vazias
                menor = count_token[i];
                cam_curto = i;
            }
        }

        if (cam_curto != -1) {
            //printf("\n VERIFICA SE: %s ==== %s\n", df->col[cam_curto].row[y-1].data, df2->col[1].row[y-2].data);
            if(strcmp(df->col[cam_curto].row[y-1].data, df2->col[1].row[y-2].data) != 0){    //Verifica se a atualização é diferente da que já lá estava, caso seja atualiza
                // Atualiza a tabela de caminhos mais curtos
                addCell(df2, 1, y-2, df->col[cam_curto].row[y-1].data); // Atualiza a rota mais curta até o nó destino
                addCell(df2, 0, y-2, df->col[0].row[y-1].data); // Atualiza o índice de nó destino
                
                if(socket != -99){  //Se o nó não estiver sozinho ou numa situação em que está a entrar um 3º nó no 
                    sprintf(message, "ROUTE %s %s %s\n", Nodei->id, linhaAtualizada, df2->col[1].row[y-2].data);
                    send_route_msg(message, socket);
                }
                //chamar função de update expd
                atualizar_tab_exp(df2, df3, linhaAtualizada);
                return 1;
            }
        }
        else{
            //printf("\n\nLinha que queremos está vazia\n");
            for(y=1; y<df2->altura; y++ ){
                if(strcmp(df2->col[0].row[y].data, linhaAtualizada) == 0){
                    addCell(df2, 1, y, "");
                }
            }
            return 3;
        }


    }

    return 0;
}



//Verifica se o a rota tem o nosso próprio nó a meio
int verify_route(Routes *rota, char id[]) {
    char *token;
    char new_route[128] = ""; // Para armazenar a nova rota
    char text[128] = "";


    strcpy(text, rota->route);
    
    //Se rota->route tiver vazio
    if(strlen(text) == 0){
        return 0;
    }

    // Use strtok para obter o primeiro token
    token = strtok(text, "-");

    // Flag para indicar se o identificador foi encontrado
    int id_found = 0;

    // Percorra os tokens até o final
    while (token != NULL) {
        // Se encontrarmos o identificador na rota
        if(strcmp(token, id) == 0) {
            // Marcamos que o identificador foi encontrado
            id_found = 1;
        } else if (id_found) { // Se o identificador foi encontrado e este não é o último token
            // Copiamos este token para a nova rota
            strcat(new_route, token);
            strcat(new_route, "-"); // Adicionamos o delimitador novamente
        }

        // Obtenha o próximo token
        token = strtok(NULL, "-");
    }

    // Se o identificador foi encontrado e copiamos algum token
    if (id_found && strlen(new_route) > 0) {
        // Removemos o último delimitador "-"
        new_route[strlen(new_route) - 1] = '\0';
    }
    else if(!id_found){
        return 0;
    }

    // Atualizamos a rota original
    strcpy(rota->route, new_route);
    if(strlen(rota->route) == 0){
        //printf("\nSOMOS O NÓ DESTINO");
        //printf("\nROTA CONNOSCO COMO DESTINO NO VERIFY ROUTE: %s\n", rota->route);
        return 1;
    }

    return 2;
}





//Função para adicionar info a uma célula da tabela
int add_info_table(DataFrame *df, DataFrame *df2, DataFrame *df3, Routes *rota, struct Node *Nodei) {
    int x, y;
    int n =-1;
    char linhaAtualizada[5];
    bool preenchido = false; // Variável para controlar se os dados foram preenchidos
    int socket = -99;

    for (y = 1; y < df->altura && !preenchido; y++) {
        // Verifica se a linha é a linha 1 e se ela já foi usada
        if (y == 1 && linha1Usada) {
            continue; // Pula para a próxima iteração, evitando sobrepor a linha 1
        }

        if (strcmp(df->col[0].row[y].data, rota->dest_id) == 0 || strlen(df->col[0].row[y].data) == 0) {    //Se encontrar um índice do nó destino coincidente ou vazio
            for (x = 1; x < df->largura && !preenchido; x++) {
                if (strcmp(df->col[x].row[0].data, rota->adj_id) == 0 || strlen(df->col[x].row[0].data) == 0) { //Se encontrar um índice do nó adjacente coincidente ou vazio
                    preenchido = true; // Define como verdadeiro para sair do loop externo
                    addCell(df, x, y, rota->route); // Preenche a célula com a rota
                    addCell(df, 0, y, rota->dest_id); // Preenche nas linhas o nó destino
                    addCell(df, x, 0, rota->adj_id); // Preenche nas colunas o nó adjacente
                                        
                    strcpy(linhaAtualizada, rota->dest_id);
                    //printf("\nlinhaAtualizada = %s", linhaAtualizada);

                    //n=find_cc(df, df2, df3,linhaAtualizada, id);
                    if(strcmp(linhaAtualizada, Nodei->id) == 0){  //Verifica se é o próprio nó
                
                        addCell(df2, 1, 0, df->col[0].row[1].data); // Atualiza a rota mais curta até o nó destino
                        addCell(df2, 0, 0, df->col[0].row[1].data); // Atualiza o índice de nó destino

                        addCell(df3, 0, 0, df->col[0].row[1].data); // Atualiza o índice de nó destino

                        //printf("\n\nAtualizei a tabela cc 1\n\n");

                        return 1;
                    }   
                    else if ((df->col[x].row[y].len < df2->col[1].row[y-1].len)||(df2->col[1].row[y-1].len == 0)){   //Verifica se a rota atualizada no encaminhamento é menor que a de cc ou se a de cc está vazia
                        addCell(df2, 1, y-1, df->col[x].row[y].data); // Atualiza a rota mais curta até o nó destino
                        addCell(df2, 0, y-1, df->col[0].row[y].data); // Atualiza o índice de nó destino

                        //printf("\n\nCaminho que já lá estava: %d\n\n Novo Caminho: %d",df2->col[1].row[y].len, df->col[x].row[y].len);

                        //printf("\n\nAtualizei a tabela cc 2\n\n");

                        atualizar_tab_exp(df2, df3, linhaAtualizada);

                        return 1;
                    }

                    n=find_cc(Nodei, df,  df2, df3, linhaAtualizada, socket);
                    
                    //Alterar para entrar na função quando o caminho mais curto tiver de ser atualizado
                }
            }
        }
    }
    return n;
}



//Função para chamar as funções que dão print das tabelas
int show_tables(DataFrame *df, DataFrame *df2, DataFrame *df3) {
    
    // Imprimindo o dataframe original
    printf("\nTabela de Encaminhamento:\n");
    printDataFrame(df);

    // Imprimindo o dataframe modificado
    printf("\nTabela de Caminho mais curto:\n");
    printDataFrame(df2);

    printf("\nTabela de Expedição:\n");
    printDataFrame(df3);

    return 0;
}


void show_path(DataFrame *df2, char linha[]){
    int y;

    for (y = 0; y < df2->altura; y++){
        if(strcmp(df2->col[0].row[y].data, linha) == 0){    //Se encontrar a linha
            printf("\n%s | %s\n", df2->col[0].row[y].data, df2->col[1].row[y].data);
            break;
        }
    }

}


int show_routing(DataFrame *df, char linha[]){


    for (int y = 0; y < df->altura; y++) {
        if(strcmp(df->col[0].row[y].data, linha) == 0){ //Se encontrar a linha
            
            for (int x = 0; x < df->largura; x++) {
                printf("%-*s", df->col[x].row[y].len, df->col[x].row[y].data);
                if (x < df->largura - 1) {
                    printf(" | ");
                }
            }
            printf("\n");

            if (y < df->largura - 1) {
                for (int i = 0; i < df->largura; i++) {
                    for (int j = 0; j < df->col[i].row[y].len; j++) {
                        printf("-");
                    }
                    if (i < df->largura - 1) {
                        printf("-+-");
                    }
                }
                printf("\n");
            }
            return 1;
        }
    }

    return 0;

}
