## Shani Thapa
* Added Code on 10/9
* Populated README on 10/10

### Code
In this lab, the LED has to intially blink at a rate of 10Hz. Then when a button is pressed, the LED should change the rate of its blinking. Two timers are needed. One has its CCR set to the initial rate of 10Hz to blink an LED. The second timer is setup but starts in STOP mode. Then when the button is pressed, the second timer starts counting. In the ISR of the second timer, a variable in incrementing. So when the button is pressed once again, the if statement in the ISR will trigger and change the frequency of the LED. 

### Differences
There were no differences in this lab that were not found in the previous labs.

##### Extra Work: Reset 
A reset button was implemeted in the F5529. A button ISR was setup and set the CCR value back to 10Hz. 
```
#pragma vector = PORT1_VECTOR               // ISR for Button P1.1
__interrupt void Port_1(void)
{
    TA0CCR0 = 12500; //10Hz reset 
    P1IFG &= ~(BIT1);
}
```   