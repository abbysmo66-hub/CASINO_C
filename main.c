#include <stdio.h>
#include <stdlib.h> //Utilidades del sistema (srand, rand)
#include <time.h>   //Manejo de tiempo para la aleatoriedad
#include <string.h> //Cadenas de texto (strcmp, strcpy, strlen)
#include <stdbool.h> //Soporte para el tipo de dato booleano (bool, true, false)
#include <ctype.h>  //Funciones para caracteres (tolower)

//Incluimos unicamente el archivo cabecera de nuestro modulo poker
//Esto nos da acceso a las estructuras y prototipos declarados en poker.h
#include "poker.h"

//Incluimos unicamente el archivo cabecera de nuestro modulo tragamonedas
//Esto nos da acceso a las estructuras y prototipos declarados en TRAGAMONEDAS.h
#include "TRAGAMONEDAS.h"

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
#define MAX_SALDO_POKER 50000.0f    //Se usa 'f' para forzar tipo float (32 bits) no double (64bits)
#define MAX_SALDO_TRAGAMONEDAS 500.0f
#define MAX_SALDO_RULETA 500.0f
#define MAX_SALDO_BLACKJACK 50000.0f

// ENUMERACIONES DEL SISTEMA PRINCIPAL
// Permiten asociar nombres legibles a las opciones del menú principal del casino
typedef enum{
    OPC_INICIAR_SESION = 1,
    OPC_REGISTRARSE,
    OPC_SALIR_CASINO,
}OpcionesPrincipal;

typedef enum{
    OPC_LISTAR_USUARIOS = 1,
    OPC_MODIFICAR_SALDO,
    OPC_ELIMINAR_USUARIO,
    OPC_SALIR_MENU_ADMIN,
}OpcionesAdmin;

typedef enum{
    OPC_RULETA = 1,
    OPC_TRAGAMONEDAS,
    OPC_POKER,
    OPC_BLACKJACK,
    OPC_CARGAR_SALDO,
    OPC_SALIR_MENU_CASINO,
}OpcionesCasino;

// ENUMERACIONES DEL MENU DE AJUSTE DE SALDO DEL ADMINISTRADOR
typedef enum{
    OPC_SUMAR_SALDO = 1,
    OPC_RESTAR_SALDO,
    OPC_FIJAR_SALDO,
    OPC_SALIR_MENU_AJUSTE,
}OpcionesAjusteAdmin;

//===============================================================================
// PROTOTIPOS DE FUNCIONES DEL SISTEMA PRINCIPAL
//===============================================================================

void ConvertirAMinusculas(char *cadena);
int IniciarSesion(Usuario listaUsuarios[], int max);
void MenuAdmin(Usuario listaUsuarios[],int max);
void RegistrarUsuario(Usuario listaUsuarios[], int max, const char palabrasAdmin[][50], int cantNoAdmin, int edadMin, int edadMax);
void MenuCasino(Usuario listaUsuarios[], int idSesion);
void CargarSaldo(float *saldoUsuario);




