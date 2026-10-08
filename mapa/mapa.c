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

    /* 1a linha do arquivo: exatamente dois numeros, "linhas colunas" */
    char linha[4096];
    int n = 0;
    char extra;
    if (fgets(linha, sizeof(linha), f))
        n = sscanf(linha, "%d %d %c", &m->linhas, &m->colunas, &extra);
    if (n != 2 || m->linhas <= 0 || m->colunas <= 0) {
        fprintf(stderr, "Erro: %s esta sem o cabecalho. A 1a linha do arquivo deve ser "
                        "\"%d %d\" (linhas colunas) e depois vem a matriz.\n",
                arquivo_txt, LINHAS_MAPA, COLUNAS_MAPA);
        free(m);
        fclose(f);
        return NULL;
    }

    if (m->linhas != LINHAS_MAPA || m->colunas != COLUNAS_MAPA) {
        fprintf(stderr, "Erro: %s tem %d x %d, mas todos os mapas devem ter %d x %d (linhas x colunas)\n",
                arquivo_txt, m->linhas, m->colunas, LINHAS_MAPA, COLUNAS_MAPA);
        free(m);
        fclose(f);
        return NULL;
    }

    m->tileset = tileset;

    /* aloca a matriz: um vetor de ponteiros e depois cada linha */
    m->tiles = malloc(m->linhas * sizeof(int *));
    for (int i = 0; i < m->linhas; i++)
        m->tiles[i] = NULL;                       /* para liberar com seguranca se der erro */

    for (int i = 0; i < m->linhas; i++) {
        m->tiles[i] = malloc(m->colunas * sizeof(int));
        for (int j = 0; j < m->colunas; j++) {
            if (fscanf(f, "%d", &m->tiles[i][j]) != 1 ||
                m->tiles[i][j] < 0 || m->tiles[i][j] >= NUM_TILES) {
                fprintf(stderr, "Erro: valor invalido ou faltando em %s (linha %d, coluna %d)\n",
                        arquivo_txt, i + 1, j + 1);
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
            /* recorta o bloco 'tipo' do tileset e desenha na posicao da celula */
            al_draw_bitmap_region(m->tileset,
                                  tipo * TILE_SIZE, 0,
                                  TILE_SIZE, TILE_SIZE,
                                  j * TILE_SIZE, i * TILE_SIZE, 0);
        }
    }
}

void mapa_liberar(Mapa *m)
{
    if (!m) return;
    for (int i = 0; i < m->linhas; i++)
        free(m->tiles[i]);                        /* free(NULL) e seguro */
    free(m->tiles);
    free(m);
}