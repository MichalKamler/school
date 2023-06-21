#include <stdio.h>
#include "computation.h"
#include "utils.h"

struct{
    double computation_real;
    double computation_imaginary;
    int n;
    double range_real_min;
    double range_real_max;
    double range_imaginary_min;
    double range_imaginary_max;
    int grid_w;
    int grid_h;
    int cur_x;
    int cur_y;
    double density_real;
    double density_imaginary;
    int number_of_chunks;
    int chunk_ID;
    double chunk_real;
    double chunk_imaginary;
    uint8_t chunk_n_real;
    uint8_t chunk_n_imaginary;
    uint8_t *grid;
    bool computing;
    bool done;
    bool abort;
}comp = {
    .computation_real = -0.4,
    .computation_imaginary = 0.6,
    .n = 60,
    .range_real_min = -1.6,
    .range_real_max = 1.6,
    .range_imaginary_min = -1.1,
    .range_imaginary_max = 1.1,
    .grid = NULL,
    .grid_w = 640,
    .grid_h = 480,
    .chunk_n_real = 64,
    .chunk_n_imaginary = 48,
    .computing = false,
    .done = false,
    .abort = false
};

void add_re(void){
    comp.computation_real += 0.1;
    printf("Complex plain is now: (%f) + (%f)i\n", comp.computation_real, comp.computation_imaginary);
}
void less_re(void){
    comp.computation_real -= 0.05;
    printf("Complex plain is now: (%f) + (%f)i\n", comp.computation_real, comp.computation_imaginary);
}
void add_im(void){
    comp.computation_imaginary += 0.1;
    printf("Complex plain is now: (%f) + (%f)i\n", comp.computation_real, comp.computation_imaginary);
}
void less_im(void){
    comp.computation_imaginary -= 0.05;
    printf("Complex plain is now: (%f) + (%f)i\n", comp.computation_real, comp.computation_imaginary);
}
void add_n(void){
    comp.n += 5;
    printf("n is %d\n", comp.n);
}
void less_n(void){
    comp.n -= 5;
    printf("n is %d\n", comp.n);
}

