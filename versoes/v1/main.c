/*
---------------------------------------
| Versão 1 - Algoritmo ABC em C / C++ |
---------------------------------------
Nome do estudante: Melvin Maradiaga

Neste documento escrevo uma versão do algoritmo ABC (Articial Bee Colony Algorithm) para ser testado
com:
- Função Esfera
- Função Rosenbrock
- Função Rastrigin

-> Explicação lógica <-
(Preliminar) Nesta primera etapa preciso inserir uns valores importantes para a execução do algoritmo.
Esses valores são:
* O dominio da função
* Quantas variáveis essa função possui
* Quantos ciclos do algoritmo pretendo rodar

(i) Para implementar este algoritmo, preciso primeiro definir a função que pretendo otimizar. Para isso,
note-se que crio três funções distinas (cada uma correspondente às funções de teste que usarei para o
ABC). No caso, a primeira função que irei testar é a função Esfera.

(ii) Depois, crio minhas primeiras soluções de teste escolhendo ao azar valores para cada Xi que a 
minha função possui. No caso da função esfera, gero quatro grupos de quatro números aleatórios dentro 
do dominio escolhido para minha função (obtendo assim 4 soluções à função).
X[0][x]= [ x1, x2, x3, x4]
Tendo feito isso, avalio a função com cada grupo de solução obtido, e registro minhas quatro soluções dentro
de uma matriz.
solFX[] = [f(X1[]), f(X2[]), f(X3[]), f(X4[])] 

- Neste ponto entramos num bucle. Assim, defino o meu primeiro ciclo do bucle como ciclo = 1.
(iii) Aqui defino um novo conjunto de soluções: Vi[]. Seguindo o descrito pelo Karaboga, uso a formula:
Vi = Xi + rand(-1,1)*(Xi - Xk) {*}
Neste caso, cada xk é uma solução próxima de xi, obtido aumentando ou diminuindo xi uma quantidade pequena.
Assim, obtenho:
solFV[] = [f(V1[]), f(V2[]), f(V3[]), f(V4[])]

(iv) Nesta parte preciso realizar uma comparação entre solF1 e solF2, escolhendo (no caso da funcao esfera,
que estou querendo minimizar) o valor menor entre cada conjunto de soluções e criando uma nova matriz
solF[], que corresponderia ao meu conjunto de soluções úteis.

(v) Depois, preciso calcular o valor de probabilidade de cada função utilizando os valores de fitness.
Esse valor de fitness vou determinar seguindo a equação apresentada no documento de Karaboga:
FITi = 1/(1+fi) para fi > 0.
FITi = 1+abs(fi) para fi < 0.
Note-se que preciso normalizar o valor de probabilidade (seguindo o que falam no roteiro) pelo qual, 
vou aplicar normalização min-max.

(vi)  Aqui preciso gerar novas soluções solFV utilizando {*] alterada. Só que, desta vez, vou avaliar alguns
parâmetros:
- Se uma solução surgir quatro vezes consecutivas, procurar outra solução mediante o algoritmo.
- As duas soluções com o menor valor de probabilidade são descartadas, procurando uma nova.
Depois, realizo a seleção entre os valores (escolhendo o menor) e escrevo em solF[].

(vii) Armazena a melhor solução achada até agora. E aumenta o ciclo com ciclo++. Para a execução ao atingir
o ciclo máximo permitido (NUMMAXCYCLE).


-> Explicação Qualitativa <-
Essencialmente, aplicamos o conceito das três abelhas neste algoritmo:
- Abelha coletora
- Abelha espectadora
- Abelha exploradora

Num primeiro momento, ao realizar (i) estou definindo o campo de flores que as abelhas irão explorar.
Depois, em (ii) definimos algumas posições onde temos alimento (neste caso, 4 posições e soluções)
Em (iii) as abelhas exploradoras vão até as soluções    
*/

