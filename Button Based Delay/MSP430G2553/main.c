#include <msp430.h> 
/**
 * main.c
 * Change the frequency of the LED blinking depending on time button is pressed
 * MSP430G2553
 * Shani Thapa
 * 10/7/17
 * Button Based Delay
 */


int count = 0;
int freq = 10;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer

P1DIR |= BIT0;          //LED output
//--------------------- Button 1 --------------------------

P1DIR &= ~(BIT3);       //Button input
P1IE |= BIT3;           //P1.3 interrupt enable
P1IES |= BIT3;          //high low edge
P1REN |= BIT3;          //enable pull up resistor on button
P1IFG &= ~BIT3;          // IFG cleared

TA0CCTL0 = CCIE;
TA0CTL = TASSEL_2 + MC_3 + ID_3;//SMCLK/8, UP Down
TA0CCR0 = 125000/freq;               // 100 Hz divided by the frequency

TA1CCTL0 = CCIE;
TA1CTL = TASSEL_2 + MC_0 + ID_3;//SMCLK/8, STOP
TA1CCR0 = 12500;               // 10 Hz

_BIS_SR(GIE);

}

#pragma vector = TIMER1_A0_VECTOR           //Timer1_A
__interrupt void TA1_ISR(void)
{
    count ++;
}
#pragma vector = TIMER0_A0_VECTOR           //Timer0_A
__interrupt void TA0_ISR(void)
{
    P1OUT ^= BIT0;
}
//
#pragma vector = PORT1_VECTOR               //ISR for Button P1.3
__interrupt void Port_1(void)
{

 if((P1IN & BIT3) == BIT3)                  //button released
 {
     TA1CTL = MC_0;                         //stop the second clock

     if(count >= 25 )                       //when the button is quickly released
     {
             TA0CCR0=count*125;
             freq = count/100;
     }
     else
     {
         freq++;
         TA0CCR0 = 125000/freq;
     }
 }
else                                        //Button is  Fully Pressed
    {
    TA1R = 0;
    count = 0;
    TA1CTL = TASSEL_2 + MC_1 + ID_3;        //Starts the clock
    }
    P1IES ^= BIT3;
    P1IFG &= ~(BIT1);

 }