void clear_buffer(void){
    for(unsigned int i=0; i<comp.grid_h*comp.grid_w; i++){
        comp.grid[i]=0;
    }
}
bool set_compute_reference(message *msg){
    comp.computation_real = -0.4;
    comp.computation_imaginary = 0.6;
    comp.n = 60;
    comp.range_real_min = -1.6;
    comp.range_real_max = 1.6;
    comp.range_imaginary_min = -1.1;
    comp.range_imaginary_max = 1.1;
    comp.grid_w = 640;
    comp.grid_h = 480;
    comp.chunk_n_real = 64;
    comp.chunk_n_imaginary = 48;
    comp.computing = false;
    comp.done = false;
    comp.abort = false;
    comp.density_real = (comp.range_real_max - comp.range_real_min)/(1.*comp.grid_w);
    comp.density_imaginary = -(comp.range_imaginary_max - comp.range_imaginary_min)/(1.*comp.grid_h);
    comp.number_of_chunks = (comp.grid_w *comp.grid_h)/(comp.chunk_n_real * comp.chunk_n_imaginary);
    my_assert(msg!=NULL, __func__, __LINE__, __FILE__);
    bool ret = !is_computing();
    if(ret){
        msg->type = MSG_SET_COMPUTE;        
        msg->data.set_compute.c_re = comp.computation_real;
        msg->data.set_compute.c_im = comp.computation_imaginary;
        msg->data.set_compute.n = comp.n; 
        msg->data.set_compute.d_re = comp.density_real;
        msg->data.set_compute.d_im = comp.density_imaginary;        
        comp.done = false;
    }
    return ret;
}
void zoom_in(void){
    double move = abs_val(comp.range_real_max, comp.range_real_min)/10;
    comp.range_real_max -= move;
    comp.range_real_min += move;
    move = abs_val(comp.range_imaginary_max, comp.range_imaginary_min)/10;
    comp.range_imaginary_max -= move;
    comp.range_imaginary_min += move;
    comp.density_real = (comp.range_real_max - comp.range_real_min)/(1.*comp.grid_w);
    comp.density_imaginary = -(comp.range_imaginary_max - comp.range_imaginary_min)/(1.*comp.grid_h);
    comp.number_of_chunks = (comp.grid_w *comp.grid_h)/(comp.chunk_n_real * comp.chunk_n_imaginary);
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
void zoom_out(void){
    double move = abs_val(comp.range_real_max, comp.range_real_min)/10;
    comp.range_real_max += move;
    comp.range_real_min -= move;
    move = abs_val(comp.range_imaginary_max, comp.range_imaginary_min)/10;
    comp.range_imaginary_max += move;
    comp.range_imaginary_min -= move;
    comp.density_real = (comp.range_real_max - comp.range_real_min)/(1.*comp.grid_w);
    comp.density_imaginary = -(comp.range_imaginary_max - comp.range_imaginary_min)/(1.*comp.grid_h);
    comp.number_of_chunks = (comp.grid_w *comp.grid_h)/(comp.chunk_n_real * comp.chunk_n_imaginary);
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
double abs_val(double max, double min){
    double ret = 0;
    if(max>0){
        ret+=max;
    }else{
        ret-=max;
    }
    if(min>0){
        ret+=min;
    }else{
        ret-=min;
    }
    return ret;
}
void move_right(void){
    double move = abs_val(comp.range_real_max, comp.range_real_min)/10;
    comp.range_real_max += move;
    comp.range_real_min += move;
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
void move_down(void){
    double move = abs_val(comp.range_imaginary_max, comp.range_imaginary_min)/10;
    comp.range_imaginary_max -= move;
    comp.range_imaginary_min -= move;
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
void move_left(void){
    double move = abs_val(comp.range_real_max, comp.range_real_min)/10;
    comp.range_real_max -= move;
    comp.range_real_min -= move;
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
void move_up(void){
    double move = abs_val(comp.range_imaginary_max, comp.range_imaginary_min)/10;
    comp.range_imaginary_max += move;
    comp.range_imaginary_min += move;
    printf("Plain will be drawn between points: A = (%f) + (%f)i and B = (%f) + (%f)i\n", 
        comp.range_real_min, comp.range_imaginary_max, comp.range_real_max, comp.range_imaginary_min);
}
void computation_init(void){
    comp.grid = my_alloc(comp.grid_w*comp.grid_h);
    comp.density_real = (comp.range_real_max - comp.range_real_min)/(1.*comp.grid_w);
    comp.density_imaginary = -(comp.range_imaginary_max - comp.range_imaginary_min)/(1.*comp.grid_h);
    comp.number_of_chunks = (comp.grid_w *comp.grid_h)/(comp.chunk_n_real * comp.chunk_n_imaginary);
}
void computation_cleanup(void){
    if(comp.grid){
        free(comp.grid);
    }
    comp.grid = NULL;
}
bool is_computing(void){
    return comp.computing;
}
bool is_done(void){
    return comp.done;
}
void abort_computation(void){
    comp.computing = false;
}
void get_grid_size(int *w, int *h){
    *w = comp.grid_w;
    *h = comp.grid_h;
}
bool set_compute(message *msg){
    my_assert(msg!=NULL, __func__, __LINE__, __FILE__);
    bool ret = !is_computing();
    if(ret){
        msg->type = MSG_SET_COMPUTE;        
        msg->data.set_compute.c_re = comp.computation_real;
        msg->data.set_compute.c_im = comp.computation_imaginary;
        msg->data.set_compute.n = comp.n; 
        msg->data.set_compute.d_re = comp.density_real;
        msg->data.set_compute.d_im = comp.density_imaginary;        
        comp.done = false;
    }
    return ret;
}
bool compute(message *msg){
    my_assert(msg!=NULL, __func__, __LINE__, __FILE__);
    if(!is_computing()){
        comp.chunk_ID = 0;
        comp.computing = true;
        comp.cur_x = comp.cur_y = 0;
        comp.chunk_real = comp.range_real_min;
        comp.chunk_imaginary = comp.range_imaginary_max;
        msg->type = MSG_COMPUTE;
    }else{
        comp.chunk_ID += 1;
        if(comp.chunk_ID<comp.number_of_chunks){
            comp.cur_x += comp.chunk_n_real;
            comp.chunk_real += comp.chunk_n_real*comp.density_real;
            if(comp.cur_x>=comp.grid_w){
                comp.cur_x = 0;
                comp.chunk_real = comp.range_real_min;
                comp.chunk_imaginary += comp.chunk_n_imaginary*comp.density_imaginary;
                comp.cur_y += comp.chunk_n_imaginary;
            }
            msg->type = MSG_COMPUTE;
        }else{

        }
    }
    if(comp.computing && msg->type == MSG_COMPUTE){
        msg->data.compute.cid = comp.chunk_ID;
        msg->data.compute.re = comp.chunk_real;
        msg->data.compute.im = comp.chunk_imaginary;
        msg->data.compute.n_re = comp.chunk_n_real;
        msg->data.compute.n_im = comp.chunk_n_imaginary;
    }
    return is_computing();
}
void update_data(const msg_compute_data *compute_data){
    my_assert(compute_data!=NULL, __func__, __LINE__, __FILE__);
    if(compute_data->cid==comp.chunk_ID){
        const int idx = comp.cur_x + compute_data->i_re + (comp.cur_y + compute_data->i_im)*comp.grid_w;
        if(idx>=0 && idx<(comp.grid_w*comp.grid_h)){
            comp.grid[idx] = compute_data->iter;         
        }
        if((comp.chunk_ID+1)>=comp.number_of_chunks && (compute_data->i_re+1)==comp.chunk_n_real && (compute_data->i_im+1)==comp.chunk_n_imaginary){
            comp.done = true;
            comp.computing = false;
        }
    }else{
        warn("Recieved chunk with unexpected chunk id");         
    }
}
void update_image(int w, int h, unsigned char *img){
    my_assert(img && comp.grid && w==comp.grid_w && h==comp.grid_h, __func__, __LINE__, __FILE__);
    for(unsigned int i = 0; i<w*h; i++){
        const double t = 1. * comp.grid[i]/(comp.n +1.0);
        *(img++) = 9*(1-t)*t*t*t * 255;
        *(img++) = 15*(1-t)*(1-t)*t*t * 255;
        *(img++) = 8.5*(1-t)*(1-t)*(1-t)*t * 255;
    }
}
void abort_comp(void){
    comp.abort = true;
}
void enable_comp(void){
    comp.abort = false;
    comp.done = false;
}
bool is_abort(void){
    return comp.abort;
}

