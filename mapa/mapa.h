#pragma once

#include <allegro5/allegro.h>
#include <stdbool.h>

#define TILE_SIZE 32   /* tamanho (px) de cada bloco no tileset e na tela */

// Tipos de bloco 
#define TILE_CHAO   0
#define TILE_GRAMA 1
#define TILE_AGUA   2

typedef struct {
    int linhas;
    int colunas;
    int **tiles;              /* matriz linhas x colunas */
    ALLEGRO_BITMAP *tileset;  /* imagem com os blocos lado a lado */
} Mapa;

/* Carrega o mapa de um .txt. Retorna NULL em caso de erro. */
Mapa *mapa_carregar(const char *arquivo_txt, ALLEGRO_BITMAP *tileset);

/* Desenha todos os blocos na tela. */
void mapa_desenhar(const Mapa *m);

/* true se o bloco (linha, coluna) bloqueia o personagem. */
bool mapa_bloco_solido(const Mapa *m, int linha, int coluna);

/* true se o retangulo (x, y, w, h) em pixels encosta em algum bloco solido. */
bool mapa_colide(const Mapa *m, float x, float y, float w, float h);

/* Libera a memoria (nao destroi o tileset, que pode ser compartilhado). */
void mapa_liberar(Mapa *m);
