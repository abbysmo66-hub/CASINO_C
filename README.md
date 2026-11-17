# 🎰 CASINO GANO - Sistema de Gestión y Juegos en C

Aplicación de consola desarrollada en **lenguaje C** que simula un casino virtual completo ("Casino Gano"). Incluye un sistema de gestión de usuarios con roles diferenciados (Administrador y Jugadores), control contable de la banca central, validaciones estrictas de entrada y un juego de **Tragamonedas** con animaciones en arte ASCII y colores ANSI.

---

## ✨ Características Principales

### 👤 Módulo de Jugadores
* **Registro e Inicio de Sesión:** Validación de edad (`18` a `120` años), nombres alfanuméricos de mínimo 6 caracteres, bloqueo de nombres reservados y contraseñas ocultas con asteriscos (`*`).
* **Bono de Bienvenida:** Cada nuevo usuario registrado recibe automáticamente **$1,000.00** descontados de la caja central.
* **Cajero VIP (Carga de Saldo):** Permite depositar dinero hasta **$1,000,000.00** por operación, respetando un tope máximo de cuenta de **$5,000,000.00**.

### 🛡️ Panel de Control (Administrador)
* **Caja Central Global:** Arranca con un capital inicial de **$2,000,000.00** y actualiza su balance en tiempo real con cada apuesta, premio, carga o retiro.
* **Auditoría Histórica:** Lista los usuarios activos mostrando su saldo y el rendimiento comercial (cuánto ganó o perdió la casa frente a cada jugador).
* **Transacciones Financieras:** Permite acreditar bonos, retener/restar fondos o procesar retiros por ventanilla.
* **Gestión de Baneos y Estadísticas:** Visualización de cupos disponibles mediante gráficos de barras en consola y eliminación/baneo de cuentas.

### 🍒 Juego: Tragamonedas (*Slots Fortune*)
* **Apuesta mínima:** **$500.00** (requiere tener al menos ese saldo para ingresar a la sala).
* **Interfaz visual:** Rodillos animados con caracteres UTF-8, emojis y colores ANSI.
* **Tabla de Pagos:**
  * 👑 👑 👑 (**3 Coronas**): **Jackpot Gigante (`x15`)**
  * 💎 / 🪙 / ⚔️ (**3 Símbolos iguales**): **Gran Premio (`x5`)**
  * ✨ (**2 Símbolos iguales**): **Recompensa (`x2`)**

---

## 🔑 Credenciales Precargadas (Modo Prueba)

El sistema cuenta con dos cuentas creadas por defecto para facilitar las pruebas:

| Rol | Usuario | Contraseña | Saldo Inicial |
| :--- | :--- | :--- | :--- |
| **Administrador** | `admin` | `1234` | N/A (Gestiona la Banca) |
| **Jugador** | `jugador1` | `0000` | `$1,000.00` |

> **Nota:** La capacidad máxima del sistema está configurada actualmente en `3` cupos totales (`MAX_USUARIOS 3`: 1 administrador y hasta 2 jugadores simultáneos).

---

## 📂 Estructura del Proyecto

* `main.c` / `main.h`: Núcleo del sistema, menús, autenticación, panel de administración, control de caja y validaciones de entrada segura.
* `TRAGAMONEDAS.c` / `TRAGAMONEDAS.h`: Lógica del juego de tragamonedas, cálculo de premios, aleatoriedad y renderizado de arte ASCII.

---

## ⚙️ Requisitos y Compilación

Este proyecto utiliza librerías específicas de **Windows** (`<windows.h>` y `<conio.h>` para `Sleep()`, `getch()` y configuración de consola UTF-8 mediante `chcp 65001`).

### Compilar desde terminal (MinGW / GCC en Windows)
```bash
gcc main.c TRAGAMONEDAS.c -o casino.exe
casino.exe
```

### Compilar desde un IDE (Code::Blocks / Dev-C++)
1. Crea o abre un proyecto en C que incluya `main.c`, `main.h`, `TRAGAMONEDAS.c` y `TRAGAMONEDAS.h`.
2. Presiona **Compilar y Ejecutar** (`F9` o `F11`).
3. *Recomendación:* Ejecútalo en la terminal moderna de Windows (*Windows Terminal*) para visualizar correctamente los emojis y bordes dobles.

---

## 👩‍💻 Autora

* **Ingrid Oviedo** — Desarrollo completo del sistema y lógica de juego.
