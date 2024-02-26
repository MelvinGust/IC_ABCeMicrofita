/*
-------------
| Main - |
-------------
In this newest version I'll be studying cycles for convergence (aka, not stopping until I reach 98% certainty)
*/

/*
-------------
| Libraries |
-------------
Here I include the libraries I'll be using for this code.
- <stdlib.h> : Contem a função rand() utilizado para gerar um valor de 0 até RAND_MAX
- <time.h> : Contem a definição de RAND_MAX.
- <math.h> : Contem as funções matemáticas mais úteis, como pow() que permite realizar a exponenciação
             de doubles.
- "opt_algorithms.h" : Contem as constantes e as variáveis que irei utilizar no meu código para gerar
as funções que pretendo otimizar.
*/
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <opt_algorithms.hpp>

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
#define NUMABELHAS 5000 //Number of Bees in my population
#define LIMITESCOUT 5 // Max number of times a scout will remain in a single point
#define NUMFORAGING 10000 // Maximum cycle number

//#define teste_valores

/*
-------------------------
|Definição de variáveis |
-------------------------
Nesta seção do código defino as variáveis que estarei utilizando no código.
Note-se que várias delas foram definidas pela sua utilidade. 
*/

// Este não é um parâmetro de controle, simplesmente existe para facilitar a programação.
#define NUMSOLF NUMABELHAS/2 
#define CONVERGENCIA 0.99

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
std::vector<float> minValues;
std::vector<float> maxValues;
bool flagMultiDim = false;
bool flagWaitInput = false;

// Initial definition of the functions used in the program
void normalizeProbability(double prob_array[], int tamanho);
void OnlookerBees(int index);
double (*optimizationFunction)(double arr[], int size); //
void initUserInput(void);
int findnumVariables(bool flagMultiDim);

/*
-----------------
| Main function |
-----------------
*/
int main(void){
    // Aqui inicializo a semente utilizada para o rand(). Assim, o resultado é distinto cada iteração.
    srand(time(NULL));
    
    // In this second part we'll select the function to be used and its constraint parameters.
    initUserInput();

    // Once the size has been determined, create variables.
    double foodsourceNeighbor[NUMSOLF][numVariables];
    double bestfoodSources[numVariables];
    double foodSources[NUMSOLF][numVariables];
    int numFoodSources = sizeof(foodSources[0])/sizeof(foodSources[1][0]);

    // INICIALIZAÇÃO 
    // Number of food sources: NUMSOLF; Number of food per food source: numVariables
    for (int i = 0; i < NUMSOLF; i++){
        for (int k = 0; k < numVariables; k++){
            foodSources[i][k] = ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k];
        }
        solutionArray[i] = (*optimizationFunction)(foodSources[i], numFoodSources);
        #ifdef teste_valores
            printf("solutionArray[%lf]\t", solutionArray[i]);
        #endif
    }

    // Define best solution and initialize attempt count.
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
                double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;

                do{ //Para garantir que este indice seja diferente aos outros.
                    randomIndex = rand()%(numVariables+1);
                }while(randomIndex == k); 

                double randomfoodSource = randomValue*(foodSources[i][k] - foodSources[i][randomIndex]); 
                // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
                foodsourceNeighbor[i][k] = foodSources[i][k] + randomfoodSource;        
                if(foodsourceNeighbor[i][k] > maxValues[k]){
                    foodsourceNeighbor[i][k] = maxValues[k];
                }else{
                    if(foodsourceNeighbor[i][k] < minValues[k]){
                        foodsourceNeighbor[i][k] = minValues[k];
                    }
                }
            }
            solutionNeighbor[i] = (*optimizationFunction)(foodsourceNeighbor[i], numFoodSources);
        }

        // Greedy selection process (only for minimization)
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
                printf("fitness[%d] = %lf\n", i, fitness[i]);
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

        normalizeProbability(probability, NUMSOLF); //Since its passed as a pointer, it changes the original function.
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
        Mudei de 50 a NUMABELHAS/2 onlookers.
        */
        int index = 0;
        while(numActiveOnlookers < NUMSOLF){
            int randomProb = (double)rand()/((double)(RAND_MAX)); //Value from 0 to 1
            // foodSources with higher probability index are more likely to be chosen.
            if(randomProb < probability[index]){
                //(1)
                for (int k = 0; k < numVariables; k++){
                    int randomIndex;
                    double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;
                    do{ //To guarantee this index is different to the others.
                    randomIndex = rand()%(numVariables+1);
                    }while(randomIndex == k); //Added index
                    double randomfoodSource = randomValue*(foodSources[index][k] - foodSources[index][randomIndex]);
                    // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
                    foodsourceNeighbor[index][k] = foodSources[index][k] + randomfoodSource;
                    if(foodsourceNeighbor[index][k] > maxValues[k]){
                        foodsourceNeighbor[index][k] = maxValues[k];
                    }else{
                        if(foodsourceNeighbor[index][k] < minValues[k]){
                            foodsourceNeighbor[index][k] = minValues[k];
                        }
                    }
                }
                solutionNeighbor[index] = (*optimizationFunction)(foodsourceNeighbor[index], numFoodSources);

                //(2)
                if (solutionNeighbor[index] < solutionArray[index]){    
                    solutionArray[index] = solutionNeighbor[index];        
                    for (int k = 0; k < numVariables; k++){
                        foodSources[index][k] = foodsourceNeighbor[index][k];
                    }
                numAttempts[index] = 0;
                } else {
                    numAttempts[index] = numAttempts[index] + 1;
                }
                numActiveOnlookers++;
            }
            index++;
            if(index==NUMSOLF){index=0;}
        }
        numActiveOnlookers = 0;

        // Após termos movimentado as onlookers, começo a procurar a  melhor fonte de comida.
        for (int i = 0; i < NUMSOLF; i++){
            if (solutionArray[i] < bestSolution){    
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
                    foodSources[i][k] = ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k];
                }
                solutionArray[i] = (*optimizationFunction)(foodSources[i], numFoodSources);
                numAttempts[i] = 0;
            }
        }
    } // Loop ends on this bracket.

    printf("bestSol: %lf\n", bestSolution );
    for (int k = 0; k < numVariables; k++){
        printf("%lf\t", bestfoodSources[k]);
    }

    return 0; //Return from main
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

