/*
------------------------------------------------------
| Optimization Benchmarking Algorithms - Header file |
------------------------------------------------------
v1: Original implementation, based on the FEX-testfunctions files inside:
    https://github.com/rodyo/FEX-testfunctions/tree/master.
    There are 27 functions in the current version, and it's implemented without
    using object oriented programming, a future version may depend on classes instead.

The functions in the library are given below in the following format: ($Name)-(#dimensions):($URL)
1 - Ackley function (Many): https://www.sfu.ca/~ssurjano/ackley.html
2 - Sphere function (Many): https://www.sfu.ca/~ssurjano/spheref.html
3 - Beale function (2D): https://www.sfu.ca/~ssurjano/beale.html
4 - Bird function (2D): https://www.indusmic.com/post/bird-function
5 - Booth function (2D): https://www.sfu.ca/~ssurjano/booth.html
6 - Bukin6 function (2D): https://www.sfu.ca/~ssurjano/bukin6.html
7 - Carromtable function (2D): https://search.r-project.org/CRAN/refmans/smoof/html/makeCarromTableFunction.html
8 - Chichinadze function (2D): https://al-roomi.org/benchmarks/unconstrained/2-dimensions/42-chichinadze-s-function
9 - Crossintray function (2D): https://www.sfu.ca/~ssurjano/crossit.html
10 - Crosslegtable function (2D): https://al-roomi.org/benchmarks/unconstrained/2-dimensions/45-cross-leg-table-function
11 - Crownedcross function
12 - Cube function
13 - Easom function
14 - Eggholder function
15 - Giunta function
16 - Goldsteinprice function    
17 - Griewank function
18 - Helicalvalley function
19 - Himmelblau function
20 - Holdertable function
21 - Leon function
22 - Levi13 function
23 - Matyas function
24 - Mccormick function
25 - Modschaffer1 function
26 - Rosenbrock function (Many): https://www.sfu.ca/~ssurjano/rosen.html
27 - Rastrigin function (Many): https://www.sfu.ca/~ssurjano/rastr.html
*/

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
    #define ROSENBROCKMIN -5
    #define ROSENBROCKMAX 10
    #define RASTRIGINMIN -5.12
    #define RASTRIGINMAX 5.12

    /*
    This description applies to every function declared.

    @brief Calculates the value of the function at a point. 
    @param var[]: Array containing the variables of the point to be evaluated.
    @return Value of function at point specified by var[].
    */
    extern double ackleyFunction(double var[]); //Evaluated on hypercube Xi-[-32.768, 32.768].
    extern double sphereFunction(double var[]); //Evaluated on hypercube Xi-[-5.12, 5.12]
    extern double bealeFunction(double var[]); //Evaluated on square Xi-[-4.5, 4.5].
    extern double birdFunction(double var[]); //Evaluated on square Xi-[-2PI, 2PI].
    extern double boothFunction(double var[]); //Evaluated on square Xi-[-10, 10].
    extern double bukin6Function(double var[]); //Evaluated on square X0-[-15, -5], X1-[-3,3].
    extern double carromtableFunction(double var[]); //Evaluated on square Xi-[-10, 10]
    extern double chichinadzeFunction(double var[]); //Evaluated on square Xi-[-30,30]
    extern double crossintrayFunction(double var[]); //Evaluated on square Xi-[-10,10]
    extern double crosslegtableFunction(double var[]); //Evaluated on square Xi-[-10,10]
    extern double rosenbrockFunction(double var[]); //Evaluated on hypercube Xi-[-5, 10]
    extern double rastriginFunction(double var[]); //Evaluated on hypercube Xi-[-5.12,5.12]

#endif
