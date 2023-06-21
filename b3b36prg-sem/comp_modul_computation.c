#include "comp_modul_computation.h"
#include "utils.h"
#include "messages.h"
#include <stdio.h>

struct comp_modul comp_modul = {
    .c_re = -0.4,
    .c_im = 0.6,
    .n = 60,
    .d_re = 0.005,
    .d_im = -0.004583
};

void set_comp_compute(const message *msg){
    comp_modul.c_re=msg->data.set_compute.c_re;
    comp_modul.c_im=msg->data.set_compute.c_im;
    comp_modul.d_re=msg->data.set_compute.d_re;
    comp_modul.d_im=msg->data.set_compute.d_im;
    comp_modul.n=msg->data.set_compute.n;
}
void update_param_for_pix(const message *msg){
    comp_modul.cid = msg->data.compute.cid;
    comp_modul.re = msg->data.compute.re;
    comp_modul.im = msg->data.compute.im;
    comp_modul.n_re = msg->data.compute.n_re;
    comp_modul.n_im = msg->data.compute.n_im;
}

void compute_pixel(uint8_t i_re, uint8_t i_im, message *msg) {
    msg->type = MSG_COMPUTE_DATA;
    msg->data.compute_data.cid = comp_modul.cid;
    msg->data.compute_data.i_re = i_re;
    msg->data.compute_data.i_im = i_im;    

    double z_re = comp_modul.re + i_re * comp_modul.d_re;
    double z_im = comp_modul.im + i_im * comp_modul.d_im;

    uint8_t iter = 0;
    double abs_z = 0.0;
    while (iter < comp_modul.n && abs_z <= 2.0) {
        double new_z_re = z_re * z_re - z_im * z_im + comp_modul.c_re;
        double new_z_im = 2.0 * z_re * z_im + comp_modul.c_im;
        z_re = new_z_re;
        z_im = new_z_im;
        abs_z = my_sqrt(z_re * z_re + z_im * z_im);
        iter++;
    }
    //(abs_z <= 2.0) ? printf("0") : printf("%d\n", iter);
    msg->data.compute_data.iter = (abs_z <= 2.0) ? 0 : iter;
}

double my_sqrt(double x) {
    if (x < 0) {
        return 0.0;
    }

    double result = x;
    double precision = 1e-6; // Adjust the precision value as needed
    double diff;

    while (1) {
        double guess = 0.5 * (result + x / result);
        diff = guess - result;
        // Check if the difference is within the desired precision
        if (diff < 0) {
            diff = -diff;
        }
        if (diff <= precision) {
            break;
        }

        result = guess;
    }

    return result;
}

