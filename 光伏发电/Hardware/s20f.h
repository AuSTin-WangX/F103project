#ifndef S20F_H
#define S20F_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Confirm these values against your positional S20F servo specification. */
#define S20F_MAX_ANGLE_DEG       180.0f
#define S20F_MIN_PULSE_US        500U
#define S20F_MAX_PULSE_US        2500U
#define S20F_CENTER_PULSE_US     1500U
#define S20F_PERIOD_US          20000U

/* Owns TIM3 and PA7. Call after system clock setup; starts at center. */
/* Returns ERROR if the timer clock cannot produce an exact 1 MHz tick. */
ErrorStatus S20F_Init(void);

/* Clamp to [0, S20F_MAX_ANGLE_DEG]. NaN is ignored. */
void S20F_SetAngle(float angle_deg);

/* Clamp to [S20F_MIN_PULSE_US, S20F_MAX_PULSE_US]. */
void S20F_SetPulseUs(uint16_t pulse_us);

/* DISABLE: PA7 goes low at the next period boundary (within 20 ms). */
/* ENABLE: resume the last command at the next period boundary. */
/* This controls PWM only, not servo power or guaranteed torque release. */
void S20F_Enable(FunctionalState state);

/* Last commanded pulse, not position feedback; 0 before successful init. */
uint16_t S20F_GetPulseUs(void);

/* Setters are ignored before successful init. Call from one context. */

#ifdef __cplusplus
}
#endif

#endif /* S20F_H */
