#ifndef HW_API_H
#define HW_API_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

// estado simulado del dispensador físico
typedef enum {
    PRIZE_IDLE = 0,
    PRIZE_DISPENSING,
    PRIZE_DONE
} prize_status_t;

// inicializa plataforma concreta (PC / RPI / etc)
void hw_init(void);

// tiempo relativo en ms desde init
uint32_t hw_get_time_ms(void);

// input abstracto (no amarra a raylib directamente)
bool hw_button_pressed(int id);

// simulación / abstracción del dispenser físico real
prize_status_t hw_dispense_request(void);
void hw_reset_dispense(void);

// retorna puntero a config cargada por hw_init()
const GameConfig *hw_get_config(void);

#endif
