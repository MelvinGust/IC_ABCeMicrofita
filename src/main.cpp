/*
--------------------------------
| Main Program - ABC Algorithm |
--------------------------------
The first three versions were compiled and written following C language standards, allowing it to be run on
microcontrollers / etc. The code worked, however it has shown itself to be very limited and complex for further
applications I wish to develop. Since this is the case, following the V4 of this program we've migrated to C++, and
are slowly trying to gain a better understanding of this language in order to properly optimize the code that
is being written. 

In this version (V5) the following modifications were made:
- Support for complex numbers via std::complex (use function overloading).
- The program stops running only when the solution found is near a specified range (98%-102%)

TODO:
- Support for maximization and minimization problems, swapping slightly the greedy functions to be utilized.
- Based on the probability function results, STOP executing the program. If we ever get minval == maxval,
  we can be sure we've properly optimized our problem.
- Include either structs or classes in order to encapsulate the many arguments my program is using, thus
  reducing the amount of parameters passed to a function.
  Note: solutionArray and bestSolution might belong to the same class.
- Learn to utilize templates to make some variables and functions more general, depending on the context
  it's used. For example, can the context valued functions become GENERAL? I'm using function overloading
  in the current version.

BUGS:
- Version with user-defined CYCLENUMBER is problematic when optimizing problems and the answer is show to be
TOO good, crashing the console.
*/

/*
-------------
| Libraries |
-------------
Here I include the libraries I'll be using for this code.
- <stdlib.h> : Contains many important elements of the C standard library, such as rand() and srand().
- <time.h> : Contains the definition of RAND_MAX
- <vector> : Contains the definition for vector<type> elements.
- <array> : Contains the definition of a safer type of arrays, array<type>.
- <complex> : Contains the definition of complex datatype.
- <fstream> : Contains the function definitions for file manipulationy7
- <opt_algorithms.hpp> : Contains constant definitions and function definitions for the equations I'll
be optimizing.
*/
#include <stdlib.h>
#include <stdio.h> // What does this do?
#include <time.h>
#include <vector> //std::
#include <array> //std::
#include <complex> //std::
#include <fstream> // std:: - Para armazenar dados em arquivos .txt
#include <opt_algorithms.hpp>

// I'd rather not write std::array, std::vector, and std::complex every single time.
using std::array;
using std::vector;
using std::complex;

/*
----------------------------
| ABC Algorithm parameters |
----------------------------
- NUMBEES : The number of bees in our population.
- SCOUTLIMIT : The maximum number of times a scout bee will remain in a single point.
- NUMFORAGING : The maximum number of cycles the program will run for.
*/
#define NUMBEES 50 // Number of bees in my population (NEEDS TO BE DIVISIBLE BY 2)
#define SCOUTLIMIT 10 // Max number of times a scout will remain for some reason, bugs out for lower numbers. 
// NOTE, Scoutlimit impacts a TON the precision of our process, apparently.
#define NUMFORAGING 300 // Maximum cycle number

// #define teste_valores
// #define teste_geral
// #define teste_employed
// #define teste_scout
// #define teste_prob

/*
-------------------------
|Definição de variáveis |
-------------------------
Nesta seção do código defino as variáveis que estarei utilizando no código.
Note-se que várias delas foram definidas pela sua utilidade. 
*/

// Este não é um parâmetro de controle, simplesmente existe para facilitar a programação.
const int numfoodSources = NUMBEES/2; 
#define CONVERGENCIA 0.99L
#define MAXPROB 1.0

// Containers: 
// - array is used for those with size defined at compile time.
// - vector is used for those with size define at run time.
array<int, numfoodSources> numAttempts = {0}; // Fills the container with 0 automatically, doesnt need to be double.
array<double, numfoodSources> fitness;
array<double, numfoodSources> probability;
vector<double> minValues;
vector<double> maxValues;

int numLoop = 0;
int numActiveOnlookers = 0;
int numVariables = 0;
double bestSolution = 0.0;
complex<double> bestComplexSolution = {0.0, 0.0};
bool flagComplex = false;
bool flagMultiDim = false;
bool flagWaitInput = false;
bool stopProgram = false;
double desiredExtrema;

std::ofstream performanceAnalysis ("pythonplotter/ABCperformance.txt"); // Opening my text file for storing data.