/*
--------------------------
|Inclusão de bibliotecas |
--------------------------
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

// Parâmetros que deverão ser manipulados dependendo da função escolhida
#define XNEG -100
#define XPOS 100
#define NUMSOL 4
#define NUMMAXCYCLE 2000
#define MAXORMIN 0 // Esta variável define o caminho a ser pegue pelo algoritmo. Caso queria achar um
                   // máximo ou um mínimo simplesmente troco esse valor.
#define LIMIT 4
#define rangeXk 1 // Aqui defino o rango de valores nos quais meu Xk pode variar de Xi.
                     // Pode se interpretar o 1000 como dizendo que haverá uma variação na escala milli
#define rangeX 100
/*
-------------------------
|Definição de variáveis |
-------------------------
*/
double X[NUMSOL][NUMSOL];
double Xk[NUMSOL][NUMSOL];
double solfX[NUMSOL];
double V[NUMSOL][NUMSOL];
double solfV[NUMSOL];
double solf[NUMSOL];
double fitF[NUMSOL];
double probF[NUMSOL];
double totalfitF = 0.0;
int counter = 0;
int bestIndex = 0;
double bestSol = 0;

// Definição inicial das funções
double funcaoEsfera(double sol_func[], int tamanho);
void normalizeProbability(double prob_array[], int tamanho);

/*
-------------------
|Função principal |
-------------------

*/
void main(void){
    // Aqui inicializo a semente utilizada para o rand().
    srand(time(NULL));

    // Aqui realizo (ii). No entanto, pelo funcionamento de rand, preencho X[][] com valores inteiros.
    for (int i = 0; i < NUMSOL; i++){
        for (int k = 0; k < NUMSOL; k++){
            X[i][k] = rand()%(XPOS - XNEG) - 50;
        }
    }

    for (int i = 0; i < NUMSOL; i++){
        solfX[i] = funcaoEsfera(X[i], NUMSOL);
    }
    for(int w = 0; w < NUMSOL; w++){
        printf("solfX[%lf]\t", solfX[w]);
    }
    printf("\n");

    while(counter < NUMMAXCYCLE){
    // Aqui realizo (iii). Primeiro, obtenho os valores Xk que irei usar.
        for (int i = 0; i < NUMSOL; i++){
            for (int k = 0; k < NUMSOL; k++){
                Xk[i][k] = X[i][k] + (double)(rand()%(rangeXk)-rangeXk/2)/((double)rangeXk);
                // Depois calculo Vi mediante Vi = Xi + rand(-1,1)*(Xi - Xk)
                V[i][k] = X[i][k] + ((double)(rand()%(200)-100)/(100.0))*(X[i][k] - Xk[i][k]);
            }
        }
        for (int i = 0; i < NUMSOL; i++){
            solfV[i] = funcaoEsfera(V[i], NUMSOL);
        }
        for(int w = 0; w < NUMSOL; w++){
            printf("solfV[%lf]\t", solfV[w]);
        }
        printf("\n");

        // Aqui realizo (iv). Para a comparação únicamente preciso distinguir qual dos dois valores em
        // cada coluna é menor entre os dois.
        // Exemplo: min(solfV[1], solfX[i]) = solF[i]
        for (int i = 0; i < NUMSOL; i++){
            solf[i] = fmin(solfV[i], solfX[i]);
        }

        for(int w = 0; w < NUMSOL; w++){
            printf("solf[%lf]\t", solf[w]);
        }
        printf("\n");

        // Aqui realizo o cálculo da função fitness da minha solução, para depois calcular a probabilidade.
        for (int i = 0; i < NUMSOL; i++){
            // Esta função fitness comentada é usada para achar o minimo (ou máximo) de uma função.
            // No caso de minimos, preciso achar os valores fitness maiores.
            fitF[i] = (solf[i] >= 0.0)?((double)(1/(1+solf[i]))):((double)(1-solf[i]));
            printf("fitF[%d] = %lf\n", i, fitF[i]);
            totalfitF = totalfitF + fitF[i];
        }
        printf("totalfitF = %lf\n", totalfitF);
        for (int k = 0; k < NUMSOL; k++){
            probF[k] = (fitF[k])/totalfitF;
        }

        printf("Prob before normalizing\n");
        for(int w = 0; w < NUMSOL; w++){
            printf("%lf\n", probF[w]);
        }

        normalizeProbability(probF, NUMSOL);
        printf("Prob after normalizing\n");
        for(int w = 0; w < NUMSOL; w++){
            printf("%lf\n", probF[w]);
        }

        // Aqui pego os valores com as menores probabilidades e descarto eles, testando outros.
        // Para testar com a normalização, vou descartar aqueles valores que sejam menores a 0,5 em pro
        for(int i = 0; i<NUMSOL; i++){
            for (int k = 0; k < NUMSOL; k++){
                if(probF[k] > 0.5){
                    X[i][k] = X[i][k] + (double)(rand()%(rangeX)-rangeX/2)/((double)rangeX);
                    Xk[i][k] = X[i][k] + (double)(rand()%(rangeXk)-rangeXk/2)/((double)rangeXk);
                    // Depois calculo Vi mediante Vi = Xi + rand(-1,1)*(Xi - Xk)
                    V[i][k] = X[i][k] + ((double)(rand()%(20)-10)/(10.0))*(X[i][k] - Xk[i][k]);
                }
            }
        }

        totalfitF = 0;
        for(int i = 0; i < NUMSOL; i++){
            solfX[i] = funcaoEsfera(X[i], NUMSOL);
            solfV[i] = funcaoEsfera(V[i], NUMSOL);
            solf[i] = fmin(solfX[i], solfV[i]);
            if( i==0 ){ bestSol = solf[i]; }
            else {
                if (solf[i] < bestSol) {
                    bestSol = solf[i];
                    bestIndex = i;
                }
            }
        }
    counter++;
    }
    printf("bestSol: %lf", bestSol);

}

