#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>
/* Inclui o cabeçalho do mapa para aceder ao TILE_SIZE, LARGURA_JANELA e struct Mapa */
#include "../mapa/mapa.h" 

#define SPRITE_LARGURA 32
#define SPRITE_ALTURA 32

enum Direcao {
    DIR_BAIXO = 0,
    DIR_ESQUERDA = 1,
    DIR_CIMA = 2,
    DIR_DIREITA = 3
};

typedef struct {
    float x, y;
    float velocidade;
    int direcao;
    int frame_atual;
    bool movendo;
} Personagem;

// A inicialização agora usa diretamente LARGURA_JANELA e ALTURA_JANELA
void inicializar_personagem(Personagem *p);

// Atualização agora recebe o Mapa ativo para calcular as colisões
void atualizar_personagem(Personagem *p, ALLEGRO_KEYBOARD_STATE *teclado, int *contador_animacao, int velocidade_animacao, const Mapa *mapa);

void desenhar_personagem(Personagem *p, ALLEGRO_BITMAP *spritesheet);

#endif