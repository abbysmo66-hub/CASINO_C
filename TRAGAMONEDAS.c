//Librerias.
//#include "../utiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "TRAGAMONEDAS.h"

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

/*Declaraciones adelantadas de las funciones.*/
void tragamonedas(struct Usuario *j);
void cartel_tragamonedas();
void apuesta_invalida();
void slots_fortune();
void palanca_01();
void palanca_02();
void girando();
void iniciando(int rueda1, int rueda2, int rueda3);
void jackpot();
void sin_fichas();

/*int main() {
    srand((unsigned int)time(NULL));

    // 1. Crear e inicializar la estructura Jugador
    struct Jugador jugador1;
    jugador1.saldo = 1000.0f;
    jugador1.continuar = true;

    // 2. Pasar la dirección de la estructura con el operador '&'
    tragamonedas(&jugador1); 
    
    return 0;
}*/

/*La función ahora recibe un puntero apuntando a la estructura Jugador*/
void tragamonedas(struct Usuario *j) {
    int apuesta, apuestaMINIMA = 10;
    int i, s, opcion;
    int rueda1, rueda2, rueda3;
    char textosimbolos[MAX_STRLEN];
    /* S para los ritmos. */

    /*Accedemos a los campos del puntero usando el operador flecha '->'*/
    while (j->saldo > 0 && j->continuar) {
        
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cartel_tragamonedas();
        
        printf("   ========================================================================\n");
        printf("    SALDO ACTUAL: [$%.2f]\n", j->saldo);
        printf("   ========================================================================\n");
        printf("   Ingresa tu apuesta (Minimo $%d): ", apuestaMINIMA);
        
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
            // Modificamos el saldo directamente en la memoria externa usando el puntero
            j->saldo = j->saldo - apuesta;
            
            // Animación del giro de palanca y rodillos
            for (i = 1; i <= 5; ++i) {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                
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

            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            
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
            printf("      |%s|\n", textosimbolos);
            printf("      |___________________________________________________________________|\n\n");

            // ========================================================================
            /*LÓGICA DE PREMIOS Y RITMOS DE SONIDO (Se suma directo al saldo real del jugador)*/
            if (rueda1 == rueda2 && rueda2 == rueda3) { /*Si las tres son iguales*/
                if (rueda1 == 1) { /*Si sacó corona*/
                    jackpot();
                    j->saldo = j->saldo + (apuesta * 15);
                    for (s = 1; s <= 12; ++s) { /*El bucle repite la instrucción dentro exactamente 12 veces .*/
                        esperar(80); /*Durante cada repetición, el programa se detiene completamente durante 80 segundos*/
                    }
                } else {
                    printf("¡Felicidades! Consiguiste 3 simbolos iguales. ¡Gran Premio! (+ x5)\n");
                    j->saldo = j->saldo + (apuesta * 5);
                    for (s = 1; s <= 5; ++s) { /*Preincremento: Primero aumenta el valor y luego entrega el nuevo valor.*/	
                        /*Es ligeramente más rápido porque no necesitaba guardar una copia temporal del valor anterior antes de sumar 1*/
                        esperar(150);
                    }
                }
            } else if (rueda1 == rueda2 || rueda2 == rueda3 || rueda1 == rueda3) {
                printf("¡Bien! 2 simbolos iguales. Duplicas tu apuesta. (+ x2)\n");
                j->saldo = j->saldo + (apuesta * 2);
                esperar(300);
            } else {
                printf("No tuviste suerte esta vez. Los rodillos no coinciden.\n");
                esperar(600);
            }

            printf("\n==========================================================================\n");
            printf("  NUEVO SALDO DISPONIBLE: $%.2f\n", j->saldo);
            printf("==========================================================================\n");

            if (j->saldo >= apuestaMINIMA) {
                printf("  1. Seguir jugando.\n");
                printf("  2. Volver al menu principal / Retirarse.\n");
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
                        printf("\n ¡Gracias por jugar! Te retiras con un total de: $%.2f\n", j->saldo);
                        while (getchar() != '\n'); 
                        getchar(); 
                        break;
                    default:
                        printf(" Opcion invalida. Continuando juego por defecto...\n");
                        esperar(1000);
                }
            } else {
                sin_fichas();
                printf("  Te has quedado sin saldo suficiente para la apuesta minima ($%d).\n", apuestaMINIMA);
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
    printf("  _______ _____           _____          __  __  ____  _   _ ______ _____           _____\n");
    printf(" |__   __|  __ \\    /\\   / ____|   /\\   |  \\/  |/ __ \\| \\ | |  ____|  __ \\   /\\    / ____|\n");
    printf("    | |  | |__) |  /  \\ | |  __   /  \\  | \\  / | |  | |  \\| | |__  | |  | | /  \\  | (___\n");
    printf("    | |  |  _  /  / /\\ \\| | |_ | / /\\ \\ | |\\/| | |  | | . ` |  __| | |  | |/ /\\ \\  \\___ \\\n");
    printf("    | |  | | \\ \\ / ____ \\ |__| |/ ____ \\| |  | | |__| | |\\  | |____| |__| / ____ \\ ____) |\n");
    printf("    |_|  |_|  \\_/_/    \\_\\_____/_/    \\_\\_|  |_|\\____/|_| \\_|______|_____/_/    \\_\\_____/\n");
    printf("\n ==========================================================================================\n");
	printf("     TABLA DE PAGOS:   [ Crown ] [ Crown ] [ Crown ] -> JACKPOT GIGANTE (x15)\n");
	printf("                       3 Simbolos Iguales          -> Gran Premio (x5)\n");
	printf("                       2 Simbolos Iguales          -> Recompensa (x2)\n");
	printf(" ==========================================================================================\n");
}

void apuesta_invalida() {
    printf(" .-------------------------------------------------------.\n");
	printf(" |¡Apuesta invalida! ¡Presiona <<Enter>> para reintentar!|\n");
	printf(" .-------------------------------------------------------.\n");
}

void slots_fortune() {
    printf("      .___________________________________________________________________.\n");
	printf("      |                    ===  SLOTS FORTUNE  ===                        |\n");
	printf("      |___________________________________________________________________|\n");
	printf("      |                 |                 |                 |             |\n");
}

void palanca_01() {
    // Cuadro 1 de animación: Corona, Moneda y Espada / Palanca arriba (==O)
    // Cada rodillo tiene exactamente 9 caracteres impresos en pantalla
    printf("      |    "   "   _/\\_   "   "   |    "   "  / \\_/ \\ "   "   |    "   "   _/\\_   "   "   |     ==O     |\n");
    printf("      |    "   "  >    <  "   "   |    "   " |       |"   "   |    "   "  >    <  "   "   |       |     |\n");
    printf("      |    "   "    \\/    "   "   |    "   "  \\     / "   "   |    "   "    \\/    "   "   |    ___|     |\n");
    printf("      |    "   "          "   "   |    "   "    \\_/   "   "   |    "   "          "   "   |   /         |\n");
}

void palanca_02() {
    // Cuadro 2 de animación: Símbolos invertidos / Palanca abajo (O)
    printf("      |    "   "   / \\    "   "   |    "   "   _/\\_   "   "   |    "   "  / \\_/ \\ "   "   |             |\n");
    printf("      |    "   "  /   \\   "   "   |    "   "  >    <  "   "   |    "   " |       |"   "   |       |     |\n");
    printf("      |    "   "  \\___/   "   "   |    "   "    \\/    "   "   |    "   "  \\     / "   "   |    ___|     |\n");
    printf("      |    "   "          "   "   |    "   "          "   "   |    "   "    \\_/   "   "   |   /   O     |\n");
}

void girando() {
    printf("      |_________________|_________________|_________________|             |\n");
	printf("      |                    [   G I R A N D O   ]            |             |\n");
	printf("      |_____________________________________________________|_____________|\n");
}

void iniciando(int rueda1, int rueda2, int rueda3) {
    const char *f1;
    const char *f2;
    const char *f3;
    
    printf("      |                 |                 |                 |             |\n");
    
    // ==================== FILA 1 DE LOS RODILLOS ====================
    switch (rueda1) {
        case 1:  f1 = ("   _/\\_   "); break;
        case 2:  f1 = ("   / \\    "); break;
        case 3:  f1 = ("  / \\_/ \\ "); break;
        default: f1 = ("   / \\    "); break;
    }
    switch (rueda2) {
        case 1:  f2 = ("   _/\\_   "); break;
        case 2:  f2 = ("   / \\    "); break;
        case 3:  f2 = ("  / \\_/ \\ "); break;
        default: f2 = ("   / \\    "); break;
    }
    switch (rueda3) {
        case 1:  f3 = ("   _/\\_   "); break;
        case 2:  f3 = ("   / \\    "); break;
        case 3:  f3 = ("  / \\_/ \\ "); break;
        default: f3 = ("   / \\    "); break;
    }
    printf("      |    %s   |    %s   |    %s   |      O      |\n", f1, f2, f3);
    
    // ==================== FILA 2 DE LOS RODILLOS ====================
    switch (rueda1) {
        case 1:  f1 = ("  >    <  "); break;
        case 2:  f1 = ("  /   \\   "); break;
        case 3:  f1 = (" |       |"); break;
        default: f1 = ("  /   \\   "); break;
    }
    switch (rueda2) {
        case 1:  f2 = ("  >    <  "); break;
        case 2:  f2 = ("  /   \\   "); break;
        case 3:  f2 = (" |       |"); break;
        default: f2 = ("  /   \\   "); break;
    }
    switch (rueda3) {
        case 1:  f3 = ("  >    <  "); break;
        case 2:  f3 = ("  /   \\   "); break;
        case 3:  f3 = (" |       |"); break;
        default: f3 = ("  /   \\   "); break;
    }
    printf("      |    %s   |    %s   |    %s   |     /       |\n", f1, f2, f3);
    
    // ==================== FILA 3 DE LOS RODILLOS ====================
    switch (rueda1) {
        case 1:  f1 = ("    \\/    "); break;
        case 2:  f1 = ("  \\   /   "); break;
        case 3:  f1 = ("  \\     / "); break;
        default: f1 = ("  \\___/   "); break;
    }
    switch (rueda2) {
        case 1:  f2 = ("    \\/    "); break;
        case 2:  f2 = ("  \\   /   "); break;
        case 3:  f2 = ("  \\     / "); break;
        default: f2 = ("  \\___/   "); break;
    }
    switch (rueda3) {
        case 1:  f3 = ("    \\/    "); break;
        case 2:  f3 = ("  \\   /   "); break;
        case 3:  f3 = ("  \\     / "); break;
        default: f3 = ("  \\___/   "); break;
    }
    printf("      |    %s   |    %s   |    %s   |    ||       |\n", f1, f2, f3);
    
    // ==================== FILA 4 DE LOS RODILLOS ====================
    switch (rueda1) {
        case 1:  f1 = ("          "); break;
        case 2:  f1 = ("   \\ /    "); break;
        case 3:  f1 = ("    \\_/   "); break;
        default: f1 = ("          "); break;
    }
    switch (rueda2) {
        case 1:  f2 = ("          "); break;
        case 2:  f2 = ("   \\ /    "); break;
        case 3:  f2 = ("    \\_/   "); break;
        default: f2 = ("          "); break;
    }
    switch (rueda3) {
        case 1:  f3 = ("          "); break;
        case 2:  f3 = ("   \\ /    "); break;
        case 3:  f3 = ("    \\_/   "); break;
        default: f3 = ("          "); break;
    }
    printf("      |    %s   |    %s   |    %s   |    ||       |\n", f1, f2, f3);
    
    printf("      |_________________|_________________|_________________|             |\n");
}

void jackpot() {
    printf("  ***********************************************************\n");
	printf("  ¡¡¡ JACKPOT GIGANTE !!! ¡REVENTASTE LA BANCA CON 3 CORONAS!\n");
	printf("  ***********************************************************\n");
}

void sin_fichas() {
    printf("  ___________________________________________________________\n");
	printf(" |                                                           |\n");
	printf(" |   Te has quedado sin fichas. ¡Mejor suerte la proxima!    |\n");
	printf(" |___________________________________________________________|\n");
}
