#include <cstdint>
#include <cstring>
#include <string>
#include <atomic>
#include <thread>
#include <jni.h>
#include <unistd.h>

#include "logging.h"
#include "memUtils.h"
#include "offsets.h"

static std::atomic<uintptr_t> g_cachedBase{0};

static inline uintptr_t getBase() {
    uintptr_t b = g_cachedBase.load(std::memory_order_acquire);
    if (!b) {
        b = getLibraryAddress("libPVZ2.so");
        g_cachedBase.store(b, std::memory_order_release);
    }
    return b;
}

static std::atomic<bool> g_highView{true};
static std::atomic<bool> g_loaded{false};

static bool getHighView() {
    if (!g_loaded.load(std::memory_order_acquire)) {
        g_highView.store(true, std::memory_order_release);
        g_loaded.store(true, std::memory_order_release);
    }
    return g_highView.load(std::memory_order_acquire);
}

static void setHighView(bool v) {
    g_highView.store(v, std::memory_order_release);
}

static std::string g_viewAngleLabel = "Full lawn (wide view)";

static uintptr_t g_lastSettingsPage   = 0;
static uintptr_t g_viewAngleContainer = 0;
static uintptr_t g_viewAngleChild     = 0;
static bool      g_injected           = false;
static uintptr_t g_captureChild       = 0;

typedef uintptr_t (*CreateTab)(uintptr_t, uint32_t, uintptr_t, uintptr_t, uintptr_t,
                               uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t,
                               uintptr_t, uintptr_t, uintptr_t);
typedef long      (*AddWidget)(uintptr_t, uintptr_t, uint8_t, float);
typedef uintptr_t (*CreateCB)(uintptr_t, uint32_t, uintptr_t, char, int);

typedef long (*Dispatch)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t,
                         uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t,
                         uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t,
                         uintptr_t, uintptr_t, uintptr_t, uintptr_t);

typedef uintptr_t (*SetChecked_t)(uintptr_t, uintptr_t, uintptr_t, char, int, int);

typedef long (*BoardChangeState_t)(uintptr_t, uintptr_t);
typedef long (*BoardRender_t)(uintptr_t, uintptr_t);

static CreateTab          oCreateTab         = nullptr;
static AddWidget          oAddWidget         = nullptr;
static CreateCB           oCreateCB          = nullptr;
static Dispatch           oDispatch          = nullptr;
static SetChecked_t       oSetChecked        = nullptr;
static BoardChangeState_t oBoardChangeState  = nullptr;
static BoardRender_t      oBoardRender       = nullptr;

static uintptr_t hkCreateTab(uintptr_t a1, uint32_t a2,
                             uintptr_t a3, uintptr_t a4, uintptr_t a5,
                             uintptr_t a6, uintptr_t a7, uintptr_t a8,
                             uintptr_t a9, uintptr_t a10,
                             uintptr_t a11, uintptr_t a12, uintptr_t a13)
{
    if (a1 != g_lastSettingsPage) {
        g_lastSettingsPage   = a1;
        g_viewAngleContainer = 0;
        g_viewAngleChild     = 0;
        g_injected           = false;
    }

    if (!g_injected && a2 == 6 && oCreateCB && oAddWidget) {
        g_injected = true;

        g_captureChild = (uintptr_t)-1;

        uintptr_t cb = oCreateCB(a1, SETTINGS_VIEW_ANGLE_ID,
                                 (uintptr_t)&g_viewAngleLabel,
                                 getHighView() ? 1 : 0, 0);

        g_viewAngleContainer = cb;
        g_viewAngleChild = (g_captureChild == (uintptr_t)-1) ? 0 : g_captureChild;
        g_captureChild = 0;

        if (cb) {
            uintptr_t cont = *(uintptr_t*)(a1 + SETTINGS_PAGE_CONTAINER);
            if (cont) oAddWidget(cont, cb, 0, 0.0f);
        }
    }

    return oCreateTab(a1, a2, a3, a4, a5,
                      a6, a7, a8, a9, a10,
                      a11, a12, a13);
}

static uintptr_t hkCreateCB(uintptr_t page, uint32_t id,
                            uintptr_t label, char init, int p5)
{
    return oCreateCB(page, id, label, init, p5);
}

static long hkAddWidget(uintptr_t cont, uintptr_t w,
                        uint8_t centered, float s)
{
    if (!w) return 0;
    return oAddWidget(cont, w, centered, s);
}

static uintptr_t hkSetChecked(uintptr_t widget, uintptr_t a2, uintptr_t a3,
                              char state, int a5, int a6)
{
    if (g_captureChild == (uintptr_t)-1) {
        g_captureChild = widget;
    }
    return oSetChecked ? oSetChecked(widget, a2, a3, state, a5, a6) : widget;
}

static long hkDispatch(uintptr_t a1, uintptr_t a2,
                       uintptr_t a3, uintptr_t a4, uintptr_t a5,
                       uintptr_t a6, uintptr_t a7, uintptr_t a8,
                       uintptr_t a9, uintptr_t a10,
                       uintptr_t a11, uintptr_t a12,
                       uintptr_t a13, uintptr_t a14,
                       uintptr_t a15, uintptr_t a16,
                       uintptr_t a17, uintptr_t a18,
                       uintptr_t a19)
{
    const uint32_t id = (uint32_t)a2;

    if (id == SETTINGS_VIEW_ANGLE_ID) {
        if (g_viewAngleChild) {
            uint8_t state = *(uint8_t*)(g_viewAngleChild + WIDGET_STATE_OFFSET);
            bool newVal = (state != 0);
            if (getHighView() != newVal) {
                setHighView(newVal);
            }
        }
        return 0;
    }

    if (id == 4) return 0;

    return oDispatch ? oDispatch(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
                                 a11, a12, a13, a14, a15, a16, a17, a18, a19)
                     : 0;
}

