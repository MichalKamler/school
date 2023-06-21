#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "prg_io_nonblock.h"
#include "utils.h"
#include "event_queue.h"
#include "comp_modul_computation.h"

#define MAJOR 1 //modify this to change the current version
#define MINOR 0
#define PATCH 1

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 100
#endif

void *comp_keyboard_thread(void *d);
void *read_pipe_thread(void *d);
void *comp_thread(void *d);
static void process_pipe_message(event *const ev);
void send_message(int pipe_out, message msg, uint8_t *msg_buf);
void send_ok();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct{
    uint8_t n_re;
    uint8_t n_im;
    uint8_t i_re;
    uint8_t i_im;
}forIterationOfCid = {
    .n_re = 64,
    .n_im = 48,
    .i_re = 0,
    .i_im = 0,
};

int main(int argc, char *argv[]){
    int ret = EXIT_SUCCESS;
    const char *fname_pipe_out = argc>1 ? argv[1] : "/tmp/computational_module.out";
    const char *fname_pipe_in = argc>2 ? argv[2] : "/tmp/computational_module.in";
    int pipe_in = io_open_read(fname_pipe_in);
    int pipe_out = io_open_write(fname_pipe_out);

    my_assert(pipe_in!=-1 && pipe_out!=-1, __func__, __LINE__, __FILE__);

    enum {KEYBOARD_THREAD, READ_PIPE_THREAD, COMP_THREAD, NUM_THREADS};
    const char *thrd_names[] = {"Keyboard", "ReadPipe", "Comp"};
    void *(*thrd_functions[])(void*) = {comp_keyboard_thread, read_pipe_thread, comp_thread};
    pthread_t threads[NUM_THREADS];
    void *thrd_data[NUM_THREADS] = {};
    thrd_data[READ_PIPE_THREAD] = &pipe_in;
    thrd_data[COMP_THREAD] = &pipe_out;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int r = pthread_create(&threads[i], NULL, thrd_functions[i], thrd_data[i]);
        printf("Create thread '%s' %s\r\n", thrd_names[i], ( r == 0 ? "OK" : "FAIL") );
    }

    int *ex;
    for (int i = 0; i < NUM_THREADS; ++i) {
        //printf("Call join to the thread %s\r\n", thrd_names[i]);
        int r = pthread_join(threads[i], (void*)&ex);
        printf("Joining the thread %s has been %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }

    io_close(pipe_in);
    io_close(pipe_out);
    return ret;
}

void *read_pipe_thread(void *d){
    my_assert(d!=NULL, __func__, __LINE__, __FILE__);
    info("The read_pipe_thread starting");
    int pipe_in = *(int*)d;
    bool end = false;
    uint8_t msg_buf[sizeof(message)];
    int i = 0;
    int len = 0;
    unsigned char c;
    while(io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c)>0){};//discarding garbage
    while(!end){
        int r = io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c);
        if(r>0){//char read
            if(i==0){
                len = 0;
                if(get_message_size(c, &len)){
                    msg_buf[i++] = c;
                }else{
                    fprintf(stderr, "ERROR: message type\n");
                }
            }else{
                msg_buf[i++] = c;
            }
            if(len>0 && i==len){
                message *msg = my_alloc(sizeof(message));
                if(parse_message_buf(msg_buf, len, msg)){
                    event ev = {.type = EV_PIPE_IN_MESSAGE};
                    ev.data.msg = msg;
                    queue_push(ev);
                }else{
                    fprintf(stderr,"ERROR: cannot parse message type %d\n", msg_buf[0]);
                    free(msg);                    
                }                
                i = len = 0;
            }
        }else if(r==0){//timeout

        }else{//error
            fprintf(stderr, "ERROR: reading pipe\n");
            set_quit();
            event ev = {.type = EV_QUIT};
            queue_push(ev);
        }


        end = is_quit();
    }
    info("The read_pipe_thread ended successfully!");
    return NULL;
}

void *comp_keyboard_thread(void *d){
    info("The comp_keyboard_thread starting");    
    call_termios(0);
    int c;
    event ev;
    while((c=getchar())!='q'){
        ev.type = EV_TYPE_NUM;
        switch (c){
        case 'a':
            ev.type = EV_ABORT;
            break;        
        default:
            break;
        }
        if(ev.type!=EV_TYPE_NUM){
            queue_push(ev);
        }
    }
    set_quit();
    ev.type = EV_QUIT;
    queue_push(ev);
    call_termios(1);
    info("The comp_keyboard_thread ended succesfully!");
    return NULL;
}

