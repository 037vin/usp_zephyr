/*
 * SX126x RF-frontend power hook — fork sentinel.
 *
 * This usp_zephyr fork's sx126x HAL (drivers/usp/sx126x/sx126x_hal.c) calls
 * sx126x_hal_radio_awake_hook() on every radio sleep/awake transition (fork
 * commit b14be7e). An application strong-overrides the weak default to key its
 * RF switch supply / antenna-path GPIOs to the radio's actual awake state.
 *
 * SX126X_HAS_RADIO_AWAKE_HOOK lets an application prove, at compile time, that
 * the hook-calling fork is the one being built. Without it (H-31): if the west
 * manifest is ever re-pinned to an upstream usp_zephyr that lacks the hook, the
 * app's strong override silently becomes an uncalled function — no link error —
 * and the RF front end stays gated off from rf_path_set(false) onward, reading
 * as an antenna fault rather than a build regression. The app guards with
 * `#if !__has_include(<sx126x_rf_frontend_hook.h>) #error ...` so that re-pin
 * fails the build instead.
 *
 * This header is exported via usp_zephyr/CMakeLists.txt `zephyr_include_directories(include)`.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SX126X_RF_FRONTEND_HOOK_H
#define SX126X_RF_FRONTEND_HOOK_H

#include <stdbool.h>

/* Present only in the fork whose sx126x_hal.c actually calls the hook. */
#define SX126X_HAS_RADIO_AWAKE_HOOK 1

/**
 * @brief Called on every SX126x sleep/awake transition, from whatever context
 *        the HAL touches the radio. Weakly defined (no-op) in sx126x_hal.c;
 *        an application strong-overrides it to gate its RF front end. Must be
 *        ISR-safe (the HAL may call it from an ISR).
 *
 * @param context  The sx126x HAL context (opaque to the app).
 * @param awake    true on wake, false on sleep.
 */
void sx126x_hal_radio_awake_hook(const void *context, bool awake);

#endif /* SX126X_RF_FRONTEND_HOOK_H */
