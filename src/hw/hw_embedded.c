#include "hw_api.h"
#include "config.h"
#include <stdio.h>

// Config global cargada
extern GameConfig config;

// GPIO: define tus pines reales aquí
#define PIN_COIN        17   // ejemplo GPIO17
#define PIN_BUTTON1     27
#define PIN_BUTTON2     22
#define PIN_DISPENSER   23

static double startTime = 0;

// Lectura real desde GPIO (reemplaza con tu HAL)
static inline bool gpio_read(int pin)
{
    // TODO: implementar según tu plataforma (wiringPi, pigpio, baremetal…)
    return false;
}

static inline void gpio_write(int pin, bool v)
{
    // TODO
}

// ========================================================
// Inicialización del hardware real
// ========================================================
void hw_init(void)
{
    // TODO: configurar GPIO
    // gpio_init(PIN_COIN, INPUT_PULLUP)
    // gpio_init(PIN_BUTTON1, INPUT_PULLUP)
    // gpio_init(PIN_DISPENSER, OUTPUT)

    if (!LoadConfig(CONFIG_PATH, &config))
        printf("⚠️ Config falló, se usarán valores default.\n");

    startTime = 0; // tu reloj propio
}

// ========================================================
// Tiempo real en ms desde init()
// ========================================================
uint32_t hw_get_time_ms(void)
{
    // TODO: reemplaza por tu timer real
    return 0;
}

// ========================================================
// Botones arcade reales
// ========================================================
bool hw_button_pressed(int id)
{
    switch (id) {
        case 0: return gpio_read(PIN_BUTTON1) == 0;
        case 1: return gpio_read(PIN_BUTTON2) == 0;
        default: return false;
    }
}

// ========================================================
// COIN MECH real (pulso) activo en LOW
// ========================================================
bool hw_coin_inserted(void)
{
    return gpio_read(PIN_COIN) == 0;
}

// ========================================================
// Dispensador real (motor / solenoide)
// ========================================================
prize_status_t hw_dispense_request(void)
{
    gpio_write(PIN_DISPENSER, true);
    // TODO según tu hardware: sensor de fin de carrera, temporizador, etc.
    return PRIZE_DISPENSING;
}

void hw_reset_dispense(void)
{
    gpio_write(PIN_DISPENSER, false);
}


bool hw_any_button_pressed(void)
{
    if (hw_button_pressed(0)) return true;
    if (hw_button_pressed(1)) return true;

    // Si tienes más botones físicos:
    // if (gpio_read(PIN_BUTTON3) == 0) return true;

    return false;
}
