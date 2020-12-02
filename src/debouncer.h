/** @file       debouncer.h
 *  @brief      File containing the header for a class that implements a debouncer program.
 * 
 *  @author Matthew Carlson
 *  @date  2020-Oct-24 Original file
 */


/** @brief   Class which implements a debouncer for button and switch noise
 *  @details Defines Protected variables that are used in the debouncer.cpp file
 *           Also sets up contructor "Debouncer" and function "update"
 * 
 */
class Debouncer
{
protected:
    /// The value of the threshold so that the debouncer knows when to switch states
    uint8_t threshold;

    /// The value of the threshold so that the debouncer knows when to switch states
    uint8_t input_pin;

    // State of the state machine
    uint8_t state = 0;   

    // define a variable "counter" that will be incrimented in a FOR loop and
    // and compared to "threshhold"
    uint8_t counter = 0;

    // This should not be needed, but it is used so that State 0 returns a FALSE and State 2 returns a TRUE
    bool button_result = 0;


public:
    // Constructor which is given a pin to look at and a threshold to compare against
    Debouncer (uint8_t pin, uint8_t threshold_constructor);

    bool update (void);
    
 
};
