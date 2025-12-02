#ifndef HW_API_H
#define HW_API_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

// Estado simulado del dispensador físico
typedef enum {
    PRIZE_IDLE = 0,
    PRIZE_DISPENSING,
    PRIZE_DONE
} prize_status_t;

// ==========================================
// Inicialización
// ==========================================
void hw_init(void);

// ==========================================
// Tiempo relativo en ms desde init()
// ==========================================
uint32_t hw_get_time_ms(void);

// ==========================================
// Input abstracto
// ==========================================
bool hw_button_pressed(int id);

// ==========================================
// Coin mech abstracto
// (PC -> tecla C, Arcade -> pulso GPIO)
// ==========================================
bool hw_coin_inserted(void);

// ==========================================
// Dispensador (motor real / fake PC)
// ==========================================
prize_status_t hw_dispense_request(void);
void hw_reset_dispense(void);

// ==========================================
// Retorna config cargada
// ==========================================
const GameConfig *hw_get_config(void);

#endif
