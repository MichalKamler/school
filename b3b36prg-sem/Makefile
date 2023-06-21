CFLAGS+= -Wall -Werror -std=gnu99 -g
LDFLAGS=-lpthread

BINARIES=prgsem computational_module

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image

all: $(BINARIES)

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
OBJ_IO=prg_io_nonblock.o utils.o event_queue.o xwin_sdl.o messages.o main.o keyboard.o gui.o computation.o comp_modul_computation.o

prgsem : prgsem.o $(OBJ_IO) 
	$(CC) $< $(OBJ_IO) $(LDFLAGS) -o $@

computational_module : computational_module.o $(OBJ_IO) 
	$(CC) $< $(OBJ_IO) $(LDFLAGS) -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) $(OBJS)
