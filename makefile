ifeq ($(OS),Windows_NT)
    CC = clang
else
    CC = gcc
endif

CFLAGS = -Wall -Wextra -g -O0 -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined
LDLIBS = -lSDL2 -lSDL2_image -lm

SRC = main.c Mj.c moteur_rendu.c agent.c action.c liste.c grid.c user.c rl.c reinforce.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

prog: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) prog.exe prog