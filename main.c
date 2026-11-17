
//Incluimos unicamente el archivo cabecera de nuestro modulo poker
//Esto nos da acceso a las estructuras y prototipos declarados en poker.h4
#include "main.h"
#include "TRAGAMONEDAS.h"

//Monto de la caja global del casino
float cajaCasino = 2000000.0f;

int main(void){

    //Forzamos la codificacion UTF-8 en la consola para que los bordes (╔, ═, ╝) se vean perfectos
    system("chcp 65001 > nul");

    //Inicializa la semilla aleatoria para el azar
    srand((unsigned int)time(NULL));

    //Declaracion del arreglo estatico de estructuras de usuarios
    Usuario_t listaUsuarios[MAX_USUARIOS];

    // PRECARGA DE DATOS INICIALES
    //Precarga del usuario 1 (Administrador)
    strcpy(listaUsuarios[0].usuario, "admin");
    strcpy(listaUsuarios[0].clave, "1234");
    listaUsuarios[0].saldo = 0.0f;
    listaUsuarios[0].edad = 30;
    listaUsuarios[0].activo = true;

    //Precarga del usuario 2 (Jugador de prueba)
    strcpy(listaUsuarios[1].usuario, "jugador1");
    strcpy(listaUsuarios[1].clave, "0000");
    listaUsuarios[1].saldo = 1000.0f;
    listaUsuarios[1].edad = 27;
    listaUsuarios[1].activo = true;
    listaUsuarios[1].gananciaAdmin = -1000.0f;
    listaUsuarios[1].dineroDepositado = 1000.0f;

    //Descontamos su saldo inicial de la caja central para mantener la contabilidad exacta
    ActualizarCajaCasino(&cajaCasino, listaUsuarios[1].saldo, 1);

    
    //Precarga de usuarios vacios
    for(int i=2; i<MAX_USUARIOS;i++){
        strcpy(listaUsuarios[i].usuario, "");
        strcpy(listaUsuarios[i].clave, "");
        listaUsuarios[i].saldo = 0.0f;
        listaUsuarios[i].edad = 0;
        listaUsuarios[i].activo = false;
        listaUsuarios[i].gananciaAdmin = 0.0f;
        listaUsuarios[i].dineroDepositado = 0.0f;
    }
    

    //Palabras prohibidas para evitar registros con nombres de administrador
    const char palabrasAdmin[NO_ADMIN][50] = {"admin", "administrador"};


    int opcionSeleccion = 0;
    int idSesion = -1; //-1 indica que no hay ninguna sesion iniciada

    //Bucle del Menu principal
    do{
       LimpiarPantalla();

        // ====================================================================
        // MEJORA VISUAL: Cabecera con marcos de doble línea y colores elegantes
        // ====================================================================
        printf("\n" T_AMARILLO "╔════════════════════════════════════════════════╗\n");
        printf("║          BIENVENIDO AL CASINO - GANO           ║\n");
        printf("╚════════════════════════════════════════════════╝\n" T_RESET);

        // Opciones del menú formateadas con colores y corchetes numéricos
        printf(T_CIAN "  [%d] " T_BLANCO "Iniciar Sesión\n", OPC_INICIAR_SESION);
        printf(T_CIAN "  [%d] " T_BLANCO "Registrarse\n", OPC_REGISTRARSE);
        printf(T_CIAN "  [%d] " T_ROJO   "Salir del Casino\n" T_RESET, OPC_SALIR_CASINO);
        
        printf(T_AMARILLO "--------------------------------------------------\n" T_RESET);
        printf("Seleccione una opcion: ");

        //Validacion segura de la entrada del usuario
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
                printf("\n" T_VERDE "Gracias por visitar el casino. Hasta luego!\n" T_RESET);
                break;
            default:
                printf("\n" T_ROJO "[X] Error: Ingrese un numero valido.\n" T_RESET);
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
//AUTENTICACION Y REGISTRO DE USUARIOS
//============================================================================

//Busca coincidencia de credenciales entre las cuentas activas
int IniciarSesion(Usuario_t listaUsuarios[], int max){
    char usuario[50], contrasena[50];
    LimpiarPantalla();

    printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
    printf("║         " T_VERDE " 🪙 INICIO DE SESIÓN - VIP 🪙 " T_AMARILLO "            ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("                                                   \n");
    printf("    " T_BLANCO "Usuario : " T_RESET);
    scanf("%49s", usuario);
    while(getchar() != '\n');

    printf("    " T_BLANCO "Clave   : " T_RESET);


    // --- LÓGICA DE CONTRASEÑA OCULTA CON ASTERISCOS (*) ---
    int i = 0;
    int c;
    
    while (1) {
        c = getch(); // Lee cada tecla de forma silenciosa sin mostrarla en pantalla
        
        // Si presiona ENTER (códigos ASCII 13 o 10, o '\r' / '\n')
        if (c == 13 || c == 10 || c == '\r' || c == '\n') {
            contrasena[i] = '\0'; // Cierra la cadena de texto de forma segura
            printf("\n");
            break;
        }
        // Si presiona BACKSPACE (Borrar, código ASCII 8)
        else if (c == 8) {
            if (i > 0) {
                i--;
                printf("\b \b"); // Borra el asterisco visualmente en la consola
            }
        }
        // Si es un carácter imprimible válido y hay espacio disponible
        else if (i < 49 && c >= 32 && c <= 126) {
            contrasena[i++] = (char)c;
            printf("*"); // Imprime visualmente un asterisco por cada letra
        }
    }
    
    printf("\n" T_RESET);
    // -------------------------------------------------------


    for(int i = 0; i < max; i++){
        if((listaUsuarios[i].activo)&&(strcmp(listaUsuarios[i].usuario,usuario) == 0)&&(strcmp(listaUsuarios[i].clave, contrasena) == 0)){
                return i; //Retorna la posicion del usuario en el arreglo (ID de Sesion)
        }
    }

    printf("\n" T_ROJO "Usuario o clave incorrectos." T_RESET "\n");
    printf("Presiones <<ENTER>> para continuar...");
    getchar();
    return -1;

}

//Proceso de registro con validaciones de edad mediante LeerEnteroValido, caracteres y duplicados
void RegistrarUsuario(Usuario_t listaUsuarios[], int max, const char palabrasAdmin[][50], int cantNoAdmin, int edadMin, int edadMax){
    int posLibre = -1; //Bandera para encontrar la posicion libre (-1 = lleno)


    for(int i = 0; i < max; i++){
        if(!listaUsuarios[i].activo){ 
            posLibre = i;
            break;
        }
    }

    if (posLibre == -1){
        LimpiarPantalla();
        printf("\n" T_ROJO "╔═════════════════════════════════════════════════╗\n");
        printf("║             [!] CUPOS LLENOS [!]                ║\n");
        printf("╚═════════════════════════════════════════════════╝" T_RESET "\n");
        printf("Lo sentimos, el casino no acepta más usuarios.\n");
        printf("Presione <<ENTER>> para continuar...");
        while(getchar() != '\n');
        getchar();
        return;
    }

    char cancelarRegistro[10];

   LimpiarPantalla();
    printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
    printf("║         " T_CIAN " 🪪 CONTROL DE INGRESO - VIP 🪪 " T_AMARILLO "          ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("║ ¿Desea continuar con el registro?               ║\n");
    printf("║ (" T_VERDE "S" T_BLANCO ": Sí / " T_ROJO "N" T_BLANCO ": No) : " T_RESET);
    scanf("%4s", cancelarRegistro);
    while(getchar() != '\n');

    //Convertimos la entrada a mayusculas para comparar facilmente
    //Uso: permite unificar el criterio de evaluacion sin importar si escribieron en minusculas o mayuscula
    for(int i=0; cancelarRegistro[i];i++){
        cancelarRegistro[i] = toupper((unsigned char)cancelarRegistro[i]);
    }

    //Bucle de validacion estricta de la opcion de control
    //Uso: Obliga al usuario a escribir unicamente "S" o "N", rechazando cualquier otra entrada incorrecta
    while(strcmp(cancelarRegistro, "S")!= 0 && strcmp(cancelarRegistro, "N")!=0){
        printf(T_ROJO "[X] Opción no válida. Coloque 'S' o 'N': " T_RESET);
        scanf("%4s",cancelarRegistro);
        while(getchar() != '\n');
        for(int i=0; cancelarRegistro[i]; i++){
            cancelarRegistro[i] = toupper((unsigned char)cancelarRegistro[i]);
        }
    }

    //Condicional de cacncelacion del registro
    //Uso: Si el usuario eligio "N", muestra un aviso y retorna al menu principal sin ocupar ningun slot
    if(strcmp(cancelarRegistro, "N") == 0){
        printf("\n" T_AMARILLO "[!] Registro cancelado. Volviendo al menú principal...\n" T_RESET);
        Sleep(1500); //Pauda visual de 1.5 segundos para que el mensaje sea legible 
        return;
    }

    //Formulario de regstro de datos personales
    char nuevoUsuario[50], nuevaClave[50];     //Variables temporales para el nuevo usaurio y contraseña
    int nuevaEdad = 0;                         //Variable para almacenar la edad ingresada
    int minCaracteres = 6;                     //Constante local para exigir el minimo estricto de 6 caracteres

    system("clear || cls");
    printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
    printf("║         " T_VERDE " 📝 FORMULARIO DE REGISTRO 📝 " T_AMARILLO "          ║\n");
    printf("╠═════════════════════════════════════════════════╣\n" T_RESET);


    //Validacion de Edad utilizando directamente la funcion del modulo de poker
    do{
       
        printf(" Ingrese su edad (" T_CIAN "permitido %d a %d años" T_RESET "): ", edadMin, edadMax);

        if(!LeerEnteroValido(&nuevaEdad)){
            nuevaEdad = -1; //Fuerza un valor invalido si ingresan letras
        }

        if((nuevaEdad <  edadMin) || (nuevaEdad > edadMax)){
           printf(T_ROJO "[X] Error: Ingrese un número válido entre %d y %d.\n\n" T_RESET, edadMin, edadMax);
        }
    }while((nuevaEdad <  edadMin) || (nuevaEdad > edadMax));

    //Validacion de Nombre de usuario
    bool nombreCorrecto = false;  //Bandera de aprobacion para el nomnbre de usuario
    char usuarioMinuscula[50];    //Copia auxiliar para evaluar el nombre en minusculas

   do {
        printf(" Ingrese nombre de usuario (" T_CIAN "mín. 6 chars, letras/números" T_RESET "): ");

       // Lectura segura verificando que se reciba la cadena correctamente
        if(scanf("%49s", nuevoUsuario) != 1) {
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n'); // Limpia el bufer de forma segura

        //Validacion de la longitud minima de 6 caracteres requerida
        //Uso: Rechaza inmediatamente nombres cortos que no cumplen con la normativa
        if(strlen(nuevoUsuario) < minCaracteres){
            printf(T_ROJO "[X] Error: Muy corto. Al menos %d caracteres.\n\n" T_RESET, minCaracteres);
            nombreCorrecto = false;
            continue;
        }

        //Copiamos y convertimos el nomnre de usuario a minusculas para evaluar
        strcpy(usuarioMinuscula, nuevoUsuario);
        ConvertirAMinusculas(usuarioMinuscula);

        bool tieneLetra = false;
        bool tieneSimboloInvalido = false;
    
        //Recorrido caracter por caracter para validar formato alfanumerico
        //Uso: Asegura que contenga al menos una letra y proibe espacios o simbolos especiales
        for(int j = 0; usuarioMinuscula[j] != '\0'; j++) {
            char c = usuarioMinuscula[j];
            if((c >= 'a') && (c <= 'z')) {
                tieneLetra = true;
            } else if((c >= '0') && (c <= '9')) {
                // Numero permitido, no hace nada
            } else {
                // Cualquier otra cosa (espacios, simbolos) invalida
                tieneSimboloInvalido = true;
            }
        }

        // Validacion de palabras prohibidas de administrador 
        //Uso: Impide que un cliente suplante la identidad del administrador corporativo del casino
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

        //Validacion de usuario ya existente en el sistema
        //Uso: Compara el nombre ingresado con el resto de cuentas activas para evitar duplicados
        bool yaExiste = false;
        for(int i = 0; i < max; i++) {
            if(listaUsuarios[i].activo && strcmp(listaUsuarios[i].usuario, nuevoUsuario) == 0) {
                yaExiste = true;
                break;
            }
        }

        //Evaluacion final de errores con mensajes descriptivos especificos para cada situacion
        if(tieneSimboloInvalido) {
            printf(T_ROJO "[X] Error: Sin símbolos ni espacios.\n\n" T_RESET);
            nombreCorrecto = false;
        } else if(!tieneLetra) {
            printf(T_ROJO "[X] Error: Debe contener al menos una letra.\n\n" T_RESET);
            nombreCorrecto = false;
        } else if(esAdminProhibido) {
            printf(T_ROJO "[X] Error: Nombre de usuario no permitido.\n\n" T_RESET);
            nombreCorrecto = false;
        } else if(yaExiste) {
            printf(T_ROJO "[X] Error: El usuario ya está en uso.\n\n" T_RESET);
            nombreCorrecto = false;
        } else {
            nombreCorrecto = true;
        }

    } while(!nombreCorrecto);

    //Lectura y validacion de la contraseña
    //Uso: Exige un limite estricto de longitud minima de 6 caracteres para la clave
    bool claveCorrecta = false;

    do{

        printf(" Ingrese clave (" T_CIAN "mín. 6 caracteres" T_RESET ") : ");
        
        int i=0;
        int c; //usamos int para evitar problemas de sigo con getch() en la terminal 

        //Bucle interactivo para capturar la contraseña tecla por tecla de forma oculta
        //Usamos getch() para evitar que el texto sea visible en la consola y mostrar "*" en su lugar
        while(1){
            c = getch(); //Lee el caracter de forma silenciosa sin mostrar en pantalla 

            //Si presiona ENTER, finaliza la carga de la clave 
            if(c == '\r' || c == '\n'){
                nuevaClave[i] = '\0'; //Cierra la cadena de texto de forma segura
                printf("\n");
                break;
            }
            //Si presiona BACKSPACE (borrar, codigo ASCII 8)
            else if(c == 8){
                if(i>0){
                    i--;
                    //Borra el asterisco visualmente en la consola retrocediendo el cursor
                    printf("\b \b");
                }
            }
            //Si es un caracter valido y hay espacio disponible en el arreglo
            else if(i<49 && c >= 32 && c <= 126){
                nuevaClave[i++] = c;
                printf("*"); //Imprime visualmente un asterisco por cada letra ingresada 
            }
        
        }


        //Validacion de la longitud minima de 6 caracteres para la contraseña
        if(strlen(nuevaClave)< minCaracteres){
            printf(T_ROJO "[X] Error: Contraseña muy corta. Mínimo %d caracteres.\n\n" T_RESET, minCaracteres);
            claveCorrecta = false;
        }else{
            claveCorrecta = true; //Contraseña aprobada
        }
    }while(!claveCorrecta);

    //Guardado de datos de la posicion libre encontrada
    listaUsuarios[posLibre].activo = true;
    strcpy(listaUsuarios[posLibre].usuario, nuevoUsuario);
    strcpy(listaUsuarios[posLibre].clave, nuevaClave);

    float saldoRegalo = 1000.0f; //Asigancion del bono de bienvenida estandar predeterminada
    listaUsuarios[posLibre].saldo = saldoRegalo;
    listaUsuarios[posLibre].edad = nuevaEdad;
    listaUsuarios[posLibre].gananciaAdmin = -saldoRegalo; // Registra que la casa arranca abajo comercialmente por el bono
    listaUsuarios[posLibre].dineroDepositado = saldoRegalo;
    strcpy(listaUsuarios[posLibre].usuarioBaneado, "");   //Inicializa vacio el historial de baneo de este slot 

    //Actualizacion contable del balance enn la cja fuerte central del casino.
    //Uso:  Descuenta formalmente el bono de bienvenida de los fondos netos globales de la banca
    ActualizarCajaCasino(&cajaCasino, saldoRegalo, 1); // 1 indica que la casa paga/resta

    printf("\n" T_VERDE "¡Usuario registrado con éxito! Saldo inicial de obsequio: " T_AMARILLO "$%.2f\n" T_RESET, saldoRegalo);
    printf("Presione <<ENTER>> para continuar...");
    while(getchar() != '\n');
    getchar();
}

void MenuAdmin(Usuario_t listaUsuarios[], int max){
    int opcionAdmin = 0;
    char usuarioBuscar[50];

    do{
        system("clear || cls");
        printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
        printf("║         " T_ROJO " 🛡️ PANEL DE CONTROL (ADMIN) 🛡️ " T_AMARILLO "          ║\n");
        printf("╠═════════════════════════════════════════════════╣\n");
        printf(T_AMARILLO "║ " T_VERDE "Caja Central : " T_AMARILLO "$%-29.2f " T_AMARILLO "  ║\n", cajaCasino);
        printf("╠═════════════════════════════════════════════════╣\n");
        printf("║                                                 ║\n");
        printf("║   " T_CIAN "[%d]" T_BLANCO " 📋 Listar usuarios y saldos               " T_AMARILLO "║\n", OPC_LISTAR_USUARIOS);
        printf("║   " T_CIAN "[%d]" T_BLANCO " 💵 Modificar saldo a un usuario           " T_AMARILLO "║\n", OPC_MODIFICAR_SALDO);
        printf("║   " T_CIAN "[%d]" T_BLANCO " 🚫 Banear / Eliminar usuario              " T_AMARILLO "║\n", OPC_GESTION_BANEOS);
        printf("║   " T_CIAN "[%d]" T_ROJO " 🚪 Cerrar Sesión de Administrador         " T_AMARILLO "║\n", OPC_SALIR_MENU_ADMIN);
        printf("║                                                 ║\n");
        printf("╚═════════════════════════════════════════════════╝" T_RESET "\n");
        printf(" Seleccione una opción: ");

        if (!LeerEnteroValido(&opcionAdmin)) {
            opcionAdmin = 0;
        }

        switch(opcionAdmin){
            case OPC_LISTAR_USUARIOS:
                system("clear || cls");
                printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
                printf("║      " T_CIAN " 📊 LIBRO DE AUDITORÍA HISTÓRICA 📊 " T_AMARILLO "      ║\n");
                printf("╚═════════════════════════════════════════════════╝" T_RESET "\n");
                printf(" " T_VERDE "Capital Disponible en Banca: " T_AMARILLO "$%.2f\n" T_RESET, cajaCasino);
                printf("---------------------------------------------------\n");
                
                //Empezamos desde i = 1 para omitir al administrador en la lista
                for(int i = 1; i< max; i++){
                    //Verificamos que la casilla este activa antes de mostrar los datos
                    if(listaUsuarios[i].activo){
                        printf(" " T_CIAN "[ID: %d]" T_BLANCO " Usuario : " T_VERDE "%s\n" T_RESET, i, listaUsuarios[i].usuario);
                        printf("  -> Edad              : %d años\n", listaUsuarios[i].edad);
                        printf("  -> Saldo del cliente : " T_AMARILLO "$%.2f\n" T_RESET, listaUsuarios[i].saldo);
        

                        //Si es positivo, el casino gano dinero con este usuario
                        //Si es negativo, el usuario le saco ganancias a la casa
                        if(listaUsuarios[i].gananciaAdmin >= 0){
                            printf("  -> Rendimiento Casa  : " T_VERDE "Casino ganó $%.2f\n" T_RESET, listaUsuarios[i].gananciaAdmin);
                        }else{
                            //Multiplicamos por -1.0f para mostrar el valor positivo de forma legible
                            printf("  -> Rendimiento Casa  : " T_ROJO "Usuario va ganando $%.2f\n" T_RESET, (listaUsuarios[i].gananciaAdmin * (-1.0f)));
                        }
                        printf("-------------------------------------------------------\n");    
                    }
                }
                printf("\nPresione <<ENTER>> para volver...");
                while(getchar() != '\n');
                getchar();
                break;

            case OPC_MODIFICAR_SALDO:
               ModificarSaldoAdministrativo(listaUsuarios,max);
               break; 
        
            case OPC_GESTION_BANEOS:
                GestionarUsuariosYBaneos(listaUsuarios,max);
                break;

            case OPC_SALIR_MENU_ADMIN:
                printf("\n" T_VERDE "Cerrando panel de administración...\n" T_RESET);
                Sleep(2000);
                break;

            default:
                printf("\n" T_ROJO "[X] Opción no válida." T_RESET "\n");
                printf("Presione <<ENTER>> para continuar...");
                while(getchar() != '\n');
                getchar();
                break;
        }
        
    }while(opcionAdmin != OPC_SALIR_MENU_ADMIN);
}

void CargarSaldo(Usuario_t *usuario){
    float monto = 0.0f;
    
    
    // Validación si ya alcanzó el tope
    if (usuario->saldo >= SALDO_MAXIMO_PERMITIDO) {
        system("clear || cls");
        printf("\n" T_ROJO "╔═════════════════════════════════════════════════╗\n");
        printf("║         [!] LÍMITE DE CUENTA ALCANZADO [!]      ║\n");
        printf("╚═════════════════════════════════════════════════╝\n" T_RESET);
        printf("Ya posees el saldo máximo permitido ($%.2f).\n", SALDO_MAXIMO_PERMITIDO);
        printf("Presione <<ENTER>> para continuar...");
        while(getchar() != '\n');
        getchar();
        return;
    }


    system("clear || cls");
    printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
    printf("║         " T_VERDE " 🪙 CAJERO VIP - CARGA DE SALDO 🪙 " T_AMARILLO "       ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf(T_AMARILLO "║ " T_VERDE "Saldo Actual : " T_AMARILLO "$%-31.2f " T_AMARILLO "║\n", usuario->saldo);
    printf("╚═════════════════════════════════════════════════╝\n" T_RESET);
    printf(" Ingrese el monto a depositar (Máx. $%.2f): " T_VERDE "$" T_RESET, MAX_CARGA_OPERACION);

    //Valida la entrada usando la funcion segura del poker
    if(!LeerFloatValido(&monto)){
        monto = 0.0f;
    }

    //Validamos usando las macros globales
    if(monto > 0 && monto <= MAX_CARGA_OPERACION){
        if (usuario->saldo + monto > SALDO_MAXIMO_PERMITIDO) {
            printf("\n" T_ROJO "[X] Error: Esta carga supera el límite máximo permitido en la cuenta ($%.2f).\n" T_RESET, SALDO_MAXIMO_PERMITIDO);
        } else {
            usuario->saldo += monto; 
            usuario->dineroDepositado += monto; //reistramos el capital que entró por ventanilla

            ActualizarCajaCasino(&cajaCasino, monto, -1); // El dinero entra a la banca
            printf("\n" T_VERDE "[✔] ¡Carga exitosa! Nuevo saldo: " T_AMARILLO "$%.2f\n" T_RESET, usuario->saldo);
        }
    }else{
        printf("\n" T_ROJO "[X] Monto inválido. Debe ser mayor a 0 y hasta $%.2f\n" T_RESET, MAX_CARGA_OPERACION);
    }
    printf("Presione <<ENTER>> para continuar...");
    getchar();
}
//============================================================================
//MENU DE CASINO (Sala de Juegos del Usuario Estandar)
//============================================================================
void MenuCasino(Usuario_t listaUsuarios[], int idSesion){
    int opcionJuego = 0;

    do {
        LimpiarPantalla();
        
        // ====================================================================
        // MEJORA VISUAL: Cabecera y panel de información del usuario con marcos
        // ====================================================================
        printf("\n" T_AMARILLO "+--------------------------------------------------+\n");
        printf("|             SALA VIP - CASINO GANO               |\n");
        printf("+--------------------------------------------------+\n" T_RESET);
        
        // Mostramos el estado actual del jugador con colores diferenciados
        printf(T_VERDE  "  Usuario Activo  : " T_BLANCO "%s\n", listaUsuarios[idSesion].usuario);
        printf(T_VERDE  "  Saldo Disponible: " T_AMARILLO "$%.2f\n" T_RESET, listaUsuarios[idSesion].saldo);
        printf(T_AMARILLO "+--------------------------------------------------+\n" T_RESET);

        // Opciones del salón de juegos formateadas con corchetes y colores
        printf(T_CIAN   "  [%d] " T_BLANCO "Jugar Tragamonedas\n", OPC_TRAGAMONEDAS);
        printf(T_CIAN   "  [%d] " T_VERDE  "Cargar Saldo\n", OPC_CARGAR_SALDO);
        printf(T_CIAN   "  [%d] " T_ROJO   "Cerrar Sesión\n" T_RESET, OPC_SALIR_MENU_CASINO);
        
        printf(T_AMARILLO "+--------------------------------------------------+\n" T_RESET);
        printf("Seleccione una opcion: ");

        if(!LeerEnteroValido(&opcionJuego)){
            opcionJuego = 0;
        }

        switch(opcionJuego){
            case OPC_TRAGAMONEDAS:
                //Validacion de saldo minimo requerido antes de permitir el ingreso al juego
                if(listaUsuarios[idSesion].saldo >= MAX_SALDO_TRAGAMONEDAS){
                     //Pasamos la direccion de memoria del saldo (&) para que las apuestas,
                    //ganancias o perdidas de la ruleta impacten directamente en la cuenta del usuario
                    JugarTragamonedas(&(listaUsuarios[idSesion]));
                }else {
                    printf("\n" T_ROJO "[X] Error: Saldo insuficiente ($%.2f). Requiere al menos $%.2f\n" T_RESET, listaUsuarios[idSesion].saldo, MAX_SALDO_TRAGAMONEDAS);
                    printf("\n" T_VERDE "favor cargue saldo (opcion %d)" T_RESET "\n",OPC_CARGAR_SALDO);
                    printf("\n" T_BLANCO "Presione ENTER para continuar..." T_RESET "\n");
                    getchar();
                } 
                break;
            case OPC_CARGAR_SALDO:
                //Llamamos a la funcion de carga enviando la direccion de memoria del saldo
                CargarSaldo(&(listaUsuarios[idSesion]));
                break;
            case OPC_SALIR_MENU_CASINO:
                printf("\n" T_VERDE "Cerrando sesion de usuario...\n" T_RESET);
                Sleep(1000); // Pausa de 1 segundo
                break;              
            default:
                printf("\n" T_ROJO "Opcion no valida. Vuelva a intentar.\n" T_RESET);
                printf("\n" T_BLANCO "Presione <<ENTER>> para continuar..." T_RESET "\n");
                getchar();
                break;
        }
    }while(opcionJuego != OPC_SALIR_MENU_CASINO);
}

//============================================================================
//PANEL DE TRANSACCIONES FINANCIERAS (Banca Central vs Clientes)
//Permite al administrador regalar bonos, retener fondos o procesar retiros
//============================================================================
void ModificarSaldoAdministrativo(Usuario_t listaUsuarios[], int max){
    char usuarioBuscar[50];
    int usuarioModificar = -1;
    int activosDisponibles = 0;

    do{
        system("clear || cls");
        printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
        printf("║       " T_VERDE " 💵 TRANSACCIONES FINANCIERAS (ADMIN) 💵 " T_AMARILLO " ║\n");
        printf("╠═════════════════════════════════════════════════╣\n");
        printf(T_AMARILLO "║ " T_VERDE "Caja Central : " T_AMARILLO "$%-29.2f " T_AMARILLO "  ║\n", cajaCasino);
        printf("╠═════════════════════════════════════════════════╣\n");
        printf("║               CLIENTES DISPONIBLES              ║\n");
        printf("╚═════════════════════════════════════════════════╝\n" T_RESET);

        activosDisponibles = 0;
        //listamos explicitamente a los usuarios activos (excluyendo al admin en posicion 0)
        for(int i = 1; i < max; i++){
            if(listaUsuarios[i].activo){
                printf("  " T_CIAN "-> [ID: %d]" T_BLANCO " Usuario: " T_VERDE "%s" T_BLANCO " | Saldo: " T_AMARILLO "$%.2f\n" T_RESET, i, listaUsuarios[i].usuario, listaUsuarios[i].saldo);
                activosDisponibles++;
            }
        }

        //Si no hay jugadores activos, pausamos y volvemos al menu anterior
        if(activosDisponibles == 0){
            printf("\n" T_ROJO "  [!] No existen usuarios activos registrados.\n" T_RESET);
            printf("--------------------------------------------------\n");
            printf("Presione <<ENTER>> para volver al menú principal...");
            while(getchar() != '\n');
            getchar(); // Espera a que el usuario presione Enter
            return; // Salimos de la funcion para volver al menu principal  
        }

        printf("--------------------------------------------------\n");
        printf("\n" T_BLANCO "Ingrese el NOMBRE exacto del usuario (o escriba 's' para volver al menu): " T_RESET);
        scanf("%49s", usuarioBuscar);
        while(getchar() != '\n'); // Limpiamos el buffer de entrada

        //Opcion de salida rapida del submenu
        if (strcasecmp(usuarioBuscar, "s")==0){
            return;
        }

        //Busqueda secuencial del cliente ingresado
        usuarioModificar = -1; // Reiniciamos la variable antes de la busqueda
        for(int i = 1; i < max; i++){
            if(listaUsuarios[i].activo && strcmp(listaUsuarios[i].usuario, usuarioBuscar)==0){
                usuarioModificar = i;
                break;
            }
        }

        //Si encontramos al usuario, abrimos su panel de configuracion financiera
        if(usuarioModificar != -1){
            int opcionAjuste = 0;
            do{
                system("clear || cls");
                printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
                printf("║      " T_VERDE " ⚙️ CONFIGURACIÓN FINANCIERA DE CUENTA ⚙️ " T_AMARILLO "   ║\n");
                printf("╠═════════════════════════════════════════════════╣\n");
                printf(T_AMARILLO "║ " T_VERDE "Caja Central : " T_AMARILLO "$%-29.2f " T_AMARILLO "  ║\n", cajaCasino);
                printf(T_AMARILLO "║ " T_CIAN "Cliente      : " T_BLANCO "%-29s" T_AMARILLO "    ║\n", listaUsuarios[usuarioModificar].usuario);
                printf(T_AMARILLO "║ " T_VERDE "Saldo Actual : " T_AMARILLO "$%-29.2f " T_AMARILLO "  ║\n", listaUsuarios[usuarioModificar].saldo);
                printf("╠═════════════════════════════════════════════════╣\n");
                printf("║                                                 ║\n");
                printf("║   " T_CIAN "[%d]" T_BLANCO " ➕ Sumar saldo al cliente             " T_AMARILLO "    ║\n", OPC_SUMAR_SALDO);
                printf("║   " T_CIAN "[%d]" T_BLANCO " ➖ Restar saldo al cliente            " T_AMARILLO "    ║\n", OPC_RESTAR_SALDO);
                printf("║   " T_CIAN "[%d]" T_BLANCO " 🪙 Retirar saldo por ventanilla       " T_AMARILLO "     ║\n", OPC_RETIRAR_SALDO);
                printf("║   " T_CIAN "[%d]" T_ROJO " 🚪 Salir del panel de transacciones   " T_AMARILLO "    ║\n", OPC_SALIR_MENU_AJUSTE);
                printf("║                                                 ║\n");
                printf("╚═════════════════════════════════════════════════╝" T_RESET "\n");
                printf(" Seleccione una opción: ");

                if(!LeerEnteroValido(&opcionAjuste)){
                    opcionAjuste = 0;
                }

                float montoAjuste = 0.0f;
                if(opcionAjuste == OPC_SUMAR_SALDO || opcionAjuste == OPC_RESTAR_SALDO || opcionAjuste == OPC_RETIRAR_SALDO){
                    printf(" Ingrese el monto: " T_VERDE "$" T_RESET);
                    if(!LeerFloatValido(&montoAjuste)){
                        montoAjuste = -1.0f;
                    }
                }

                switch(opcionAjuste){
                    case OPC_SUMAR_SALDO:
                        if(montoAjuste > 0){
                            if(montoAjuste <= cajaCasino){
                                listaUsuarios[usuarioModificar].saldo += montoAjuste;
                                ActualizarCajaCasino(&cajaCasino, montoAjuste, 1);  //Descuenta de la caja central
                                listaUsuarios[usuarioModificar].gananciaAdmin -= montoAjuste;
                                printf("\n" T_VERDE ">> [✔] ACREDITACIÓN EXITOSA. Nuevo saldo: $%.2f\n" T_RESET, listaUsuarios[usuarioModificar].saldo);
                            }else{
                                printf("\n" T_ROJO ">> [X] Error: Fondos de la banca insuficientes.\n" T_RESET); 
                            }
                        }
                        break;
                    case OPC_RESTAR_SALDO:
                        if(montoAjuste > 0 && listaUsuarios[usuarioModificar].saldo >= montoAjuste){
                            listaUsuarios[usuarioModificar].saldo -= montoAjuste;
                            ActualizarCajaCasino(&cajaCasino, montoAjuste, -1);  //Suma a la caja central
                            listaUsuarios[usuarioModificar].gananciaAdmin += montoAjuste;
                            printf("\n" T_VERDE ">> [✔] PENALIZACIÓN EXITOSA. Fondos retenidos a la Banca.\n" T_RESET);
                        }else{
                            printf("\n" T_ROJO ">> [X] Error: Saldo insuficiente o monto inválido.\n" T_RESET); 
                        }
                        break;
                    case OPC_RETIRAR_SALDO:
                        if(montoAjuste > 0 && listaUsuarios[usuarioModificar].saldo >= montoAjuste){
                            listaUsuarios[usuarioModificar].saldo -= montoAjuste;

                            // CORRECCIÓN CONTABLE: Descontamos el efectivo entregado de la caja central
                            ActualizarCajaCasino(&cajaCasino, montoAjuste, 1); // 1 descuenta de la caja

                            printf("\n" T_VERDE ">> [✔] RETIRO PROCESADO. Dinero entregado en efectivo.\n" T_RESET);
                        }else{
                            printf("\n" T_ROJO ">> [X] Error: Saldo insuficiente o monto inválido.\n" T_RESET);
                        }
                        break;
                    case OPC_SALIR_MENU_AJUSTE:
                        printf("\n" T_VERDE "Saliendo del panel de transacciones...\n" T_RESET);
                        break;
                    default:
                        printf("\n" T_ROJO ">> [X] Error: Opción inválida.\n" T_RESET);
                        break;
                }

                if(opcionAjuste != OPC_SALIR_MENU_AJUSTE){
                    printf("Presione <<ENTER>> para continuar...");
                    getchar();
                }

            }while (opcionAjuste != OPC_SALIR_MENU_AJUSTE);

        }else{
            printf("\n" T_ROJO "[X] Error: El usuario no existe o está inactivo.\n" T_RESET);
            printf("Presione <<ENTER>> para continuar...");
            while(getchar() != '\n');
            getchar(); 
        }

    }while (true); // Bucle infinito hasta que el usuario decida salir
}

//============================================================================
//GESTION DE BANEOS CON ESTADISTICAS VISUALES (Graficos de barras en consola)
//============================================================================
void GestionarUsuariosYBaneos(Usuario_t listaUsuarios[],int max){
    int totalSlots = max - 1; // Excluyendo al administrador
    char opcionBanear[10]; //Almacena la decision del admin (S/N)
    char usuarioBuscar[50]; //Nombre del usuario a banear

    //Bucle principal del gestor de baneos y metricas
    while(true){
        int activos = 0;
        int baneados = 0;

        //Recuento dinamico de cuentas activas y cupos libres o baneados en el sstema
        for (int i=1; i < max; i++){
            if(listaUsuarios[i].activo){
                activos++;
            }else{
                baneados++;
            }
        }

        system("clear || cls");
        printf("\n" T_AMARILLO "╔═════════════════════════════════════════════════╗\n");
        printf("║      " T_VERDE " 🚫 GESTIÓN DE BANEOS Y ESTADÍSTICAS 🚫 " T_AMARILLO "   ║\n");
        printf("╠═════════════════════════════════════════════════╣\n");

        //grafico de barras para usuarios activos
        printf("Usuarios Activos: [%d]: [",activos);
        for(int i=1; i<= totalSlots; i++){
            if(i <= activos) printf(T_VERDE "X" T_RESET);
            else printf(".");
        }
        printf("]\n");

        //grafico de barras para usuarios baneados
        printf(" Cupos Libres/Baneados: [%d]: [",baneados);
            for(int i=1; i<= totalSlots; i++){
                if(i <= baneados) printf(T_ROJO "X" T_RESET);
                else printf(".");
            }
            
        printf("]\n");
        printf(" Capacidad máxima   : %d usuarios.\n", totalSlots);
        printf("╠═════════════════════════════════════════════════╣\n");
        printf("║           LISTA DE USUARIOS EN SISTEMA          ║\n");
        printf("╚═════════════════════════════════════════════════╝\n" T_RESET);

        //Impresion detallada del estado fisico de cada slot del arreglo
        for(int i=1; i< max; i++){
            if (listaUsuarios[i].activo){
                printf("  " T_CIAN "[Cupo %d]" T_VERDE " Activo: %s " T_BLANCO "(Edad: %d) Saldo: " T_AMARILLO "$%.2f\n" T_RESET, i, listaUsuarios[i].usuario, listaUsuarios[i].edad, listaUsuarios[i].saldo);
            }else{
                if(strlen(listaUsuarios[i].usuarioBaneado) > 0){
                    printf("  " T_CIAN "[Cupo %d]" T_ROJO " BANEADO: %s eliminó cuenta.\n" T_RESET, i, listaUsuarios[i].usuarioBaneado);
                }else{
                    printf("  " T_CIAN "[Cupo %d]" T_BLANCO " VACÍO: Cupo libre disponible.\n" T_RESET, i);
                }
            }
        }
        printf("==================================================\n\n");

        printf("¿Desea banear a algún usuario? (" T_VERDE "S" T_BLANCO "/" T_ROJO "N" T_BLANCO "): ");
        scanf("%4s", opcionBanear);
        while(getchar() != '\n'); // Limpiar buffer

        //Si elige "S" o "s", procede a solicitar el nombre del usuario a banear
        if(strcasecmp(opcionBanear, "S") == 0){
            printf("\n" T_BLANCO "Ingrese el NOMBRE exacto del usuario a banear: " T_RESET);
            scanf("%49s", usuarioBuscar);
            while(getchar() != '\n'); // Limpiar buffer

            int posBan = -1;
            //Busqueda secuencial evitando afectar a la cuenta protegida del admin
            for (int i=1; i<max; i++){
                if(listaUsuarios[i].activo && strcmp(listaUsuarios[i].usuario, usuarioBuscar)== 0 && strcmp(usuarioBuscar, "admin")!=0){
                posBan = i;
                break;
                }
            }

            if(posBan != -1){
                 //Resguardamos el nombre actual en el historial de baneados del slot antes de borrar 
                strcpy(listaUsuarios[posBan].usuarioBaneado, listaUsuarios[posBan].usuario);

                //Blanqueo completo del slot para liberar la posicion de memoria  para nuevos registros 
                listaUsuarios[posBan].activo = false;
                strcpy(listaUsuarios[posBan].usuario, "");
                strcpy(listaUsuarios[posBan].clave, "");
                listaUsuarios[posBan].saldo = 0.0f;
                listaUsuarios[posBan].edad = 0; 
                listaUsuarios[posBan].gananciaAdmin = 0.0f; // Reiniciamos la ganancia del admin para este slot
                
                printf("\n" T_VERDE ">> [✔] OPERACIÓN EXITOSA: %s ha sido baneado.\n" T_RESET, usuarioBuscar);
            }else {
                if(strcasecmp(usuarioBuscar, "admin") == 0){
                    printf("\n" T_ROJO "[X] Error: No está permitido banear a la cuenta admin.\n" T_RESET);
                }else{
                    printf("\n" T_ROJO "[X] Error: El usuario no existe o está inactivo.\n" T_RESET);
                }
            }
                
            
            printf("Presione <<ENTER>> para continuar...");
            while(getchar() != '\n');
            getchar(); // Pausa para que el usuario pueda leer y presionar Enter
            
        }else if(strcasecmp(opcionBanear, "N") == 0){
            break; // Sale del bucle principal y vuelve al menu anterior
        }else{
            printf("\n" T_ROJO "[X] Opción no válida. Ingrese S o N.\n" T_RESET);
            printf("Presione <<ENTER>> para continuar...");
            while(getchar() != '\n');
            getchar(); // Pausa para que el usuario pueda leer y presionar Enter 
        }
    }        
}

// ============================================================================
// FUNCIONES AUXILIARES DE VALIDACIÓN Y CAJA CENTRAL
// ============================================================================

// Lee un número entero de forma segura evitando bucles infinitos si ingresan letras
bool LeerEnteroValido(int *valor) {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return false;
    }
    
    // Elimina el salto de línea final
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) return false;

    char *fin;
    long num = strtol(buffer, &fin, 10);

    // Verifica que toda la cadena haya sido un número válido (sin letras mezcladas)
    if (*fin != '\0') {
        return false;
    }

    *valor = (int)num;
    return true;
}

// Lee un número decimal (float) de forma segura para cargas de saldo y apuestas
bool LeerFloatValido(float *valor) {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return false;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) return false;

    char *fin;
    float num = strtof(buffer, &fin);

    if (*fin != '\0') {
        return false;
    }

    *valor = num;
    return true;
}

// Actualiza el dinero global del casino
// operacion = 1  -> El casino paga o entrega dinero (RESTA de la caja)
// operacion = -1 -> El casino recibe dinero (SUMA a la caja)
void ActualizarCajaCasino(float *caja, float monto, int operacion) {
    if (operacion == 1) {
        *caja -= monto;
    } else if (operacion == -1) {
        *caja += monto;
    }
}

void LimpiarPantalla(void) {
    // Código ANSI estándar: borra toda la pantalla
    printf("\033[2J\033[H");
    fflush(stdout);
}
