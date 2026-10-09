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
constexpr uintptr_t OFF_BoardChangeState       = 0x1542840;
constexpr uintptr_t OFF_BoardRender            = 0x1544D5C;

constexpr uintptr_t BOARD_270                  = 1064;
constexpr uintptr_t BOARD_280                  = 1104;
constexpr uintptr_t BOARD_283                  = 1116;
constexpr uintptr_t BOARD_284                  = 1120;
constexpr uintptr_t BOARD_285                  = 1124;
constexpr uintptr_t BOARD_286                  = 1128;

// Extra camera transform fields read by sub_1544D5C at the tail
constexpr uintptr_t BOARD_TRANSFORM_X          = 1108;
constexpr uintptr_t BOARD_TRANSFORM_Y          = 1112;

// ---------- App global (13.4 VERIFIED) --------------------------------------
// qword_2E17A20 = app singleton pointer; +132 = screen width, +136 = height
constexpr uintptr_t OFF_G_LawnApp              = 0x2E17A20;
constexpr uintptr_t APP_SCREEN_WIDTH           = 132;
constexpr uintptr_t APP_SCREEN_HEIGHT          = 136;