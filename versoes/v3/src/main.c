/*
---------------------------------
| v3 - ABC Algorithm in C / C++ |
---------------------------------
* THIS VERSION ONLY WORKS WITH BIDIMENSIONAL FUNCTIONS *

This third version focuses mostly on algorithm improvement aswell as some slight abstraction in order
to have an easy manner with which we can implement global optimization functions to be tested.

This newest rewritting was made based on the following article: 
https://www.researchgate.net/publication/221498082_Artificial_Bee_Colony_ABC_Optimization_Algorithm_for_Solving_Constrained_Optimization_Problems

*/

/*
-------------
| Libraries |
-------------
Nesta seção do código realizo a inclusão das bibliotecas que irei utilizar.
- <stdlib.h> : Contem a função rand() utilizado para gerar um valor de 0 até RAND_MAX
- <time.h> : Contem a definição de RAND_MAX.
- <math.h> : Contem as funções matemáticas mais úteis, como pow() que permite realizar a exponenciação
             de doubles.
- "opt_algorithms.h" : Contem as constantes e as variáveis que irei utilizar no meu código para gerar
as funções que pretendo otimizar.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "opt_algorithms.h"
/*
------------------------
| Constant definitions |
------------------------
The constants used in this program belong in two different groups:
(i) ABC algorithm parameters: This algorithm counts with 4 distinct parameters used for controlling the bees.
(ii) Code-specific parameters: 
(iii) Test modules:
*/

// ABC Algorithm parameters
#define NUMABELHAS 100 // Number of Bees in my population
#define NUMSOLF NUMABELHAS/2 
#define LIMITESCOUT 100 // Max number of times a scout will remain in a single point
#define NUMFORAGING 10000 // Maximum cycle number

//#define teste_valores

/*
-------------------------
|Definição de variáveis |
-------------------------
Nesta seção do código defino as variáveis que estarei utilizando no código.
Note-se que várias delas foram definidas pela sua utilidade. 
*/
//Para fazer operações com as abelhas
double solutionNeighbor[NUMSOLF];
double limit_counter[NUMSOLF];
double bestSolution = 0.0;
double solutionArray[NUMSOLF];
double numAttempts[NUMSOLF]; // Preencher com 0.
double fitness[NUMSOLF];
double probability[NUMSOLF];

int numLoop = 0;
int numActiveOnlookers = 0;
int numVariables = 0;
float xMin;
float xMax;
float yMin;
float yMax;
_Bool flagMultiDim = 0;
_Bool flagWaitInput = 0;
// Initial definition of the functions used in the program
void normalizeProbability(double prob_array[], int tamanho);
void OnlookerBees(int index);
double (*optimizationFunction)(double arr[]); //

/*
-----------------
| Main function |
-----------------
*/