void initUserInput(void){
    int numAlgorithm;

    printf("Select the algorithm you'll use: ");
    scanf("%d", &numAlgorithm);
    switch(numAlgorithm){
        case 1:
            printf("You chose the Ackley Function. min=0 at (0,0,...,0) \n");
            optimizationFunction = &ackleyFunction;
            flagMultiDim = true;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(ACKLEYMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(ACKLEYMAX);}
            break;
        case 2:
            printf("You chose the Sphere Function. min=0 at (0,0,..,0) \n");
            optimizationFunction = &sphereFunction;
            flagMultiDim = true;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(SPHEREMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(SPHEREMAX);}
            break;
        case 3:
            printf("You chose the Beale Function. min=0 at (3, 0.5) \n");
            optimizationFunction = &bealeFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(BEALEMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(BEALEMAX);}
            break;
        case 4:
            printf("You chose the Bird Function. min=-106.764537 at (4.70104, 3.15294) and (-1.58214, -3.13024) \n");
            optimizationFunction = &birdFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(BIRDMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(BIRDMAX);}
            break;
        case 5:
            printf("You chose the Booth Function. min=0 at (1, 3) \n");
            optimizationFunction = &boothFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(BOOTHMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(BOOTHMAX);}
            break;
        case 6:
            printf("You chose the Bukin6 Function. min=0 at (-10, 1) \n");
            optimizationFunction = &bukin6Function;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){(i==0)?(minValues.push_back(BUKIN6XMIN)):(minValues.push_back(BUKIN6YMIN));}
            for(int i = 0; i < numVariables; i++){(i==0)?(maxValues.push_back(BUKIN6XMAX)):(maxValues.push_back(BUKIN6YMAX));}
            break;
        case 7:
            printf("You chose the Carromtable Function. min=-24.1568 at (+-9.64615, +-9.64615) \n");
            optimizationFunction = &carromtableFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(CARROMTABLEMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(CARROMTABLEMAX);}
            break;
        case 8:
            printf("You chose the Chichinadze Function. min=-42.944387 at (6.189866, 0.5) \n");
            optimizationFunction = &chichinadzeFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(CHICHINADZEMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(CHICHINADZEMAX);}
            break;
        case 9:
            printf("You chose the Cross-in-tray Function. min=-2.06261 at (+-1.3491, +-1.3491) \n");
            optimizationFunction = &crossintrayFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(CROSSINTRAYMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(CROSSINTRAYMAX);}
            break;
        case 10:
            printf("You chose the Cross-Leg Table Function. min=-1 at (0, X) or (X, 0) \n");
            optimizationFunction = &crosslegtableFunction;
            flagMultiDim = false;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(CROSSLEGTABLEMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(CROSSLEGTABLEMAX);}
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
            //Find way to deal with this case
    }
}

int findnumVariables(bool flagMultiDim){
    int inputNumVar;
    if(flagMultiDim == true){
        flagWaitInput = true;
        while(flagWaitInput){
            printf("How many dimensions will you be working with? ");
            scanf("%d", &inputNumVar);
            if(inputNumVar < 2){
                printf("ERROR: Invalid input. Write an integer starting from 2.");
            } else {
                flagWaitInput = false;
            }
        }
        return inputNumVar;
    }
    return 2;
}

