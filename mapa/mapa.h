#pragma once

#include <allegro5/allegro.h>

/* ---------- tamanhos ---------- */
#define TILE_SIZE 20            /* lado de cada bloco, em pixels */

/* TODOS os mapas tem o mesmo tamanho: 30 colunas x 20 linhas.
 * Entao a janela tem tamanho fixo: 30*20 = 600 por 20*20 = 400. */
#define COLUNAS_MAPA 30
#define LINHAS_MAPA  20
#define LARGURA_JANELA (COLUNAS_MAPA * TILE_SIZE)   /* 600 */
#define ALTURA_JANELA  (LINHAS_MAPA  * TILE_SIZE)   /* 400 */

/* ---------- caminhos dos arquivos (relativos a pasta do projeto) ---------- */
#define CAMINHO_TILESET "imagens/tileset.png"
#define CAMINHO_MAPA1   "mapas/mapa1.txt"
#define CAMINHO_MAPA2   "mapas/mapa2.txt"

/* ---------- tipos de bloco ----------
 * O numero no .txt e a posicao do bloco no tileset (blocos lado a lado). */
#define TILE_CHAO   0
#define TILE_PAREDE 1
#define TILE_AGUA   2
#define NUM_TILES   3

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

/* Libera a memoria (nao destroi o tileset, que pode ser compartilhado). */
void mapa_liberar(Mapa *m);