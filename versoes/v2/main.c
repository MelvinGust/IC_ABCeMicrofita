/*
---------------------------------------
| Versão 2 - Algoritmo ABC em C / C++ |
---------------------------------------
Nome do estudante: Melvin Maradiaga

Parte vital de esta reescrita foi entender melhor o funcionamento genético real de cada abelha
no algoritmo. Na versão anterior, não tinha um entendimento formado sobre o papel que cada abelha desempenha.
Assim, preciso primeiro detalhar sobre os papéis adotados pelos três tipos de abelhas:

- Employed Bees: As abelhas empregadas vao olhar os comidas disponiveis seletadas na inicialização do programa
                 e vão ir nas regiões (ou muito próximo delas). Elas são as abelhas que fazem a descoberta inicial 
                 das soluções (na prática, são aquelas que vão voar até um ponto que pareça conter comida).
                 Como vão estar próximas à região, elas vão chegar lá e ver se tem comida no ponto (ou próximo).

- Onlooker Bees: As onlookers são abelhas que vão partir de uma food source e olhar para outras fontes de cômida
                 próximas. Utilizam o MESMO principio de funcionamento das Employed Bees, essencialmente. No entanto, 
                 elas únicamente operam em soluções (food sources) cuja probabilidade está embaixo de um certo ponto.
                 Assim, só as piores soluções são trocadas pelo processo anterior.

- Scout Bees: As scouts são aquelas que voltam às food sources para determinarse já estive MUITO tempo na mesma
              food source e se ela acabou.

Além disso, alguns problemas do algoritmo anterior foram corrigidos nesta nova versão. Como:
(*) Em algumas partes do código usamos rand() sem nenhum limitante para o seu valor. Isso aqui fez que
    algumas as fontes de comida obtidas estiverem fora do dominio da função original.
    Isso foi corrigido nesta versão com um: [ (double)rand() / ((double)(RAND_MAX)+(double)(1)) ]
(*) No geral, todo o começo do código anterior estava fortemente desotimizado, por conta de não ter utilizado
    bem as funções nativas de C/C++.

Assim, irei reestruturar o meu algoritmo a atender minhas novas necessidades.
Explico melhor na seção do código correspondente à função principal as ideias que foram
aplicadas.
*/

/*
---------------------------
| Inclusão de bibliotecas |
---------------------------
Nesta seção do código realizo a inclusão das bibliotecas que irei utilizar.
- <stdlib.h> : Contem a função rand() utilizado para gerar um valor de 0 até RAND_MAX
- <time.h> : Contem a definição de RAND_MAX.
- <math.h> : Contem as funções matemáticas mais úteis, como pow() que permite realizar a exponenciação
             de doubles.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/*
---------------------------
| Definição de constantes |
---------------------------
Nesta seção do código defino as constantes que irei utilizar no meu programa. No geral, estas constantes
são paramêtros que irão ficar "fixos" ao longo da execução do código. Pensem neles como configurações
essenciais para ter o programa funcionando.

Por conta disso, definimos os parâmetros em três grupos distintos:
(i) Parâmetros das abelhas: São parâmetros específicos sobre as abelhas que estou usando nesta
    aplicação.
(ii) Parâmetros da função: São parâmetros importantes para a otimização da função. Note-se que estes
    parâmetros geralmente vão mudar conforme as nossas necessidades.
(iii) Parâmetros do código: São parâmetros específicos da montagem do código que estou fazendo.
    Seria uma boa ideia não depender deles, pelo fato que me parecem desnecessários. Mas essas mudanças
    ficam pendentes para uma próxima versão.
*/

// Parâmetros das abelhas
#define NUMABELHAS 100
#define NUMSOLF NUMABELHAS/2
#define LIMITESCOUT 100
#define NUMFORAGING 10000

// Parâmetros da função sendo otimizada
#define RANGENEG -100
#define RANGEPOS 100
#define NUMPARAMETERS 2 
#define ROSENBROCKCONSTANT1 2
#define ROSENBROCKCONSTANT2 100
#define RASTRIGINPOS 5.12
#define RASTRIGINNEG -5.12
#define RASTRIGINCONSTANT 10
#define PI 3.14159265

// Parâmetros de código
// Para testar Rastrigin preciso diminuir bastante estes parâmetros. 0.1 -0.1 e 0.5 -0.5
#define MAXVARIACAOEMPLOYED 0.5
#define MINVARIACAOEMPLOYED -0.5
#define MAXVARIACAOONLOOKER 0.5
#define MINVARIACAOONLOOKER -0.5

