#ifndef PROTOCOLO_ENC_H
#define PROTOCOLO_ENC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

/*
   Estrutura: Node

   Descrição:
   Esta estrutura representa um nó na rede. Contém informações como ID, endereço IP, porta TCP e ponteiros para nós sucessores e predecessores.

   Membros:
   - id: Uma cadeia de caracteres que armazena o identificador único do nó.
   - IP: Uma cadeia de caracteres que armazena o endereço IP do nó.
   - TCP: Uma cadeia de caracteres que armazena a porta TCP do nó.
   - sucessor: Um ponteiro para o nó sucessor na rede.
   - segundo_sucessor: Um ponteiro para o segundo nó sucessor na rede.
   - predecessor: Um ponteiro para o nó predecessor na rede.
*/
struct Node {
    char id[5];
    char IP[50];
    char TCP[10];
    struct Node *sucessor;
    struct Node *segundo_sucessor;
    struct Node *predecessor;
};


#define MAX_ROWS 100
#define MAX_COLS 100

/*
   Estrutura: DataFrame

   Descrição:
   Esta estrutura representa um DataFrame, que é uma estrutura de dados tabular. É composta por um conjunto de colunas, onde cada coluna contém um conjunto de células.

   Membros:
   - largura: Um inteiro que indica o número de colunas no DataFrame.
   - altura: Um inteiro que indica o número de linhas no DataFrame.
   - col: Um array de estruturas Column que representa as colunas do DataFrame.
*/
typedef struct {
    char data[100];
    int len;
} Cell;

typedef struct {
    Cell row[MAX_ROWS];
} Column;

typedef struct {
    int largura;
    int altura;
    Column col[MAX_COLS];
} DataFrame;


/*
   Estrutura: Routes

   Descrição:
   Esta estrutura representa uma rota na rede. Contém informações sobre o nó de origem, nó de destino, nó adjacente e a rota propriamente dita.

   Membros:
   - orig_id: Uma cadeia de caracteres que armazena o ID do nó de origem.
   - dest_id: Uma cadeia de caracteres que armazena o ID do nó de destino.
   - adj_id: Uma cadeia de caracteres que armazena o ID do nó adjacente.
   - route: Uma cadeia de caracteres que armazena a rota do nó de origem ao nó de destino.
*/
typedef struct {
    char orig_id[5];
    char dest_id[5];
    char adj_id[5];
    char route[100];
} Routes;


/*
   Função: adj_node

   Descrição:
   Esta função é responsável por processar informações sobre vizinhos adjacentes contidas nas mensagens do tipo ROUTE.
   Ela atualiza a estrutura de dados 'rota' com o id do vizinho adjacente recebido na mensagem ROUTE.
   Essa atualização pode envolver a modificação de atributos específicos dentro da estrutura 'rota' para refletir
   as novas informações de camiinho obtidas.

   Parâmetros:
   - rota: Um ponteiro para a estrutura de dados 'Routes' que contém as informações de roteamento atualmente conhecidas.
           Este argumento é modificado pela função para refletir as novas informações do vizinho adjacente.

   Retorno:
   Esta função não retorna nenhum valor.

   Observações:
   - A função presume que a estrutura 'Routes' já está inicializada e contém dados válidos antes da chamada.
   - Os detalhes exatos de como a função atualiza a estrutura 'rota' dependem do formato das mensagens ROUTE e do
     design do sistema de roteamento.
   - É importante garantir que quaisquer modificações na estrutura 'Routes' sejam consistentes com as expectativas
     de outras partes do sistema que utilizam essas informações.
*/
void adj_node(Routes *rota);


/*
   Função: atualizar_tab_exp

   Descrição:
   Esta função atualiza a tabela de expedição com base em uma linha atualizada na tabela de caminhos mais curtos.
   
   Parâmetros:
   - df2: Ponteiro para a Tabela de Caminhos mais curtos.
   - df3: Ponteiro para o Tabela de Expedição.
   - linhaAtualizada: Uma string contendo o id destino da linha atualizada na tabela de caminhos mais curtos.

   Retorno:
   Esta função não retorna nenhum valor.

   Observações:
   A função atualiza o DataFrame df3 com base na linha atualizada.
*/
void atualizar_tab_exp(DataFrame *df2, DataFrame *df3, char linhaAtualizada[]);


