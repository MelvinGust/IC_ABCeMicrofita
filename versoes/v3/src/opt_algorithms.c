/*
----------------------------------------
| Optimization Benchmarking Algorithms |
----------------------------------------
v1: Original implementation, based on the FEX-testfunctions files inside:
    https://github.com/rodyo/FEX-testfunctions/tree/master .


In this library there'll be a small implementation of many types of common global optimization 
benchmarking functions of varying complexity and size, focusing mostly on single-objective-unconstrained
functions.
*/

#include "opt_algorithms.h"
#include <math.h>

double ackleyFunction(double var[]){
    double sum_varsqr, sum_varcos;
    // I'm using the suggested values for these variables.
    double a=20;
    double b=0.2;
    double c=2*PI;
    int size = sizeof(var)/sizeof(var[0]);
    for(int w = 0; w < size; w++){
        sum_varsqr = pow(var[w],2);
        sum_varcos = cos(c*var[w]);
    }
    return (-a)*exp((-b)*sqrt((1.0/(double)size)*(sum_varsqr)))-exp((1.0/(double)size)*sum_varcos)+a+exp(1.0);
}

double sphereFunction(double var[]){
    double result = 0.0;
    int size = sizeof(var)/sizeof(var[0]);
    for(int w = 0; w < size; w++){
        result = result + pow(var[w],2);
    }
    return result;
}

double bealeFunction(double var[]){
    double exp1 = pow((1.5-var[0]+var[0]*var[1]),2);
    double exp2 = pow((2.25-var[0]+var[0]*pow(var[1],2)),2);
    double exp3 = pow((2.625-var[0]+var[0]*pow(var[1],3)),2);
    return exp1+exp2+exp3;
}

double birdFunction(double var[]){
    double exp1 = sin(var[0])*exp(pow(1-cos(var[1]),2));
    double exp2 = cos(var[1])*exp(pow(1-sin(var[0]),2));
    double exp3 = pow((var[0]-var[1]),2);
    return exp1+exp2+exp3; 
}

double boothFunction(double var[]){
    double exp1 = pow(var[0]+2*var[1]-7,2);
    double exp2 = pow(2*var[0]+var[1]-5,2);
    return exp1+exp2;   
}

double bukin6Function(double var[]){
    return 100*sqrt(fabs(var[1]-0.01*pow(var[0],2)))+0.01*fabs(var[0]+10);
}

double carromtableFunction(double var[]){
    double exp1 = pow(pow(var[0],2)+pow(var[1],2),0.5);
    double exp2 = exp(fabs(1-pow(exp1/PI,2)));
    return ((-1.0)/(30.0))*(cos(var[0])*exp2);
}

double chichinadzeFunction(double var[]){
    double exp1 = pow(var[0],2)-12*var[0]+11;
    double exp2 = 10*cos(0.5*PI*var[0])+8*sin(2.5*PI*var[0]);
    double exp3 = (-0.2)*(sqrt(5.0)/(exp(0.5*pow(var[1]-0.5,2))));
    return exp1+exp2+exp3;
}

double crossintrayFunction(double var[]){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = exp(fabs(100.0-exp1/PI))*sin(var[0])*sin(var[1]);
    return (-0.0001)*pow(exp2+1,0.1);
}

double crosslegtableFunction(double var[]){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = fabs(exp(fabs(100.0-exp1/PI))*sin(var[0])*sin(var[1]));
    return (-1.0)/(pow(exp2+1.0,0.1));
}

double rosenbrockFunction(double var[]){
    int size = sizeof(var)/sizeof(var[0]);
    double result = 0;
    for(int w = 0; w < size-1; w++){
        result = result + (100.0)*pow((var[w+1]-pow(var[w],2)),2) + pow(var[w]-1,2);
    }
    return result; 
}

double rastriginFunction(double var[]){
    int size = sizeof(var)/sizeof(var[0]);
    double result = (10.0)*((double)(size));
    for(int w = 0; w < size; w++){
        result = result + pow(var[w],2)-(10.0)*cos(2*PI*var[w]);
    }
    return result;
}
