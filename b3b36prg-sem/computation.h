#ifndef __COMPUTATION_H__
#define __COMPUTATION_H__
#include <stdbool.h>
#include "messages.h"

void add_re(void);
void less_re(void);
void add_im(void);
void less_im(void);
void add_n(void);
void less_n(void);
void clear_buffer(void);
bool set_compute_reference(message *msg);
void zoom_in(void);
void zoom_out(void);
double abs_val(double max, double min);
void move_right(void);
void move_down(void);
void move_left(void);
void move_up(void);
void computation_init(void);
void computation_cleanup(void);
bool is_computing(void);
bool is_done(void);
void abort_computation(void);
bool set_compute(message *msg);
bool compute(message *msg);
void update_data(const msg_compute_data *compute_data);
void get_grid_size(int *w, int *h);
void update_image(int w, int h, unsigned char *img);
void abort_comp(void);
void enable_comp(void);
bool is_abort(void);
#endif
