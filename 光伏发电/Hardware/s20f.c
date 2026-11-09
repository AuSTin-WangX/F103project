#include "s20f.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define S20F_TICK_HZ 1000000UL

#if (S20F_MIN_PULSE_US == 0U) || \
    (S20F_MIN_PULSE_US >= S20F_CENTER_PULSE_US) || \
    (S20F_CENTER_PULSE_US >= S20F_MAX_PULSE_US) || \
    (S20F_MAX_PULSE_US >= S20F_PERIOD_US) || \
    (S20F_PERIOD_US > 65536UL)
#error Invalid S20F pulse configuration
#endif

static uint8_t s_initialized = 0U;
static uint8_t s_enabled = 0U;
static uint16_t s_pulse_us = 0U;

ErrorStatus S20F_Init(void)
{
    RCC_ClocksTypeDef clocks;
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef time_base;
    TIM_OCInitTypeDef oc;
    uint32_t timer_clock_hz;
    uint32_t divider;

    /* Refresh CMSIS clock information before querying SPL clocks. */
    SystemCoreClockUpdate();
    RCC_GetClocksFreq(&clocks);
    timer_clock_hz = clocks.PCLK1_Frequency;

    /* APB1 divider = 1: TIM3CLK = PCLK1; otherwise TIM3CLK = 2*PCLK1. */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0U)
    {
        timer_clock_hz *= 2U;
    }

    divider = timer_clock_hz / S20F_TICK_HZ;
    if ((timer_clock_hz % S20F_TICK_HZ != 0U) ||
        (divider == 0U) || (divider > 65536UL))
    {
        return ERROR;
    }

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,
                          ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Reserve the complete timer: no other TIM3 user may reconfigure it. */
    TIM_DeInit(TIM3);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, DISABLE);

    /* Hold the signal low while preparing the timer. */
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);

    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseStructInit(&time_base);
    time_base.TIM_Prescaler = (uint16_t)(divider - 1U);
    time_base.TIM_Period = (uint16_t)(S20F_PERIOD_US - 1U);
    time_base.TIM_CounterMode = TIM_CounterMode_Up;
    time_base.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &time_base);

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    oc.TIM_Pulse = 0U;
    TIM_OC2Init(TIM3, &oc);

    /* Preloading avoids changing pulse width partway through a period. */
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Active CCR2 is zero; load the center command at the first rollover. */
    /* Start during the low phase; the first high pulse is a full pulse. */
    TIM_SetCompare2(TIM3, S20F_CENTER_PULSE_US);
    TIM_SetCounter(TIM3, S20F_PERIOD_US - 1U);
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

    s_pulse_us = S20F_CENTER_PULSE_US;
    s_enabled = 1U;
    s_initialized = 1U;
    TIM_Cmd(TIM3, ENABLE);
    return SUCCESS;
}

void S20F_SetPulseUs(uint16_t pulse_us)
{
    if (s_initialized == 0U)
    {
        return;
    }

    if (pulse_us < S20F_MIN_PULSE_US)
    {
        pulse_us = S20F_MIN_PULSE_US;
    }
    else if (pulse_us > S20F_MAX_PULSE_US)
    {
        pulse_us = S20F_MAX_PULSE_US;
    }

    s_pulse_us = pulse_us;
    if (s_enabled != 0U)
    {
        TIM_SetCompare2(TIM3, pulse_us);
    }
}

void S20F_SetAngle(float angle_deg)
{
    float pulse_us;

    if (angle_deg != angle_deg) /* Reject NaN before float-to-int conversion. */
    {
        return;
    }
    if (angle_deg < 0.0f)
    {
        angle_deg = 0.0f;
    }
    else if (angle_deg > S20F_MAX_ANGLE_DEG)
    {
        angle_deg = S20F_MAX_ANGLE_DEG;
    }

    pulse_us = (float)S20F_MIN_PULSE_US +
               angle_deg / S20F_MAX_ANGLE_DEG *
               (float)(S20F_MAX_PULSE_US - S20F_MIN_PULSE_US);
    S20F_SetPulseUs((uint16_t)(pulse_us + 0.5f));
}

void S20F_Enable(FunctionalState state)
{
    if (s_initialized == 0U)
    {
        return;
    }

    s_enabled = (state != DISABLE) ? 1U : 0U;
    TIM_SetCompare2(TIM3, (s_enabled != 0U) ? s_pulse_us : 0U);
}

uint16_t S20F_GetPulseUs(void)
{
    return s_pulse_us;
}
