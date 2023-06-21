#ifndef __COMP_MODUL_COMPUTATION_H__
#define __COMP_MODUL_COMPUTATION_H__
#include "messages.h"

struct comp_modul{
    double c_re; //re (x) part of the c constant in recursive equation
    double c_im; //im (y) part of the c constant in recursive equation
    int n; //number of iterations per pixel
    double d_re; //increment in the x-coords
    double d_im; //increment in the y-coords
    int cid; //chunk id
    double re; //start of the x-coords (real)
    double im; //start of the y-coords (imaginary)
    uint8_t n_re; //number of cells in x-coords
    uint8_t n_im; //number of cells in y-coords    
};

void set_comp_compute(const message *msg);
void update_param_for_pix(const message *msg);
void compute_pixel(uint8_t i_re, uint8_t i_im, message *msg);
double my_sqrt(double x);
#endif
