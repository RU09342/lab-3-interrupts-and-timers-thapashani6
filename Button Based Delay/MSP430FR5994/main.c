#include <msp430.h> 

/**
 * main.c
 * Change the frequency of the LED blinking depending on time button is pressed
 * MSP430FR5994
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
    P1DIR |= BIT1;          //Test LED Output
    P1OUT = BIT1;

    P5DIR &= ~(BIT5);       //Button input
    P5IE |= BIT5;           //P1.3 interrupt enable
    P5IES |= BIT5;          //high low edge
    P5REN |= BIT5;          //enable pull up resistor on button
    P5IFG &= ~(BIT5);          // IFG cleared
    P5OUT |= BIT5;

    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;//SMCLK/8, UP
    TA0CCR0 = 125000/freq;               // 100 Hz divided by the frequency

    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_0 + ID_3;//SMCLK/8, STOP
    TA1CCR0 = 12500;               // 10 Hz

    _BIS_SR(GIE);

    #pragma vector = TIMER1_A0_VECTOR           //Timer1_A
    __interrupt void TA1_ISR(void)
    {
        count ++;
        P1OUT ^= BIT1;     //Test LED
    }
    #pragma vector = TIMER0_A0_VECTOR           //Timer0_A
    __interrupt void TA0_ISR(void)
    {
        P1OUT ^= BIT0;
    }

    #pragma vector = PORT5_VECTOR               //ISR for Button P5.5
    __interrupt void Port_5(void)
{
     if((P5IN & BIT5) == BIT5)                  //button released
     {
         TA1CTL = MC_0;                         //stop the second clock

         if(count >= 30 )                       //when the button is quickly released
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
        P5IES ^= BIT5;
        P5IFG &= ~(BIT5);
}

