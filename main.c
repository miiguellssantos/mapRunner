#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "personagem/personagem.h"
#include "mapa/mapa.h"

const float FPS = 60.0f;

int main() {
    //inicializando o allegro e os addons
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);

    // usa as configs do mapa.h pra criar a janela do game
    ALLEGRO_DISPLAY *display = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    al_set_window_title(display, "Jogo Integrado - Mapa e Personagem");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    
    //carregando os recursos (sprites e tileset)
    ALLEGRO_BITMAP *spritesheet = al_load_bitmap("sprites.png"); 
    ALLEGRO_BITMAP *tileset = al_load_bitmap(CAMINHO_TILESET);
    
    if (!spritesheet || !tileset) {
        printf("erro: falha ao carregar imagens (spritesheet ou tileset).\n");
        return -1;
    }

    // inicializando o primeiro mapa
    bool usando_mapa1 = true;
    Mapa *mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
    if (!mapa_atual) {
        return -1; 
    }
    
    // registrando as fontes de eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    // inicializando o player
    Personagem player;
    inicializar_personagem(&player);

    int contador_animacao = 0;
    const int VELOCIDADE_ANIMACAO = 8;
    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

    // loop principal do jogo
    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            rodando = false;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            
            // tecla m: troca de cenario manualmente
            if (evento.keyboard.keycode == ALLEGRO_KEY_M) {
                mapa_liberar(mapa_atual);
                
                if (usando_mapa1) {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA2, tileset);
                    usando_mapa1 = false;
                } else {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
                    usando_mapa1 = true;
                }
                
                if (!mapa_atual) {
                    printf("erro ao carregar o novo mapa!\n");
                    rodando = false;
                }
            }
            
            // tecla r: recarrega o txt do mapa atual (hot-reloadzin)
            else if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                mapa_liberar(mapa_atual);
                
                if (usando_mapa1) {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
                } else {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA2, tileset);
                }
                
                if (!mapa_atual) {
                    printf("erro ao recarregar o mapa atual!\n");
                    rodando = false;
                } else {
                    printf("mapa atualizado com as mudancas do txt!\n");
                }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER){
            ALLEGRO_KEYBOARD_STATE teclado;
            al_get_keyboard_state(&teclado);
            
            // atualiza a posicao e animacao do boneco
            atualizar_personagem(&player, &teclado, &contador_animacao, VELOCIDADE_ANIMACAO, mapa_atual);
            
            // --- logica de transicao de mapa pelas bordas ---
            bool trocar_mapa = false;
            
            if (player.x > LARGURA_JANELA) { 
                // saiu pela direita, aparece na esquerda
                player.x = -SPRITE_LARGURA + player.velocidade; 
                trocar_mapa = true;
            } 
            else if (player.x + SPRITE_LARGURA < 0) { 
                // saiu pela esquerda, aparece na direita
                player.x = LARGURA_JANELA - player.velocidade;
                trocar_mapa = true;
            } 
            else if (player.y > ALTURA_JANELA) { 
                // saiu por baixo, aparece em cima
                player.y = -SPRITE_ALTURA + player.velocidade;
                trocar_mapa = true;
            } 
            else if (player.y + SPRITE_ALTURA < 0) { 
                // saiu por cima, aparece embaixo
                player.y = ALTURA_JANELA - player.velocidade;
                trocar_mapa = true;
            }

            // se bateu na borda, troca o cenario
            if (trocar_mapa) {
                mapa_liberar(mapa_atual);
                
                // inverte o mapa atual
                usando_mapa1 = !usando_mapa1; 
                
                if (usando_mapa1) {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
                } else {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA2, tileset);
                }
                
                if (!mapa_atual) {
                    printf("erro ao carregar mapa durante a transicao!\n");
                    rodando = false;
                }
            }
            // ----------------------------------------

            redesenhar = true;
        }

        // renderizacao na tela
        if (redesenhar && al_is_event_queue_empty(fila_eventos)){
            redesenhar = false;
            
            // desenha o fundo (mapa) primeiro
            mapa_desenhar(mapa_atual);
            
            // desenha o player por cima
            desenhar_personagem(&player, spritesheet);
            
            al_flip_display();
        }
    }

    // limpando tudo e liberando memoria
    mapa_liberar(mapa_atual);
    al_destroy_bitmap(tileset);
    al_destroy_bitmap(spritesheet);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(display);
    
    return 0;
}