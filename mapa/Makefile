CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lm -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives -lallegro_acodec -lallegro_audio

SRCS = teste.c mapa.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	$(CC) -o jogo.exe $(CFLAGS) $(OBJS) $(LDLIBS)