## Shani Thapa 
* Uploaded Code at 10/9
* Populated 10/9

### Code 
Using Timers adds more complexity to the code. They have thier own version of the ISR code that needs to be added to use them: ```#pragma vector = TIMER0_B0_VECTOR 
__interrupt void Timer_B (void) ```. First, the interrupts from a specific timer must be enabled. The timer will request an interrupt when it reaches a certin value that can be set by the user. The timer can be then be configured to use a certain clock source, which mode it will run, and be given a clock divider. Once, all these are set, in the timer ISR, the LEDs can be set to blink. 
* TxxCCTLx is the capture/compare control registor of the timer.  
  * the x's are simply numbers and letters signifiying the speficic timer and capture/control register.
  * For example, TA0CCTLO represents Timer0_A and Capture/Control Register 0.  
* TxxCTL is the timer control, here is where the clock, mode, and clock divider needs to be selected. 
  * x's specify the timer 
* TxxCCRx is the timer capture/compare which is the value the timer counts up to
  * x's specify the timer and register 
* CCIE means enable interrupt for capture/compare registers
* TxSSEL_x determines the clock source select which has 4 different clocks determines by its bits
  * x can be 0, 1, 2, or 3; which represents TACLK, ACLK, SMCLK and INCLK(device specific). 
* ID_x is the input divider, it simply divides the clock by this amount
  * x can be 0, 1, 2, or 3; which simply divides the clock rate by 1, 2, 4, 8 respectivly
* MC_x is the mode control which decides between stop, up, continous, and up/down modes 
  * x can be 0, 1, 2, or 3; representing Stop, Up, Continuous, and Up/Down mode respectivly

#### Differences
Besides the previous differences encountered in the previous labs, such as the pinouts for the LEDs and buttons; the main distinction were the amount of timers and thier names between the boards. The following is a list and names of all the timers found in the five boards: 
* Each board had a different amount of timers and seperated into Timer A or B. 
  * FR2311: Timer0_A, Timer1_A, Timer0_B, Timer2_A, and Timer3_A 
  * FR6989: Timer0_B and Timer1_B
  * FR5994: Timer0_A, Timer1_A, Timer2_A,, Timer0_A, Timer3_A, and Timer0_B 
  * F5529: Timer0_A, Timer1_A, Timer2_A, and Timer0_B. 
  * G2553: Timer0_A, and Timer1_A 

##### Code Differences between G2 and FR2311 
```
\\ G2553
void inittimer(int hertz)       
{

    TA0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
    TA0CTL |= TASSEL_2 | MC_1 | ID_3;             // SMCLK/8, Up mode
    int capture = (125000)/hertz;                 // capture = 125000Hz/10Hz
    TA0CCR0 = capture;                            // (SMCLK/8)(12500Hz)

}
#pragma vector = TIMER0_A0_VECTOR   // Timer A1 interrupt service routine
__interrupt void Timer0_A0 (void)

{   if(timercount >= 100)
    {
        P1OUT ^= BIT6;
        P1OUT ^= BIT0;             // Toggle LEDs
        timercount = 0;            // reset counter
    }
    else
        timercount++;                 // increment timercount until 100
}

```
\\ FR2311
void inittimer(int hertz)
{

    TB0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
    TB0CTL |= TBSSEL_2 | MC_1 | ID_3;             // SMCLK/8, Up mode
    int capture = (125000)/hertz;                 // capture = 125000Hz/10Hz
    TB0CCR0 = capture;                            // (SMCLK/8)/(12500Hz) 

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
 

```