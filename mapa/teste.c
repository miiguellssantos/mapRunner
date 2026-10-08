/* teste.c - programa principal (teste do modulo do mapa).
 * Setas: movem o quadrado amarelo (sem colisao).
 * Teclas 1 e 2: trocam de mapa.  ESC: sai. */

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "mapa.h"

#define VEL 2    /* pixels por quadro (60 quadros por segundo) */
#define TAM 12   /* lado do quadrado de teste, em pixels */

/* Carrega outro mapa, libera o antigo e recoloca o jogador no inicio.
 * A janela NAO muda de tamanho (todos os mapas tem o mesmo tamanho). */
static void trocar_mapa(Mapa **mapa, const char *arquivo, ALLEGRO_BITMAP *tileset,
                        float *x, float *y)
{
    Mapa *novo = mapa_carregar(arquivo, tileset);
    if (!novo) return;                 /* deu erro: continua com o mapa atual */

    mapa_liberar(*mapa);               /* liberar(NULL) e seguro na 1a vez */
    *mapa = novo;

    printf("Mapa carregado: %s (%d linhas x %d colunas)\n",
           arquivo, novo->linhas, novo->colunas);

    *x = TILE_SIZE + 4;                /* dentro da celula (1,1), que e chao */
    *y = TILE_SIZE + 4;
}

int main(void)
{
    /* ---- 1. inicializar a biblioteca e os modulos ---- */
    if (!al_init()) { fprintf(stderr, "Erro: al_init\n"); return 1; }
    if (!al_init_image_addon()) { fprintf(stderr, "Erro: image addon\n"); return 1; }
    if (!al_init_primitives_addon()) { fprintf(stderr, "Erro: primitives addon\n"); return 1; }
    if (!al_install_keyboard()) { fprintf(stderr, "Erro: teclado\n"); return 1; }

    /* ---- 2. janela, timer e fila de eventos ---- */
    al_set_new_display_flags(ALLEGRO_OPENGL);   /* evita tela preta no Windows (Direct3D) */
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    if (!janela || !timer || !fila) {
        fprintf(stderr, "Erro ao criar janela/timer/fila\n");
        return 1;
    }
    al_set_window_title(janela, "teste do mapa");

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));

    /* ---- 3. carregar o tileset e o primeiro mapa ---- */
    ALLEGRO_BITMAP *tileset = al_load_bitmap(CAMINHO_TILESET);
    if (!tileset) {
        fprintf(stderr, "Erro: nao achei %s (rode de dentro da pasta do projeto)\n", CAMINHO_TILESET);
        return 1;
    }
    printf("Tileset: %d x %d (esperado: %d x %d)\n",
           al_get_bitmap_width(tileset), al_get_bitmap_height(tileset),
           NUM_TILES * TILE_SIZE, TILE_SIZE);

    Mapa *mapa = NULL;
    float x = 0, y = 0;
    trocar_mapa(&mapa, CAMINHO_MAPA1, tileset, &x, &y);
    if (!mapa) return 1;

    /* ---- 4. loop do jogo ---- */
    bool teclas[ALLEGRO_KEY_MAX] = {false};
    bool rodando = true;
    bool redesenhar = true;            /* true: ja desenha o primeiro quadro */

    al_start_timer(timer);             /* sem isso o timer nunca dispara */

    while (rodando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            teclas[ev.keyboard.keycode] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) rodando = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_1)
                trocar_mapa(&mapa, CAMINHO_MAPA1, tileset, &x, &y);
            if (ev.keyboard.keycode == ALLEGRO_KEY_2)
                trocar_mapa(&mapa, CAMINHO_MAPA2, tileset, &x, &y);
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            teclas[ev.keyboard.keycode] = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (teclas[ALLEGRO_KEY_RIGHT]) x += VEL;
            if (teclas[ALLEGRO_KEY_LEFT])  x -= VEL;
            if (teclas[ALLEGRO_KEY_DOWN])  y += VEL;
            if (teclas[ALLEGRO_KEY_UP])    y -= VEL;
            redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;

            al_set_target_backbuffer(janela);          /* garante que desenha na janela */
            al_clear_to_color(al_map_rgb(0, 0, 0));
            mapa_desenhar(mapa);
            al_draw_filled_rectangle(x, y, x + TAM, y + TAM, al_map_rgb(255, 255, 0));
            al_flip_display();
        }
    }

    /* ---- 5. liberar tudo ---- */
    mapa_liberar(mapa);
    al_destroy_bitmap(tileset);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);
    return 0;
}