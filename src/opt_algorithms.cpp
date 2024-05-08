/*
----------------------------------------
| Optimization Benchmarking Algorithms |
----------------------------------------
v1: Original implementation, based on the FEX-testfunctions files inside:
    https://github.com/rodyo/FEX-testfunctions/tree/master .


In this library there'll be a small implementation of many types of common global optimization 
benchmarking functions of varying complexity and numVar, focusing mostly on single-objective-unconstrained
functions.

I'm passing a const reference (const <datatype> &<variable>) in order to not add unnecessary overhead
into my function calls. Without it, since I'm using an std::vector<>, I'd end up making copies of the
data structure every time it's called.

TODO:
- Note I can change the iteration method used to go through every element inside of my vector into something
more general and intuitive.
- Add support for templates in order to add complex number optimization.
*/

#include <opt_algorithms.hpp>
#include <vector>
#include <cmath>

double ackleyFunction(const std::vector<double> &var, int numVar){ // var[] ou *var
    double sum_varsqr = 0.0;
    double sum_varcos = 0.0;
    // I'm using the suggested values for these variables.
    double a=20;
    double b=0.2;
    double c=2*PI;
    for(int index = 0; index < numVar; index++){
        sum_varsqr = sum_varsqr + pow(var[index],2);
        sum_varcos = sum_varcos + cos(c*var[index]);
    }
    return (-a)*exp((-b)*sqrt((1.0/(double)numVar)*(sum_varsqr)))-exp((1.0/(double)numVar)*sum_varcos)+a+exp(1.0);
}

double sphereFunction(const std::vector<double> &var, int numVar){
    double result = 0.0;
    for(int index = 0; index < numVar; index++){
        result = result + pow(var.at(index),2);
    }
    return result;
}

double rosenbrockFunction(const std::vector<double> &var, int numVar){
    double result = 0;
    for(int w = 0; w < numVar-1; w++){
        result = result + (100.0)*pow((var[w+1]-pow(var[w],2)),2) + pow(var[w]-1,2);
    }
    return result; 
}

double rastriginFunction(const std::vector<double> &var, int numVar){
    double result = (10.0)*((double)(numVar));
    for(int w = 0; w < numVar; w++){
        result = result + pow(var[w],2)-(10.0)*cos(2*PI*var[w]);
    }
    return result;
}

double griewankFunction(const std::vector<double> &var, int numVar){
    double sumresult = 0.0; // Sum Identity
    double productresult = 1.0; // Product Identity
    for(int w = 0; w < numVar; w++){
        sumresult = sumresult + pow(var[w],2)/(4000.0);
        //The 1.0 is present since we are counting starting from 0, which leads to bugs.
        productresult = productresult*cos(var[w]/(sqrt(w+1.0)));
    }
    return sumresult-productresult+1.0;
}

double bealeFunction(const std::vector<double> &var, int numVar){
    int trash = numVar;
    double exp1 = pow((1.5-var[0]+var[0]*var[1]),2);
    double exp2 = pow((2.25-var[0]+var[0]*pow(var[1],2)),2);
    double exp3 = pow((2.625-var[0]+var[0]*pow(var[1],3)),2);
    return exp1+exp2+exp3;
}

double birdFunction(const std::vector<double> &var, int numVar){
    double exp1 = sin(var[0])*exp(pow(1-cos(var[1]),2));
    double exp2 = cos(var[1])*exp(pow(1-sin(var[0]),2));
    double exp3 = pow((var[0]-var[1]),2);
    return exp1+exp2+exp3; 
}

// Is this function well defined?
double boothFunction(const std::vector<double> &var, int numVar){
    double exp1 = pow(var[0]+2*var[1]-7, 2);
    double exp2 = pow(2*var[0]+var[1]-5, 2);
    return exp1+exp2;   
}   

// HARD to optimize
double bukin6Function(const std::vector<double> &var, int numVar){
    return 100.0*sqrt(fabs(var[1]-0.01*pow(var[0],2)))+0.01*fabs(var[0]+10.0);
}

double carromtableFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = exp((2.0)*fabs(1-exp1/PI));
    double exp3 = pow(cos(var[0]),2)*pow(cos(var[1]),2);
    return ((-1.0)/(30.0))*exp3*exp2;
}

double chichinadzeFunction(const std::vector<double> &var, int numVar){
    double exp1 = pow(var[0],2)-12*var[0]+11;
    double exp2 = 10*cos(0.5*PI*var[0])+8*sin(2.5*PI*var[0]);
    double exp3 = (-0.2)*(sqrt(5.0)/(exp(0.5*pow(var[1]-0.5,2))));
    return exp1+exp2+exp3;
}

double crossintrayFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = exp(fabs(100.0-exp1/PI))*sin(var[0])*sin(var[1]);
    return (-0.0001)*pow(fabs(exp2)+1,0.1);
}

// HARD to optimize
double crosslegtableFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = fabs(exp(fabs(100.0-exp1/PI))*sin(var[0])*sin(var[1]));
    return (-1.0)/(pow(exp2+1.0,0.1));
}

double crownedcrossFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2));
    double exp2 = exp(fabs(100.0-exp1/PI))*sin(var[0])*sin(var[1]);
    return (0.0001)*pow(fabs(exp2)+1,0.1);
}

double cubeFunction(const std::vector<double> &var, int numVar){
    double exp1 = 100.0*pow(var[1]-pow(var[0],3),2);
    double exp2 = pow(1-var[0],2);
    return exp1 + exp2;
}

double easomFunction(const std::vector<double> &var, int numVar){
    double exp1 = -pow(var[0]-PI,2)-pow(var[1]-PI,2);
    double exp2 = -cos(var[0])*cos(var[1])*exp(exp1);
    return exp2;
}

double eggholderFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(fabs(var[1]+var[0]/2.0+47.0));
    double exp2 = sqrt(fabs(var[0]-(var[1]+47.0)));
    return -(var[1]+47.0)*sin(exp1)-var[0]*sin(exp2);
}

double giuntaFunction(const std::vector<double> &var, int numVar){
    double result = 0.0;
    for(int w = 0; w < numVar; w++){
        result = result + sin(((16.0)/(15.0))*var[w]-1.0)+pow(sin(((16.0)/(15.0))*var[w]-1.0),2)+((1.0)/(50.0))*sin((4.0)*(((16.0)/(15.0))*var[w]-1));
    }
    return 0.6+result;
}

double goldsteinpriceFunction(const std::vector<double> &var, int numVar){
    double exp1 = 19.0-(14.0)*var[0]+(3.0)*pow(var[0],2)-(14.0)*var[1]+(6.0)*var[0]*var[1]+(3.0)*pow(var[1],2);
    double exp2 = 18.0-(32.0)*var[0]+(12.0)*pow(var[0],2)+(48.0)*var[1]-(36.0)*var[0]*var[1]+(27.0)*pow(var[1],2);
    return (1+pow(var[0]+var[1]+1.0,2)*exp1)*(30+pow(2*var[0]-3*var[1],2)*exp2);
}

double helicalvalleyFunction(const std::vector<double> &var, int numVar){
    double thetafunc = 0.0;
    if((var[0]>0.0)||(var[0]==0.0)){ //Divisao entre 0 é problemático
        thetafunc = ((1.0)/(2.0*PI))*atan(var[1]/var[0]);
    }else{
        thetafunc = ((1.0)/(2.0*PI))*(atan(var[1]/var[0])+PI);
    }
    return (100.0)*(pow(var[2]-(10.0)*thetafunc,2)+pow((sqrt(pow(var[0],2)+pow(var[1],2))-1.0),2))+pow(var[2],2);
}

double himmelblauFunction(const std::vector<double> &var, int numVar){
    double exp1 = pow(pow(var[0],2)+var[1]-11.0,2);
    double exp2 = pow(var[0]+pow(var[1],2)-7.0,2);
    return exp1+exp2;
}

double holdertableFunction(const std::vector<double> &var, int numVar){
    double exp1 = sqrt(pow(var[0],2)+pow(var[1],2))/PI;
    return -fabs(sin(var[0])*cos(var[1])*exp(fabs(1.0-exp1)));
}

double leonFunction(const std::vector<double> &var, int numVar){
    return (100.0)*pow(var[1]-pow(var[0],2),2)+pow(1.0-var[0],2);
}

double levi13Function(const std::vector<double> &var, int numVar){
    double exp1 = pow(var[0]-1.0,2)*(1.0+pow(sin(3.0*PI*var[1]),2));
    double exp2 = pow(var[1]-1.0,2)*(1.0+pow(sin(2.0*PI*var[1]),2));
    return pow(sin(3.0*PI*var[0]),2)+exp1+exp2;
}

double matyasFunction(const std::vector<double> &var, int numVar){
    return (0.26)*(pow(var[0],2)+pow(var[1],2))-(0.48)*var[0]*var[1];
}

double mccormickFunction(const std::vector<double> &var, int numVar){
    return sin(var[0]+var[1])+pow(var[0]-var[1],2)-(1.5)*var[0]+(2.5)*var[1]+1.0;
}

double schafferN1Function(const std::vector<double> &var, int numVar){
    double exp1 = pow(var[0],2)+pow(var[1],2);
    double exp2 = pow(sin(pow(exp1,2)),2)-0.5;
    double exp3 = pow(1.0+(0.001)*(pow(var[0],2)+pow(var[1],2)),2);
    return 0.5+exp2/exp3;
}

// HARD to optimize
double schafferN4Function(const std::vector<double> &var, int numVar){
    double exp1 = sin(abs(pow(var[0],2)-pow(var[1],2)));
    double exp2 = pow(cos(exp1),2)-0.5;
    double exp3 = pow(1.0 + 0.001*(pow(var[0],2)+pow(var[1],2)),2);
    return 0.5 + exp2/exp3;
}

// Aqui estou utilizando as versões complexas destas funções.
std::complex<double> rootsFunction(const std::vector<std::complex<double>> &var, int numVar){
    std::complex<double> exp1 = 1+std::abs(pow(var[0],6)-1.0);
    return 1.0/exp1;
}
