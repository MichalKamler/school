#include "gui.h"
#include "xwin_sdl.h"
#include "utils.h"
#include "computation.h"
#include "event_queue.h"
#include <SDL.h>

#ifndef SDL_EVENT_POLL_WAIT_MS
#define SDL_EVENT_POLL_WAIT_MS 10
#endif

static struct{
    int w;
    int h;
    unsigned char *img;    
} gui = {.img = NULL};

void gui_cleanup(void){
    if(gui.img){
        free(gui.img);
        gui.img=NULL;
    }
    xwin_close();
}

void gui_init(void){
    get_grid_size(&gui.w, &gui.h);
    gui.img = my_alloc(gui.w * gui.h * 3);
    my_assert(xwin_init(gui.w, gui.h)==0, __func__, __LINE__, __FILE__);
}

void gui_refresh(void){
    if(gui.img){
        update_image(gui.w, gui.h, gui.img);
        xwin_redraw(gui.w, gui.h, gui.img);
    }
}

void *gui_win_thread(void *d){
    info("gui_win_thread - start");
    bool quit = false;
    SDL_Event event;
    struct event ev;
    while(!quit){
        ev.type = EV_TYPE_NUM;
        if(SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                case SDLK_q:
                    ev.type = EV_QUIT;
                    break;
                case SDLK_s:
                    ev.type = EV_SET_COMPUTE;
                    break;
                case SDLK_g:
                    ev.type = EV_GET_VERSION;
                    break;
                case SDLK_a:
                    abortKeyMainData.abortFromKey = 1;
                    ev.type = EV_ABORT;
                    break;
                case SDLK_c:
                    abortKeyMainData.abortFromKey = 0;
                    ev.type = EV_COMPUTE_REFERENCE;
                    break;
                case SDLK_1:
                    abortKeyMainData.abortFromKey = 0;
                    ev.type = EV_COMPUTE;
                    break;
                case SDLK_RIGHT:
                    ev.type = EV_MOVE_RIGHT;
                    break;
                case SDLK_DOWN:
                    ev.type = EV_MOVE_DOWN;
                    break;
                case SDLK_LEFT:
                    ev.type = EV_MOVE_LEFT;
                    break;
                case SDLK_UP:
                    ev.type = EV_MOVE_UP;
                    break;
                case SDLK_o:
                    ev.type = EV_ZOOM_OUT;
                    break;
                case SDLK_i:
                    ev.type = EV_ZOOM_IN;
                    break;
                case SDLK_h:
                    ev.type = EV_PRINT_HELP;
                    break;
                case SDLK_l:
                    ev.type = EV_CLEAR_BUFFER;
                    break;
                case SDLK_e:
                    ev.type = EV_ADD_RE;
                    break;
                case SDLK_r:
                    ev.type = EV_LESS_RE;
                    break;
                case SDLK_t:
                    ev.type = EV_ADD_IM;
                    break;
                case SDLK_y:
                    ev.type = EV_LESS_IM;
                    break;
                case SDLK_m:
                    ev.type = EV_ADD_N;
                    break;
                case SDLK_n:
                    ev.type = EV_LESS_N;
                    break;
                default:
                    break;
                }
            }else if(event.type==SDL_KEYUP){
                //info("keyup");
            }else{

            }
        }
        if(ev.type != EV_TYPE_NUM){
            queue_push(ev);
        }
        SDL_Delay(SDL_EVENT_POLL_WAIT_MS);
        quit = is_quit();
    }
    info("gui_win_thread - end");
    return NULL;
}
