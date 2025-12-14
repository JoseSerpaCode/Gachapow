<img src="https://i.imgur.com/KxW5kAj.png">

# 🎮 Gachapow V2.0

> **Versión actual:** `v1.1.0-prealpha`  
> **Estado:** En desarrollo (Alpha)  
> **Repositorio oficial del proyecto Gachapow V2.0**

---

## 📘 Descripción general

**Gachapow V2.0** es un proyecto que une **videojuego + hardware físico** en una experiencia interactiva de recompensas reales.  
El objetivo es desarrollar un **juego embebido de bajo costo**, ejecutable directamente sobre un **microcontrolador sin sistema operativo**, con un **sistema físico de premios (dispensador de cápsulas o fichas)** controlado por el mismo hardware.

El proyecto nace como una **reingeniería completa** del producto original **Gachapow v1.0.0**, replanteando desde cero su arquitectura, motor, hardware y costos de producción.

---

## 🧭 Contexto y propósito

La primera versión de Gachapow fue un prototipo desarrollado en entorno de alto nivel (Java) y hardware de consumo general.  
**Gachapow V2.0** busca ir más allá:
- Rediseñar completamente el sistema para **hardware embebido de bajo costo**.  
- Implementar el juego directamente sobre el **microcontrolador**.  
- Desarrollar el **sistema de premios físico** controlado por el MCU.  
- Optimizar memoria, tiempos y rendimiento (“bare metal”).  
- Consolidar un modelo de negocio viable para replicar el sistema.

---

🧠 Filosofía de desarrollo

“Tocar metal para entender el juego desde la raíz.”

Gachapow V2.0 busca que el equipo trabaje directamente sobre los fundamentos del sistema embebido, optimizando recursos y aprendiendo desde la base:
control de memoria, sincronización, periféricos, temporización y gráficos sin SO.

---

## 🧩 Roadmap y etapas del desarrollo

| Etapa | Versión | Estado | Descripción breve |
|--------|----------|---------|--------------------|
| **Pre-Alpha** | `v1.1.0-prealpha` | 🟢 Activo | Fundamentos del juego y del sistema de premios físico. |
| **Alpha** | `v1.3.0-alpha` | 🟡 En desarrollo | Core funcional completo y arquitectura validada. |
| **Beta** | `v1.6.0-beta` | 🔜 Pendiente | Integración total en MCU + PCB prototipo funcional. |
| **Release Candidate** | `v1.9.0-rc1` | 🔜 Pendiente | Versión casi final con QA aprobado. |
| **Final / Gold** | `v2.0.0` | 🔜 Objetivo | Producto final listo para presentación y réplica. |

📘 Roadmap técnico completo (v2.0): https://roadmap.sh/r/roadmap-project-gachapow-v20

---

## ⚙️ Sistema de premios físico

El **sistema de premios** es el elemento diferenciador del proyecto:  
una cápsula o ficha real es dispensada cuando el jugador alcanza ciertas condiciones de victoria.

---

🧾 Control de versiones

Nota:
Este proyecto continúa la línea del producto original Gachapow v1.0.0 como una reingeniería total.
Se conserva la numeración v1.x.x durante las fases de desarrollo y se alcanza la nueva release oficial en v2.0.0, que marca el inicio del nuevo ciclo técnico.

📜 Licencia

Este proyecto se desarrolla con fines académicos y de aprendizaje.
El código fuente, documentación y hardware pueden ser reutilizados bajo la licencia MIT, siempre que se otorgue el crédito correspondiente.

© 2025 Proyecto Gachapow V2.0 — Desarrollo académico e ingenieril
“Entre bits y cápsulas, un juego que se vuelve real.”

---

## 🖥️ Desarrollo y compilación

### 🔹 Requisitos (para entorno PC)
- GCC / Clang  
- SDL2  
- CMake (opcional)  
- Git

**Compilación en PC:**
```bash
git clone https://github.com/<usuario>/gachapow-v2.git
cd gachapow-v2/platform/pc
make run


