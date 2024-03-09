/*
------------------------------------------------------
| Optimization Benchmarking Algorithms - Header file |
------------------------------------------------------
v1: Original implementation, based on the FEX-testfunctions files inside:
    https://github.com/rodyo/FEX-testfunctions/tree/master.
    There are 27 functions in the current version, and it's implemented without
    using object oriented programming, a future version may depend on classes instead.

The functions in the library are given below in the following format: ($Name)-(#dimensions):($URL)
1 - (*) Ackley function (Many): https://www.sfu.ca/~ssurjano/ackley.html
2 - Sphere function (Many): https://www.sfu.ca/~ssurjano/spheref.html
3 - (!) Rosenbrock function (Many): https://www.sfu.ca/~ssurjano/rosen.html
4 - (!) Rastrigin function (Many): https://www.sfu.ca/~ssurjano/rastr.html
5 - (!) Griewank function (Many) : https://www.sfu.ca/~ssurjano/griewank.html
6 - (!) Helicalvalley function (3D) : http://galton.uchicago.edu/~lekheng/courses/302/classics/fletcher-powell.pdf
7 - Beale function (2D): https://www.sfu.ca/~ssurjano/beale.html
8 - Bird function (2D): https://www.indusmic.com/post/bird-function
9 - Booth function (2D): https://www.sfu.ca/~ssurjano/booth.html
10 - (!)(!) Bukin6 function (2D): https://www.sfu.ca/~ssurjano/bukin6.html
11 - Carromtable function (2D): https://search.r-project.org/CRAN/refmans/smoof/html/makeCarromTableFunction.html
12 - Chichinadze function (2D): https://al-roomi.org/benchmarks/unconstrained/2-dimensions/42-chichinadze-s-function
13 - Crossintray function (2D): https://www.sfu.ca/~ssurjano/crossit.html
14 - (!) Crosslegtable function (2D): https://al-roomi.org/benchmarks/unconstrained/2-dimensions/45-cross-leg-table-function
15 - (!) Crownedcross function (2D) : https://mpra.ub.uni-muenchen.de/2718/1/MPRA_paper_2718.pdf
16 - (!) Cube function (2D) : https://arxiv.org/pdf/1308.4008.pdf
17 - Easom function (2D) : https://arxiv.org/pdf/1308.4008.pdf
18 - Eggholder function (2D) : https://www.sfu.ca/~ssurjano/egg.html
19 - Giunta function (2D) : https://arxiv.org/pdf/1308.4008.pdf
20 - Goldsteinprice function (2D) : https://www.sfu.ca/~ssurjano/goldpr.html   
21 - Himmelblau function (2D) : https://en.wikipedia.org/wiki/Himmelblau%27s_function
22 - Holdertable function (2D) : https://www.sfu.ca/~ssurjano/holder.html
23 - Leon function (2D) : https://arxiv.org/pdf/1308.4008.pdf
24 - Levi13 function (2D) : https://www.sfu.ca/~ssurjano/levy13.html
25 - Matyas function (2D) : https://www.sfu.ca/~ssurjano/matya.html
26 - Mccormick function (2D) : https://www.sfu.ca/~ssurjano/mccorm.html
27 - (!) Schaffer No.1 Function (2D) : https://mpra.ub.uni-muenchen.de/2718/1/MPRA_paper_2718.pdf
28 - (!) (*) Schaffer No.4 Function (2D): https://www.sfu.ca/~ssurjano/schaffer4.html
29 - (*) Root function (1D-Complex): An artificial immune network for multimodal function optimization IEEE

(*) Important functions
(!) Difficulty markers. The more, the harder the problem is.
Some functions can be found in various research papers, such as:
(i) Some new test functions for global optimization and performance of repulsive particle swarm method.
    https://mpra.ub.uni-muenchen.de/2718/1/MPRA_paper_2718.pdf, 23 of August 2006
(ii)
*/
#include <vector>
#include <complex>