void *comp_thread(void *d){
    my_assert(d!=NULL, __func__, __LINE__, __FILE__);
    info("The comp_thread starting");
    int pipe_out = *(int*)d;
    message msg;
    uint8_t msg_buf[sizeof(message)];
    bool quit = false;
    event ev = {.type = EV_START_UP};
    queue_push(ev);
    do{
        event ev = queue_pop();
        msg.type = MSG_NBR;
        switch (ev.type){
            case EV_START_UP:
                msg = (message){ .type = MSG_STARTUP, .data.startup.message = { 'K', 'A', 'M', 'L', 'E', 'M', 'I', 'C'} };
                sleep(1);
                break;
            case EV_SEND_OK:
                msg.type = MSG_OK;
                break;    
            case EV_QUIT:
                debug("Quit received in comp_thread");
                break;
            case EV_ABORT:
                msg.type = MSG_ABORT;
                break;
            case EV_SEND_VERSION:
                msg.type = MSG_VERSION;
                msg.data.version.major = MAJOR;                
                msg.data.version.minor = MINOR;
                msg.data.version.patch = PATCH;      
                break;
            case EV_COMPUTE:
                if(forIterationOfCid.i_re < forIterationOfCid.n_re){
                    if(forIterationOfCid.i_im < forIterationOfCid.n_im){
                        compute_pixel(forIterationOfCid.i_re, forIterationOfCid.i_im++, &msg);
                        send_message(pipe_out, msg, msg_buf);
                        
                    }else{
                        forIterationOfCid.i_im = 0;
                        forIterationOfCid.i_re += 1;
                    }
                    ev.type = EV_COMPUTE;            
                    queue_push(ev);
                }else{
                    msg.type = MSG_DONE;
                }                
                break;
            case EV_PIPE_IN_MESSAGE:
                process_pipe_message(&ev);
                break;                       
            default:
                break;
        }
        send_message(pipe_out, msg, msg_buf);
        quit = is_quit();
    }while(!quit);
    info("The comp_thread ended successfully!");
    return NULL;
}

void send_message(int pipe_out, message msg, uint8_t *msg_buf){
    int msg_len;
    if(msg.type!=MSG_NBR){
        my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
        write(pipe_out, msg_buf, msg_len);
        // if(write(pipe_out, msg_buf, msg_len)==msg_len){
        //     debug("send data to pipe_out");
        // }else{
        //     error("send message fail!");
        // }
    }
}

static void process_pipe_message(event *const ev){
    my_assert(ev!=NULL && ev->type==EV_PIPE_IN_MESSAGE && ev->data.msg, __func__, __LINE__, __FILE__);
    ev->type = EV_TYPE_NUM;
    const message *msg = ev->data.msg;
    switch(msg->type){
        case MSG_OK:
            info("OK");
            break;
        case MSG_SET_COMPUTE:
            event ev_set_comp = {.type = EV_SEND_OK};
            queue_push(ev_set_comp);
            set_comp_compute(msg);
            break;
        case MSG_GET_VERSION:
            info("msg get version recieved");
            event ev_version  = {.type = EV_SEND_VERSION};
            queue_push(ev_version );
            break;
        case MSG_COMPUTE:
            forIterationOfCid.n_re = msg->data.compute.n_re;
            forIterationOfCid.n_im = msg->data.compute.n_im;
            forIterationOfCid.i_re = 0;
            forIterationOfCid.i_im = 0;
            update_param_for_pix(msg);
            event ev_comp = {.type = EV_SEND_OK};
            queue_push(ev_comp);
            ev_comp.type = EV_COMPUTE;            
            queue_push(ev_comp);
            break;
        case MSG_ABORT:
            event ev_abort = {.type = EV_ABORT};
            queue_push(ev_abort);
            info("Computation aborted");
            break;
        default:
            fprintf(stderr, "Not implemented type of message %d\n", msg->type);
            break;
    }
    free(ev->data.msg);
    ev->data.msg = NULL;
}
