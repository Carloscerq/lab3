#include "buttons.h"
#include "ezdsp5502_i2cgpio.h"

static Uint8 nextButtonState = 0;
static Uint8 selectButtonState = 0;

/*
 * Setup I2C GPIOs for Switches
 */
void initI2CButtons(){
    EZDSP5502_I2CGPIO_configLine(  SW0, IN );
    EZDSP5502_I2CGPIO_configLine(  SW1, IN );
}

/*
 *   Check if SW1 or SW2 was pressed.
 *     SW1 controls next selection
 *     SW2 controls select option
 */
void checkButtons(void){
    /* Check SW1 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW0))) // Is SW1 pressed?
    {
        if(!selectButtonState){
            nextButtonState = 1;     // Set state to 0 to allow only single press
        }
    } else {
        nextButtonState = 0;
    }

    /* Check SW2 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW1))) // Is SW2 pressed?
    {
        if(!nextButtonState){
            selectButtonState = 1;     // Set state to 0 to allow only single press
        }
    } else {
        selectButtonState = 0;
    }
}

void changeButtonState(Uint8 id){
    if(id == BUTTON_NEXT) {
        nextButtonState ^= 1;
    } else {
        selectButtonState ^= 1;
    }
}

Uint8 getButtonState(Uint8 id){
    if(id == BUTTON_NEXT) {
        return nextButtonState;
    } else {
        return selectButtonState;
    }
}
