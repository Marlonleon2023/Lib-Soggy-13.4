// =============================================================================
//  PvZ2 13.4 offsets
// =============================================================================
#pragma once
#include <cstdint>

// ---------- Settings (13.4 VERIFIED) ----------------------------------------
constexpr uintptr_t OFF_SettingsCreate         = 0x147E67C;
constexpr uintptr_t OFF_SettingsTabCreate      = 0x147F9E8;
constexpr uintptr_t OFF_SettingsAddWidget      = 0x147FCA4;
constexpr uintptr_t OFF_CheckboxCreate         = 0x14801B4;
constexpr uintptr_t OFF_SettingsSliderCreate   = 0x147FEAC;
constexpr uintptr_t OFF_SetChecked             = 0x1FFE9DC;
constexpr uintptr_t OFF_SettingsDispatch       = 0x1483ED0;   // thunk, safe for And64InlineHook

constexpr uintptr_t SETTINGS_PAGE_CONTAINER    = 240;
constexpr uintptr_t WIDGET_STATE_OFFSET        = 0x180;
constexpr uint32_t  SETTINGS_VIEW_ANGLE_ID     = 0x100;

// ---------- Board (13.4 VERIFIED) -------------------------------------------
// Funciones del binario original (vaddr == file offset en el LOAD r-x):
//   OFF_BoardChangeState = sub_1542840  (prologo verificado: sub sp,#0xb0)
//   OFF_BoardRender      = sub_1544D5C  (prologo verificado: sub sp,#0x1a0)
//   sub_152DE68 = "BoardZoom2": calcula b280 (escala), b281, b282 (layout).
//   sub_1528B9C / sub_152DABC = "ApplyZoom" (embebido en cambio de estado /
//     init): calcula b283..b286 y escribe board+0x4C (ancho del cesped).
//   sub_153EC84 = conversion pantalla<->mundo:
//     pantalla = b281 + b280*(mundo - b281 + b17)   (b17 = Board+0x44)
//   sub_144ACF0 = tabla de direcciones de camara (usa b283/b284/b285/b286).
//   sub_11E492C = limitador/objetivo de pan (usa b283/b284/b285).
constexpr uintptr_t OFF_BoardChangeState       = 0x1542840;
constexpr uintptr_t OFF_BoardRender            = 0x1544D5C;

// Campos de tipo entero del Board (pan/objetivos de camara). En vanilla:
//   b284 = b270 (=0 en vanilla) y b285 lo calcula ApplyZoom.
// NO escribirlos desde el mod: alteran la posicion en reposo de la camara
// (via sub_144ACF0/sub_11E492C -> b17) y desplazan el tablero.
constexpr uintptr_t BOARD_270                  = 1064;   // 0x428 b270 (int)
constexpr uintptr_t BOARD_283                  = 1116;   // 0x45C b283 (int, negativo = -prop*A*escala)
constexpr uintptr_t BOARD_284                  = 1120;   // 0x460 b284 (int, vanilla = b270 = 0)
constexpr uintptr_t BOARD_285                  = 1124;   // 0x464 b285 (int, objetivos de pan/seleccion)
constexpr uintptr_t BOARD_286                  = 1128;   // 0x468 b286 (int, anchoCesped - viewW)

// Escala PPU (float). El mod SI la fuerza a 1.0 (full lawn).
constexpr uintptr_t BOARD_280                  = 1104;   // 0x450 b280 (float)

// b17: offset de camara de render en PX (0x44; b18 = 0x48 es el Y).
// Con escala 1.0: pantalla = mundo - b17 (ver sub_153EC84; el mismo b17 se
// usa en el touch, asi que forzarlo mantiene el touch alineado).
// Escrito por las acciones de pan (0x12FC2xx) y por el layout (0x152DA70).
constexpr uintptr_t BOARD_CAM_RENDER_X          = 68;     // 0x44 b17 (int)
constexpr uintptr_t BOARD_CAM_RENDER_Y          = 72;     // 0x48 b18 (int)

// Entradas del transform de render (float), calculadas por Zoom2 (sub_152DE68).
// Con escala=1.0 se cancelan en la formula de render; el mod NO debe escribirlas.
constexpr uintptr_t BOARD_TRANSFORM_X          = 1108;   // 0x454 b281 (float)
constexpr uintptr_t BOARD_TRANSFORM_Y          = 1112;   // 0x458 b282 (float)

// ---------- App global (13.4 VERIFIED) --------------------------------------
// qword_2E17A20 = app singleton pointer; +132 = screen width, +136 = height
constexpr uintptr_t OFF_G_LawnApp              = 0x2E17A20;
constexpr uintptr_t APP_SCREEN_WIDTH           = 132;
constexpr uintptr_t APP_SCREEN_HEIGHT          = 136;