#include "personagem.h"

void inicializar_personagem(Personagem *p, float tela_largura, float tela_altura) {
    p->x = tela_largura / 2.0f - SPRITE_LARGURA / 2.0f;
    p->y = tela_altura / 2.0f - SPRITE_ALTURA / 2.0f;
    p->velocidade = 3.0f;
    p->direcao = DIR_BAIXO;
    p->frame_atual = 1;
    p->movendo = false;
}

void atualizar_personagem(Personagem *p, ALLEGRO_KEYBOARD_STATE *teclado, int *contador_animacao, int velocidade_animacao, float tela_largura, float tela_altura) {
    p->movendo = false;

    if (al_key_down(teclado, ALLEGRO_KEY_UP) || al_key_down(teclado, ALLEGRO_KEY_W)){
        p->direcao = DIR_CIMA;
        p->y -= p->velocidade;
        p->movendo = true;
    } else if (al_key_down(teclado, ALLEGRO_KEY_DOWN) || al_key_down(teclado, ALLEGRO_KEY_S)){
        p->direcao = DIR_BAIXO;
        p->y += p->velocidade;
        p->movendo = true;
    }

    if (al_key_down(teclado, ALLEGRO_KEY_LEFT) || al_key_down(teclado, ALLEGRO_KEY_A)){
        p->direcao = DIR_ESQUERDA;
        p->x -= p->velocidade;
        p->movendo = true;
    } else if (al_key_down(teclado, ALLEGRO_KEY_RIGHT) || al_key_down(teclado, ALLEGRO_KEY_D)){
        p->direcao = DIR_DIREITA;
        p->x += p->velocidade;
        p->movendo = true;
    }

    // Animação
    if (p->movendo){
        (*contador_animacao)++;
        if (*contador_animacao >= velocidade_animacao){
            if (p->frame_atual == 0) p->frame_atual = 2;
            else p->frame_atual = 0;
            *contador_animacao = 0;
        }
    } else {
        p->frame_atual = 1;
        *contador_animacao = 0;
    }

    // Limites da tela
    if (p->x < 0) p->x = 0;
    if (p->y < 0) p->y = 0;
    if (p->x > tela_largura - SPRITE_LARGURA) p->x = tela_largura - SPRITE_LARGURA;
    if (p->y > tela_altura - SPRITE_ALTURA) p->y = tela_altura - SPRITE_ALTURA;
}

void desenhar_personagem(Personagem *p, ALLEGRO_BITMAP *spritesheet) {
    int x_origem = p->frame_atual * SPRITE_LARGURA;
    int y_origem = p->direcao * SPRITE_ALTURA;
    al_draw_bitmap_region(spritesheet, x_origem, y_origem, SPRITE_LARGURA, SPRITE_ALTURA, p->x, p->y, 0);
}