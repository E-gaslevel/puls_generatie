/**
 * @file timer.c
 *
 * @date Jan 6, 2021
 * @author marcel
 */

#include "timer.h"
#include "logger.h"
#include "gpio.h"

#include <math.h>

#include "sl_sleeptimer.h"
#include "sl_bt_api.h"
//#include "sl_udelay.h"

//#include "app_log.h"

#include "em_timer.h"
#include "em_cmu.h"
#include "em_emu.h"

#define TICTOC_TIMER  (TIMER1)

static void timer_reset(void);
//#define CLOCK 38400000.0//1000000.0

static uint32_t overflowcounter = 0;
static bool timer_interrupt_flag = false;
static uint32_t total_time = 0;
static uint32_t pulseCount = 0;
static bool pulse_pwm = false;
bool pulse_done = false;
uint8_t periodsPerPulse = 1;   //amount of periods per pulse

void timer_pwm_pulse_init(uint32_t frequency, uint32_t dutycycle, uint8_t pulses)
{
    pulse_pwm = true;
    pulse_done = false;
    periodsPerPulse = pulses;   //amount of periods per pulse
    // Enable clock for TIMER0 module
    CMU_ClockEnable(cmuClock_TIMER0, true);

    // Configure TIMER0 Compare/Capture for output compare
    // Use PWM mode, which sets output on overflow and clears on compare events
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    timerCCInit.eventCtrl = timerEventRising;
    timerCCInit.mode = timerCCModePWM;
    timerCCInit.outInvert = true;
    TIMER_InitCC(TIMER0, 0, &timerCCInit);

    // Route TIMER0 CC0 to location 15 and enable CC0 route pin
    // TIM0_CC0 #15 is GPIO Pin PC10
    TIMER0->ROUTELOC0 |=  TIMER_ROUTELOC0_CC0LOC_LOC22;
    TIMER0->ROUTEPEN |= TIMER_ROUTEPEN_CC0PEN;

    // Set top value to overflow at the desired PWM_FREQ frequency
    TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_TIMER0) / frequency);

    // Set compare value for initial duty cycle
    TIMER_CompareSet(TIMER0, 0, (TIMER_TopGet(TIMER0) * dutycycle) / 100);

    // Initialize the timer
    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
    TIMER_Init(TIMER0, &timerInit);

    // Enable TIMER0 compare event interrupts to update the duty cycle
    TIMER_IntEnable(TIMER0, TIMER_IEN_CC0);
    NVIC_EnableIRQ(TIMER0_IRQn);
}


void timer_pulse_init(uint32_t frequency)
{
  pulse_pwm = false;
	pulse_done = false;
	uint32_t top_time = clock_freq/frequency/2; //38.4MHz/frequency devided by 2 due to high and low time of DAC/

	// Enable clock for TIMER1 module
	CMU_ClockEnable(cmuClock_TIMER0, true); //clock is 38.4MHz

	// Initialize and start timer with highest prescale
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	timerInit.enable = false;
	timerInit.prescale = timerPrescale1;

	//  timerInit.oneShot = true; // Generate only one pulse
	TIMER_Init(TIMER0, &timerInit);

	/* Enable TIMER0 interrupt in NVIC */
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* Set TIMER0 Top value */
	TIMER_TopSet(TIMER0, top_time);

	/* Enable TIMER0 IRQ on Overflow */
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);
	// Enable the TIMER
	TIMER_Enable(TIMER0, true);
}

bool timer_get_pulse_done(){
	return pulse_done;
}

/**
 * @brief create a pulse on the interrupt
 */
void TIMER0_IRQHandler (void)
{
    if(pulse_pwm)
    {
        /* Clear the interrupt flag in the beginning */
        uint32_t flags = TIMER_IntGet(TIMER0);
        TIMER_IntClear(TIMER0, flags);
        //pulseCount >= periodsPerPulse*2 ||
        if (pulseCount >= periodsPerPulse)
        {

            TIMER_Enable(TIMER0, false);
            pulse_done = true;
            pulseCount = 0;


            //TIMER_Reset(TIMER0);
            //GPIO_PinOutSet (PULSE_PORT, PULSE_PIN);
            //GPIO_PinModeSet (PULSE_PORT, PULSE_PIN, gpioModePushPull, 1);
        }
        else
        {
            pulseCount++; //16 cycles
        }
    } else {

        /* Clear the interrupt flag in the beginning */
        uint32_t flags = TIMER_IntGet(TIMER0);
        TIMER_IntClear (TIMER0, flags);
        for(uint8_t i = 0; i < periodsPerPulse; i++){
            GPIO_PinOutToggle (PULSE_PORT, PULSE_PIN);
        }
        GPIO_PinOutSet (PULSE_PORT, PULSE_PIN);
        TIMER_Enable(TIMER0, false);
        pulse_done = true;
  }
}


uint32_t timer_get_overflow_counter(){
  return overflowcounter;
}


static void timer_reset()
{
  TIMER_IntClear(TICTOC_TIMER, TIMER_IF_OF);
  TIMER_Enable(TICTOC_TIMER, false);
  timer_interrupt_flag = false;
  overflowcounter = 0;
}

