#include <msp430.h>
/**
 * main.c
 * Change the frequency of the LED blinking depending on time button is pressed
 * MSP430FR2311
 * Shani Thapa
 * 10/7/17
 * Button Based Delay
 */



int count = 0;
int freq = 10;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode

P1DIR |= BIT0;          //LED output
P2DIR |= BIT0;          //Test LED Output
P2OUT = BIT0;


P1DIR &= ~(BIT1);       //Button input
P1IE |= BIT1;           //P1.3 interrupt enable
P1IES |= BIT1;          //high low edge
P1REN |= BIT1;          //enable pull up resistor on button
P1IFG &= ~(BIT1);          // IFG cleared
P1OUT |= BIT1;

TB0CCTL0 = CCIE;
TB0CTL = TBSSEL_2 + MC_1 + ID_3;//SMCLK/8, UP
TB0CCR0 = 125000/freq;               // 100 Hz divided by the frequency


TB1CCTL0 = CCIE;
TB1CTL = TBSSEL_2 + MC_0 + ID_3;//SMCLK/8, STOP
TB1CCR0 = 12500;               // 10 Hz

_BIS_SR(GIE);


}

#pragma vector = TIMER1_B0_VECTOR           //Timer1_B ISR
__interrupt void TB1_ISR(void)
{
    count ++;
    P2OUT ^= BIT0;     //Test LED
}
#pragma vector = TIMER0_B0_VECTOR           //Timer0_B
__interrupt void TB0_ISR(void)
{
    P1OUT ^= BIT0;
}

#pragma vector = PORT1_VECTOR               // ISR for button P1.1
__interrupt void Port_1(void)
{

 if((P1IN & BIT1) == BIT1)                  //button released
 {
     TB1CTL = MC_0;                         //STOP mode for second timer

     if(count >= 30 )                       //when the button is quickly released
     {
             TB0CCR0=count*100;
             freq = count/100;
     }
     else
     {
         freq++;
         TB0CCR0 = 125000/freq;
     }
 }
else                                        //Button is  Fully Pressed
    {
    TB1R = 0;
    count = 0;
    TB1CTL = TBSSEL_2 + MC_1 + ID_3;        //Starts the clock
    }
    P1IES ^= BIT1;
    P1IFG &= ~(BIT1);

 }

