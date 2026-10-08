#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>

Mapa *mapa_carregar(const char *arquivo_txt, ALLEGRO_BITMAP *tileset)
{
    FILE *f = fopen(arquivo_txt, "r");
    if (!f) {
        fprintf(stderr, "Erro: nao abriu %s\n", arquivo_txt);
        return NULL;
    }

    Mapa *m = malloc(sizeof(Mapa));
    if (!m) { fclose(f); return NULL; }

    /* Formato: 1a linha = "linhas colunas", depois a matriz */
    if (fscanf(f, "%d %d", &m->linhas, &m->colunas) != 2 ||
        m->linhas <= 0 || m->colunas <= 0) {
        fprintf(stderr, "Erro: cabecalho invalido em %s\n", arquivo_txt);
        free(m); fclose(f);
        return NULL;
    }

    m->tileset = tileset;
    m->tiles = malloc(m->linhas * sizeof(int *));
    for (int i = 0; i < m->linhas; i++) {
        m->tiles[i] = malloc(m->colunas * sizeof(int));
        for (int j = 0; j < m->colunas; j++) {
            if (fscanf(f, "%d", &m->tiles[i][j]) != 1) {
                fprintf(stderr, "Erro: mapa incompleto em %s\n", arquivo_txt);
                m->linhas = i + 1;   /* libera so o que foi alocado */
                mapa_liberar(m);
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);
    return m;
}

void mapa_desenhar(const Mapa *m)
{
    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            int tipo = m->tiles[i][j];
            /* recorta o bloco 'tipo' do tileset (blocos lado a lado) */
            al_draw_bitmap_region(m->tileset,
                                  tipo * TILE_SIZE, 0,
                                  TILE_SIZE, TILE_SIZE,
                                  j * TILE_SIZE, i * TILE_SIZE, 0);
        }
    }
}

bool mapa_bloco_solido(const Mapa *m, int linha, int coluna)
{
    /* fora do mapa conta como parede */
    if (linha < 0 || linha >= m->linhas || coluna < 0 || coluna >= m->colunas)
        return true;
    int t = m->tiles[linha][coluna];
    return t == TILE_PAREDE || t == TILE_AGUA;
}

bool mapa_colide(const Mapa *m, float x, float y, float w, float h)
{
    /* converte as bordas do retangulo para indices de bloco */
    int c1 = (int)(x / TILE_SIZE);
    int c2 = (int)((x + w - 1) / TILE_SIZE);
    int l1 = (int)(y / TILE_SIZE);
    int l2 = (int)((y + h - 1) / TILE_SIZE);
    if (x < 0) c1 = -1;
    if (y < 0) l1 = -1;

    for (int l = l1; l <= l2; l++)
        for (int c = c1; c <= c2; c++)
            if (mapa_bloco_solido(m, l, c))
                return true;
    return false;
}

void mapa_liberar(Mapa *m)
{
    if (!m) return;
    for (int i = 0; i < m->linhas; i++)
        free(m->tiles[i]);
    free(m->tiles);
    free(m);
}