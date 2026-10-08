#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "personagem/personagem.h"
#include "mapa/mapa.h"

const float FPS = 60.0f;

int main() {
    // 1. Inicialização do Allegro
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);

    // Usa as definições do seu mapa.h para criar a janela
    ALLEGRO_DISPLAY *display = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    al_set_window_title(display, "Jogo Integrado - Mapa e Personagem");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    
    // 2. Carregamento de Recursos (Imagens)
    ALLEGRO_BITMAP *spritesheet = al_load_bitmap("sprites.png"); 
    ALLEGRO_BITMAP *tileset = al_load_bitmap(CAMINHO_TILESET);
    
    if (!spritesheet || !tileset) {
        printf("Erro: Falha ao carregar imagens (spritesheet ou tileset).\n");
        return -1;
    }

    // 3. Inicialização do Mapa
    bool usando_mapa1 = true;
    Mapa *mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
    if (!mapa_atual) {
        return -1; 
    }
    
    // 4. Registro de Eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    // 5. Inicialização do Personagem
    Personagem player;
    inicializar_personagem(&player);

    int contador_animacao = 0;
    const int VELOCIDADE_ANIMACAO = 8;
    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

    // 6. Loop Principal
    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            rodando = false;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            
            // Tecla M: Alterna entre os cenários
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
                    printf("Erro ao carregar o novo mapa!\n");
                    rodando = false;
                }
            }
            
            // Tecla R: Recarrega o ficheiro de texto do mapa atual (Hot-reload)
            else if (evento.keyboard.keycode == ALLEGRO_KEY_R) {
                mapa_liberar(mapa_atual);
                
                if (usando_mapa1) {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
                } else {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA2, tileset);
                }
                
                if (!mapa_atual) {
                    printf("Erro ao recarregar o mapa atual!\n");
                    rodando = false;
                } else {
                    printf("Mapa atualizado com as mudancas do txt!\n");
                }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER){
            ALLEGRO_KEYBOARD_STATE teclado;
            al_get_keyboard_state(&teclado);
            
            // O boneco se move
            atualizar_personagem(&player, &teclado, &contador_animacao, VELOCIDADE_ANIMACAO, mapa_atual);
            
            // --- NOVA LÓGICA DE TRANSIÇÃO DE MAPA ---
            bool trocar_mapa = false;
            
            if (player.x > LARGURA_JANELA) { 
                // Saiu pela direita, aparece na esquerda
                player.x = -SPRITE_LARGURA + player.velocidade; 
                trocar_mapa = true;
            } 
            else if (player.x + SPRITE_LARGURA < 0) { 
                // Saiu pela esquerda, aparece na direita
                player.x = LARGURA_JANELA - player.velocidade;
                trocar_mapa = true;
            } 
            else if (player.y > ALTURA_JANELA) { 
                // Saiu por baixo, aparece em cima
                player.y = -SPRITE_ALTURA + player.velocidade;
                trocar_mapa = true;
            } 
            else if (player.y + SPRITE_ALTURA < 0) { 
                // Saiu por cima, aparece embaixo
                player.y = ALTURA_JANELA - player.velocidade;
                trocar_mapa = true;
            }

            // Se o boneco tocou em alguma borda aberta, troca o cenário
            if (trocar_mapa) {
                mapa_liberar(mapa_atual);
                
                // Inverte rapidamente qual mapa estamos usando
                usando_mapa1 = !usando_mapa1; 
                
                if (usando_mapa1) {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA1, tileset);
                } else {
                    mapa_atual = mapa_carregar(CAMINHO_MAPA2, tileset);
                }
                
                if (!mapa_atual) {
                    printf("Erro ao carregar mapa durante a transição!\n");
                    rodando = false;
                }
            }
            // ----------------------------------------

            redesenhar = true;
        }

        // 7. Desenho na Tela
        if (redesenhar && al_is_event_queue_empty(fila_eventos)){
            redesenhar = false;
            
            // Desenha o fundo (Mapa) primeiro
            mapa_desenhar(mapa_atual);
            
            // Desenha o personagem por cima do mapa
            desenhar_personagem(&player, spritesheet);
            
            al_flip_display();
        }
    }

    // 8. Limpeza de Memória
    mapa_liberar(mapa_atual);
    al_destroy_bitmap(tileset);
    al_destroy_bitmap(spritesheet);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(display);
    
    return 0;
}