void main(void){
    // Aqui inicializo a semente utilizada para o rand(). Assim, o resultado é distinto cada iteração.
    srand(time(NULL));
    
    // In this second part we'll select the function to be used and its constraint parameters.
    int numAlgorithm;
    printf("Select the algorithm you'll use: ");
    scanf("%d", &numAlgorithm);
    switch(numAlgorithm){
        case 1:
            printf("You chose the Ackley Function. min=0 at (0,0,...,0) \n");
            optimizationFunction = &ackleyFunction;
            xMin = yMin = ACKLEYMIN;
            xMax = yMax = ACKLEYMAX;
            flagMultiDim = 1;
            break;
        case 2:
            printf("You chose the Sphere Function. min=0 at (0,0,..,0) \n");
            optimizationFunction = &sphereFunction;
            xMin = yMin = SPHEREMIN;
            xMax = yMax = SPHEREMAX;
            flagMultiDim = 1;
            break;
        case 3:
            printf("You chose the Beale Function. min=0 at (3, 0.5) \n");
            optimizationFunction = &bealeFunction;
            xMin = yMin = BEALEMIN;
            xMax = yMax = BEALEMAX;
            break;
        case 4:
            printf("You chose the Bird Function. min=-106.764537 at (4.70104, 3.15294) and (-1.58214, -3.13024) \n");
            optimizationFunction = &birdFunction;
            xMin = yMin = BIRDMIN;
            xMax = yMax = BIRDMAX;
            break;
        case 5:
            printf("You chose the Booth Function. min=0 at (1, 3) \n");
            optimizationFunction = &boothFunction;
            xMin = yMin = BOOTHMIN;
            xMax = yMax = BOOTHMAX;
            break;
        case 6:
            printf("You chose the Bukin6 Function. min=0 at (-10, 1) \n");
            optimizationFunction = &bukin6Function;
            xMin = BUKIN6XMIN;
            xMax = BUKIN6XMAX;
            yMin = BUKIN6YMIN;
            yMax = BUKIN6YMAX;
            break;
        case 7:
            printf("You chose the Carromtable Function. min=0 at (1, 3) \n");
            optimizationFunction = &boothFunction;
            xMin = yMin = BOOTHMIN;
            xMax = yMax = BOOTHMAX;
            break;
        case 8:
            printf("You chose the Chichinadze Function. min=-42.944387 at (6.189866586965680, 0.5) \n");
            optimizationFunction = &chichinadzeFunction;
            xMin = yMin = CHICHINADZEMIN;
            xMax = yMax = CHICHINADZEMAX;
            break;
        case 9:
            printf("You chose the Cross-in-tray Function. min=-2.06261 at (+-1.3491, +-1.3491) \n");
            optimizationFunction = &crossintrayFunction;
            xMin = yMin = CROSSINTRAYMIN;
            xMax = yMax = CROSSINTRAYMAX;
            break;
        case 10:
            printf("You chose the Cross-Leg Table Function. min=-1 at (0, 0) \n");
            optimizationFunction = &crosslegtableFunction;
            xMin = yMin = CROSSLEGTABLEMIN;
            xMax = yMax = CROSSLEGTABLEMAX;
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        default:
            printf("ERROR: Invalid input. Write an integer from 0-29.");
            return;
    }

    if(flagMultiDim = 1){
        flagWaitInput = 1;
        while(flagWaitInput){
            printf("How many dimensions will you be working with? ");
            scanf("%d", &numVariables);
            if(numVariables < 2){
                printf("ERROR: Invalid input. Write an integer starting from 2.");
            } else {
                flagWaitInput = 0;
            }
        }
    } else {
        numVariables = 2;
    }

    // Once the size has been determined, create variables.
    double foodsourceNeighbor[NUMSOLF][numVariables];
    double bestfoodSources[numVariables];
    double foodSources[NUMSOLF][numVariables];

    // In this step I populate my domain with random food sources.
    // Number of food sources: NUMSOLF; Number of food per food source: numVariables
    for (int i = 0; i < NUMSOLF; i++){
        for (int k = 0; k < numVariables; k++){
            // This limits my possible test functions to only functions defined in a 2D space.
            // I may need to make xMax and yMax become values inside of a maxValue array.
            foodSources[i][k] = ((double)rand()/((double)(RAND_MAX)))*(xMax - xMin) + xMin;
        }

        // It's written this way to leave explicitly clear that we have a function pointer.
        solutionArray[i] = (*optimizationFunction)(foodSources[i]);
        #ifdef teste_valores
            printf("solutionArray[%lf]\t", solutionArray[i]);
        #endif
    }

    // Aqui realizo (ii)
    bestSolution = solutionArray[0];
    for (int k = 0; k < numVariables; k++){
        bestfoodSources[k] = foodSources[0][k];
    }
    for (int i = 0; i < NUMSOLF; i++){
        numAttempts[i] = 0;
    }

    /* ------------------------------------------- ENTRO LOOP ----------------------------------------- */
    while(numLoop < NUMFORAGING){
        // Increment numLoop every cycle.
        numLoop++;

        // I'm storing the best solution I currently have inside of the bestSolution variable.
        for (int i = 0; i < NUMSOLF; i++){
            if (solutionArray[i] < bestSolution ){    
                bestSolution = solutionArray[i];        
                for (int k = 0; k < numVariables; k++){
                    bestfoodSources[k] = foodSources[i][k];
                }    
            }
        }
        
        /*
        -----------------
        | Employed Bees |
        -----------------   
        */

        //(1)
        for (int i = 0; i < NUMSOLF; i++){
            for (int k = 0; k < numVariables; k++){
                int randomIndex;
                double randomValue = ((double)rand()/((double)(RAND_MAX))+(double)rand()/((double)(RAND_MAX)-1.0));
                do{ //Para garantir que este indice seja diferente aos outros.
                    randomIndex = rand()%(numVariables+1);
                }while(randomIndex == k); 
                double randomfoodSource = randomValue*(foodSources[k] - foodSources[randomIndex]);
                // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
                foodsourceNeighbor[i][k] = foodSources[i][k] + randomfoodSource;
                // When introducing std::vector this code segment will change.
                if(foodsourceNeighbor[i][k] > xMax){
                    foodsourceNeighbor[i][k] = xMax;
                }else{
                    if(foodsourceNeighbor[i][k] < xMin){
                        foodsourceNeighbor[i][k] = xMin;
                    }
                }
            }
            solutionNeighbor[i] = (*optimizationFunction)(foodsourceNeighbor[i]);
        }

        // Greedy selection process
        for (int i = 0; i < NUMSOLF; i++){
            if (solutionNeighbor[i] < solutionArray[i]){    
                solutionArray[i] = solutionNeighbor[i];        
                for (int k = 0; k < numVariables; k++){
                    foodSources[i][k] = foodsourceNeighbor[i][k];
                }
                numAttempts[i] = 0;
            } else {
                numAttempts[i] = numAttempts[i] + 1;
            }
        }

        // In the future fitness expression should become customizable, reading TeX math equations.
        double totalFitness = 0.0;
        for (int i = 0; i < NUMSOLF; i++){
            fitness[i] = (solutionArray[i] >= 0.0)?((double)(1/(1+solutionArray[i]))):((double)(1-solutionArray[i]));
            totalFitness = totalFitness + fitness[i];
            #ifdef teste_valores
                printf("fitness[%lf] = %lf\n", i, fitness[i]);
            #endif
        }
        #ifdef teste_valores
            printf("\nProb before normalizing\n");
        #endif
        for (int i = 0; i < NUMSOLF; i++){
            probability[i] = (fitness[i])/totalFitness;
            #ifdef teste_valores
                printf("%lf\t", probability[i]);
            #endif
        }
        normalizeProbability(probability, NUMSOLF);
        #ifdef teste_valores
            printf("\nProb after normalizing\n");
            for (int i = 0; i < NUMSOLF; i++){
                printf("%lf\t", probability[i]);
            }
        #endif

        /*
        -----------------
        | Onlooker Bees |
        -----------------
        */
        int index = 0;
        while(numActiveOnlookers < 50){
            int randomProb = (double)rand()/((double)(RAND_MAX)); //Value from 0 to 1
            // foodSources with higher probability index are more likely to be chosen.
            if(randomProb < probability[index]){
                //(1)
                for (int k = 0; k < numVariables; k++){
                    int randomIndex;
                    double randomValue = ((double)rand()/((double)(RAND_MAX))+(double)rand()/((double)(RAND_MAX)-1.0));
                    do{ //To guarantee this index is different to the others.
                    randomIndex = rand()%(numVariables+1);
                    }while(randomIndex == k); 
                    double randomfoodSource = randomValue*(foodSources[k] - foodSources[randomIndex]);
                    // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
                    foodsourceNeighbor[index][k] = foodSources[index][k] + randomfoodSource;
                    if(foodsourceNeighbor[index][k] > xMax){
                        foodsourceNeighbor[index][k] = xMax;
                    }else{
                        if(foodsourceNeighbor[index][k] < xMin){
                            foodsourceNeighbor[index][k] = xMin;
                        }
                    }
                }
                solutionNeighbor[index] = (*optimizationFunction)(foodsourceNeighbor[index]);

                //(2)
                if (solutionNeighbor[index] < solutionArray[index]){    
                    solutionArray[index] = solutionNeighbor[index];        
                    for (int k = 0; k < numVariables; k++){
                        foodSources[index][k] = foodsourceNeighbor[index][k];
                    }
                numAttempts[index] = 0;
                } else {
                    numAttempts[index] = numAttempts[index] + 1;
                }//
                numActiveOnlookers++;
            }
            index++;
            if(index==NUMSOLF){index==0;}
        }
        numActiveOnlookers = 0;

        // Após termos movimentado as onlookers, começo a procurar a  melhor fonte de comida.
        for (int i = 0; i < NUMSOLF; i++){
            if (solutionArray[i] < bestSolution ){    
                bestSolution = solutionArray[i];        
                for (int k = 0; k < numVariables; k++){
                    bestfoodSources[k] = foodSources[i][k];
                }    
            }
        }
        
        /*
        --------------
        | Scout Bees |
        --------------
        */
        for(int i = 0; i < NUMSOLF; i++){
            if( numAttempts[i] >= LIMITESCOUT ){
                for (int k = 0; k < numVariables; k++){
                    // Find a new food source.
                    foodSources[i][k] = ((double)rand()/((double)(RAND_MAX)))*(xMax - xMin) + xMin;
                }
                solutionArray[i] = (*optimizationFunction)(foodSources[i]);
                numAttempts[i] = 0;
            }
        }
    } // Loop ends on this bracket.

    printf("bestSol: %lf\n", bestSolution );
    for (int k = 0; k < numVariables; k++){
        printf("%lf\t", bestfoodSources[k]);
    }
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
    // Aqui insiro meus novos valores dentro da matriz de probability.
    for(int k = 0; k < tamanho; k++){
       prob_array[k] = (prob_array[k] - minval)/(maxval-minval);
    }
}

