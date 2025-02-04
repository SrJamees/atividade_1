#include "pico/stdlib.h"
#include <stdio.h>

// Defina os pinos dos LEDs
#define LED_RED 13
#define LED_YELLOW 14
#define LED_GREEN 15

// Variável para controlar o estado do semáforo
typedef enum {
    STATE_GREEN,
    STATE_YELLOW,
    STATE_RED
} TrafficLightState;

TrafficLightState current_state = STATE_GREEN;

// Função de callback do timer
bool timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Alterna o estado do semáforo
    switch (current_state) {
        case STATE_GREEN:
            gpio_put(LED_YELLOW, 1); // Liga amarelo
            current_state = STATE_YELLOW;
            break;
        case STATE_YELLOW:
            gpio_put(LED_RED, 1); // Liga vermelho
            current_state = STATE_RED;
            break;
        case STATE_RED:
            gpio_put(LED_GREEN, 1); // Liga verde
            current_state = STATE_GREEN;
            break;
    }

    return true; // Mantém o timer ativo
}

int main() {
    // Inicializa os pinos dos LEDs como saída
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Liga o LED verde inicialmente
    gpio_put(LED_GREEN, 1);

    // Configura o timer repetitivo
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, timer_callback, NULL, &timer);

    // Loop infinito
    while (1) {
        // O timer cuida da alternância dos LEDs
        tight_loop_contents();
    }

    return 0;
}