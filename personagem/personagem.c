#include "personagem.h"

void inicializar_personagem(Personagem *p) {
    p->x = LARGURA_JANELA / 2.0f - SPRITE_LARGURA / 2.0f;
    p->y = ALTURA_JANELA / 2.0f - SPRITE_ALTURA / 2.0f;
    p->velocidade = 3.0f;
    p->direcao = DIR_BAIXO;
    p->frame_atual = 1;
    p->movendo = false;
}

/* Função auxiliar privada para detetar colisão num ponto futuro */
bool colisao_mapa(float x, float y, const Mapa *m) {
    int tile_esq = x / TILE_SIZE;
    int tile_dir = (x + SPRITE_LARGURA - 1) / TILE_SIZE;
    int tile_cima = y / TILE_SIZE;
    int tile_baixo = (y + SPRITE_ALTURA - 1) / TILE_SIZE;

    // Se o boneco estiver a sair da tela, restringimos a verificação aos limites da matriz
    if (tile_esq < 0) tile_esq = 0;
    if (tile_cima < 0) tile_cima = 0;
    if (tile_dir >= m->colunas) tile_dir = m->colunas - 1;
    if (tile_baixo >= m->linhas) tile_baixo = m->linhas - 1;

    for (int i = tile_cima; i <= tile_baixo; i++) {
        for (int j = tile_esq; j <= tile_dir; j++) {
            if (m->tiles[i][j] != TILE_CHAO) {
                return true; 
            }
        }
    }
    return false;
}

void atualizar_personagem(Personagem *p, ALLEGRO_KEYBOARD_STATE *teclado, int *contador_animacao, int velocidade_animacao, const Mapa *mapa) {
    p->movendo = false;
    
    // Variáveis que testam o próximo passo antes de o dar definitivamente
    float prox_x = p->x;
    float prox_y = p->y;

    /* --- TESTE VERTICAL --- */
    if (al_key_down(teclado, ALLEGRO_KEY_UP) || al_key_down(teclado, ALLEGRO_KEY_W)){
        p->direcao = DIR_CIMA;
        prox_y -= p->velocidade;
        p->movendo = true;
    } else if (al_key_down(teclado, ALLEGRO_KEY_DOWN) || al_key_down(teclado, ALLEGRO_KEY_S)){
        p->direcao = DIR_BAIXO;
        prox_y += p->velocidade;
        p->movendo = true;
    }

    // Aplica Y apenas se esse passo não causar colisão no mapa
    if (p->y != prox_y && !colisao_mapa(p->x, prox_y, mapa)) {
        p->y = prox_y;
    }

    /* --- TESTE HORIZONTAL --- */
    if (al_key_down(teclado, ALLEGRO_KEY_LEFT) || al_key_down(teclado, ALLEGRO_KEY_A)){
        p->direcao = DIR_ESQUERDA;
        prox_x -= p->velocidade;
        p->movendo = true;
    } else if (al_key_down(teclado, ALLEGRO_KEY_RIGHT) || al_key_down(teclado, ALLEGRO_KEY_D)){
        p->direcao = DIR_DIREITA;
        prox_x += p->velocidade;
        p->movendo = true;
    }

    // Aplica X apenas se esse passo não causar colisão no mapa
    if (p->x != prox_x && !colisao_mapa(prox_x, p->y, mapa)) {
        p->x = prox_x;
    }

    /* --- ANIMAÇÃO --- */
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
}

void desenhar_personagem(Personagem *p, ALLEGRO_BITMAP *spritesheet) {
    int x_origem = p->frame_atual * SPRITE_LARGURA;
    int y_origem = p->direcao * SPRITE_ALTURA;
    al_draw_bitmap_region(spritesheet, x_origem, y_origem, SPRITE_LARGURA, SPRITE_ALTURA, p->x, p->y, 0);
}