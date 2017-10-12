
#include <msp430.h>
/**
 * main.c
 * Use Timers to blink LEDs
 * MSP430FR2311
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
    P1OUT &= ~BIT0;                              // turn off LED1
    P2DIR |= BIT0;                                // set P2.0 as an output
    P2OUT &= ~BIT0;                              // turn off LED2

    PM5CTL0 &= ~LOCKLPM5;                         // Disable the GPIO power-on default high-impedance mode to activate
                                                  // previously configured port settings
    inittimer(10);                                // initialize timer at 10Hz
    __bis_SR_register(LPM0_bits | GIE);           // Enter LPM3 w/ interrupts
}

void inittimer(int hertz)
{

    TB0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
    TB0CTL |= TBSSEL_2 | MC_1 | ID_3;             // SMCLK/8, Up mode
    int capture = (125000)/hertz;                 // capture = 125000Hz/10Hz
    TB0CCR0 = capture;                            // (SMCLK/8)/(12500Hz) the values where up mode resets after reaching

}
#pragma vector = TIMER0_B0_VECTOR   // Timer B0 interrupt service routine
__interrupt void Timer_B (void)

{   if(timercount >= 100)
    {
        P1OUT ^= BIT0;             // Toggle LEDs
        P2OUT ^= BIT0;
        timercount = 0;            // reset counter
    }
else timercount++;                 // increment timercount until 100
}