//#define teste_valores
//#define usarEsfera
#define usarRastrigin
//#define usarRosenbrock

/*
-------------------------
|Definição de variáveis |
-------------------------
Nesta seção do código defino as variáveis que estarei utilizando durante sua execução.

Note-se que várias delas foram definidas pela sua utilidade. 

*/
//Para fazer operações com as abelhas
double fontes_vizinhas[NUMSOLF][NUMPARAMETERS];
double solucao_vizinha[NUMSOLF];
double limit_counter[NUMSOLF];
double melhorSolucao = 0.0;
double msolucao_parametros[NUMPARAMETERS];

double fontes_comida[NUMSOLF][NUMPARAMETERS];
double solucoes[NUMSOLF];
double tentativas[NUMSOLF]; // Preencher com 0.
double fitness[NUMSOLF];
double probabilidade[NUMSOLF];

int numBucle = 0;
int numOnlookersAtivas = 0;

// Definição inicial das funções
double funcaoEsfera(double sol_func[], int tamanho);
double funcaoRosenbrock(double sol_func[]);
double funcaoRastrigin(double sol_func[], int tamanho);
void normalizeProbability(double prob_array[], int tamanho);
void AbelhasOnlookers(int index);

/*
--------------------
| Função principal |
--------------------
Nesta seção do código explico o código principal. Essencialmente, irei realizar a montagem do algoritmo
ABC em cima da v1 que montei, só que desta vez detalhando cada vez mais nos principios de funcionamento
dele. A continuação apresento as distintas partes que compõem este código.

(i) Primeiro fazemos a distribuição das abelhas empregadas no dominio da nossa função.
    A ideia é simplesmente espalhar elas no nosso campo para depois começar a realizar todo o processo
    de avaliação com cada abelha.
    CASO QUEIRA MUDAR A FUNÇÃO SENDO UTILIZADA, PRECISO TROCAR A FUNCAO NESTA PARTE.

(ii) Nesta etapa preciso inicializar alguns elementos que irei utilizar ao longo do programa, como por exemplo:
     - msolucao_parametros[]
     - tentativas[]
     - melhorSolucao

----- Entrar no LOOP -----

(iii) Nesta etapa preciso avaliar qual é a melhor solução que possuo atualmente junto com os seus parâmetros.

(iv) Nesta seção do código realizo a avaliação das abelhas empregadas. Essencialmente, vou olhar para o
      meu conjunto de solução atual, procurar uma NOVA solução proxima a atual e atualizar a posição
      dependendo de qual me fornecer o melhor valor.

(v) Nesta etapa preciso avaliar, de novo, qual é a melhor solução que possuo atualmente junto com
     os seus parâmetros. Depois disso, preciso calcular o fitness da minha solução e calcular a probabilidade
     normalizada de cada uma delas.
 
(vi) Aqui movimento as minhas abelhas onlookers. Elas irão se movimentar para algumas soluções com base na
    probabilidade detectada anteriormente e irão achar uma NOVA solução.
*/