// Declaração das funções a serem utilizadas
double funcaoEsfera(double sol_func[], int tamanho){
    double resultado = 0.0;
    for(int w = 0; w < tamanho; w++){
        resultado =+ pow(sol_func[w],2);
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
/*
---------------
| Observações |
---------------

(a) No momento de escolher a melhor solução mediante o algoritmo guloso provavelmente compensaria
mais descartar permanente as soluções que o algoritmo ignora (ou simplesmente colocar menor peso nas
soluções próximas a ela). A implementação poderia ser pensada em termos de um limite, onde nosso epsilon
é de um tamanho definitivo e atribuimos peso proporcional à distância.

(b) Não consegui entender a necessidade dessa definição de um novo conjunto de soluções após entrar
no bucle. Na minha visão não faz sentido, mas pode ser algo necessário.

(c) Neste tipo de algoritmo seria interessante definir uma constante que, se estiver presente,
faça com que o código que esteja rodando seja uma versão mais de exploração do que de identificação de
mínimos. Atualmente, meu código meio que tenta achar mínimos mas não consegue se dar bem com algumas outras partes.
*/

/*
--------------
| Sugestões |
-------------

(i) Vou realizar a primeira montagem do program utilizando duas variáveis únicamente para entender certinho
onde meu código está ficando presso.

(ii) Vou começar com uma quantidade maior de abelhas para começar a caçar as coisas. Devo aumentar 

(iii) Olhar para o código de Python que simula bonitinho o ABC para entender BEM como ele funciona
com gráficos.

(iv) No momento de procurar novas soluções devem ser de uma região bastante ampla, conferir todo o dominio.
Não tentar me manter preso numa mesma região.
*/

/*
--------------
| Explicação |
-------------

As onlookers podem se posicionar num memso ponto determinado por uma abelha empregada.
*/