/*
   Função: addCell

   Descrição:
   Adiciona uma célula ao dataframe especificado, localizando-a nas coordenadas (x, y) fornecidas e
   preenchendo-a com os dados especificados. A função verifica primeiro se as coordenadas fornecidas
   estão dentro dos limites do dataframe. Se as coordenadas estiverem fora dos limites, uma mensagem
   de erro é exibida e a função retorna sem alterar o dataframe. Caso contrário, o dado é copiado para
   a célula especificada, e o comprimento do dado é atualizado na estrutura da célula.

   Parâmetros:
   - df: Um ponteiro para o dataframe onde a célula será adicionada. O dataframe deve ser pré-inicializado
         e ter espaço alocado para as colunas e linhas.
   - x: A coordenada x da célula dentro do dataframe (coluna).
   - y: A coordenada y da célula dentro do dataframe (linha).
   - data: Uma string contendo os dados que serão armazenados na célula. A função assume que esta string
           está terminada em nulo ('\0') e será copiada para a célula.

   Retorno:
   Esta função não retorna nenhum valor.

   Observações:
   - Esta função assume que o dataframe já foi inicializado e tem memória alocada suficiente para
     as colunas e linhas definidas na sua criação.
   - A função verifica os limites do dataframe com base em sua 'largura' e 'altura' para prevenir
     acessos fora dos limites do array, protegendo contra sobrescrita de memória.
   - Se os índices fornecidos estiverem fora dos limites permitidos, a função simplesmente imprime uma
     mensagem de erro e retorna sem fazer alterações, garantindo a integridade do dataframe.
   - A função usa 'strcpy' para copiar os dados para a célula, o que implica que os dados não devem
     exceder o tamanho máximo da célula. Além disso, atualiza o campo 'len' da célula com o comprimento
     da string 'data', usando 'strlen' para determinar esse comprimento.
*/
void addCell(DataFrame *df, int x, int y, const char *data);


/*
   Função: printDataFrame

   Descrição:
   Esta função é responsável por imprimir o conteúdo de um DataFrame no terminal, formatando-o de forma a facilitar
   a visualização dos dados contidos em suas células. A função percorre cada célula do DataFrame, imprimindo seu
   conteúdo, e utiliza linhas verticais (|) para separar as colunas e linhas horizontais (-) para separar as linhas do DataFrame.

   Parâmetros:
   - df: Um ponteiro para o DataFrame que se deseja imprimir. Assume-se que o DataFrame já está inicializado e contém os dados
         a serem impressos.

   Comportamento:
   - A função inicia percorrendo cada linha (y) do DataFrame, de cima para baixo.
   - Para cada linha, ela percorre cada coluna (x), da esquerda para a direita.
   - Para cada célula, imprime o conteúdo da célula ajustado à esquerda, seguido por um separador vertical "|" entre as colunas,
     exceto após a última coluna de cada linha.
   - Após imprimir todas as colunas de uma linha, imprime uma quebra de linha (\n) para começar a impressão da próxima linha.
   - Entre as linhas do DataFrame, exceto após a última linha, imprime uma linha de separação composta por hífens (-), adaptada
     ao comprimento do conteúdo das células, e utiliza "+-" como separador entre as colunas.

   Observações:
   - O parâmetro 'df->col[x].row[y].len' é utilizado para determinar o espaçamento de cada célula, garantindo que o conteúdo seja
     impresso de forma alinhada.
   - Esta função assume que 'df', 'df->col', e 'df->col[x].row' estão corretamente inicializados e contêm os dados válidos para
     impressão.
   - A estética da tabela impressa é ajustável pela modificação dos caracteres utilizados para desenhar as linhas de separação e os
     separadores de coluna.
   - Esta função não retorna valores e tem como único efeito a impressão do DataFrame no terminal.

   Exemplo de uso:
   - Para imprimir um DataFrame 'df' previamente preenchido com dados, basta chamar 'printDataFrame(&df);'.
*/
void printDataFrame(DataFrame *df);