void main(void){
    // Aqui inicializo a semente utilizada para o rand(). Assim, o resultado é distinto cada iteração.
    srand(time(NULL));

    // Neste começo realizo (i)
    for (int i = 0; i < NUMSOLF; i++){
        for (int k = 0; k < NUMPARAMETERS; k++){
            // Obtenho valores dentro do dominio definido para serem minhas fontes de comida.
            #if defined(usarEsfera)||defined(usarRosenbrock)
                fontes_comida[i][k] = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(RANGEPOS - RANGENEG) - RANGEPOS;
            #endif
            #ifdef usarRastrigin
                fontes_comida[i][k] = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(RASTRIGINPOS - RASTRIGINNEG) - RASTRIGINPOS;
            #endif
        }
        #ifdef usarEsfera
            solucoes[i] = funcaoEsfera(fontes_comida[i], NUMPARAMETERS);
        #endif

        #ifdef usarRosenbrock
            solucoes[i] = funcaoRosenbrock(fontes_comida[i]);
        #endif

        #ifdef usarRastrigin
            solucoes[i] = funcaoRastrigin(fontes_comida[i], NUMPARAMETERS);
        #endif

        #ifdef teste_valores
            printf("solucoes[%lf]\t", solucoes[i]);
        #endif
    }

    // Aqui realizo (ii)
    melhorSolucao = solucoes[0];
    for (int k = 0; k < NUMPARAMETERS; k++){
        msolucao_parametros[k] = fontes_comida[0][k];
    }
    for (int i = 0; i < NUMSOLF; i++){
        tentativas[i] = 0;
    }

    /* ------------------------------------------- ENTRO LOOP ----------------------------------------- */
    while(numBucle < NUMFORAGING){
        // Lembrar que preciso aumentar numBucle cada ciclo.
        numBucle++;

        // Aqui realizo (iii). Note-se que não criei uma função específica para realizar esta tarefa
        // pois ela precisaria de vários argumentos.
        for (int i = 0; i < NUMSOLF; i++){
            if (solucoes[i] < melhorSolucao){    
                melhorSolucao = solucoes[i];        
                for (int k = 0; k < NUMPARAMETERS; k++){
                    msolucao_parametros[k] = fontes_comida[i][k];
                }    
            }
        }
        
        /*
        ----------------------
        | Abelhas empregadas |
        ----------------------
        Aqui realizo (iv).

        Detalhando por partes, primeiro gero uma solução vizinha cuja posição será ao redor do ponto original
        (numa região quadrada próxima). O seu tamanho é dado pelas constantes MAXVARIACAO e MINVARIACAO.
        Note-se que, como conto com únicamente 50 abelhas empregadas, faço a exploração com elas.

        Aqui preciso implementar um cuidado, que seria caso a minha nova solução esteja saindo dos limites
        definidos anteriormente como dominio da função.

        Logo depois, preciso comprar as minhas soluções vizinhas com a solução determinada anteriormente e
        ver qual é melhor em termos de magnitude.

        Note-se que, depois, preciso contar quais soluções minhas não mudaram. É pouco provável (mas possível)
        que elas simplesmentes não mudem. Nesse caso, preciso começar a realizar a contagem de tentativas que
        houveram para determinar uma nova solução que deram errado. Assim, começo minha contagem na matriz
        tentativas.
        */

        //(1)
        for (int i = 0; i < NUMSOLF; i++){
            for (int k = 0; k < NUMPARAMETERS; k++){
                // 
                double valor_aleatorio = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(MAXVARIACAOEMPLOYED - MINVARIACAOEMPLOYED) - MAXVARIACAOEMPLOYED;
                fontes_vizinhas[i][k] = fontes_comida[i][k] + valor_aleatorio;
                // Aqui realizo a correção para colocar as soluções na posição certa.
                #if defined(usarEsfera)||defined(usarRosenbrock)
                    if(fontes_vizinhas[i][k] > RANGEPOS){
                        fontes_vizinhas[i][k] = RANGEPOS;
                    }else{
                        if(fontes_vizinhas[i][k] < RANGENEG){
                        fontes_vizinhas[i][k] = RANGENEG;
                        }
                    }
                #endif
                #ifdef usarRastrigin
                    if(fontes_vizinhas[i][k] > RASTRIGINPOS){
                        fontes_vizinhas[i][k] = RASTRIGINPOS;
                    }else{
                        if(fontes_vizinhas[i][k] < RASTRIGINNEG){
                        fontes_vizinhas[i][k] = RASTRIGINNEG;
                        }
                    }
                #endif
            }
            #ifdef usarEsfera
                solucao_vizinha[i] = funcaoEsfera(fontes_vizinhas[i], NUMPARAMETERS);
            #endif

            #ifdef usarRosenbrock
                solucao_vizinha[i] = funcaoRosenbrock(fontes_vizinhas[i]);
            #endif

            #ifdef usarRastrigin
                solucao_vizinha[i] = funcaoRastrigin(fontes_vizinhas[i], NUMPARAMETERS);
            #endif

        }
        //(2)
        for (int i = 0; i < NUMSOLF; i++){
            if (solucao_vizinha[i] < solucoes[i]){    
                solucoes[i] = solucao_vizinha[i];        
                for (int k = 0; k < NUMPARAMETERS; k++){
                    fontes_comida[i][k] = fontes_vizinhas[i][k];
                }
                tentativas[i] = 0;
            } else {
                tentativas[i] = tentativas[i] + 1;
            }
        }

        // Logo a continuação realizo a etapa (v).
        double totalFitness = 0.0;
        for (int i = 0; i < NUMSOLF; i++){
            fitness[i] = (solucoes[i] >= 0.0)?((double)(1/(1+solucoes[i]))):((double)(1-solucoes[i]));
            totalFitness = totalFitness + fitness[i];
            #ifdef teste_valores
                printf("fitness[%lf] = %lf\n", i, fitness[i]);
            #endif
        }
        #ifdef teste_valores
            printf("\nProb before normalizing\n");
        #endif
        for (int i = 0; i < NUMSOLF; i++){
            probabilidade[i] = (fitness[i])/totalFitness;
            #ifdef teste_valores
                printf("%lf\t", probabilidade[i]);
            #endif
        }
        normalizeProbability(probabilidade, NUMSOLF);
        #ifdef teste_valores
            printf("\nProb after normalizing\n");
            for (int i = 0; i < NUMSOLF; i++){
                printf("%lf\t", probabilidade[i]);
            }
        #endif

        /*
        ---------------------
        | Abelhas onlookers |
        ---------------------
        Aqui realizo a etapa (vi). Para esta etapa, dependo bastante dos valores de probabilidade.
        Essecialmente, as minhas 50 abelhas onlookers vão escolher algum elemento entre as 50 food sources
        para aprimorar, com base na sua probabilidade.

        Uma maior probabilidade implica numa maior chance para essa food source ser escolhida. Isso aqui
        será refletido mediante uma maior quantidade de "dados" sendo jogados.

        Essencialmente, para cada onlooker vou jogar um dado a depender da probabilidade associada à food
        source que estou avaliando. A lógica é a seguinte:
        - Caso a probabilidade for menor que 0,3 vou jogar três dados. Caso os três dados forneçam uma
          resposta cima de 0,5, a onlooker vai procurar uma nova resposta.
        - Caso a probabilidade for maior que 0,3 e menor que 0,6 vou jogar dois dados. Caso os dois dados
          forneçam uma resposta acima de 0,5, a onlooker vai procurar uma nova resposta.
        - Caso a probabilidade for maior que 0,6 vou jogar um dado. Caso o dado forneça uma resposta acima de
          0,5, a onlooker vai procurar uma nova resposta.

        Se alguns dos casos não for satisfeitos, vou avaliar a próxima resposta SEM movimentar a onlooker. Vou 
        ficar neste loop indo de index 0-50 até movimentar todas as onlookers.
        */
        int index = 0;
        while(numOnlookersAtivas < 50){
            //Primeiro caso
            if (probabilidade[index] < 0,3){
                int dado1 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                int dado2 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                int dado3 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                if((dado1 >= 0,5)&&(dado2 >= 0,5)&&(dado3 >= 0,5)){
                    AbelhasOnlookers(index);
                    numOnlookersAtivas++;
                }
            }
            //Segundo caso
            if((probabilidade[index] > 0,3)&&(probabilidade[index] < 0,6)){
                int dado1 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                int dado2 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                if((dado1 >= 0,5)&&(dado2 >= 0,5)){
                    AbelhasOnlookers(index);
                    numOnlookersAtivas++;
                }
            }
            //Terceiro caso
            if(probabilidade[index] > 0,6){
                int dado1 = (double)rand() / ((double)(RAND_MAX)+(double)(1));
                if(dado1 >= 0,5){
                    AbelhasOnlookers(index);
                    numOnlookersAtivas++;
                }
            }
            index++;
            if(index==NUMSOLF){index==0;}
        }

        // Após termos movimentado as onlookers, começo a procurar a  melhor fonte de comida.
        for (int i = 0; i < NUMSOLF; i++){
            if (solucoes[i] < melhorSolucao){    
                melhorSolucao = solucoes[i];        
                for (int k = 0; k < NUMPARAMETERS; k++){
                    msolucao_parametros[k] = fontes_comida[i][k];
                }    
            }
        }
        
        /*
        -----------------
        | Abelhas Scout |
        -----------------
        Aqui simplesmente confiro se alguma solução minha ultrapassou o limite de tentativas permitidas por etapa.

        Vou olhar cada solução um por um.
        */
        for(int i = 0; i < NUMSOLF; i++){
            if( tentativas[i] >= LIMITESCOUT ){
                for (int k = 0; k < NUMPARAMETERS; k++){
                // Defino uma nova fonte de comida.
                #if defined(usarEsfera)||defined(usarRosenbrock)
                    fontes_comida[i][k] = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(RANGEPOS - RANGENEG) - RANGEPOS;
                #endif
                #ifdef usarRastrigin
                    fontes_comida[i][k] = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(RASTRIGINPOS - RASTRIGINNEG) - RASTRIGINPOS;
                #endif
            }

            #ifdef usarEsfera
                solucoes[i] = funcaoEsfera(fontes_comida[i], NUMPARAMETERS);
            #endif

            #ifdef usarRosenbrock
                solucoes[i] = funcaoRosenbrock(fontes_comida[i]);
            #endif

            #ifdef usarRastrigin
                solucoes[i] = funcaoRastrigin(fontes_comida[i], NUMPARAMETERS);
            #endif

            }
        }
    }
    printf("bestSol: %lf\n", melhorSolucao);
    for (int k = 0; k < NUMPARAMETERS; k++){
        printf("%lf\t", msolucao_parametros[k]);
    }
}

