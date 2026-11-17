//Librerias.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>


#include "TRAGAMONEDAS.h"
#include "main.h"

/*Compatibilidad multiplataforma para pausar hilos (animación)*/
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

/*Definición de la función esperar en milisegundos*/
void esperar(double t) {
#ifdef _WIN32
    Sleep((DWORD)t);
#else
    usleep((useconds_t)(t * 1000));
#endif
}

/* Funcion auxiliar para el manejo de cstrings temporales */
char *get_aux_buffer(double t) {
    static char buf[32]; // Añadido tamaño estático [32] para que sea seguro
    snprintf(buf, sizeof(buf), "%.2f", t);
    return buf;
}

#define MAX_STRLEN 256 
/*La constante MAX_STRLEN define el tamaño maximo que se utiliza en este programa para
   cualquier cadena. */

/*La función ahora recibe un puntero apuntando a la estructura Usuario_t, lo que permite modificar directamente el saldo del usuario en la memoria externa.*/
void JugarTragamonedas(Usuario_t *j) {
    int apuesta, apuestaMINIMA = 500; /*Apuesta minima para jugar. Se puede cambiar a cualquier valor entero positivo.*/
    int i, s, opcion;
    int rueda1, rueda2, rueda3;
    char textosimbolos[MAX_STRLEN];
    /* S para los ritmos. */
    j->continuar = true; /*Inicializamos la bandera de control del bucle a true para que el juego comience.*/
    /*Accedemos a los campos del puntero usando el operador flecha '->'*/
    while (j->continuar) {
        
        LimpiarPantalla();

        cartel_tragamonedas();
        
        printf("   " T_AMARILLO "========================================================================\n" T_RESET);
        printf("    " T_VERDE "SALDO ACTUAL: " T_AMARILLO "[$%.2f]\n" T_RESET, j->saldo);
        printf("   " T_AMARILLO "========================================================================\n" T_RESET);
        printf("   " T_BLANCO "Ingresa tu apuesta (Minimo $%d): " T_VERDE "$" T_RESET, apuestaMINIMA);
        
        if (scanf("%i", &apuesta) != 1) { /*El programa espera que el jugador escriba un número entero (%i) por su apuesta.*/
            while (getchar() != '\n'); /*Sigue agarrando y borrando caracteres hasta que pulsa la Enter.*/
            /*Vacía por completo el búfer de entrada sucio, dejándolo perfectamente limpio para el siguiente intento*/
            continue;
            /*Se salta por completo las comprobaciones de apuestas y la animación de la tragamonedas, 
            volviendo directamente al principio del whilebucle para redibujar la pantalla 
            y pedirle al jugador que introduzca el número correcto de nuevo.*/
        }

        if (apuesta > j->saldo || apuesta < apuestaMINIMA) {
            apuesta_invalida();
            esperar(1800);
        } else {
            // 1. DESCUENTO AL JUGADOR Y ENTRADA A LA CAJA GLOBAL
            j->saldo -= apuesta;
            j->gananciaAdmin += apuesta;
            ActualizarCajaCasino(&cajaCasino, (float)apuesta, -1); // -1 suma a la banca
            
            // Animación del giro de palanca y rodillos
            for (i = 1; i <= 5; ++i) {
                LimpiarPantalla();
                
                cartel_tragamonedas();
                slots_fortune();
                
                /*Cambia el estado de la palanca de la maquina segun el numero.*/
                /*Simulando el movimiento.*/
                if (i % 2 == 0) {
                    palanca_01();
                    /*Si i es par (vueltas 2 y 4).*/
                } else {
                    palanca_02();
                    /*Si i es impar (vueltas 1, 3 y 5).*/
                }
                girando();
                /*Simula el mecanismo mecanico de giro*/
                esperar(250); 
            }

            /*Generación aleatoria de los 3 rodillos (1 al 4)*/
            rueda1 = (rand() % 4) + 1;
            rueda2 = (rand() % 4) + 1;
            rueda3 = (rand() % 4) + 1;

            LimpiarPantalla();
            
            cartel_tragamonedas();
            slots_fortune();
            iniciando(rueda1, rueda2, rueda3);
            /*Su funcion es crear una linea de texto dinamica y autoajustable.*/

            // ==================== SECCIÓN DE SÍMBOLOS ENCAJADOS ====================
            strcpy(textosimbolos, "  Simbolos: ");

            /*Concatenar el primer simbolo*/
            switch (rueda1) {
                case SIMBOLO_CROWN: strcat(textosimbolos, "[Crown] "); break;
                case SIMBOLO_DIAMANTE: strcat(textosimbolos, "[Diamante] "); break;
                case SIMBOLO_MONEDA: strcat(textosimbolos, "[Moneda] "); break;
                default: strcat(textosimbolos, "[Espada] "); break;
            }
            /*Concatenar el segundo simbolo*/
            switch (rueda2) {
                case SIMBOLO_CROWN: strcat(textosimbolos, "[Crown] "); break;
                case SIMBOLO_DIAMANTE: strcat(textosimbolos, "[Diamante] "); break;
                case SIMBOLO_MONEDA: strcat(textosimbolos, "[Moneda] "); break;
                default: strcat(textosimbolos, "[Espada] "); break;
            }
            /*Concatenar el tercer simbolo*/
            switch (rueda3) {
                case SIMBOLO_CROWN: strcat(textosimbolos, "[Crown]"); break;
                case SIMBOLO_DIAMANTE: strcat(textosimbolos, "[Diamante]"); break;
                case SIMBOLO_MONEDA: strcat(textosimbolos, "[Moneda]"); break;
                default: strcat(textosimbolos, "[Espada]"); break;
            }

            /*Relleno matem?tico automatico para cerrar la caja a la perfecci?n (67 caracteres de ancho interno).*/ 
            while (strlen(textosimbolos) < 67) { /*cuenta cuatas letras y espacios tiene la frase en ese instante.*/
                strcat(textosimbolos, " "); 
                /*El bucle dice: "Si la frase mide menos de 67 caracteres, agregale un espacio en blanco al final y vuelve a revisar".*/
            }
            /*El resultado: Si la combinacion de simbolos fue corta, el bucle agregara muchos espacios*/
            /*Si fue larga, agregara pocos. No importa que simbolos salga, la frase siempre medirá exactamente 67 caracteres.*/

            /*Imprime la fila completamente alineada*/
            printf("      " T_AMARILLO "|" T_CIAN "%s" T_AMARILLO "|\n" T_RESET, textosimbolos);
            printf("      " T_AMARILLO "|___________________________________________________________________|\n\n" T_RESET);


            // ========================================================================
            /*LÓGICA DE PREMIOS Y RITMOS DE SONIDO (Se suma directo al saldo real del jugador)*/
             float premio = 0.0f;

            if (rueda1 == rueda2 && rueda2 == rueda3) { /*Si las tres son iguales*/
                if (rueda1 == 1) { /*Si sacó corona*/
                    jackpot();
                     premio = apuesta * 15.0f;
                    for (s = 1; s <= 12; ++s) { /*El bucle repite la instrucción dentro exactamente 12 veces .*/
                        esperar(80); /*Durante cada repetición, el programa se detiene completamente durante 80 segundos*/
                    }
                } else {
                    printf("  " T_VERDE "¡Felicidades! Consiguiste 3 simbolos iguales. ¡Gran Premio! (+ x5)\n" T_RESET);
                    premio = apuesta * 5.0f;
                    for (s = 1; s <= 5; ++s) { /*Preincremento: Primero aumenta el valor y luego entrega el nuevo valor.*/	
                        /*Es ligeramente más rápido porque no necesitaba guardar una copia temporal del valor anterior antes de sumar 1*/
                        esperar(150);
                    }
                }
            } else if (rueda1 == rueda2 || rueda2 == rueda3 || rueda1 == rueda3) {
                printf("  " T_AMARILLO "¡Bien! 2 simbolos iguales. Duplicas tu apuesta. (+ x2)\n" T_RESET);
                premio = apuesta * 2.0f;
                esperar(300);
            } else {
                printf("  " T_ROJO "No tuviste suerte esta vez. Los rodillos no coinciden.\n" T_RESET);
                esperar(600);
            }

            // Si ganó algo, acreditamos al usuario y descontamos de la caja central
            if (premio > 0.0f) {
                j->saldo += premio;
                j->gananciaAdmin -= premio;
                ActualizarCajaCasino(&cajaCasino, premio, 1); // 1 resta de la banca
            }

            printf("\n" T_AMARILLO "==========================================================================\n" T_RESET);
            printf("  " T_VERDE "NUEVO SALDO DISPONIBLE: " T_AMARILLO "$%.2f\n" T_RESET, j->saldo);
            printf(T_AMARILLO "==========================================================================\n" T_RESET);

            if (j->saldo >= apuestaMINIMA) {
                printf("  " T_CIAN "1." T_BLANCO " Seguir jugando.\n");
                printf("  " T_CIAN "2." T_ROJO " Volver al menu principal / Retirarse.\n" T_RESET);
                printf("  Seleccione una opcion: ");
                
                if (scanf("%i", &opcion) != 1) {
                    while (getchar() != '\n');
                    opcion = 1; 
                }
                
                switch (opcion) {
                    case 1:
                        j->continuar = true;
                        break;
                    case 2:
                        j->continuar = false;
                        printf("\n  " T_VERDE "¡Gracias por jugar! Te retiras con un total de: $%.2f\n" T_RESET, j->saldo);
                        while (getchar() != '\n'); 
                        getchar(); 
                        break;
                    default:
                        printf("  " T_ROJO "Opcion invalida. Continuando juego por defecto...\n" T_RESET);
                        esperar(1000);
                }
            } else {
                sin_fichas();
                printf("  " T_ROJO "Te has quedado sin saldo suficiente para la apuesta minima ($%d).\n" T_RESET, apuestaMINIMA);
                printf("  Gracias por participar.\n");
                j->continuar = false;
                while (getchar() != '\n');
                getchar();
            }
        }
    }
}

