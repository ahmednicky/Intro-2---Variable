/*==============================================================================
 Project: Intro-2-Variables
 Date:    March 1, 2022
 
 This example program demonstrates the use of byte (char) constants and
 variables to count button presses and trigger actions when a limit is reached.
  
 Additional program analysis and programming activities demonstrate using bit
 (Boolean, or bool) variables to store state for operations such as preventing 
 multiple counting of a singe button press during successive program loops.
 Additional activities include the creation of a two-player rapid-clicker game,
 simulating a real-world toggle button, and counting switch contact bounce.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP410.h"         // Include UBMP4.1 constant and function definitions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Program constant definitions
const unsigned char maxCount = 50;
 #define pressed 0
 #define notPressed 1

// Program variable definitions
unsigned char SW2Count = 0;
unsigned char SW5Count = 0;
unsigned char loops = 0;
bool SW2Pressed = false;
bool SW5Pressed = false;

int main(void)
{
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
    // Code in this while loop runs repeatedly.
    while(1)
	{
        // Adding SW2Count & while loop
        if(SW2 == 0)
        {
            LED3 = 1;
            if(SW2Count < 255)
            {
                SW2Count ++;
                while(SW2 == 0);
                __delay_ms(50);
            }
        }
        // Turn on D3
        if(SW2Count == 1)
        {
            LED3 = 1;
        }
        // Turn on D4
        if(SW2Count == 2)
        {
            LED4 = 1;
        }
        // Reset it
        if(SW3 == 0 && SW2 == 1)
        {
            LED3 = 0;
            LED4 = 0;
            LED5 = 0;
            LED6 = 0;
            SW2Count = 0;
        }

        // Add a short delay to the main while loop.
        __delay_ms(10);

        // Toggle button

        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

/* Program Analysis
 * 
 * 1. The 'SW2Count' variable is created within RAM as an 8-bit memory location
 *    by the declaration: 'unsigned char SW2Count = 0;'
 *    What is the the maximum value an 8-bit variable can store? What are some
 *    of the benefits and drawbacks of using 8-bit variables in an 8-bit
 *    microcontroller?
 ~ The maximum value of 8-bit variable is 255. Longer battery life and the 8-bit
 * microcontroller can withstand 8-bit variable. An 8-bit microcontroller would normally only allow arithmetic operations
 * that output numbers ranging from 0 to 255, which is a limitation. 
 * 2. The constant 'maxCount' is defined using a declaration similar to that
 *    used for the SW2Count variable, but with the 'const' prefix added in the
 *    declaration. Can you think of some advantages of declaring a constant like
 *    this, using a separate statement above the main code, rather than just
 *    embedding the value of the constant where it is needed in the code?
 ~ Putting the declaration above the main code will tell the code to anticipate/familiarize itself. 
 * Putting the constant where it is needed in the code... the code will think that this is a code and print an error. 
 * 3. This program should light LED D3 every time SW2 is pressed, and light
 *    LED D4 once the count reaches 50. Try it, and count how many times you
 *    have to press the button until LED D4 turns on. SW3 resets the count so
 *    you can perform repeated attempts.
 ~ It took me 9 presses to turn on LED D4.  
 *    Did your count reach 50? Can you describe what the program is doing?
 *    (Hint: try pressing and releasing the button at different rates of speed.)
 ~ My count didn't reach 50. Each time I press SW2, LED3 turns on and SW2Count 
 * 4. Modify the second 'if' structure to add the else block, as shown below:

        if(SW2Count >= maxCount)
        {
            LED4 = 1;
        }
        else
        {
            LED4 = 0;
        }

 *    Now, press and hold pushbutton SW2 for at least 10 seconds while watching
 *    LED D4. LED D4 should stay on continuously while the value of SW2Count is
 *    higher than maxCount. If LED D4 turns off, what can you infer about the
 *    value of the SW2Count variable? Can you explain what happens to the
 *    SW2Count variable as the SW2 button is held?
 ~ If D4 turns off, that means the maximum value has been reached, so it starts again from 0. 
 * And zero is less/not equal to max count. The SW2Count is been added by 1 while holding SW2.   
 * 5. We can set a limit on the SW2Count variable by encapsulating its increment
 *    statement inside a conditional statement. In your program, replace the
 *    line 'SW2Count = SW2Count + 1;' with the code, below:
 
            if(SW2Count < 255)
            {
                SW2Count += 1;
            }

 *    This code demonstrates the use of the assignment operator '+=' to shorten
 *    the statement 'SW2Count = SW2Count + 1;' The same operation is performed,
 *    but in a more compact form. After adding this code, what is the maximum
 *    value that the SW2Count variable will reach? How does this affect the
 *    operation of LED D4 when SW2 is held?
 ~ The maximum value SW2Count can reach is 255. LED D4 won't turn it's self off because the if statement says if(SW2Count < 255) add 1 to SW2Count. 
 * And when you hold SW2Count will equal to 255, so it won't add 1 value to start the code again.   
 * 6. The fundamental problem with this program is that pushbutton SW2 is sensed
 *    in each cycle of the loop, and if its state is read as pressed, another
 *    count is added to the SW2Count variable. The program needs to be made to
 *    respond only to each new press, rather than just switch state -- in other
 *    words, to a *change* of SW2 state, from not-pressed to pressed. Doing this
 *    requires the use of another variable to store the prior state of SW2, so
 *    that its current state can be evaluated as being the same, or different
 *    from its state in the previous loop. Replace the initial if-else condition 
 *    with the following two if conditions:

        // Count new SW2 button presses
        if(SW2 == 0 && SW2Pressed == false)
        {
            LED3 = 1;
            SW2Pressed = true;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
        }

        // Clear pressed state if released
        if(SW2 == 1)
        {
            LED3 = 0;
            SW2Pressed = false;
        }
        
 *    These two if conditions make use of the Boolean SW2Pressed variable to
 *    store the current state of SW2 for the next cycle of the main while loop.
 *    Boolean variables can store 0/false or 1/true, interchangeably. The first
 *    if condition, above, compares the current SW2 state with the previously
 *    stored SW2Pressed variable so that a new count is only added when the SW2
 *    button is pressed and SW2Pressed is false. In the if structure, SW2Pressed
 *    is set to true before a count is added. The following if structure resets
 *    SW2Pressed to false when the button is released. Try the code to verify
 *    that it works.
 * 
 *    The conditional statement in the first if condition can also be written:

        if(SW2 == 0 && !SW2Pressed)

 *    The '!SW2Pressed' expression is read as 'not SW2Pressed' and is equivalent
 *    to SW2Pressed being false. Similarly, using the variable name by itself
 *    (eg. SW2Pressed) in a condition is equivalent to SW2Pressed being true.
 * 
 * 7. A pushbutton's logic state can also be defined as a word in a similar way
 *    to a variable (eg. the way SW2Pressed represents 1 or 0, or true or false)
 *    which can help to make the code more readable. Add the following lines to
 *    the 'Program constant definitions' section at the top of the code:
 
 #define pressed 0
 #define notPressed 1

 *    Now, instead of comparing the state of the button to 0 or 1, the button
 *    input can be compared with the named definition for 0 or 1, making the
 *    program more readable at the expense of hiding the actual switch value in
 *    the definition statement instead of making it obvious in the if structure.
 *    Try it in your code, and modify the SW3 reset button to work with the same
 *    pressed and notPressed definitions.
 
        // Count new SW2 button presses
        if(SW2 == pressed && SW2Pressed == false)
        {
            LED3 = 1;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
            SW2Pressed = true;
        }

        // Clear pressed state if released
        if(SW2 == notPressed)
        {
            LED3 = 0;
            SW2Pressed = false;
        }
        
 * 
 * Programming Activities
 * 
 * 1. Can you make a two-player rapid-clicker style game using this program as 
 *    a starting point? Let's use SW5 for the second player's pushbutton so that
 *    the two players can face each other from across the UBMP4 circuit board.
 *    Duplicate SW2Count and SW2Pressed to create SW5Count and SW5Pressed
 *    variables. Then, duplicate the required if condition structures and modify
 *    the variable names to represent the second player. LED D4 can still light
 *    if player 1 is the first to reach maxCount. Use LED D5 to show if the
 *    second palyer wins. Use a logical condition statement to reset the game
 *    by clearing the count and turning off the LEDs if either SW3 or SW4 is
 *    pressed.
      while(1)
	{
       // Player 1 
        if(SW2 == pressed && SW2Pressed == false)
        {
            LED3 = 1;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
            SW2Pressed = true;
        }

        // Clear pressed state if released for SW2
        if(SW2 == notPressed)
        {
            LED3 = 0;
            SW2Pressed = false;
        }

        // Turn on LED D4 for P1
        if(SW2Count >= maxCount)
        {
            LED4 = 1;
        }
        else
        {
            LED4 = 0;
        }

        // Player 2   
        if(SW5 == pressed && SW5Pressed == false)
        {
            LED6 = 1;
            if(SW5Count < 255)
            {
                SW5Count = SW5Count + 1;
            }
            SW5Pressed = true;
        }

        // Clear pressed state if released for SW5
        if(SW5 == notPressed)
        {
            LED6 = 0;
            SW5Pressed = false;
        }

          //Turn on LED D5 for P2
        if(SW5Count >= maxCount)
        {
            LED5 = 1;
        }
        else
        {
            LED5 = 0;
        }

        // Reset count and turn off LED D4
        if(SW3 == 0 || SW4 == 0)
        {
            LED4 = 0;
            LED5 = 0;
            SW2Count = 0;
            SW5Count = 0;
        }
        
 * 2. Use your knowledge of Boolean variables and logical conditions to simulate
 *    a toggle button. Each new press of the toggle button will 'toggle' an LED
 *    to its opposite state. (Toggle buttons are commonly used as push-on, 
 *    push-off power buttons in digital devices.)
        // Toggle button
        if(SW2 == 0 && SW2Pressed == false)
        {
            SW2Pressed = true;
            LED4 = !LED4;
        }
        if(SW2 == 1)
        {
            SW2Pressed = false;
        }

 * 3. A multi-function button can be used to enable one action when pressed, and
 *    a second or alternate action when held. A variable that counts loop cycles
 *    can be used to determine how long a button is held (just as the first
 *    program unitentionally did, because of the loop structure). Make a
 *    multifunction button that lights one LED when a button is pressed, and
 *    lights a second LED after the button is held for more that one second.

      unsigned char loops = 0;
    // Turn on D3 & add +1 to loops
        if(SW2 == 0)
        {
            LED3 = 1;
            if(loops < 255)
            {
                loops ++;
            }
        }
        // Turn on D4
        if(loops >= 100)
        {
            LED4 = 1;
        }
        // Turn off D3 & D4 & reset loops
        if(SW2 == 1)
        {
            loops = 0;
            LED3 = 0;
            LED4 = 0;
        }
        // Add a short delay to the main while loop.
        __delay_ms(10);

 * 4. Do your pushbuttons bounce? Switch bounce is the term that describes
 *    switch contacts repeatedly closing and opening before settling in their
 *    final (usually closed) state. Switch bounce in a room's light switch is
 *    not a big concern, but switch bounce can be an issue in a toggle button
 *    because the speed of a microcontroller lets it see each bounce as a new,
 *    separate event. Use a variable to count the number of times a pushbutton
 *    is pressed and display the count on the LEDs. Use a separate pushbutton
 *    to reset the count and turn off the LEDs so that the test can be repeated.
 *    To determine if your switches bounce, try pressing them at various speeds
 *    and using different amounts of force.
 
         // Adding SW2Count & while loop
        if(SW2 == 0)
        {
            SW2Count ++;
            while(SW2 == 0);
        }
        // One Bounce
        if(SW2Count == 1)
        {
            LED3 = 1;
        }
        // Two Bounces
        if(SW2Count == 2)
        {
            LED4 = 1;
        }
        // Three Bounces
        if(SW2Count == 3)
        {
            LED5 = 1;
        }
        // Four Bounces
        if(SW2Count == 4)
        {
            LED6 = 1;
        }
        // Reset it
        if(SW3 == 0 && SW2 == 1)
        {
            LED3 = 0;
            LED4 = 0;
            LED5 = 0;
            LED6 = 0;
            SW2Count = 0;
        }

        // Add a short delay to the main while loop.
        __delay_ms(10);
        
 * 5. Did your pushbuttons bounce? Can you think of a technique similar to the
 *    multi-function button that could be implemented to make your program
 *    ignore switch bounces. Multiple switch activations within a 50ms time span
 *    might indicate switch bounce and can be safely ignored.
 */