void verify_removeRow(DataFrame *df, DataFrame *df2, DataFrame *df3, char linha[]);

void limpar_linha(DataFrame *tabela, char linha[]);

/*
   Função: update_enc_removal

   Descrição:
   Esta função é responsável por atualizar a tabela de encaminhamento de um nó na rede quando uma mensagem indicando a remoção de uma adjacência é recebida.
   A função busca na tabela de encaminhamento (DataFrame df) a célula correspondente ao nó de origem (coluna) e destino (linha) especificados, e atualiza esta célula
   para indicar que não há mais uma rota direta entre esses dois nós. Após a atualização da tabela de encaminhamento, a função invoca `find_cc` para possivelmente recalcular a componente conexa, considerando a remoção da adjacência.

   Parâmetros:
   - Nodei: Um ponteiro para a estrutura do Nó atual, contendo informações sobre o próprio nó na rede.
   - df: Um ponteiro para o DataFrame que representa a tabela de encaminhamento atual do nó.
   - df2, df3: Ponteiros para DataFrames auxiliares que podem ser utilizados durante o recalculo da componente conexa pela função `find_cc`.
   - coluna: Uma string representando o identificador do nó de origem da adjacência removida.
   - linha: Uma string representando o identificador do nó de destino da adjacência removida.
   - socket: Um inteiro representando o socket através do qual a mensagem de remoção foi recebida, utilizado possivelmente pela função `find_cc`.

   Funcionamento:
   - A função itera sobre as linhas do DataFrame df, começando da segunda linha (índice 1), para ignorar o cabeçalho.
   - Para cada linha, verifica se o valor da primeira coluna (destino) corresponde ao identificador fornecido em 'linha'.
   - Quando encontra a linha correta, itera sobre as colunas do DataFrame df, começando da segunda coluna (índice 1), para encontrar a coluna que corresponde ao identificador fornecido em 'coluna'.
   - Ao encontrar a célula correta (interseção da coluna e linha especificadas), limpa o conteúdo dessa célula (definindo para uma string vazia "") e atualiza o comprimento do dado para 0, indicando a remoção da adjacência.
   - Após atualizar a tabela de encaminhamento, chama a função `find_cc` passando os DataFrames relevantes, o identificador do nó de origem da adjacência removida e o socket, para avaliar o impacto da remoção na conectividade da rede.

   Notas:
   - Esta função assume que tanto 'coluna' quanto 'linha' são strings válidas e correspondem a identificadores de nós presentes na tabela de encaminhamento.
   - A função `find_cc` chamada no final é responsável por realizar um recálculo ou verificação da componente conexa da rede, ajustando outras estruturas de dados conforme necessário após a remoção da adjacência.

   Exemplo de Uso:
   - Para atualizar a tabela de encaminhamento após receber uma mensagem indicando que a adjacência entre os nós com identificadores "NodeA" e "NodeB" foi removida, e usando o socket 5, a função seria chamada da seguinte forma:
     `update_enc_removal(&meuNode, &dfEncaminhamento, &dfAux1, &dfAux2, "NodeA", "NodeB", 5);`
*/
void update_enc_removal(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char coluna[], char linha[], int socket);