int main(void){

    //Inicializa la semilla aleatoria para el azar
    srand((unsigned int)time(NULL));

    //Declaracion del arreglo estatico de estructuras de usuarios
    Usuario listaUsuarios[MAX_USUARIOS];

    // PRECARGA DE DATOS INICIALES
    //Precarga del usuario 1 (Administrador)
    strcpy(listaUsuarios[0].usuario, "admin");
    strcpy(listaUsuarios[0].clave, "1234");
    listaUsuarios[0].saldo = 5000.0f;
    listaUsuarios[0].edad = 30;
    listaUsuarios[0].activo = true;

    //Precarga del usuario 2 (Jugador de prueba)
    strcpy(listaUsuarios[1].usuario, "jugador1");
    strcpy(listaUsuarios[1].clave, "0000");
     listaUsuarios[1].saldo = 1000.0f;
    listaUsuarios[1].edad = 27;
    listaUsuarios[1].activo = true;

     //Precarga del usuario 3 (Vacia)
    strcpy(listaUsuarios[2].usuario, "");
    strcpy(listaUsuarios[2].clave, "");
     listaUsuarios[2].saldo = 0.0f;
    listaUsuarios[2].edad = 0;
    listaUsuarios[2].activo = false;

    //Palabras prohibidas para evitar registros con nombres de administrador
    const char palabrasAdmin[NO_ADMIN][50] = {"admin", "administrador"};


    int opcionSeleccion = 0;
    int idSesion = -1; //-1 indica que no hay ninguna sesion iniciada

    //Bucle del Menu principal
    do{
        system("clear || cls");
        printf("\n==========================================\n");
        printf("          BIENVENIDO AL CasiNOgano          \n");
        printf("\n==========================================\n");
        printf("%d. Iniciar Sesion.\n", OPC_INICIAR_SESION);
        printf("%d. Registrarse.\n", OPC_REGISTRARSE);
        printf("%d. Salir del Casino.\n", OPC_SALIR_CASINO);
        printf("\n==========================================\n");
        printf("Seleccione una opcion: \n");

         if(!LeerEnteroValido(&opcionSeleccion)){
            opcionSeleccion = 0; //Si ingresan letras o basura, fuerza 0 para caer en 'default'
        }

        switch(opcionSeleccion){
            case OPC_INICIAR_SESION:
                idSesion = IniciarSesion(listaUsuarios, MAX_USUARIOS);
                if(idSesion != -1){
                    //Evaluacion de roles por comparacion de cadenas (strcmp devuelve 0 si son identicas)
                    if(strcmp(listaUsuarios[idSesion].usuario, "admin") == 0){
                        MenuAdmin(listaUsuarios, MAX_USUARIOS);
                    }else{
                        MenuCasino(listaUsuarios, idSesion);
                    }
                }
                break;
            case OPC_REGISTRARSE:
                RegistrarUsuario(listaUsuarios, MAX_USUARIOS, palabrasAdmin, NO_ADMIN, EDAD_MIN, EDAD_MAX);
                break;
            case OPC_SALIR_CASINO:
                printf("\nGracias por visitar el casino.Hasta luego!\n");
                break;
            default:
                printf("[X] Error: Ingrese un numero valido.\n");
                printf("Presione <<ENTER>> para continuar\n");
                getchar();  //Pausa para esperar la tecla del usuario
                break;
       }
    }while (opcionSeleccion != OPC_SALIR_CASINO);

return 0;
}

//Convierte toda una cadena a minusculas para comparaciones uniformes
void ConvertirAMinusculas(char *cadena){
    for(int i=0; cadena[i]; i++){
        cadena[i] = tolower((unsigned char)cadena[i]);
    }
}

//============================================================================
//AUTENTICACI�N Y REGISTRO DE USUARIOS
//============================================================================

//Busca coincidencia de credenciales entre las cuentas activas
int IniciarSesion(Usuario listaUsuarios[], int max){
    char usuario[50], contrasena[50];
    system("clear || cls");

    printf("=== INICIO DE SESION ===\n");
    printf("Usuario: ");

    //Lee el texto de forma segura limitando a 49 caracteres
    //para evitar un desbordamiento de bufer
    scanf("%49s", usuario);

    printf("Clave: ");
    scanf("%49s", contrasena);
    while(getchar() !=  '\n'); //Limpia el bufer residual de scanf

    for(int i = 0; i < max; i++){
        if((listaUsuarios[i].activo)&&(strcmp(listaUsuarios[i].usuario,usuario) == 0)&&(strcmp(listaUsuarios[i].clave, contrasena) == 0)){
                return i; //Retorna la posicion del usuario en el arreglo (ID de Sesion)
        }
    }

    printf("\nUsuario o clave incorrectos.\n");
    printf("Presiones <<ENTER>> para continuar...");
    getchar();
    return -1;

}

