#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>

#define SPRITE_LARGURA 32
#define SPRITE_ALTURA 32

enum Direcao {
    DIR_BAIXO = 0,
    DIR_ESQUERDA = 1,
    DIR_DIREITA = 3,
    DIR_CIMA = 2
};

typedef struct {
    float x, y;
    float velocidade;
    int direcao;
    int frame_atual;
    bool movendo;
} Personagem;

// Assinaturas das funções que vão gerenciar o boneco
void inicializar_personagem(Personagem *p, float tela_largura, float tela_altura);
void atualizar_personagem(Personagem *p, ALLEGRO_KEYBOARD_STATE *teclado, int *contador_animacao, int velocidade_animacao, float tela_largura, float tela_altura);
void desenhar_personagem(Personagem *p, ALLEGRO_BITMAP *spritesheet);

#endif