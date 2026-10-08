#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "personagem/personagem.h" // Incluindo nosso novo módulo

const float FPS = 60.0f;
const float TELA_LARGURA = 600.0f;
const float TELA_ALTURA = 400.0f;

int main() {
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);

    ALLEGRO_DISPLAY *display = al_create_display(TELA_LARGURA, TELA_ALTURA);
    al_set_window_title(display, "Movimento Personagem Modularizado");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    
    // Ajuste o caminho dependendo de onde o main.exe roda
    ALLEGRO_BITMAP *spritesheet = al_load_bitmap("sprites.png"); 
    
    if (!spritesheet) return -1;
    
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    // Utilizando as funções do personagem.h
    Personagem player;
    inicializar_personagem(&player, TELA_LARGURA, TELA_ALTURA);

    int contador_animacao = 0;
    const int VELOCIDADE_ANIMACAO = 8;

    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            rodando = false;
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER){
            ALLEGRO_KEYBOARD_STATE teclado;
            al_get_keyboard_state(&teclado);
            
            // Atualiza a posição e estado enviando os dados necessários
            atualizar_personagem(&player, &teclado, &contador_animacao, VELOCIDADE_ANIMACAO, TELA_LARGURA, TELA_ALTURA);
            
            redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)){
            redesenhar = false;
            al_clear_to_color(al_map_rgb(200, 220, 200)); 
            
            // Desenha com base nos dados calculados
            desenhar_personagem(&player, spritesheet);
            
            al_flip_display();
        }
    }

    al_destroy_bitmap(spritesheet);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(display);
    return 0;
}