/*--- IMPLEMENTACIÓN DE ARTE ASCII Y FUNCIONES DE TEXTO ---*/
void cartel_tragamonedas() {
    printf("\n" T_AMARILLO "      ╔═════════════════════════════════════════════════╗\n");
    printf("      ║         " T_VERDE " 🪙  T R A G A M O N E D A S  🪙  " T_AMARILLO "        ║\n");
    printf("      ╠═════════════════════════════════════════════════╣\n");
    printf("      ║ " T_CIAN "TABLA DE PAGOS:" T_BLANCO "                                 " T_AMARILLO "║\n");
    printf("      ║  • 👑 👑 👑  -> JACKPOT GIGANTE (x15)          " T_AMARILLO" ║\n");
    printf("      ║  • 3 Iguales -> Gran Premio (x5)                " T_AMARILLO "║\n");
    printf("      ║  • 2 Iguales -> Recompensa (x2)                 " T_AMARILLO "║\n");
    printf("      ╚═════════════════════════════════════════════════╝\n" T_RESET);
}


void apuesta_invalida() {
    printf(" " T_ROJO ".-------------------------------------------------------.\n");
	printf(" | ¡Apuesta inválida! ¡Presiona <<Enter>> para reintentar! |\n");
	printf(" .-------------------------------------------------------." T_RESET "\n");
}