static constexpr uintptr_t BOARD_STATE_PLAYING = 1;

// Offset extra hacia la izquierda (px). POSITIVO = más pan target a la izquierda
// (porque pantalla = mundo - b17; b17 positivo => cámara se mueve a la izquierda).
// Ajusta este valor según necesites (ej: 200-400).
static constexpr int EXTRA_LEFT_PAN = 300;

// Aplica la corrección de cámara full-lawn al board.
// SOLO establece los objetivos de pan (b270, b284, b285, b280).
// NO toca b17 (BOARD_CAM_RENDER_X) para permitir que la animación
// de entrada del juego (selector semillas -> partida) sea suave.
// La cámara animará naturalmente hacia estos objetivos.
static void ApplyFullLawnCamera(uintptr_t board) {
    // Full lawn (vista ancha): escala 1.0 + alineación izquierda del tablero.
    //
    // Escala (b280): igual que el hook post-BoardZoom2 de Soggylib 9.6.1.
    *(float*)(board + BOARD_280) = 1.0f;

    // b283 = offset de alineación en PX.
    // En vanilla PLAYING, ApplyZoom lo pone en 0 (centrado).
    // Para full lawn left-align: necesitamos b283 NEGATIVO para que -b283 sea POSITIVO.
    // Forzamos b283 a un valor negativo grande (lawn width approx - screen width).
    // Luego los pan targets usan -b283 + EXTRA_LEFT_PAN.
    int b283 = *(int*)(board + BOARD_283);
    int b286 = *(int*)(board + BOARD_286);

    // Si b283 es 0 o positivo (vanilla), forzar a negativo para left-align.
    // b286 = lawnWidth - viewWidth (aprox). Usamos b286 como referencia.
    if (b283 >= 0) {
        // Estimamos: left-align requiere -b283 ≈ b286 (ancho excedente del césped)
        // Pero limitamos a un valor razonable.
        int desired_b283 = -b286;
        if (desired_b283 > -50) desired_b283 = -250; // fallback
        *(int*)(board + BOARD_283) = desired_b283;
        b283 = desired_b283;
    }

    // Objetivos de pan en reposo: b270 = b284 = -b283 + extra
    // -b283 es POSITIVO (left-align), EXTRA_LEFT_PAN añade más margen izquierdo.
    int targetPan = -b283 + EXTRA_LEFT_PAN;
    *(int*)(board + BOARD_270) = targetPan;
    *(int*)(board + BOARD_284) = targetPan;
    *(int*)(board + BOARD_285) = (b283 + b286) / 2 + EXTRA_LEFT_PAN;

    // NOTA: No escribimos BOARD_CAM_RENDER_X (b17) aquí.
    // El juego anima b17 hacia b270/b284 via sub_11E492C/sub_144ACF0.
    // Forzar b17 inmediato rompe la animación de entrada (selector->partida).
}

// Igual que el .so original: BoardChangeState solo loguea y reenvía.
// Llamamos al original PRIMERO (que ejecuta ApplyZoom y recalcula b283/b286),
// luego SOBREESCRIBIMOS los objetivos de pan con nuestra corrección.
static long hkBoardChangeState(uintptr_t board, uintptr_t newState) {
    LOGI("=== BoardChangeState board=%p state=%lu ===",
         (void*)board, (unsigned long)newState);

    long result = oBoardChangeState ? oBoardChangeState(board, newState) : 0;

    if (getHighView()) {
        ApplyFullLawnCamera(board);
    }

    return result;
}

static long hkBoardRender(uintptr_t board, uintptr_t a2) {
    if (!getHighView()) {
        return oBoardRender ? oBoardRender(board, a2) : 0;
    }

    // Aplicar corrección ANTES del original para que la lógica de render
    // y animación de cámara de este frame use nuestros objetivos corregidos.
    ApplyFullLawnCamera(board);

    return oBoardRender ? oBoardRender(board, a2) : 0;
}

static void ApplyHooks() {
    uintptr_t base = 0;
    while ((base = getLibraryAddress("libPVZ2.so")) == 0) usleep(100000);
    g_cachedBase.store(base, std::memory_order_release);
    LOGI("base = %p", (void*)base);

    // Settings
    PVZ2HookFunction(OFF_SettingsTabCreate, (void*)hkCreateTab, (void**)&oCreateTab);
    PVZ2HookFunction(OFF_SettingsAddWidget, (void*)hkAddWidget, (void**)&oAddWidget);
    PVZ2HookFunction(OFF_SettingsDispatch,  (void*)hkDispatch,  (void**)&oDispatch);
    PVZ2HookFunction(OFF_CheckboxCreate,    (void*)hkCreateCB,  (void**)&oCreateCB);
    PVZ2HookFunction(OFF_SetChecked,        (void*)hkSetChecked,(void**)&oSetChecked);

    // Board: igual que el original, ambos hooks
    PVZ2HookFunction(OFF_BoardRender,       (void*)hkBoardRender,      (void**)&oBoardRender);
    PVZ2HookFunction(OFF_BoardChangeState,  (void*)hkBoardChangeState, (void**)&oBoardChangeState);

    LOGI("Soggylib hookde");
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*, void*) {
    std::thread(ApplyHooks).detach();
    return JNI_VERSION_1_6;
}
