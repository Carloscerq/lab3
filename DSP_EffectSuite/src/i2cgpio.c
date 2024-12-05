#include "i2cgpio.h"
#include "ezdsp5502_i2cgpio.h"

void initI2C()
{
    /* Setup I2C GPIOs for Switches */
    EZDSP5502_I2CGPIO_configLine(  SW0, IN );
    EZDSP5502_I2CGPIO_configLine(  SW1, IN );
}
