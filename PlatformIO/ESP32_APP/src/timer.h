#ifndef ESP32_APP__HW_TIMER
#define ESP32_APP__HW_TIMER

#include <Arduino.h>

volatile int interruptCounter; // for counting interrupt
int totalInterruptCounter;     // total interrupt counting
int LED_STATE = LOW;
hw_timer_t *timer = NULL; // H/W timer defining (Pointer to the Structure)

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer()
{ // Defining Inerrupt function with IRAM_ATTR for faster access
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void hw_timer_setup()
{
    timer = timerBegin(0, 80, true);             // timer 0, prescalar: 80, UP counting
    timerAttachInterrupt(timer, &onTimer, true); // Attach interrupt
    timerAlarmWrite(timer, 1000000, true);       // Match value= 1000000 for 1 sec. delay.
    timerAlarmEnable(timer);                     // Enable Timer with interrupt (Alarm Enable)
}

void hw_timer_loop()
{
    if (interruptCounter > 0)
    {

        portENTER_CRITICAL(&timerMux);
        interruptCounter--;
        portEXIT_CRITICAL(&timerMux);

        totalInterruptCounter++; // counting total interrupt

        LED_STATE = !LED_STATE;           // toggle logic
        
        Serial.print("An interrupt as occurred. Total number: ");
        Serial.println(totalInterruptCounter);
        // Serial.println(getSystemISOTimeString());

        // led_matrix_clearRectangle(1, 35, 128, 8);
        // led_matrix_write_str(1, 35, getSystemISOTimeString());
    }
}

#endif