void slots_fortune() {
    printf("\n" T_AMARILLO "      ┌─────────────────────────────────────────────────┐\n");
    printf("      │              " T_CIAN "=== SLOTS FORTUNE ===" T_AMARILLO "              │\n");
    printf("      ├─────────────────────────────────────────────────┤\n");
}

void palanca_01() {
    printf("      │           " T_VERDE "🎰 [   GIRANDO...   ] 🎰           " T_AMARILLO "   │\n");
}

void palanca_02() {
    printf("      │           " T_VERDE "🎰 [  * GIRANDO *   ] 🎰           " T_AMARILLO "   │\n");
}

void girando() {
    printf("      └─────────────────────────────────────────────────┘\n" T_RESET);
}

void iniciando(int rueda1, int rueda2, int rueda3) {
    const char *s1, *s2, *s3;

    switch (rueda1) {
        case SIMBOLO_CROWN:    s1 = " 👑 "; break;
        case SIMBOLO_DIAMANTE: s1 = " 💎 "; break;
        case SIMBOLO_MONEDA:   s1 = " 🪙 "; break;
        default:               s1 = " ⚔️ "; break;
    }
    switch (rueda2) {
        case SIMBOLO_CROWN:    s2 = " 👑 "; break;
        case SIMBOLO_DIAMANTE: s2 = " 💎 "; break;
        case SIMBOLO_MONEDA:   s2 = " 🪙 "; break;
        default:               s2 = " ⚔️ "; break;
    }
    switch (rueda3) {
        case SIMBOLO_CROWN:    s3 = " 👑 "; break;
        case SIMBOLO_DIAMANTE: s3 = " 💎 "; break;
        case SIMBOLO_MONEDA:   s3 = " 🪙 "; break;
        default:               s3 = " ⚔️ "; break;
    }

    printf("\n" T_AMARILLO "      ┌──────────────────┬──────────────────┬──────────────────┐\n");
    printf("      │" T_CIAN "    RODILLO 1     " T_AMARILLO "│" T_CIAN "    RODILLO 2     " T_AMARILLO "│" T_CIAN "    RODILLO 3     " T_AMARILLO "│\n");
    printf("      ├──────────────────┼──────────────────┼──────────────────┤\n");
    printf("      │" T_BLANCO "  %-18s" T_AMARILLO "│" T_BLANCO "     %-18s" T_AMARILLO "│" T_BLANCO "   %-18s" T_AMARILLO "│\n", s1, s2, s3);
    printf("      └──────────────────┴──────────────────┴──────────────────┘\n" T_RESET);
}

void jackpot() {
    printf("  " T_AMARILLO "***********************************************************\n");
	printf("  " T_VERDE "¡¡¡ JACKPOT GIGANTE !!! ¡REVENTASTE LA BANCA CON 3 CORONAS!\n");
	printf("  " T_AMARILLO "***********************************************************" T_RESET "\n");
}

void sin_fichas() {
    printf("  " T_ROJO "___________________________________________________________\n");
	printf(" |                                                           |\n");
	printf(" |   Te has quedado sin fichas. ¡Mejor suerte la próxima!    |\n");
	printf(" |___________________________________________________________|" T_RESET "\n");
}
