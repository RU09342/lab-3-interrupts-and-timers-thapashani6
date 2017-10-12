## Shani Thapa 
* Uploaded Code at 10/9
* Populated 10/9

### Code 
The code gets quite a bit more complex in this lab due to the use of interrupts, several new registers and ports must be manipulated. First, to make use of a button interrupt, the interrupt for that specific button must be enabled. Then the edge select must be chosen, where if the bit is set high(1) then the interrupt is triggered when it goes from High -> Low; or if set low(0) it will trigger when it goes from Low -> High. Finally, the interrupt flag must be cleared, set low, so it can be triggered again. Also, to use the Interrupt Service Routine the line of code is needed: ``` #pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)``` 
* PxIE can enable interrupts for that specific port if high(1)
* PxIES is the edge selection
* PxIFG can set or clear the interrupt flag
* _BIS_SR(GIE) enables global interrupt so interrupts can be used

### Differences 
The only differences between the boards are the different ports used due to different pinouts for the buttons. 

#### Example Code for Interrupt setup and ISR
```
//Interrupt Setup 

P1IE  |= BIT1;      // Interrupt Enable on Port 1;
P1IES |= BIT1;      // Interrupt Edge Select, from High -> Low, when button is pressed
P1IFG &= ~BIT1;     // Clear Interrupt Flag  

#pragma vector=PORT1_VECTOR    // ISR for P1.1
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;             // Toggle P1.0, LED1, with XOR
    P1IFG &= ~BIT1;            // Clear Interrupt flag at the end of the ISR
}
```

### Extra Work: Multiple Buttons
This is fairly simple just configure another button with an interrupt 
#### Example Code for F5529
```
    P1IE  |= BIT1;               // Interrupt Enable on Port 1;
    P1IES |= BIT1;               // Interrupt Edge Select, from High -> Low, when button is pressed
    P1IFG &= ~BIT1;              // Clear Interrupt Flag

    P2IE  |= BIT1;               // Interrupt Enable on Port 2;
    P2IES |= BIT1;               // Interrupt Edge Select, from High -> Low, when button is pressed
    P2IFG &= ~BIT1;              // Clear Interrupt Flag

    P1REN |= BIT1;               // enable resistor on P1.1
    P1OUT |= BIT1;               // set resistor to pull up on switch P1.1
    P1DIR |= BIT0;               // set P1.0 as an output

    P2REN |= BIT1;               // enable resistor on P2.1
    P2OUT |= BIT1;               // set resistor to pull up on switch P2.1
    P4DIR |= BIT7;               // set P4.7 as an output
```
#### Board Reasoning 
The reason I choose the F5529 was simply because it had the two free buttons to program for the lab. The FR2311 and G2553 both only had one other button which were already assigned as the board's reset. I choose the F5529 since would be the cheapest option between it, the FR6989, and the FR5994 
#### Known Issues
There is the issue of the other button interrup firing when it's in the middle of the other button interrupt. Tried to disable the IE for the other button but didn't have a way to renalbe afterwards. Could be solved with timers to renable the button IE.   