/*
   Função: Get_tabela_expedicao

   Descrição:
   Esta função procura na tabela de expedição (representada pelo DataFrame df3) a entrada correspondente ao destino especificado. Quando encontra esta entrada, copia o nome do vizinho (a próxima hop na rota para o destino) para a variável fornecida 'vizinho'.

   Parâmetros:
   - df3: Um ponteiro para o DataFrame que representa a tabela de expedição. Espera-se que a primeira coluna de df3 contenha os destinos (identificadores dos nós de destino) e a segunda coluna os vizinhos correspondentes (próximos hops).
   - vizinho: Um array de char para o qual o nome do vizinho será copiado. Deve ser grande o suficiente para armazenar o nome do vizinho.
   - destino: Uma string (char pointer) que representa o destino cujo vizinho (próximo hop) estamos procurando na tabela de expedição.

   Funcionamento:
   - A função itera sobre todas as linhas do DataFrame df3, começando do índice 0.
   - Para cada linha, compara o conteúdo da primeira coluna (destino) com a string 'destino' fornecida. A comparação é feita usando a função strcmp, que retorna 0 se as strings forem iguais.
   - Se a entrada correspondente ao destino especificado for encontrada, a função copia o conteúdo da segunda coluna dessa linha (o vizinho) para a variável 'vizinho' usando strcpy.
   - A função não retorna um valor, mas modifica o conteúdo da variável 'vizinho' passada por referência.

   Notas:
   - Esta função pressupõe que a tabela de expedição (df3) está corretamente formatada, com destinos na primeira coluna e vizinhos na segunda.
   - A função altera diretamente a variável 'vizinho', por isso é importante que 'vizinho' tenha espaço suficiente para armazenar o nome do vizinho sem causar overflow.
   - Se múltiplas entradas para o mesmo destino existirem, a função copiará o vizinho da última entrada correspondente encontrada.
   - Se o destino não for encontrado, 'vizinho' não será alterado. Portanto, pode ser útil inicializar 'vizinho' com um valor conhecido antes de chamar esta função.

*/
void Get_tabela_expedicao(DataFrame *df3, char vizinho[], char *destino);


/*
   Função: removeColumn

   Descrição:
   Esta função tem como objetivo remover uma coluna específica, identificada por 'col', de um DataFrame 'df'. Adicionalmente, realiza verificações em outro DataFrame 'df2' para possivelmente
   acionar a função 'find_cc' com base em condições específicas relacionadas a 'col'.

   Parâmetros:
   - Nodei: Um ponteiro para a estrutura Node, que representa o nó atual na rede.
   - df: Um ponteiro para o DataFrame do qual uma coluna será removida.
   - df2: Um ponteiro para um segundo DataFrame, utilizado para realizar verificações adicionais após a remoção da coluna em 'df'.
   - df3: Um ponteiro para um terceiro DataFrame, utilizado na função 'find_cc'.
   - col: Uma string que representa o identificador da coluna a ser removida.
   - socket: Um inteiro que pode representar um identificador de socket, utilizado na função 'find_cc'.

   Funcionamento:
   - Inicialmente, a função procura pela coluna a ser removida em 'df', identificada pela primeira linha (row[0].data) de cada coluna.
   - Quando encontra a coluna correspondente a 'col', ou após ter encontrado uma coluna anteriormente, move os dados das colunas subsequentes uma posição para a esquerda, efetivamente removendo a coluna desejada.
   - Em seguida, a função itera sobre as linhas do DataFrame 'df2', usando sscanf para extrair e comparar dados específicos. Se a condição de comparação for satisfeita, a função 'find_cc' é chamada.
   - A função retorna um valor inteiro 'n', que é o resultado da última chamada a 'find_cc', ou -1 caso 'find_cc' não seja chamada.

   Notas:
   - A função assume que a primeira linha de cada coluna em 'df' contém o identificador da coluna.
   - O parâmetro 'flag' é usado para indicar se a coluna a ser removida já foi encontrada, otimizando o processo de remoção.
   - O uso de 'sscanf' em 'df2' sugere que os dados estão em um formato específico que permite a extração de dois componentes, possivelmente relacionados a algum tipo de mapeamento ou relação entre os dados.
   - É importante notar que a remoção de colunas pode alterar a estrutura do DataFrame de maneira significativa, possivelmente afetando outras operações que dependem de sua estrutura original.

*/
int removeColumn(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char col[], int socket);