// Initial definition of the functions used in the program
double (*optimizationFunction)(const vector<double> &arr, int size); //
complex<double> (*optimizationComplexFunction)(const vector<complex<double>> &arr, int size);
void initUserInput(void);
int findnumVariables(bool flagMultiDim);
void saveBestSolution(vector<double> &bestSolution, const array<vector<double>, numfoodSources> &foodSources, const array<double, numfoodSources> &solutionArray, int numSolutions, int numVariables);
void employedBees(array<vector<double>, numfoodSources> &foodSources, array<vector<double>, numfoodSources> &foodsourceNeighbor, array<double, numfoodSources> &solutionArray, array<double, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables);
void onlookerBees(array<vector<double>, numfoodSources> &foodSources, array<vector<double>, numfoodSources> &foodsourceNeighbor, array<double, numfoodSources> &solutionArray, array<double, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables);
void evaluationPhase(const array<double, numfoodSources> &solutionArray, int size);
void saveBestSolution(vector<complex<double>> &bestSolution, const array<vector<complex<double>>, numfoodSources> &foodSources, const array<complex<double>, numfoodSources> &solutionArray, int numSolutions, int numVariables);
void employedBees(array<vector<complex<double>>, numfoodSources> &foodSources, array<vector<complex<double>>, numfoodSources> &foodsourceNeighbor, array<complex<double>, numfoodSources> &solutionArray, array<complex<double>, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables);
void onlookerBees(array<vector<complex<double>>, numfoodSources> &foodSources, array<vector<complex<double>>, numfoodSources> &foodsourceNeighbor, array<complex<double>, numfoodSources> &solutionArray, array<complex<double>, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables);
void evaluationPhase(const array<complex<double>, numfoodSources> &solutionArray, int size);

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
        { // To limit the scope of the variables I've defined inside of this code.
            array<complex<double>, numfoodSources> solutionNeighbor;
            array<complex<double>, numfoodSources> solutionArray; 
            vector<complex<double>> bestfoodSources;
            array<vector<complex<double>>, numfoodSources> foodsourceNeighbor;       
            array<vector<complex<double>>, numfoodSources> foodSources;

            /* ----- Initialize ----- */
            for (int i = 0; i < numfoodSources; i++){
                for (int k = 0; k < numVariables; k++){
                    foodSources[i].push_back(complex<double>(((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k], ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k]));
                    foodsourceNeighbor[i].push_back(complex<double>(0.0, 0.0)); 
                    bestfoodSources.push_back(complex<double>(0.0, 0.0)); // Remember to initialize vectors.
                }
                solutionArray[i] = (*optimizationComplexFunction)(foodSources[i], numfoodSources);
                #ifdef teste_valores
                    printf("solutionArray[%lf]\n", solutionArray[i]);
                #endif
            }

            // Define best solution and its parameters.
            bestComplexSolution = solutionArray[0];
            for (const auto& fsource_itr : foodSources[0]){  // It's a range for loop, copies value by reference.
                bestfoodSources.push_back(fsource_itr);
                #ifdef teste_valores
                    printf("Salvei em bestfoodSources: (%lf, %lf)\n", std::real(fsource_itr), std::imag(fsource_itr));
                #endif
            }
            /* ----- End of initialization ----- */

            /* ------------------------------------------ ENTER LOOP ----------------------------------------- */
            while(numLoop < NUMFORAGING){
                numLoop++; // Increment numLoop every cycle.
                #ifdef teste_geral
                    printf("Estamos no loop num: %d\n", numLoop);
                #endif
                saveBestSolution(bestfoodSources, foodSources, solutionArray, numfoodSources, numVariables); // CHECK
                #ifdef teste_geral
                    printf("Entrando em employedBees\n");
                #endif
                employedBees(foodSources, foodsourceNeighbor, solutionArray, solutionNeighbor, numfoodSources, numVariables); // CHECK
                #ifdef teste_geral
                    printf("Entrando em evaluationPhase\n");
                #endif
                evaluationPhase(solutionNeighbor, numfoodSources); // CHECK
                #ifdef teste_geral
                    printf("Entrando em onlookerBees\n");
                #endif
                onlookerBees(foodSources, foodsourceNeighbor, solutionArray, solutionNeighbor, numfoodSources, numVariables); // CHECK 
                saveBestSolution(bestfoodSources, foodSources, solutionArray, numfoodSources, numVariables);        
                // If the best solution found is good enough, stop looping.
                /*
                if((bestSolution  > CONVERGENCIA*desiredExtrema)&&(bestSolution < desiredExtrema + (1-CONVERGENCIA)*desiredExtrema)){
                    stopProgram = true;
                }
                */

                /* ------- Scout Bee Phase ------- */
                for(int i = 0; i < numfoodSources; i++){
                    if( numAttempts[i] >= SCOUTLIMIT ){
                        for (int k = 0; k < numVariables; k++){
                        foodSources[i].push_back(complex<double>(((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k], ((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k]));
                        }
                        solutionArray[i] = (*optimizationComplexFunction)(foodSources[i], numfoodSources);
                        numAttempts[i] = 0;
                    }
                } 

                if(performanceAnalysis.is_open()){
                    performanceAnalysis << numLoop << " " << abs(bestComplexSolution) << "\n";
                }
                /* ------- End of scout bee phase ------- */
            }
            performanceAnalysis.close();
            /* ---------------------------------------- OUT OF LOOP ------------------------------------------ */
            printf("The best solution found is %lf+i%lf\n", real(bestComplexSolution), imag(bestComplexSolution));
            for (int k = 0; k < numVariables; k++){
                printf("%lf+i%lf\t", real(bestfoodSources[k]), imag(bestfoodSources[k]));
            }
            printf("\nIt took %d cycles", numLoop);
            break;
        }
        case false:
        {
            // Here we define the vectors we'll use for this program. Their size is numVariables.
            // Please note I need to initialize everything properly, in order to not have problems later on
            // with dimensioning.
            array<double, numfoodSources> solutionNeighbor; // Talvez precise mudar par std::complex
            array<double, numfoodSources> solutionArray; // Talvez precise fazer ele std::complex
            vector<double> bestfoodSources;
            array<vector<double>, numfoodSources> foodsourceNeighbor;       
            array<vector<double>, numfoodSources> foodSources; // Seems unnecesary, might change later.

            /* ----- Initialize ----- */
            for (int i = 0; i < numfoodSources; i++){
                for (int k = 0; k < numVariables; k++){ // This determines the 2D-ness of foodSources.
                    foodSources.at(i).push_back(((double)rand()/((double)(RAND_MAX)))*(maxValues[k] - minValues[k]) + minValues[k]);
                    #ifdef teste_valores
                        printf("foodSources[%d][%d] = %lf\t", i, k, foodSources[i].at(k));
                    #endif
                    foodsourceNeighbor[i].push_back(0.0); // Since I'm using vectors I need to initialize them
                    bestfoodSources.push_back(0.0); // Remember to initialize vectors.
                }
                solutionArray.at(i) = (*optimizationFunction)(foodSources[i], numVariables);
                #ifdef teste_valores
                    printf("\nsolutionArray[%d] = %lf\n", i, solutionArray[i]);
                #endif
            }

            // Define best solution and its parameters.
            bestSolution = solutionArray[0];
            for (const auto& fsource_itr : foodSources[0]){  // It's a range for loop, copies value by reference.
                bestfoodSources.push_back(fsource_itr);
                #ifdef teste_valores
                    printf("Salvei em bestfoodSources: %lf\n", fsource_itr);
                #endif
            }
            /* ----- End of initialization ----- */

            /* ------------------------------------------ ENTER LOOP ----------------------------------------- */
            while(numLoop < NUMFORAGING){
                numLoop++; // Increment numLoop every cycle.
                #ifdef teste_scout
                    for(int i = 0; i < numfoodSources; i++){
                        printf("1: numAttempts[%d] = %d\t", i, numAttempts[i]);
                    }
                    printf("\n\n");
                #endif
                #ifdef teste_geral
                    printf("Estamos no loop num: %d\n", numLoop);
                #endif
                saveBestSolution(bestfoodSources, foodSources, solutionArray, numfoodSources, numVariables); // CHECK
                #ifdef teste_geral
                    printf("Entrando em employedBees\n");
                #endif
                employedBees(foodSources, foodsourceNeighbor, solutionArray, solutionNeighbor, numfoodSources, numVariables); // CHECK
                #ifdef teste_geral
                    printf("Entrando em evaluationPhase\n");
                #endif
                #ifdef teste_scout
                    for(int i = 0; i < numfoodSources; i++){
                        printf("2: numAttempts[%d] = %d\t", i, numAttempts[i]);
                    }
                    printf("\n\n");
                #endif
                evaluationPhase(solutionNeighbor, numfoodSources); // CHECK
                #ifdef teste_geral
                    printf("Entrando em onlookerBees\n");
                #endif
                onlookerBees(foodSources, foodsourceNeighbor, solutionArray, solutionNeighbor, numfoodSources, numVariables); // CHECK 
                saveBestSolution(bestfoodSources, foodSources, solutionArray, numfoodSources, numVariables);     
                #ifdef teste_geral
                    printf("Entrando em scoutBees\n");
                #endif   
                // If the best solution found is good enough, stop looping.
                /*
                if((bestSolution  > CONVERGENCIA*desiredExtrema)&&(bestSolution < desiredExtrema + (1-CONVERGENCIA)*desiredExtrema)){
                    stopProgram = true;
                }
                */
                #ifdef teste_scout
                    for(int i = 0; i < numfoodSources; i++){
                        printf("3: numAttempts[%d] = %d\t", i, numAttempts[i]);
                    }
                    printf("\n\n");
                #endif

                /* ------- Scout Bee Phase ------- */
                for(int i = 0; i < numfoodSources; i++){
                    if( numAttempts.at(i) > 30 ){
                        #ifdef teste_geral
                        printf("Entrando em scoutBeePhase\n");
                        #endif
                        for (int k = 0; k < numVariables; k++){
                            foodSources.at(i).at(k) = ((double)rand()/((double)(RAND_MAX)))*(maxValues.at(k) - minValues.at(k) + minValues.at(k));
                        }
                        solutionArray.at(i) = (*optimizationFunction)(foodSources[i], numVariables);
                        numAttempts.at(i) = 0;
                    }
                }
                /* ------- End of scout bee phase ------- */

                if(performanceAnalysis.is_open()){
                    performanceAnalysis << numLoop << " " << bestSolution << "\n";
                }
            }
            performanceAnalysis.close();
            /* ---------------------------------------- OUT OF LOOP ------------------------------------------ */
            printf("The best solution found is %lf\n", bestSolution);
            for (int k = 0; k < numVariables; k++){
                printf("%lf\t", bestfoodSources[k]);
            }
            printf("\nIt took %d cycles", numLoop);
            break;
        }
    }

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
            printf("You chose the Modified Schaffer N4 Function. min=0.292579 at (0, +-1.253132) and (+-1.253132, 0)  \n");
            flagComplex = false;
            flagMultiDim = false;
            optimizationFunction = &schafferN4Function;
            numVariables = findnumVariables(flagMultiDim);
            for(int i = 0; i < numVariables; i++){minValues.push_back(SCHAFFERN4MIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(SCHAFFERN4MAX);}
            break;
        case 12:
            printf("You chose the Roots Function. max=abs(f(z))=1 at the sixth roots of unity. \n");
            flagComplex = true;
            flagMultiDim = false;
            optimizationComplexFunction = &rootsFunction;
            numVariables = 1; //I've got how many variables, again? This might cause problems.
            for(int i = 0; i < numVariables; i++){minValues.push_back(ROOTSMIN);}
            for(int i = 0; i < numVariables; i++){maxValues.push_back(ROOTSMAX);}
            break;
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

/* 
INPUT:
The other arrays that are modified are global to the program.

- &bestfoodSources : Passed by reference. Since it has no const keyword, it can be
  modified inside of the function.
- const &foodSources: Passed by reference with const keyword, can't be modified
  inside of the function.
- numSolutions
- numVariables
*/
void saveBestSolution(vector<double> &bestfoodSources, const array<vector<double>, numfoodSources> &foodSources, const array<double, numfoodSources> &solutionArray, int numSolutions, int numVariables){
    for (int i = 0; i < numSolutions; i++){
        if (solutionArray.at(i) < bestSolution ){ 
            bestSolution = solutionArray.at(i);        
            for (int k = 0; k < numVariables; k++){
                bestfoodSources[k] = foodSources[i].at(k);
            }    
        }
    }
}

void employedBees(array<vector<double>, numfoodSources> &foodSources, array<vector<double>, numfoodSources> &foodsourceNeighbor, array<double, numfoodSources> &solutionArray, array<double, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables){
    for (int i = 0; i < numSolutions; i++){
        for (int k = 0; k < numVariables; k++){
            #ifdef teste_employed
                printf("Consigo accesar vetores aqui, accesando [%d][%d] que tem %lf de foodSources\n", i, k, foodSources[i][k]);
            #endif
            int randomIndex;
            double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;

            do{ // Calculate a randomIndex until it's different than the selected index (TROQUEI numVariables por numSolutions)
                randomIndex = rand()%(numSolutions); // Vai de [0 : numSolutions-1]
            }while(randomIndex == i); 

            #ifdef teste_employed
                printf("O index aleatorio calculado is %d\n", randomIndex);
                printf("O valor aleatorio calculado is %lf\n", randomValue);
            #endif
            double randomfoodSource = randomValue*(foodSources[i].at(k) - foodSources[randomIndex].at(k)); 
            #ifdef teste_employed
                printf("Nao tive problemas para determinar o randomfoodSource, obtendo: %lf \n", randomfoodSource);
            #endif
            // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
            foodsourceNeighbor[i].at(k) = foodSources[i].at(k) + randomfoodSource;
            #ifdef teste_employed
                printf("Estou tendo problemas para modificar foodsourceNeighbor");
            #endif

            // Here we position our foodsource back in the constraints defined previously for our problem.        
            if(foodsourceNeighbor[i].at(k) > maxValues[k]){
                foodsourceNeighbor[i].at(k) = maxValues[k];
            } else {
                if(foodsourceNeighbor[i].at(k) < minValues[k]){
                    foodsourceNeighbor[i].at(k) = minValues[k];
                }
            }
        }
        solutionNeighbor[i] = (*optimizationFunction)(foodsourceNeighbor[i], numVariables);
    }

    #ifdef teste_employed
        printf("Comecamos o Greedy Selection Process.");
    #endif

    // Greedy selection process (only for minimization)
    for (int i = 0; i < numfoodSources; i++){
        if (solutionNeighbor[i] < solutionArray[i]){    
            solutionArray[i] = solutionNeighbor[i];        
            for (int k = 0; k < numVariables; k++){
                foodSources[i].at(k) = foodsourceNeighbor[i].at(k);
            }
            numAttempts[i] = 0;
        } else {
            numAttempts[i] = numAttempts[i] + 1;
        }
    }
}

void onlookerBees(array<vector<double>, numfoodSources> &foodSources, array<vector<double>, numfoodSources> &foodsourceNeighbor, array<double, numfoodSources> &solutionArray, array<double, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables){
    int index = 0;
    while(numActiveOnlookers < numSolutions){
        double randomProb = (double)rand()/((double)(RAND_MAX)); // randomProb -> [0,1]
        // foodSources with higher probability index are more likely to be chosen.
        #ifdef teste_prob
            printf("%lf\t", probability[index]);
        #endif
        if(randomProb < probability[index]){ // Runs until every single onlooker has been used.
            for (int k = 0; k < numVariables; k++){
                // Same procedure as applied with the employed bee, used to add bees in the neighborhood.
                int randomIndex;
                double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;
                do{ // Calculate a randomIndex until it's different than the selected index
                randomIndex = rand()%(numSolutions);
                }while(randomIndex == index); 
                double randomfoodSource = randomValue*(foodSources[index].at(k) - foodSources[randomIndex].at(k));
                foodsourceNeighbor[index].at(k) = foodSources[index].at(k) + randomfoodSource;
                if(foodsourceNeighbor[index].at(k) > maxValues[k]){ // Move foodsource back to constraints if needed
                    foodsourceNeighbor[index].at(k) = maxValues[k];
                }else{
                    if(foodsourceNeighbor[index].at(k) < minValues[k]){
                        foodsourceNeighbor[index].at(k) = minValues[k];
                    }
                }
            }
            solutionNeighbor[index] = (*optimizationFunction)(foodsourceNeighbor[index], numVariables);

            // Greedy selection of the foodsources
            if (solutionNeighbor[index] < solutionArray[index]){    
                solutionArray[index] = solutionNeighbor[index];        
                for (int k = 0; k < numVariables; k++){
                    foodSources[index].at(k) = foodsourceNeighbor[index].at(k);
                }
                numAttempts[index] = 0;
            } else {
                numAttempts[index] = numAttempts[index] + 1;
            }
            numActiveOnlookers++;
        }
        index++;
        if(index==numfoodSources){index=0;}
    }
    numActiveOnlookers = 0;
}

void evaluationPhase(const array<double, numfoodSources> &solutionArray, int size){
    double totalFitness = 0.0;
    for (int i = 0; i < size; i++){
        fitness[i] = (solutionArray[i] >= 0.0)?((double)(1/(1+solutionArray[i]))):((double)(1-solutionArray[i]));
        totalFitness = totalFitness + fitness[i];
        #ifdef teste_valores
                printf("fitness[%d] = %lf\n", i, fitness[i]);
        #endif
    }
    for (int i = 0; i < size; i++){
        probability[i] = (fitness[i])/totalFitness;
    }

    // I'm gonna find, first, the biggest and the smallest value in our probability array.
    double minval = probability[0];
    double maxval = probability[0];
    for (int i = 0; i < size; i++){
        if (probability[i] < minval) {
            minval = probability[i];
        }
        else if (probability[i] > maxval) {   
            maxval = probability[i];
        }
    }

    // Here I normalize a probability vector.
    for(int k = 0; k < size; k++){
        if(minval == maxval){
            probability[k] = MAXPROB;
            // TODO: Stop program since we've reached the best solution.
        } else {
            probability[k] = (probability[k] - minval)/(maxval-minval);
        }
    }
}

/* --------------------------------------------- COMPLEX --------------------------------------------- */
// These functions actually maximize the result from the complex calculation, looking for the sup(f(z)).
void saveBestSolution(vector<complex<double>> &bestfoodSources, const array<vector<complex<double>>, numfoodSources> &foodSources, const array<complex<double>, numfoodSources> &solutionArray, int numSolutions, int numVariables){
    for (int i = 0; i < numSolutions; i++){
        if (std::abs(solutionArray[i]) > std::abs(bestSolution)){ //Maximixação
            bestComplexSolution = solutionArray[i];        
            for (int k = 0; k < numVariables; k++){
                bestfoodSources[k] = foodSources[i].at(k);
            }    
        }
    }
}

void employedBees(array<vector<complex<double>>, numfoodSources> &foodSources, array<vector<complex<double>>, numfoodSources> &foodsourceNeighbor, array<complex<double>, numfoodSources> &solutionArray, array<complex<double>, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables){
    for (int i = 0; i < numSolutions; i++){
        for (int k = 0; k < numVariables; k++){
            #ifdef teste_employed
                printf("Consigo accesar vetores aqui, accesando [%d][%d] que tem %lf de foodSources\n", i, k, foodSources[i][k]);
            #endif
            int randomIndex;
            double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;

            do{ // Calculate a randomIndex until it's different than the selected index
                randomIndex = rand()%(numSolutions); // Vai de [0 : numSolutions-1]
            }while(randomIndex == i); 
            #ifdef teste_employed
                printf("O index aleatorio calculado is %d\n", randomIndex);
                printf("O valor aleatorio calculado is %lf\n", randomValue);
            #endif
            complex<double> randomfoodSource = {randomValue*(std::real(foodSources[i].at(k)) - std::real(foodSources[randomIndex].at(k))) , randomValue*(std::imag(foodSources[i].at(k)) - std::imag(foodSources[randomIndex].at(k)))}; 
            #ifdef teste_employed
                printf("Nao tive problemas para determinar o randomfoodSource, obtendo: %lf + i%lf \n", real(randomfoodSource), imag(randomfoodSource));
            #endif
            // Note that as the difference between foodSources becomes smaller, so does the perturbation of the foodSource.
            foodsourceNeighbor[i].at(k) = foodSources[i].at(k) + randomfoodSource;

            // Constraints for real(z)      
            if(real(foodsourceNeighbor[i].at(k)) > maxValues[k]){
                foodsourceNeighbor[i].at(k).real(maxValues[k]);
            } else {
                if(real(foodsourceNeighbor[i].at(k)) < minValues[k]){
                    foodsourceNeighbor[i].at(k).real(minValues[k]);
                }
            }

            // Constraints for imag(z)
            if(imag(foodsourceNeighbor[i].at(k)) > maxValues[k]){
                foodsourceNeighbor[i].at(k).imag(maxValues[k]);
            } else {
                if(imag(foodsourceNeighbor[i].at(k)) < minValues[k]){
                    foodsourceNeighbor[i].at(k).imag(minValues[k]);
                }
            }
        }
        solutionNeighbor[i] = (*optimizationComplexFunction)(foodsourceNeighbor[i], numVariables);
    }

    #ifdef teste_employed
        printf("Comecamos o Greedy Selection Process.");
    #endif

    // Greedy selection process (only for maximization of complex numbers)
    for (int i = 0; i < numfoodSources; i++){
        if (abs(solutionNeighbor[i]) > abs(solutionArray[i])){    
            solutionArray[i] = solutionNeighbor[i];        
            for (int k = 0; k < numVariables; k++){
                foodSources[i].at(k) = foodsourceNeighbor[i].at(k);
            }
            numAttempts[i] = 0;
        } else {
            numAttempts[i] = numAttempts[i] + 1;
        }
    }
}

void onlookerBees(array<vector<complex<double>>, numfoodSources> &foodSources, array<vector<complex<double>>, numfoodSources> &foodsourceNeighbor, array<complex<double>, numfoodSources> &solutionArray, array<complex<double>, numfoodSources> &solutionNeighbor, int numSolutions, int numVariables){
    int index = 0;
    while(numActiveOnlookers < numSolutions){
        int randomProb = (double)rand()/((double)(RAND_MAX)); // randomProb -> [0,1]
        // foodSources with higher probability index are more likely to be chosen.
        if(randomProb < probability[index]){ // Runs until every single onlooker has been used.
            for (int k = 0; k < numVariables; k++){
                // Same procedure as applied with the employed bee, used to add bees in the neighborhood.
                int randomIndex;
                double randomValue = (2.0)*((double)rand()/((double)(RAND_MAX)))-1.0;
                do{ // Calculate a randomIndex until it's different than the selected index
                randomIndex = rand()%(numSolutions);
                }while(randomIndex == index); 
                complex<double> randomfoodSource = {randomValue*(std::real(foodSources[index].at(k)) - std::real(foodSources[randomIndex].at(k))) , randomValue*(std::imag(foodSources[index].at(k)) - std::imag(foodSources[randomIndex].at(k))) }; 
                foodsourceNeighbor[index].at(k) = foodSources[index].at(k) + randomfoodSource;
                if(real(foodsourceNeighbor[index].at(k)) > maxValues[k]){
                        foodsourceNeighbor[index].at(k).real(maxValues[k]);
                } else {
                    if(real(foodsourceNeighbor[index].at(k)) < minValues[k]){
                            foodsourceNeighbor[index].at(k).real(minValues[k]);
                    }
                }

                    // Constraints for imag(z)
                if(imag(foodsourceNeighbor[index].at(k)) > maxValues[k]){
                    foodsourceNeighbor[index].at(k).imag(maxValues[k]);
                } else {
                    if(imag(foodsourceNeighbor[index].at(k)) < minValues[k]){
                       foodsourceNeighbor[index].at(k).imag(minValues[k]);
                    }
                }
            }
            solutionNeighbor[index] = (*optimizationComplexFunction)(foodsourceNeighbor[index], numVariables);
            // Greedy selection of the foodsources
            if (abs(solutionNeighbor[index]) > abs(solutionArray[index])){    
                solutionArray[index] = solutionNeighbor[index];        
                for (int k = 0; k < numVariables; k++){
                    foodSources[index].at(k) = foodsourceNeighbor[index].at(k);
                }
                numAttempts[index] = 0;
            } else {
                numAttempts[index] = numAttempts[index] + 1;
            }
            numActiveOnlookers++;
        } 
        index++;
        if(index==numfoodSources){index=0;}
    }
    numActiveOnlookers = 0;
}

void evaluationPhase(const array<complex<double>, numfoodSources> &solutionArray, int size){
    double totalFitness = 0.0;
    for (int i = 0; i < size; i++){
        fitness[i] = (1+abs(solutionArray[i])); //Meio trivial, né?
        totalFitness = totalFitness + fitness[i];
        #ifdef teste_valores
            printf("fitness[%d] = %lf\n", i, fitness[i]);
        #endif
    }
    // Separated since I need totalFitness to have been calculated before.
    for (int i = 0; i < size; i++){
        probability[i] = (fitness[i])/totalFitness;
    }

    // I'm gonna find, first, the biggest and the smallest value in our probability array.
    double minval = probability[0];
    double maxval = probability[0];
    for (int i = 0; i < size; i++){
        if (probability[i] < minval) {
            minval = probability[i];
        }
        else if (probability[i] > maxval) {   
            maxval = probability[i];
        }
    }

    // Here I normalize a probability vector.
    for(int k = 0; k < size; k++){
        if(maxval == minval){
            probability[k] = MAXPROB;
        } else {
            probability[k] = (probability[k] - minval)/(maxval-minval);
        }
    }
}