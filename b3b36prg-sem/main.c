#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "utils.h"
#include "main.h"
#include "messages.h"
#include "event_queue.h"
#include "computation.h"
#include "gui.h"


static void process_pipe_message(event *const ev);

void *main_thread(void *d){
    my_assert(d!=NULL, __func__, __LINE__, __FILE__);
    int pipe_out = *(int*)d;
    message msg;
    uint8_t msg_buf[sizeof(message)];
    int msg_len;
    bool quit = false;
    computation_init();
    gui_init();
    //initialize computation, visualization
    do{
        event ev = queue_pop();
        msg.type = MSG_NBR;    
        switch (ev.type){
            case EV_QUIT:
                set_quit();
                debug("Quit received");
                break;
            case EV_GET_VERSION:
                msg.type = MSG_GET_VERSION;
                break;        
            case EV_SET_COMPUTE:
                info(set_compute(&msg) ? "set compute" : "fail to set compute");
                break;
            case EV_COMPUTE:
                if(abortKeyMainData.abortFromKey==1){
                    break;
                }
                // if(is_computing){

                // }
                enable_comp();
                info(compute(&msg) ? "compute" : "fail to compute");                
                break;
            case EV_ABORT:
                msg.type = MSG_ABORT;
                break;
            case EV_PIPE_IN_MESSAGE:
                process_pipe_message(&ev);
                break;
            case EV_MOVE_RIGHT:
                move_right();
                break;
            case EV_MOVE_DOWN:
                move_down();
                break;
            case EV_MOVE_LEFT:
                move_left();
                break;
            case EV_MOVE_UP:
                move_up();
                break;
            case EV_ZOOM_IN:
                zoom_in();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);
                break;
            case EV_ZOOM_OUT:
                zoom_out();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);
                break;
            case EV_PRINT_HELP:
                FILE *fptr;
                fptr = fopen("to_print.txt", "r");
                if(fptr==NULL){
                    warn("cannot open file to_print.txt");
                    break;
                }
                char c=fgetc(fptr);
                while(c!=EOF){
                    printf("%c", c);
                    c=fgetc(fptr);
                }
                fclose(fptr);
                break;
            case EV_COMPUTE_REFERENCE:
                if(!is_computing()){                
                    info(set_compute_reference(&msg) ? "set compute" : "fail to set compute");                
                    ev.type = EV_COMPUTE;
                    queue_push(ev);
                }
                break;
            case EV_CLEAR_BUFFER:                
                clear_buffer();
                gui_refresh();
                break;
            case EV_ADD_RE:
                add_re();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);                
                break;
            case EV_LESS_RE:
                less_re();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);                
                break;
            case EV_ADD_IM:
                add_im();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);                
                break;
            case EV_LESS_IM:                
                less_im();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);
                break;
            case EV_ADD_N:
                add_n();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);
                break;
            case EV_LESS_N:
                less_n();
                ev.type = EV_SET_COMPUTE;
                queue_push(ev);                
                break;
            default:
                break;
        }
        if(msg.type!=MSG_NBR){
            my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
            if(write(pipe_out, msg_buf, msg_len)==msg_len){
                //debug("send data to pipe_out");
            }else{
                error("send message fail!");
            }
        }
        quit = is_quit();
    }while(!quit);
    gui_cleanup();
    computation_cleanup();
    //cleanup computation, visualization

    return NULL;
}

static void process_pipe_message(event *const ev){
    my_assert(ev!=NULL && ev->type==EV_PIPE_IN_MESSAGE && ev->data.msg, __func__, __LINE__, __FILE__);
    ev->type = EV_TYPE_NUM;
    const message *msg = ev->data.msg;
    switch(msg->type){
        case MSG_STARTUP:
            printf("%s\n", msg->data.startup.message);
            break;
        case MSG_OK:
            info("OK!");
            break;
        case MSG_VERSION:
            fprintf(stderr, "INFO: Module version %d.%d-p%d\n", msg->data.version.major, msg->data.version.minor, msg->data.version.patch);
            break;
        case MSG_COMPUTE_DATA:
            //debug("msgcoputedata recieved");
            if(!is_abort()){
                update_data(&(msg->data.compute_data));
            }    
            break;
        case MSG_DONE:
            gui_refresh();
            if(is_done()){
                info("Computation done");
            }else{                
                event ev = {.type = EV_COMPUTE};
                queue_push(ev);
            }
            break;
        case MSG_ABORT:
            info("Computation aborted");
            abort_computation();
            break;
        default:
            fprintf(stderr, "Not implemented type of message %d\n", msg->type);
            break;
    }
    free(ev->data.msg);
    ev->data.msg = NULL;
}

