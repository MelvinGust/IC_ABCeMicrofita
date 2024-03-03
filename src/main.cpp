/*
----------------
| Main Program |
----------------
The following modifications were made:
- Support for maximization and minimization problems, swapping slightly the greedy functions to be utilized.
(Use function overloading)
- Support for complex numbers via std::complex. 
- The program stops running only when the solution found is near a specified range (98%-102%)
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
// #include <beeroutines.hpp>
// #include <cbeeroutines.hpp>

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
#define NUMABELHAS 5000U //Number of Bees in my population
#define LIMITESCOUT 5U // Max number of times a scout will remain in a single point
#define NUMFORAGING 10000U // Maximum cycle number

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
#define CONVERGENCIA 0.99L

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
bool flagComplex = false;
bool flagMultiDim = false;
bool flagWaitInput = false;
bool stopProgram = false;
double desiredExtrema;

// Initial definition of the functions used in the program
void normalizeProbability(double prob_array[], int tamanho);
double (*optimizationFunction)(double arr[], int size); //
void initUserInput(void);
int findnumVariables(bool flagMultiDim);
void saveBestSolution(double bestfoodSources[], double *foodSources, int numSolutions);
void inicialize(double bestfoodSources[], double foodSources[], int numSolutions);
void employedBees(double foodSources[], double foodsourceNeighbor[], int numSolutions);
void evaluationPhase(int tamanho);
void onlookerBees(double foodSources[], double foodsourceNeighbor[], int numSolutions);
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

    switch(flagComplex){
        case true:
            break;
        case false:
            break;
    }

    // Once the size has been determined, create variables. I'll reorganize the arrays to allow
    // for them to be passed directly into the function.
    double bestfoodSources[numVariables];
    double foodsourceNeighbor[NUMSOLF][numVariables];       
    double foodSources[NUMSOLF][numVariables];
    /* I need to adopt C convention to pass this properly into my functions. Using std::vector<> will
    only create a copy of the original vector, I wouldn't be capable of modifying it. */
    int numFoodSources = sizeof(foodSources[0])/sizeof(foodSources[1][0]);

    inicialize(bestfoodSources, foodSources, numFoodSources);
    /* ------------------------------------------ ENTER LOOP ----------------------------------------- */
    while(stopProgram){
        numLoop++; // Increment numLoop every cycle.
        saveBestSolution(bestfoodSources, foodSources, numFoodSources); // CHECK
        employedBees(foodSources, foodsourceNeighbor, numFoodSources); // CHECK
        /* ------ Evaluation Phase ------ */
            evaluationPhase(NUMSOLF); // CHECK
            normalizeProbability(probability, NUMSOLF); // CHECK
        /* ---- End Evaluation Phase ---- */
        onlookerBees(foodSources, foodsourceNeighbor, numFoodSources); // CHECK 
        saveBestSolution(bestfoodSources, foodSources, numFoodSources);        
        // If the best solution found is good enough, stop looping.
        if((bestSolution  > CONVERGENCIA*desiredExtrema)&&(bestSolution < desiredExtrema + (1-CONVERGENCIA)*desiredExtrema)){
            stopProgram = true;
        }

        /* ------- Scout Bee Phase ------- */
        for(int i = 0; i < NUMSOLF; i++){
            if( numAttempts[i] >= LIMITESCOUT ){
                for (int k = 0; k < numVariables; k++){
                    foodSources[i][k] = ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k];
                }
                solutionArray[i] = (*optimizationFunction)(foodSources[i], numFoodSources);
                numAttempts[i] = 0;
            }
        }
        /* ------- End of scout bee phase ------- */
    }
    /* ---------------------------------------- OUT OF LOOP ------------------------------------------ */

    printf("The best solution found is %lf\n", bestSolution);
    for (int k = 0; k < numVariables; k++){
        printf("%lf\t", bestfoodSources[k]);
    }
    printf("\nIt took %d cycles", numLoop);

    return 0; //Return from main
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
            /*
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
            printf("You chose the Roots Function. min=... at (0, X) or (X, 0) \n");
            flagComplex = true;
            flagMultiDim = false;
            optimizationFunction = &crosslegtableFunction;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(CROSSLEGTABLEMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(CROSSLEGTABLEMAX);}
            break;
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
        */
        default:
            printf("ERROR: Invalid input. Write an integer from 0-29.");
            //Find way to deal with this case
    }
}

int findnumVariables(bool flagMultiDim){ //Why does this function require flagMultiDim?
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

// Remember every array is stored in a contiguous line of memory. In that sense, we can 
// actually  manipulate it via normal pointer access.
void inicialize(double bestfoodSources[], double *foodSources, int numSolutions){
    for (int i = 0; i < numSolutions; i++){
        for (int k = 0; k < numVariables; k++){
            foodSources[i*numSolutions+k] = ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k];
        }
        solutionArray[i] = (*optimizationFunction)(foodSources[i], numFoodSources);
        #ifdef teste_valores
            printf("solutionArray[%lf]\t", solutionArray[i]);
        #endif
    }

    bestSolution = solutionArray[0]; // Define best solution
    for (int k = 0; k < numVariables; k++){ // [i*0+k]
        bestfoodSources[k] = foodSources[k];
    }
    for (int i = 0; i < NUMSOLF; i++){ // Initialize attempt count.
        numAttempts[i] = 0;
    }
}