/*
   Função: limparTabela

   Descrição:
   Esta função é responsável por limpar os dados de todas as células de um DataFrame, definindo o conteúdo de cada célula como uma string vazia ("") e o comprimento dos dados como 0. A operação é realizada em todas as colunas e linhas da tabela.

   Parâmetros:
   - tabela: Um ponteiro para o DataFrame que será limpo.

   Funcionamento:
   - A função itera sobre cada célula do DataFrame, acessada pela combinação dos índices 'y' (linha) e 'x' (coluna).
   - Dentro do loop duplo, a string de cada célula é definida como vazia (""), e o comprimento da string (len) é definido como 0. Isso efetivamente "limpa" o conteúdo da célula, indicando que não há dados armazenados nela.

   Notas:
   - A função assume que o DataFrame é estruturado de tal forma que cada célula pode ser acessada e modificada diretamente pelos índices 'x' e 'y', através de 'tabela->col[x].row[y].data' e 'tabela->col[x].row[y].len'.
   - Limpar a tabela desta forma não altera as dimensões do DataFrame (ou seja, a quantidade de colunas e linhas permanece a mesma), apenas remove o conteúdo das células.

*/
void limparTabela(DataFrame *tabela);


/*
   Função: find_cc

   Descrição:
   Esta função é responsável por encontrar o caminho mais curto na tabela de encaminhamento e atualizar a tabela de caminhos mais curtos de acordo com a linha atualizada recebida. O objetivo é determinar o melhor caminho para um determinado destino e manter a tabela de caminhos mais curtos atualizada.

   Parâmetros:
   - Nodei: Um ponteiro para a estrutura do nó que está executando o algoritmo de roteamento.
   - df: Um ponteiro para o DataFrame principal que contém a tabela de encaminhamento.
   - df2: Um ponteiro para o DataFrame que representa a tabela de caminhos mais curtos.
   - df3: Um ponteiro para o DataFrame que representa a tabela de expedição.
   - linhaAtualizada: Uma string que indica a linha da tabela de encaminhamento que foi atualizada recentemente.
   - socket: O socket associado ao nó, utilizado para envio de mensagens.

   Funcionamento:
   - A função começa inicializando um array 'count_token' com zeros para armazenar a contagem de tokens em cada célula da linha atualizada.
   - Em seguida, itera sobre as células da linha atualizada, calculando a contagem de tokens em cada célula.
   - Após calcular a contagem de tokens para cada célula, a função determina o índice da coluna com o menor número de tokens, representando o caminho mais curto.
   - Se um caminho mais curto for encontrado, a função atualiza a tabela de caminhos mais curtos com o novo caminho e envia uma mensagem ROUTE para atualizar os nós vizinhos.
   - Se a linha atualizada estiver vazia, a função limpa a entrada correspondente na tabela de caminhos mais curtos.
   - Caso a linha atualizada não seja encontrada na tabela de encaminhamento, a função imprime uma mensagem de erro.

   Retorna:
   - Um inteiro indicando o resultado da operação:
     - 1: Sucesso na atualização da tabela de caminhos mais curtos.
     - 3: A linha que está sendo atualizada está vazia.
     - 0: Nenhuma ação é realizada se a linha atualizada não for encontrada.

   Considerações:
   - Esta função é essencial em algoritmos de roteamento para manter as tabelas de encaminhamento e de caminhos mais curtos atualizadas, garantindo que os pacotes sejam roteados pelo caminho mais eficiente.
*/
int find_cc(struct Node *Nodei, DataFrame *df, DataFrame *df2, DataFrame *df3, char linhaAtualizada[], int socket);

