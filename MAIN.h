#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h> //Utilidades del sistema (srand, rand)
#include <time.h>   //Manejo de tiempo para la aleatoriedad
#include <string.h> //Cadenas de texto (strcmp, strcpy, strlen)
#include <stdbool.h> //Soporte para el tipo de dato booleano (bool, true, false)
#include <ctype.h>  //Funciones para caracteres (tolower)
#include <windows.h> //Para usar Sleep() y system("cls") en Windows
#include <conio.h>  //necesario para getch() y ocultar la contraseña con asteriscos

// ============================================================================
// DEFINICIÓN DE COLORES ANSI PARA LA CONSOLA (Estética de Casino VIP)
// ============================================================================
#define T_RESET   "\033[0m"
#define T_ROJO    "\033[1;31m"
#define T_VERDE   "\033[1;32m"
#define T_AMARILLO "\033[1;33m"
#define T_CIAN    "\033[1;36m"
#define T_BLANCO  "\033[1;37m"


//=========================================================================
// CONSTANTES MACRO (#define)
//Permiten cambiar valores globales sin necesidad de modificar multiples
//lineas de codigo
//=========================================================================
#define MAX_USUARIOS 3    //Capacidad maxima de usuarios en el arreglo
#define NO_ADMIN 2        //Cantidad de nombres prohibidos para registro de sdmin
#define EDAD_MIN 18       //Edad minima permitida
#define EDAD_MAX 120      //Edad maxima permitida

//Topes de saldo minimo exigidos para ingresar a cada juego
#define MAX_SALDO_TRAGAMONEDAS 500.0f    //Se usa 'f' para forzar tipo float (32 bits) no double (64bits)

#define SALDO_MAXIMO_PERMITIDO  5000000.0f // Tope máximo que puede tener un usuario
#define MAX_CARGA_OPERACION  1000000.0f   // Tope maximo que puede tener el usuario para cargar dinero
  

//Variable global que acumula las ganancias o perdidas del casino


extern float cajaCasino;
// ============================================================================
// ESTRUCTURAS Y CONSTANTES GLOBALES
// ============================================================================
typedef struct{
    char usuario[50];       //Cadena de caracteres para el nombre de usuario
    char clave[50];         //Cadena de caracteres para la contraseña
    int edad;               //Edad del usuario
    float saldo;            //Diinero disponible en la cuenta
    bool activo;            //Bandera: true = casillero ocupado, false = casillero libre
    bool continuar;         //Bandera para controlar el flujo de los juegos (true = seguir jugando, false = salir)
    float dineroDepositado; //Rendimiento del usuario para saber cuanto puso de su bolsillo
    float gananciaAdmin;    //Rendimiento comercial: cuanto le gano/perdio el casino al usuario
    char usuarioBaneado[50]; //Historial del slot en caso de baneo 
}Usuario_t;

// ENUMERACIONES DEL SISTEMA PRINCIPAL
// Permiten asociar nombres legibles a las opciones del menú principal del casino
typedef enum{
    OPC_INICIAR_SESION = 1,
    OPC_REGISTRARSE,
    OPC_SALIR_CASINO,
}OpcionesPrincipal_t;

typedef enum{
    OPC_LISTAR_USUARIOS = 1,
    OPC_MODIFICAR_SALDO,
    OPC_GESTION_BANEOS,
    OPC_SALIR_MENU_ADMIN,
}OpcionesAdmin_t;

typedef enum{
    OPC_TRAGAMONEDAS = 1,
    OPC_CARGAR_SALDO,
    OPC_SALIR_MENU_CASINO,
}OpcionesCasino_t;

// ENUMERACIONES DEL MENU DE AJUSTE DE SALDO DEL ADMINISTRADOR
typedef enum{
    OPC_SUMAR_SALDO = 1,
    OPC_RESTAR_SALDO,
    OPC_RETIRAR_SALDO,
    OPC_SALIR_MENU_AJUSTE,
}OpcionesAjusteAdmin_t;


//===============================================================================
// PROTOTIPOS DE FUNCIONES DEL SISTEMA PRINCIPAL
//===============================================================================

void ConvertirAMinusculas(char *cadena);
int IniciarSesion(Usuario_t listaUsuarios[], int max);
void MenuAdmin(Usuario_t listaUsuarios[],int max);
void RegistrarUsuario(Usuario_t listaUsuarios[], int max, const char palabrasAdmin[][50], int cantNoAdmin, int edadMin, int edadMax);
void MenuCasino(Usuario_t listaUsuarios[], int idSesion);
void CargarSaldo(Usuario_t *usuario);
void GestionarUsuariosYBaneos(Usuario_t listaUsuarios[], int max);
void ModificarSaldoAdministrativo(Usuario_t listaUsuarios[], int max);

// Funciones auxiliares de validación y contabilidad
bool LeerEnteroValido(int *valor);
bool LeerFloatValido(float *valor);
void ActualizarCajaCasino(float *caja, float monto, int operacion);

void LimpiarPantalla(void);

#endif