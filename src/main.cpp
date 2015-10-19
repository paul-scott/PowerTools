//
//  main.cpp
//  MP2DAT
//
//  Created by Hassan Hijazi on 14/01/13.
//  Copyright (c) 2013 NICTA. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include "PowerTools++/Solver.h"

using namespace std;

//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time(){
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq)){
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)){
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time(){
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
        (double)(d.dwLowDateTime |
                 ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }else{
        //  Handle error
        return 0;
    }
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif

int main (int argc, const char * argv[])
{
    //  Start Timers
    double wall0 = get_wall_time();
//    double cpu0  = get_cpu_time();
    SolverType st = ipopt;
    Model model;
    Solver solver(&model, st);

    meta_var x("x", &model);
    meta_var y("y", &model);
    meta_constant a("a", &model);
    meta_constant b("b", &model);
    meta_Constraint cons;
    
//    cons = a*x*y - ((2*y)^2)*b + cos(x);
    cons = log(x*y) + x*expo(y);
    cons >= 0;
    cons.print();
    model.addMetaConstraint(cons);
    model.init_functions(1);
//    model.print_functions();
    
    /* First derivatives */
    cons.compute_dfdx(&x);
    cout << "dfdx : ";
    cons._dfdx[x.get_idx()]->print(true);
    cons.compute_dfdx(&y);
    cout << "dfdy : ";
    cons._dfdx[y.get_idx()]->print(true);
    
    /* Second derivatives */
    cons._dfdx[x.get_idx()]->compute_dfdx(&y);
    cout << "dfdxdy : ";
    cons._dfdx[x.get_idx()]->_dfdx[y.get_idx()]->print(true);
    cons._dfdx[y.get_idx()]->compute_dfdx(&y);
    cout << "dfdydx : ";
    cons._dfdx[y.get_idx()]->_dfdx[x.get_idx()]->print(true);
    
    
    
    
    /* Optimisation model based on my function */
//    var<> x1 ("x", -1, 1);
//    model.addVar(x1);
//    var<> y1 ("y", 0, 1);
//    model.addVar(y1);
//    x = x1;
//    y = y1;
//    a = 2;
//    b = 4.2;
//    model.concretise(cons, 0, "cons1");
//    
//    Function* obj = new Function();
//    *obj += x1;
//    model.setObjective(obj);
//    model.setObjectiveType(minimize);
//    
//    solver.run(0, false);
    
    
    //  Stop timers
    double wall1 = get_wall_time();
//    double cpu1  = get_cpu_time();
    cout << "Computation total time = " << wall1 - wall0 << endl;
    
    return 0;

}
