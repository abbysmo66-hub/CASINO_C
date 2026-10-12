#ifndef TRAGAMONEDAS_H
#define TRAGAMONEDAS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//==============================================================================
// ESTRUCTURA DE DATOS (struct)
//Agrupa variables de distinto tipo en una sola entidad logica llamada 'Usuario'
//==============================================================================
typedef struct Usuario{
    char usuario[50];  //Cadena de caracteres para el nombre de usuario
    char clave[50];    //Cadena de caracteres para la contrase�a
    int edad;          //Edad del usuario
    float saldo;       //Diinero disponible en la cuenta
    bool activo;       //Bandera: true = casillero ocupado, false = casillero libre
    bool continuar;   //Bandera para controlar el flujo de los juegos (true = seguir jugando, false = salir)
}Usuario;

// Permiten asociar nombres legibles a las opciones del menú del juego poker
typedef enum{
    SIMBOLO_CROWN = 1,
    SIMBOLO_DIAMANTE,
    SIMBOLO_MONEDA,
    SIMBOLO_ESPADA,
} Simbolos;

/* Prototipos de funciones del tragamonedas */
void tragamonedas(Usuario *j);
void cartel_tragamonedas();
void apuesta_invalida();
void slots_fortune();
void palanca_01();
void palanca_02();
void girando();
void iniciando(int rueda1, int rueda2, int rueda3);
void jackpot();
void sin_fichas();

#endif /* TRAGAMONEDAS_H */