void saveBestSolution(double bestSolution[], double *foodSources, int numSolutions){
    for (int i = 0; i < numSolutions; i++){
        if (solutionArray[i] < bestSolution ){    
            bestSolution = solutionArray[i];        
            for (int k = 0; k < numVariables; k++){
                bestfoodSources[k] = foodSources[i*numSolutions+k];
            }    
        }
    }
}

void employedBees(double *foodSources, double *foodsourceNeighbor, int numSolutions){
    for (int i = 0; i < numSolutions; i++){
        for (int k = 0; k < numVariables; k++){
            int randomIndex;
            double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;

            do{ // Calculate a randomIndex until it's different than the selected index
                randomIndex = rand()%(numVariables+1);
            }while(randomIndex == k); 

            double randomfoodSource = randomValue*(foodSources[i*numSolutions+k] - foodSources[i*numSolutions+randomIndex]); 
            // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
            foodsourceNeighbor[i*numSolutions+k] = foodSources[i*numSolutions+k] + randomfoodSource;

            // Here we position our foodsource back in the constraints defined previously for our problem.        
            if(foodsourceNeighbor[i*numSolutions+k] > maxValues[k]){
                foodsourceNeighbor[i*numSolutions+k] = maxValues[k];
            } else {
                if(foodsourceNeighbor[i*numSolutions+k] < minValues[k]){
                    foodsourceNeighbor[i*numSolutions+k] = minValues[k];
                }
            }
        }
        solutionNeighbor[i] = (*optimizationFunction)(&foodsourceNeighbor[i*numSolutions], numFoodSources);
        // I need to properly adjust cpp and hpp in order to navigate my functions
        // Do I need to use &foodsourceNeighbor or is just foodsourceNeighbor fine.
    }

    // Greedy selection process (only for minimization)
    for (int i = 0; i < NUMSOLF; i++){
        if (solutionNeighbor[i] < solutionArray[i]){    
            solutionArray[i] = solutionNeighbor[i];        
            for (int k = 0; k < numVariables; k++){
                foodSources[i*numSolutions+k] = foodsourceNeighbor[i*numSolutions+k];
            }
            numAttempts[i] = 0;
        } else {
            numAttempts[i] = numAttempts[i] + 1;
        }
    }
}

void onlookerBees(double *foodSources, double *foodsourceNeighbor, int numSolutions){
    int index = 0;
    while(numActiveOnlookers < NUMSOLF){
        int randomProb = (double)rand()/((double)(RAND_MAX)); // randomProb -> [0,1]
        // foodSources with higher probability index are more likely to be chosen.
        if(randomProb < probability[index]){ // Runs until every single onlooker has been used.
            for (int k = 0; k < numVariables; k++){
                // Same procedure as applied with the employed bee, used to add bees in the neighborhood.
                int randomIndex;
                double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;
                do{ // Calculate a randomIndex until it's different than the selected index
                randomIndex = rand()%(numVariables+1);
                }while(randomIndex == k); 
                double randomfoodSource = randomValue*(foodSources[index*numSolutions+k] - foodSources[index*numSolutions+k]);
                foodsourceNeighbor[index*numSolutions+k] = foodSources[index*numSolutions+k] + randomfoodSource;
                if(foodsourceNeighbor[index*numSolutions+k] > maxValues[k]){ // Move foodsource back to constraints if needed
                    foodsourceNeighbor[index*numSolutions+k] = maxValues[k];
                }else{
                    if(foodsourceNeighbor[index*numSolutions+k] < minValues[k]){
                        foodsourceNeighbor[index*numSolutions+k] = minValues[k];
                    }
                }
            }
            solutionNeighbor[index] = (*optimizationFunction)(&foodsourceNeighbor[index*numSolutions], numFoodSources);

            // Greedy selection of the foodsources
            if (solutionNeighbor[index] < solutionArray[index]){    
                solutionArray[index] = solutionNeighbor[index];        
                for (int k = 0; k < numVariables; k++){
                    foodSources[index*numSolutions+k] = foodsourceNeighbor[index*numSolutions+k];
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
}

void evaluationPhase(int tamanho){
    double totalFitness = 0.0;
    for (int i = 0; i < tamanho; i++){
        fitness[i] = (solutionArray[i] >= 0.0)?((double)(1/(1+solutionArray[i]))):((double)(1-solutionArray[i]));
        totalFitness = totalFitness + fitness[i];
    }
    for (int i = 0; i < tamanho; i++){
        probability[i] = (fitness[i])/totalFitness;
    }

    // I'm gonna find, first, the biggest and the smallest value in our probability array.
    double minval = probability[0];
    double maxval = probability[0];
    for (int i = 0; i < tamanho; i++){
        if (probability[i] < minval) {
            minval = probability[i];
        }
        else if (probability[i] > maxval) {   
            maxval = probability[i];
        }
    }

    // Here I normalize a probability vector.
    for(int k = 0; k < tamanho; k++){
       probability[k] = (probability[k] - minval)/(maxval-minval);
    }
    #ifdef teste_valores
                printf("fitness[%d] = %lf\n", i, fitness[i]);
    #endif
}
