#pragma once
 
#include <allegro5/allegro.h>
 
#define TILE_SIZE 20   /* 33 colunas x 20 = 660 ; 21 linhas x 20 = 420 */
 
/* Caminhos dos arquivos (definidos so aqui; relativos a pasta do projeto) */
#define CAMINHO_TILESET "imagens/tileset.png"
#define CAMINHO_MAPA1   "mapas/mapa1.txt"
#define CAMINHO_MAPA2   "mapas/mapa2.txt"
 
/* Tipos de bloco (o numero no arquivo .txt = coluna no tileset) */
#define TILE_CHAO   0
#define TILE_PAREDE 1
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
 
/* Libera a memoria (nao destroi o tileset, que pode ser compartilhado). */
void mapa_liberar(Mapa *m);
 
 