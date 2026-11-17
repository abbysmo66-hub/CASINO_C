#ifndef TRAGAMONEDAS_H
#define TRAGAMONEDAS_H

#include "main.h"

typedef enum {
    SIMBOLO_CROWN = 1,
    SIMBOLO_DIAMANTE,
    SIMBOLO_MONEDA,
    SIMBOLO_ESPADA,
} Simbolos_t;

// Cambiamos el nombre para que coincida con la llamada en main.c
void JugarTragamonedas(Usuario_t *j); 
void cartel_tragamonedas();
void apuesta_invalida();
void slots_fortune();
void palanca_01();
void palanca_02();
void girando();
void iniciando(int rueda1, int rueda2, int rueda3);
void jackpot();
void sin_fichas();

#endif