#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>

Mapa* mapa_carregar(const char *arquivo_txt, ALLEGRO_BITMAP *tileset) {
    FILE* f = fopen(arquivo_txt, "r");
    if (!f) {
        printf("Erro: nao abriu %s\n", arquivo_txt);
        return NULL;
    }

    Mapa* m = malloc(sizeof(Mapa));
    if (!m) { 
        fclose(f); 
        return NULL; 
    }


    if (fscanf(f, "%d %d", &m->linhas, &m->colunas) != 2) {
        printf("Erro: cabecalho invalido em %s\n", arquivo_txt);
        free(m);
        fclose(f);
        return NULL;
    }
    // le a primeira linha: numero de linhas e colunas

    if (m->linhas != LINHAS_MAPA || m->colunas != COLUNAS_MAPA) {
    printf("Erro: %s deve ter %d x %d\n", arquivo_txt, LINHAS_MAPA, COLUNAS_MAPA);
    free(m);
    fclose(f);
    return NULL;
}

   m->tileset = tileset;

     
    m->tiles = malloc(m->linhas * sizeof(int*));
    for (int i = 0; i < m->linhas; i++)
        m->tiles[i] = NULL;
        // aloca a matriz

    for (int i = 0; i < m->linhas; i++) {
        m->tiles[i] = malloc(m->colunas * sizeof(int));
        for (int j = 0; j < m->colunas; j++) {
            if (fscanf(f, "%d", &m->tiles[i][j]) != 1 || m->tiles[i][j] < 0 || m->tiles[i][j] >= NUM_TILES) {
                printf("Erro: valor invalido em %s (linha %d, coluna %d)\n", arquivo_txt, i + 1, j + 1);
                mapa_liberar(m);
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);
    return m;
}

void mapa_desenhar(const Mapa* m) {
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            int tipo = m->tiles[i][j];
            al_draw_bitmap_region(m->tileset, tipo * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, j * TILE_SIZE, i * TILE_SIZE, 0);
        }
    }
}

void mapa_liberar(Mapa* m) {
    if (!m) {
     return;
    }

    for (int i = 0; i < m->linhas; i++)
        free(m->tiles[i]);
    free(m->tiles);
    free(m);
}