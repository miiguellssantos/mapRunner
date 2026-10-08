/* main.c - programa de TESTE do modulo do mapa.
 * Mostra o mapa, um quadrado amarelo controlado pelas setas (sem colisao)
 * e troca de mapa com as teclas 1 e 2. ESC sai. */

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "mapa.h"

#define VEL 2   /* pixels por quadro (60 quadros/s) */
#define TAM 12  /* lado do quadrado de teste, em pixels */

/* Carrega outro mapa, libera o antigo, ajusta a janela e reposiciona o jogador. */
static void trocar_mapa(Mapa **mapa, const char *arquivo, ALLEGRO_BITMAP *tileset,
                        ALLEGRO_DISPLAY *janela, float *x, float *y)
{
    Mapa *novo = mapa_carregar(arquivo, tileset);
    if (!novo) return;               /* deu erro: continua com o mapa atual */

    mapa_liberar(*mapa);             /* liberar(NULL) e seguro na 1a vez */
    *mapa = novo;

    /* a janela passa a ter o tamanho exato do mapa */
    al_resize_display(janela, novo->colunas * TILE_SIZE, novo->linhas * TILE_SIZE);

    /* celula (1,1) e chao nos dois mapas */
    *x = TILE_SIZE + 4;
    *y = TILE_SIZE + 4;
}

int main(void)
{
    /* ---- 1. inicializar a biblioteca e os modulos ---- */
    if (!al_init() || !al_init_image_addon() ||
        !al_init_primitives_addon() || !al_install_keyboard()) {
        fprintf(stderr, "Erro ao iniciar o Allegro\n");
        return 1;
    }

    /* ---- 2. janela, timer e fila de eventos ---- */
    ALLEGRO_DISPLAY *janela = al_create_display(660, 420);
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
    ALLEGRO_BITMAP *tileset = al_load_bitmap("imagens/tileset.png");
    if (!tileset) {
        fprintf(stderr, "Nao achei imagens/tileset.png (rode de dentro da pasta do projeto)\n");
        return 1;
    }

    Mapa *mapa = NULL;
    float x = 0, y = 0;
    trocar_mapa(&mapa, "mapas/mapa1.txt", tileset, janela, &x, &y);
    if (!mapa) return 1;

    /* ---- 4. loop do jogo ---- */
    bool teclas[ALLEGRO_KEY_MAX] = {false};
    bool rodando = true;
    bool redesenhar = false;

    al_start_timer(timer);

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
                trocar_mapa(&mapa, "mapas/mapa1.txt", tileset, janela, &x, &y);
            if (ev.keyboard.keycode == ALLEGRO_KEY_2)
                trocar_mapa(&mapa, "mapas/mapa2.txt", tileset, janela, &x, &y);
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