void AbelhasOnlookers(int index){
    //(1)
    for (int k = 0; k < NUMPARAMETERS; k++){
        double valor_aleatorio = ((double)rand() / ((double)(RAND_MAX)+(double)(1)))*(MAXVARIACAOONLOOKER - MINVARIACAOONLOOKER) - MAXVARIACAOONLOOKER;
        fontes_vizinhas[index][k] = fontes_comida[index][k] + valor_aleatorio;
        // Aqui realizo a correção para colocar as soluções na posição certa.
        #if defined(usarEsfera)||defined(usarRosenbrock)
            if(fontes_vizinhas[index][k] > RANGEPOS){
                fontes_vizinhas[index][k] = RANGEPOS;
            }else{
                if(fontes_vizinhas[index][k] < RANGENEG){
                fontes_vizinhas[index][k] = RANGENEG;
                }
            }
        #endif
        #ifdef usarRastrigin
            if(fontes_vizinhas[index][k] > RASTRIGINPOS){
                fontes_vizinhas[index][k] = RASTRIGINNEG;
            }else{
                if(fontes_vizinhas[index][k] < RASTRIGINNEG){
                fontes_vizinhas[index][k] = RASTRIGINNEG;
                }
            }
        #endif
    }

    #ifdef usarEsfera
        solucao_vizinha[index] = funcaoEsfera(fontes_vizinhas[index], NUMPARAMETERS);
    #endif
    #ifdef usarRosenbrock
        solucao_vizinha[index] = funcaoRosenbrock(fontes_vizinhas[index]);
    #endif
    #ifdef usarRastrigin
        solucao_vizinha[index] = funcaoRastrigin(fontes_vizinhas[index], NUMPARAMETERS);
    #endif

    //(2)
    if (solucao_vizinha[index] < solucoes[index]){    
        solucoes[index] = solucao_vizinha[index];        
        for (int k = 0; k < NUMPARAMETERS; k++){
            fontes_comida[index][k] = fontes_vizinhas[index][k];
        }
        tentativas[index] = 0;
    } else {
        tentativas[index] = tentativas[index] + 1;
    }
}