/**
 * @brief start counting and keep track of overflow interrupts.
 */
void timer_tick()
{
    TIMER_CounterSet(TICTOC_TIMER, 0);
    TIMER_Enable(TICTOC_TIMER, true);
}


/**
 * @brief stop the timer and return the time in us.
 * @return time in us
 */
uint32_t timer_tock()
{
    uint32_t ticks_remaining = TIMER_CounterGet(TICTOC_TIMER);
    uint32_t overflow_ticks = overflowcounter * 65536;
    uint32_t total_ticks = ticks_remaining + overflow_ticks;
    double microsec = (double)((double)total_ticks / (double)total_time) * 1000000.0;

    TIMER_Enable(TICTOC_TIMER, false);
    overflowcounter = 0;
    timer_interrupt_flag = false;
    NVIC_EnableIRQ(TIMER1_IRQn);

    return microsec;
}

/**
 * @brief configure Timer1 to count.
 * @param[in] tictoc whether we use the function for tictoc or just counting up once.
 */
void timer_set_tictoc_us()
{
    /* setting the prescaler. The prescaler is also used for calculating the topset time. */
    TIMER_Prescale_TypeDef prescale;
    prescale = timerPrescale1;

    /* Enable clock for TIMER1 */
     CMU_ClockEnable(cmuClock_TIMER1, true);



    /* Initialize TIMER1 in Up mode with prescaling */
    //timerCCModeCompare
    TIMER_Init_TypeDef initValues = TIMER_INIT_DEFAULT;
    initValues.enable   = false;
    initValues.prescale = prescale;
    initValues.mode     = timerModeUp;
    initValues.debugRun = false;
    initValues.oneShot  = false;
    TIMER_Init(TICTOC_TIMER, &initValues);

    /* Set TIMER1 Top value */
    uint32_t power = pow(prescale, 2);
     if(power == 0)
       total_time = CMU_ClockFreqGet(cmuClock_TIMER1);
     else
       total_time = CMU_ClockFreqGet(cmuClock_TIMER1) / power;
    TIMER_TopSet(TICTOC_TIMER, total_time); //38.400.000/8 = 480000 ticks per second


    /* Enable underflow and overflow interrupt for TIMER1*/
     TIMER_IntEnable(TICTOC_TIMER, TIMER_IEN_OF);

     /* Enable TIMER1 interrupt vector in NVIC */
     NVIC_EnableIRQ(TIMER1_IRQn);

    /* Start TIMER1 */
    //TIMER_Enable(TICTOC_TIMER, true);
}


void TIMER1_IRQHandler(void)
{
    uint32_t flags = TIMER_IntGet(TIMER1);
    TIMER_IntClear(TICTOC_TIMER, flags);
    // Check if the timer overflowed
    if (flags & TIMER_IF_OF) {
      overflowcounter++;
    }
}

void timer_start_cryotimer()
{
	  CRYOTIMER_Enable(true);
}

void timer_stop_cryotimer()
{
	  CRYOTIMER_Enable(false);
}


/**
 * @brief goto EM1 sleep
 */
void timer_go_to_sleep(void)
{
	  EMU_EnterEM1();
}

/**
 * @brief goto EM3 sleep. Wake up after timer has anded.
 */
void timer_go_to_deep_sleep(void)
{
	  sl_udelay_wait(100); // small delay to make sure UART is done printing stuff.
	  CRYOTIMER_Enable(true);
#if defined(DEBUG) && !DEBUG
	EMU_EnterEM3(true);
#endif
}


void reset_cryo_timer()
{
    CRYOTIMER_IntClear(CRYOTIMER_IF_PERIOD);
    CRYOTIMER_Enable(false);
}

/**
 * @brief init the ultra low energy timer.
 * The Timer is not yet enabled this is done by calling: CRYOTIMER_Enable(true);
 *
 * @param timer timer can be fast, slow or init.
 */
void timer_set_cryto(CRYOTIMER_Period_TypeDef period)
{
    CMU_ClockEnable(cmuClock_CRYOTIMER, true);

    CRYOTIMER_Init_TypeDef init = CRYOTIMER_INIT_DEFAULT;
    init.enable = false;
    init.osc 	= cryotimerOscULFRCO;
    init.presc 	= cryotimerPresc_1;
    init.period = period;
    CRYOTIMER_Init(&init);

    CRYOTIMER_IntClear(CRYOTIMER_IF_PERIOD);
    CRYOTIMER_IntEnable(CRYOTIMER_IEN_PERIOD);
    NVIC_EnableIRQ(CRYOTIMER_IRQn);
}


/**
 * @brief ultra low energy timer interrupt handler
 */
void CRYOTIMER_IRQHandler()
{
    CRYOTIMER_IntClear(CRYOTIMER_IF_PERIOD);
    CRYOTIMER_IntEnable(CRYOTIMER_IEN_PERIOD);
    CRYOTIMER_Enable(false);
    sl_bt_external_signal(EXT_SIGNAL_TIMER_EXPIRED_FLAG);
}
