#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "keyboard.h"
#include "utils.h"
#include "event_queue.h"

struct abortKeyMain abortKeyMainData;

void *keyboard_thread(void *d){
    fprintf(stderr, "keyboard thread starting \n");
    call_termios(0);
    int c;
    event ev;
    
    while((c=getchar())!='q'){
        ev.type = EV_TYPE_NUM;
        switch(c){
            case 'g':
                //get version
                ev.type = EV_GET_VERSION;
                break;
            case 'a':
                //abort
                abortKeyMainData.abortFromKey = 1;
                ev.type = EV_ABORT;
                break;
            case 's':
                //set compute 
                ev.type = EV_SET_COMPUTE;   
                break;
            case 'c':
                //compute
                abortKeyMainData.abortFromKey = 0;
                ev.type = EV_COMPUTE;
                break;
            case '1':                
                abortKeyMainData.abortFromKey = 0;
                ev.type = EV_COMPUTE;
                break;
            case 'h':
                ev.type = EV_PRINT_HELP;
                break;
            case 'l':
                ev.type = EV_CLEAR_BUFFER;
                break;
            case 'e':
                ev.type = EV_ADD_RE;
                break;
            case 'r':
                ev.type = EV_LESS_RE;
                break;
            case 't':
                ev.type = EV_ADD_IM;
                break;
            case 'y':
                ev.type = EV_LESS_IM;
                break;
            case 'm':
                ev.type = EV_ADD_N;
                break;
            case 'n':
                ev.type = EV_LESS_N;
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
    fprintf(stderr, "keyboard thread ending \n");
    return NULL;
}