double funcaoEsfera(double sol_func[], int tamanho){
    double resultado = 0.0;
    for(int w = 0; w < tamanho; w++){
        resultado = resultado + pow(sol_func[w],2);
    }
    return resultado;
}

// Só usar essa aqui com dois variáveis independentes
// Aqui definimos RosenbrockConstant1 e RosenbrockConstant2 no começo.
double funcaoRosenbrock(double sol_func[]){
    double resultado = pow((double)ROSENBROCKCONSTANT1 - sol_func[0],2) + (double)ROSENBROCKCONSTANT2*pow(sol_func[1]-pow(sol_func[0],2),2);
    return resultado; 
}

double funcaoRastrigin(double sol_func[], int tamanho){
    double resultado = (double)(RASTRIGINCONSTANT*tamanho);
    for(int w = 0; w < tamanho; w++){
        resultado = resultado + pow(sol_func[w],2)-(double)RASTRIGINCONSTANT*cos(2*PI*sol_func[w]);
    }
    return resultado;
}
// Como os arrays são encaminhados como ponteiros a uma função, consigo modificar o original nesta
// função.
void normalizeProbability(double prob_array[], int tamanho){
    //Vou achar o menor e o maior valor dentro do meu array
    double minval = prob_array[0];
    double maxval = prob_array[0];
    for (int i = 0; i < tamanho; i++) {
        if (prob_array[i] < minval) {
            minval = prob_array[i];
        }
        else if (prob_array[i] > maxval) {   
            maxval = prob_array[i];
        }
    }
    // Aqui insiro meus novos valores dentro da matriz de probabilidade.
    for(int k = 0; k < tamanho; k++){
       prob_array[k] = (prob_array[k] - minval)/(maxval-minval);
    }
}
