
#include <msp430.h>
/**
 * main.c
 * Use Timers to blink LEDs
 * MSP430FR6989
 * Author: Shani Thapa
 * Last Updated: 10/7
 * 10/7/17
 * TIMER a Blink
 */
void inittimer(int capture);
unsigned int timercount = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;                                // set P1.0 as an output
    P1OUT &= ~BIT0;                               // turn off LED1
    P9DIR |= BIT7;                                // set P2.0 as an output
    P9OUT &= ~BIT7;                               // turn off LED2

    PM5CTL0 &= ~LOCKLPM5;                         // Disable the GPIO power-on default high-impedance mode to activate
                                                  // previously configured port settings
    inittimer(10);                                // initialize timer at 10Hz = 0.1s
    __bis_SR_register(LPM0_bits | GIE);           // Enter LPM0 w/ interrupts
}

void inittimer(int hertz)
{

    TA1CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
    TA1CTL |= TASSEL_2 | MC_1 | ID_3;             // SMCLK/8, Up mode
    int capture = (125000)/hertz;                 // capture = 125000Hz/10Hz
    TA1CCR0 = capture;                            // (SMCLK/8)/(12500Hz)

}
#pragma vector = TIMER1_A0_VECTOR   // Timer A1 interrupt service routine
__interrupt void Timer1_A0 (void)

{   if(timercount >= 100)          // when counter reaches 100Hz or 10s
    {
        P9OUT ^= BIT7;
        P1OUT ^= BIT0;             // Toggle LEDs
        timercount = 0;            // reset counter
    }
else timercount++;                 // increment timercount until 100
}
