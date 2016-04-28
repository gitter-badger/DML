#ifndef LR_H_
#define LR_H_

#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <deque>
#include <pthread.h>
#include "load_data.h"

class LR{

public:
    LR(Load_Data* data, int total_num_proc, int my_rank);
    ~LR();
    void run();

private:
    //training data
    Load_Data* data;

    //MPI process info
    int num_proc; // total num of process in MPI comm world
    int rank; // my process rank in MPT comm world

    //iteration step
    size_t step;

    //l1 norm parameter
    double c; //l1 norm parameter

    //parameter
    double* glo_w; //global model parameter
    double* glo_new_w; //model paramter after line search

    //gradient
    double* loc_g; //gradient of loss function compute by data on this process
    double* glo_g; //gradient of loss function compute by data on all process
    double* loc_new_g; //new local gradient 
    double* glo_new_g; //new global gradient 

    //sub gradient
    double* glo_sub_g; //global sub gradient
    //double *next_g;

    //search direction
    double* glo_q; //global search direction

    //two loop
    int m; //number memory data we want in owlqn(lbfgs)
    int now_m; //num of memory data we got now
    double** glo_s_list; //global s list in lbfgs two loop
    double** glo_y_list; //global y list in lbfgs two loop
    double* glo_alpha_list; //global alpha list in lbfgs two loop
    double* glo_ro_list; //global ro list in lbfgs two loop
    
    //loss
    double loc_loss; //local loss
    double glo_loss; //global loss
    double loc_new_loss; //new local loss
    double glo_new_loss; //new global loss

    //line search
    double lambda; //learn rate in line search
    double backoff; //back rate in line search

    void init();
    void owlqn();
    void calculate_gradient();
    void calculate_subgradient();
    void two_loop();
    void line_search();
    double calculate_loss(double *w);
    double sigmoid(double x);
    void fix_dir();
};
#endif // LR_H_