//Proceso de registro con validaciones de edad mediante LeerEnteroValido, caracteres y duplicados
void RegistrarUsuario(Usuario listaUsuarios[], int max, const char palabrasAdmin[][50], int cantNoAdmin, int edadMin, int edadMax){
    int posLibre = -1;

    for(int i = 0; i < max; i++){
        if(!listaUsuarios[i].activo){
            posLibre = i;
            break;
        }
    }

    if (posLibre == -1){
        printf("\nLo sentimos, el casino no acepta mas usuarios (Cupos Llenos).\n");
        printf("Presione <<ENTER>> �para continuar...");
        getchar();
        return;
    }

    char nuevoUsuario[50], nuevaClave[50];
    int nuevaEdad = 0;

    system("clear || cls");
    printf("=== REGISTRO DE USUARIO ===\n");

    //Validacion de Edad utilizando directamente la funcion del modulo de poker
    do{
        printf("Ingrese su edad (permitido %d a %d a�os): ", edadMin, edadMax);

        if(!LeerEnteroValido(&nuevaEdad)){
            nuevaEdad = -1; //Fuerza un valor invalido si ingresan letras
        }

        if((nuevaEdad <  edadMin) || (nuevaEdad > edadMax)){
            printf("[X]Error: Debe ingresar un numero entero valido entre %d y %d.\n\n", edadMin, edadMax);
        }
    }while((nuevaEdad <  edadMin) || (nuevaEdad > edadMax));

    //Validacion de Nombre de usuario
    bool nombreCorrecto = false;
    char usuarioMinuscula[50];

   do {
        printf("Ingrese nombre de usuario (Debe llevar letras, pero puede incluir numeros): ");

       // Lectura segura verificando que se reciba la cadena correctamente
        if(scanf("%49s", nuevoUsuario) != 1) {
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n'); // Limpia el b�fer de forma segura

        // 1. Convertimos a min�sculas para evaluar
        strcpy(usuarioMinuscula, nuevoUsuario);
        ConvertirAMinusculas(usuarioMinuscula);

        bool tieneLetra = false;
        bool tieneSimboloInvalido = false;

        for(int j = 0; usuarioMinuscula[j] != '\0'; j++) {
            char c = usuarioMinuscula[j];
            if((c >= 'a') && (c <= 'z')) {
                tieneLetra = true;
            } else if((c >= '0') && (c <= '9')) {
                // N�mero permitido, no hace nada
            } else {
                // Cualquier otra cosa (espacios, s�mbolos) invalida
                tieneSimboloInvalido = true;
            }
        }

        // 2. Verificamos si es palabra prohibida de admin
        bool esAdminProhibido = false;
        for(int i = 0; i < cantNoAdmin; i++) {
            char adminMin[50];
            strcpy(adminMin, palabrasAdmin[i]);
            ConvertirAMinusculas(adminMin);
            if(strcmp(usuarioMinuscula, adminMin) == 0) {
                esAdminProhibido = true;
                break;
            }
        }

        // 3. Verificamos si ya existe en el arreglo
        bool yaExiste = false;
        for(int i = 0; i < max; i++) {
            if(listaUsuarios[i].activo && strcmp(listaUsuarios[i].usuario, nuevoUsuario) == 0) {
                yaExiste = true;
                break;
            }
        }

        // 4. Evaluamos errores y mostramos mensajes claros
        if(tieneSimboloInvalido) {
            printf("[X] Error: El usuario no puede contener simbolos ni espacios.\n\n");
            nombreCorrecto = false;
        } else if(!tieneLetra) {
            printf("[X] Error: El nombre de usuario no puede ser solo numeros, debe tener letras.\n\n");
            nombreCorrecto = false;
        } else if(esAdminProhibido) {
            printf("[X] Error: Este nombre de usuario no esta permitido.\n\n");
            nombreCorrecto = false;
        } else if(yaExiste) {
            printf("[X] Error: Ese nombre de usuario ya esta en uso. Intente con otro.\n\n");
            nombreCorrecto = false;
        } else {
            nombreCorrecto = true; // �Todo OK! Rompe el do-while
        }

    } while(!nombreCorrecto);

    printf("Ingrese clave: ");
    if(scanf("%49s", nuevaClave) == 1) {
        while(getchar() != '\n');
    }

    //Guardado de datos de la posicion libre encontrada
    listaUsuarios[posLibre].activo = true;
    strcpy(listaUsuarios[posLibre].usuario, nuevoUsuario);
    strcpy(listaUsuarios[posLibre].clave, nuevaClave);
    float saldito = 1000.0f;
    listaUsuarios[posLibre].saldo = saldito;
    listaUsuarios[posLibre].edad = nuevaEdad;

    printf("\n�Usuario registrado con exito! Saldo inicial de obsequio: $%.2f\n",saldito);
    printf("Presione <<ENTER>> para continuar...");
    getchar();
}

void MenuAdmin(Usuario listaUsuarios[], int max){
    int opcionAdmin = 0;
    char usuarioBuscar[50];

    do{
        system("clear || cls");
        printf("===================================================\n");
        printf("       PANEL DE CONTROL (ADMINISTRADOR)            \n");
        printf("===================================================\n");
        printf("%d. Listar todos los usuarios y saldos\n", OPC_LISTAR_USUARIOS);
        printf("%d. Modificar saldo a un usuario\n", OPC_MODIFICAR_SALDO);
        printf("%d. Banear / Eliminar usuario\n", OPC_ELIMINAR_USUARIO);
        printf("%d. Cerrar Sesion de Administrador\n", OPC_SALIR_MENU_ADMIN);
        printf("===================================================\n");
        printf("Seleccione una opcion: ");

        if (!LeerEnteroValido(&opcionAdmin)) {
            opcionAdmin = 0;
        }

        switch(opcionAdmin){
            case OPC_LISTAR_USUARIOS:
                system("clear || cls");
                printf("=== REPORTES DEL SISTEMA ===\n");
                printf("POS | USUARIO         | EDAD    | SALDO\n");
                printf("------------------------------------------\n");
                // Recorre todo el arreglo de usuarios buscando los activos
                for(int i = 0; i < max; i++){
                    if(listaUsuarios[i].activo){
                        // Imprime los datos formateados. %-15s alinea el texto a la izquierda
                        printf("[%d]  %-15s | %d anos | $%.2f\n", i, listaUsuarios[i].usuario, listaUsuarios[i].edad, listaUsuarios[i].saldo);
                    }else{
                        // Si la casilla está libre, le avisa al administrador
                        printf("[%d]  (Espacio Libre)\n", i);
                    }
                }

                printf("---------------------------------------------\n");
                printf("Presione <<ENTER>> para continuar...");
                getchar();
                break;

            case OPC_MODIFICAR_SALDO:{
                // CASO 2: MODIFICAR SALDO DE UN USUARIO ESPECIFICO
                system("clear || cls");
                printf("=== MODIFICAR SALDO DE USUARIO ===\n");
                printf("Ingrese el NOMBRE de usuario a modificar: ");
                scanf("%49s", usuarioBuscar);
                while(getchar() != '\n');

                int usuarioModificar = -1;

                // Búsqueda secuencial para encontrar al usuario (protegiendo la cuenta "admin")
                for(int i = 0; i < max; i++){
                    if((listaUsuarios[i].activo) && (strcmp(listaUsuarios[i].usuario, usuarioBuscar) == 0) && (strcmp(usuarioBuscar, "admin") != 0)){
                        usuarioModificar = i; // Guarda la posicion exacta del arreglo
                        break;
                    }
                }

                // Si se encontró el usuario válido
                if(usuarioModificar != -1){
                    int opcionAjuste = 0;
                    do{
                        system("clear || cls");
                        printf("=== AJUSTE DE SALDO PARA USUARIO ===\n");
                        printf("\nUsuario seleccionado: %s\n", listaUsuarios[usuarioModificar].usuario);
                        printf("Saldo actual: $%.2f\n", listaUsuarios[usuarioModificar].saldo);
                        printf("----------------------------------------------\n");
                        printf("1. Sumar saldo\n");
                        printf("2. Restar saldo\n");
                        printf("3. Fijar saldo (sobreescribir)\n");
                        printf("4. Salir\n");
                        printf("Seleccione la operacion: ");

                        if(!LeerEnteroValido(&opcionAjuste)){
                            opcionAjuste = 0;
                        }

                        float montoAjuste = 0.0f;
                        if((opcionAjuste >= 1) && (opcionAjuste <= 3)){
                            printf("Ingrese el monto: $");
                            // Lee el monto de manera segura validando que sean numeros flotantes
                            if(!LeerFloatValido(&montoAjuste)){
                                // Se le asigna -1.0f como código de error interno
                                montoAjuste = -1.0f;
                            }
                        }

                        // Ejecución de la transacción contable usando switch
                        switch(opcionAjuste){
                            case OPC_SUMAR_SALDO:
                                if(montoAjuste > 0){
                                    listaUsuarios[usuarioModificar].saldo += montoAjuste;
                                    printf("¡Monto acreditado! Nuevo Saldo: $%.2f\n", listaUsuarios[usuarioModificar].saldo);
                                }else{
                                    printf("[X]Error: Monto invalido.\n");
                                }
                                break;

                            case OPC_RESTAR_SALDO:
                                if((montoAjuste > 0) && (listaUsuarios[usuarioModificar].saldo >= montoAjuste)){
                                    listaUsuarios[usuarioModificar].saldo -= montoAjuste;
                                    printf("¡Monto debitado! Nuevo saldo: $%.2f\n", listaUsuarios[usuarioModificar].saldo);
                                }else{
                                    printf("[X]Error: Monto invalido o saldo insuficiente.\n");
                                }
                                break;

                            case OPC_FIJAR_SALDO:
                                if(montoAjuste >= 0){
                                    listaUsuarios[usuarioModificar].saldo = montoAjuste;
                                    printf("¡Saldo fijado correctamente! Nuevo saldo: $%.2f\n", listaUsuarios[usuarioModificar].saldo);
                                }else{
                                    printf("[X]Error: Monto invalido.\n");
                                }
                                break;

                            case OPC_SALIR_MENU_AJUSTE:
                                printf("Saliendo del menu de ajustes...\n");
                                printf("Presione <<ENTER>> para continuar...");
                                getchar(); // Pausa antes de salir
                                break;

                            default:
                                printf("[X]Error: Operacion invalida.\n");
                                break;
                        }

                        // Pausa solo si no eligió salir
                        if(opcionAjuste != OPC_SALIR_MENU_AJUSTE){
                            printf("Presione <<ENTER>> para continuar...");
                            getchar();
                        }

                    }while(opcionAjuste != OPC_SALIR_MENU_AJUSTE);

                }else{
                    // Mensajes de error personalizados según el caso
                    if(strcmp(usuarioBuscar, "admin") == 0){
                        printf("[X]Error: No esta permitido modificar el saldo de la cuenta de administrador.\n");
                    }else{
                        printf("[X]Error: El usuario no existe en el sistema.\n");
                    }
                    printf("Presione <<ENTER>> para continuar...");
                    getchar();
                }
                break;
            }

            case OPC_ELIMINAR_USUARIO:
                // BANEAR / ELIMINAR USUARIO
                system("clear || cls");
                printf("=== BANEAR / ELIMINAR USUARIO ===\n");
                printf("Ingrese el NOMBRE del usuario a banear: ");
                scanf("%49s", usuarioBuscar);
                while(getchar() != '\n');

                int posBan = -1;
                for (int i = 0; i < max; i++){
                    if((listaUsuarios[i].activo) && (strcmp(listaUsuarios[i].usuario, usuarioBuscar) == 0) && (strcmp(usuarioBuscar, "admin") != 0)){
                        posBan = i;
                        break;
                    }
                }

                if(posBan != -1){
                    // Para banear, marcamos el casillero como inactivo y blanqueamos sus datos para liberar el cupo
                    listaUsuarios[posBan].activo = false;
                    strcpy(listaUsuarios[posBan].usuario, "");
                    strcpy(listaUsuarios[posBan].clave, "");
                    listaUsuarios[posBan].saldo = 0.0f;
                    listaUsuarios[posBan].edad = 0;
                    printf("¡El usuario %s ha sido baneado y su cupo ha sido liberado!\n", usuarioBuscar);
                }else{
                    if(strcmp(usuarioBuscar, "admin") == 0){
                        printf("[X]Error: No esta permitido banear a la cuenta de administrador.\n");
                    }else{
                        printf("[X]Error: El usuario no existe en el sistema.\n");
                    }
                }
                printf("Presione <<ENTER>> para continuar...");
                getchar();
                break;

            case OPC_SALIR_MENU_ADMIN:
                printf("Cerrando panel de administracion...\n");
                break;

            default:
                printf("Opcion no valida.\n");
                printf("Presione <<ENTER>> para continuar...");
                getchar();
                break;
        }
    }while(opcionAdmin != OPC_SALIR_MENU_ADMIN);
}

void CargarSaldo(float *saldoUsuario){
    float monto = 0.0f;

    system("clear || cls");
    printf("=== CARGA DE SALDO ===\n");
    printf("Saldo actual: $%.2f\n", *saldoUsuario);
    printf("Ingrese el monto a ingresar: $");

    //Valida la entrada usando la funcion segura del poker
    if(!LeerFloatValido(&monto)){
        monto = 0.0f;
    }

    //Si el monto ingresado es mayor a 0, se le suma a la cuenta
    if(monto > 0){
        *saldoUsuario += monto; //Modifica el valor original mediante el puntero
        printf("�Carga exitosa! Nuevo saldo: $%.2f\n",*saldoUsuario);
    }else{
        printf("Monto invalido. La carga debe ser mayor a 0.\n");
    }
    printf("Presione <<ENTER>> para continuar...");
    getchar();
}
//============================================================================
//MENU DE CASINO (Sala de Juegos del Usuario Estandar)
//============================================================================
void MenuCasino(Usuario listaUsuarios[], int idSesion){
    int opcionJuego = 0;

    do {
        system("clear || cls");
        printf("==================================================\n");
        printf("  USUARIO ACTIVO: %s\n", listaUsuarios[idSesion].usuario);
        printf("  SALDO DISPONIBLE: $%.2f\n", listaUsuarios[idSesion].saldo);
        printf("==================================================\n");
        printf("%d. Jugar Ruleta\n", OPC_RULETA);
        printf("%d. Jugar Tragamonedas (Slot)\n", OPC_TRAGAMONEDAS);
        printf("%d. Jugar Poker\n", OPC_POKER);
        printf("%d. Jugar BlackJack\n", OPC_BLACKJACK);
        printf("%d. Cargar Saldo\n", OPC_CARGAR_SALDO);
        printf("%d. Cerrar Sesion.\n", OPC_SALIR_MENU_CASINO);
        printf("==================================================\n");
        printf("Seleccione una opcion: ");

        if(!LeerEnteroValido(&opcionJuego)){
            opcionJuego = 0;
        }

        switch(opcionJuego){
            case OPC_RULETA:
                //Ruleta
                printf("Presione ENTER para continuar...");
                getchar();
                break;
            case OPC_TRAGAMONEDAS:
                //Validacion de saldo minimo requerido antes de permitir el ingreso al juego
                if(listaUsuarios[idSesion].saldo >= MAX_SALDO_TRAGAMONEDAS){
                    
                    //Inicializamos la bandera en true para que el bucle del tragamonedas arranque 
                    listaUsuarios[idSesion].continuar = true;

                    //Pasamos la direccion de memoria de todoo el usuario (&)
                    tragamonedas(&(listaUsuarios[idSesion]));
                }else {
                    printf("Error: Saldo insuficiente ($%.2f). Requiere al menos $%.2f\n", listaUsuarios[idSesion].saldo, MAX_SALDO_TRAGAMONEDAS);
                    printf("Por favor cargue saldo (opcion 5)\n");
                    printf("Presione ENTER para continuar...");
                    getchar();
                }
                break;
            case OPC_POKER:
                //Validacion de saldo minimo requerido antes de permitir el ingreso al juego
                if(listaUsuarios[idSesion].saldo >= MAX_SALDO_POKER){
                     //Pasamos la direccion de memoria del saldo (&) para que las apuestas,
                    //ganancias o perdidas del poker impacten directamente en la cuenta del usuario
                    AnimacionBienvenida();
                    MenuJuegoPoker(&(listaUsuarios[idSesion].saldo));
                }else {
                    printf("Error: Saldo insuficiente ($%.2f). Requiere al menos $%.2f\n", listaUsuarios[idSesion].saldo, MAX_SALDO_POKER);
                    printf("Por favor cargue saldo (opcion 5)\n");
                    printf("Presione ENTER para continuar...");
                    getchar();
                }
                break;
            case OPC_BLACKJACK:
                //BlackJack
                printf("Presione ENTER para continuar...");
                getchar();
                break;
            case OPC_CARGAR_SALDO:
                //Llamamos a la funcion de carga enviando la direccion de memoria del saldo
                CargarSaldo(&(listaUsuarios[idSesion].saldo));
                break;
            case OPC_SALIR_MENU_CASINO:
                printf("Cerrando sesion de usuario...\n");
                getchar();
                break;              
            default:
                printf("Opcion no valida. Vuelva a intentar.\n");
                printf("Presione <<ENTER>> para continuar...");
                getchar();
                break;
        }
    }while(opcionJuego != OPC_SALIR_MENU_CASINO);
}