#ifndef HEADEROPTALGORITHM
    #define HEADEROPTALGORITHM

    #define PI 3.14159265

    /*
    Constant definitions for every function's domain.
    */
    #define ACKLEYMIN -32.768
    #define ACKLEYMAX 32.768
    #define SPHEREMIN -5.12
    #define SPHEREMAX 5.12
    #define ROSENBROCKMIN -5
    #define ROSENBROCKMAX 10
    #define RASTRIGINMIN -5.12
    #define RASTRIGINMAX 5.12
    #define GRIEWANKMIN -600
    #define GRIEWANKMAX 600
    #define BEALEMIN -4.5
    #define BEALEMAX 4.5
    #define BIRDMIN -2*PI
    #define BIRDMAX 2*PI
    #define BOOTHMIN -10
    #define BOOTHMAX 10
    #define BUKIN6XMIN -15
    #define BUKIN6XMAX -5
    #define BUKIN6YMIN -3
    #define BUKIN6YMAX 3
    #define CARROMTABLEMIN -10
    #define CARROMTABLEMAX 10
    #define CHICHINADZEMIN -30
    #define CHICHINADZEMAX 30
    #define CROSSINTRAYMIN -10
    #define CROSSINTRAYMAX 10
    #define CROSSLEGTABLEMIN -10
    #define CROSSLEGTABLEMAX 10
    #define CROWNEDCROSSMIN -10
    #define CROWNEDCROSSMAX 10
    #define CUBEMIN -10
    #define CUBEMAX 10
    #define EASOMMIN -100
    #define EASOMMAX 100
    #define EGGHOLDERMIN -512
    #define EGGHOLDERMAX 512
    #define GIUNTAMIN -1
    #define GIUNTAMAX 1
    #define GOLDSTEINPRICEMIN -2
    #define GOLDSTEINPRICEMAX 2
    #define HELICALVALLEYMIN -10
    #define HELICALVALLEYMAX 10
    #define HIMMELBLAUMIN -5
    #define HIMMELBLAUMAX 5
    #define HOLDERTABLEMIN -10
    #define HOLDERTABLEMAX 10
    #define LEONMIN -1.2
    #define LEONMAX 1.2
    #define LEVI13MIN -10
    #define LEVI13MAX 10
    #define MATYASMIN -10
    #define MATYASMAX 10
    #define MCCORMICKXMIN -1.5
    #define MCCORMICKXMAX 4
    #define MCCORMICKYMIN -3
    #define MCCORMICKYMAX 4
    #define SCHAFFERN1MIN -100
    #define SCHAFFERN1MAX 100
    #define SCHAFFERN4MIN -100
    #define SCHAFFERN4MAX 100
    #define ROOTSMIN -2
    #define ROOTSMAX 2

    /*
    Definitions for extrema of each function
    */
   #define ACKLEYEXTREMA 0.0
   #define SPHEREEXTREMA 0.0
   #define BEALEEXTREMA 0.0
   #define BIRDEXTREMA -106.764537
   #define BOOTHEXTREMA 0.0
   #define BUKIN6EXTREMA 0.0
   #define CARROMTABLEEXTREMA -24.1568
   #define CHICHINADZEEXTREMA -42.944387
   #define CROSSINTRAYEXTREMA -2.06261
   #define CROSSLEGTABLEEXTREMA -1.0
   #define ROOTEXTREMA 
   #define SCHAFFERN4EXTREMA 

    /*
    This description applies to every function declared.

    @brief Calculates the value of the function at a point. 
    @param var[]: Array containing the variables of the point to be evaluated.
    @return Value of function at point specified by var[].
    */
    extern double ackleyFunction(const std::vector<double> &var, int numVar); //Evaluated on hypercube Xi-[-32.768, 32.768].
    extern double sphereFunction(const std::vector<double> &var, int numVar); //Evaluated on hypercube Xi-[-5.12, 5.12]
    extern double rosenbrockFunction(const std::vector<double> &var, int numVar); //Evaluated on hypercube Xi-[-5, 10]
    extern double rastriginFunction(const std::vector<double> &var, int numVar); //Evaluated on hypercube Xi-[-5.12,5.12]
    extern double griewankFunction(const std::vector<double> &var, int numVar); //Evaluated on hypercube Xi-[-600,600]
    extern double bealeFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-4.5, 4.5].
    extern double birdFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-2PI, 2PI].
    extern double boothFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-10, 10].
    extern double bukin6Function(const std::vector<double> &var, int numVar); //Evaluated on rectangle X0-[-15, -5], X1-[-3,3].
    extern double carromtableFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-10, 10]
    extern double chichinadzeFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-30,30]
    extern double crossintrayFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-10,10]
    extern double crosslegtableFunction(const std::vector<double> &var, int numVar); //Evaluated on square Xi-[-10,10]
    extern double crownedcrossFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double cubeFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double easomFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double eggholderFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double giuntaFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double goldsteinpriceFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[
    extern double helicalvalleyFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-10,10]
    extern double himmelblauFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-5,5]
    extern double holdertableFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-10,10]
    extern double leonFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-1.2,1.2]
    extern double levi13Function(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-10,10]
    extern double matyasFunction(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-10,10]
    extern double mccormickFunction(const std::vector<double> &var, int numVar); // Evaluated on rectangle X0-[-1.5,4] e X1-[-3,4]
    extern double schafferN1Function(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-100,100]
    extern double schafferN4Function(const std::vector<double> &var, int numVar); // Evaluated on square Xi-[-100,100]

    extern std::complex<double> rootsFunction(const std::vector<std::complex<double>> &var, int numVar);
    // extern double schafferFunction(const std::vector<double> &var, int numVar);

#endif
