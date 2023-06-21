#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "main.h"
#include "keyboard.h"
#include "event_queue.h"
#include "utils.h"
#include "prg_io_nonblock.h"
#include "gui.h"

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 100
#endif

void *read_pipe_thread(void *d);

int main(int argc, char *argv[]){

    int ret = EXIT_SUCCESS;
    const char *fname_pipe_in = argc>1 ? argv[1] : "/tmp/computational_module.out";
    const char *fname_pipe_out = argc>2 ? argv[2] : "/tmp/computational_module.in";
    int pipe_in = io_open_read(fname_pipe_in);
    int pipe_out = io_open_write(fname_pipe_out);

    my_assert(pipe_in!=-1 && pipe_out!=-1, __func__, __LINE__, __FILE__);

    enum {KEYBOARD_THREAD, READ_PIPE_THREAD, MAIN_THREAD, WIN_THRD, NUM_THREADS};
    const char *thrd_names[] = {"Keyboard", "ReadPipe", "Main", "GuiWin"};
    void *(*thrd_functions[])(void*) = {keyboard_thread, read_pipe_thread, main_thread, gui_win_thread};
    pthread_t threads[NUM_THREADS];
    void *thrd_data[NUM_THREADS] = {};
    thrd_data[READ_PIPE_THREAD] = &pipe_in;
    thrd_data[MAIN_THREAD] = &pipe_out;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int r = pthread_create(&threads[i], NULL, thrd_functions[i], thrd_data[i]);
        printf("Create thread '%s' %s\r\n", thrd_names[i], ( r == 0 ? "OK" : "FAIL") );
    }

    int *ex;
    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Call join to the thread %s\r\n", thrd_names[i]);
        int r = pthread_join(threads[i], (void*)&ex);
        printf("Joining the thread %s has been %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }

    io_close(pipe_in);
    io_close(pipe_out);

    return ret;
}

void *read_pipe_thread(void *d){
    my_assert(d!=NULL, __func__, __LINE__, __FILE__);
    int pipe_in = *(int*)d;
    fprintf(stderr, "read_pipe_thread starting \n");
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
    fprintf(stderr, "read_pipe_thread ending \n");
    return NULL;
}