/*
   Função: verify_route

   Descrição:
   Esta função verifica se a rota contém o próprio nó como um dos nós intermediários. Se o nó for encontrado na rota, ele é removido, e a nova rota é atualizada.

   Parâmetros:
   - rota: Um ponteiro para a estrutura que representa a rota a ser verificada.
   - id: Uma string que representa o identificador do próprio nó.

   Funcionamento:
   - A função inicializa uma nova string 'new_route' para armazenar a nova rota sem o nó próprio.
   - Se a rota estiver vazia, a função retorna imediatamente.
   - Em seguida, a função utiliza strtok para dividir a rota em tokens usando o delimitador "-".
   - A função percorre os tokens, procurando pelo identificador do próprio nó.
   - Se o identificador for encontrado, os tokens subsequentes são copiados para a nova rota.
   - Após percorrer todos os tokens, a nova rota é atualizada e a rota original é substituída pela nova rota.
   - Se a rota original ficar vazia após a remoção do nó próprio, a função imprime uma mensagem indicando que o nó é o destino final.

   Retorna:
   - Um inteiro indicando o resultado da operação:
     - 0: Nenhum nó próprio foi encontrado na rota.
     - 1: O próprio nó foi encontrado na rota e a rota foi atualizada para indicar que o nó é o destino final.
     - 2: O próprio nó foi encontrado na rota e a rota foi atualizada sem indicar que o nó é o destino final.

   Considerações:
   - Esta função é útil em algoritmos de roteamento para evitar que os nós enviem mensagens para si mesmos, removendo o próprio nó da rota, se necessário.
*/
int verify_route(Routes *rota, char *id);


/*
   Função: add_info_table

   Descrição:
   Esta função adiciona informações a uma célula específica na tabela de encaminhamento.

   Parâmetros:
   - df: Um ponteiro para a estrutura DataFrame que representa a tabela de encaminhamento.
   - df2: Um ponteiro para a estrutura DataFrame que representa a tabela de caminhos mais curtos.
   - df3: Um ponteiro para a estrutura DataFrame que representa a tabela de expedição.
   - rota: Um ponteiro para a estrutura Routes que contém as informações da rota a ser adicionada à tabela.
   - Nodei: Um ponteiro para a estrutura Node que representa o nó atual.
   
   Funcionamento:
   - A função percorre a tabela de encaminhamento e verifica se há uma célula vazia ou se uma célula correspondente à rota já existe.
   - Se uma célula vazia ou correspondente não for encontrada, a função preenche a célula com a rota, o nó de destino e o nó adjacente.
   - Em seguida, a função verifica se a rota atualizada na tabela de encaminhamento é menor do que a rota armazenada na tabela de caminhos mais curtos. Se for o caso, a tabela de caminhos mais curtos é atualizada.
   - Após atualizar a tabela de caminhos mais curtos, a função chama a função 'find_cc' para atualizar a tabela de encaminhamento expandida, se necessário.

   Retorna:
   - Um inteiro que indica o resultado da operação:
     - -1: Nenhuma rota válida foi adicionada à tabela.
     - 1: A rota foi adicionada à tabela com sucesso e a tabela de caminhos mais curtos foi atualizada.
*/
int add_info_table(DataFrame *df, DataFrame *df2, DataFrame *df3, Routes *rota, struct Node *Nodei);



/*
   Função: show_tables

   Descrição:
   Esta função chama as funções responsáveis por imprimir as tabelas de encaminhamento, caminho mais curto e expedição.

   Parâmetros:
   - df: Um ponteiro para a estrutura DataFrame que representa a tabela de encaminhamento.
   - df2: Um ponteiro para a estrutura DataFrame que representa a tabela de caminho mais curto.
   - df3: Um ponteiro para a estrutura DataFrame que representa a tabela de expandição.
   
   Funcionamento:
   - A função chama a função 'printDataFrame' para imprimir a tabela de encaminhamento.
   - Em seguida, chama a função 'printDataFrame' novamente para imprimir a tabela de caminho mais curto.
   - Por fim, chama 'printDataFrame' para imprimir a tabela de expedição.
   - Esta função pode ser utilizada para visualizar o conteúdo das tabelas durante a execução do programa.

   Retorna:
   - Um inteiro que indica o resultado da operação. Neste caso, sempre retorna 0.
*/
int show_tables(DataFrame *df, DataFrame *df2, DataFrame *df3);

void show_path(DataFrame *df2, char linha[]);

int show_routing(DataFrame *df, char linha[]);



#endif
