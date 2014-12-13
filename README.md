#Cinder-USBRelay

This is a Cinder-Block for the robot-electronics USB relay, the library has been tested with the [USB-RLY02](http://www.robot-electronics.co.uk/htm/usb_rly02tech.htm), but it should work with all the USB models.

For the USB Relay documentation please refer to the [Robot Electronics website](http://www.robot-electronics.co.uk/)

#### BEFORE you turn anything on please ensure you know what you are doing, 220V hurt!

## How to use it
```
// include the header
#include "USBRelay.h"

..

// create the device passing the serial device name and the number of outputs for the relay
mUSBRelay = USBRelay::create( "tty.usbmodem00015481", 2 ); 

..

// set the output( output/relay 0 == ON )
mUSBRelay->setOutput( 0, true );
```

##License
The MIT License (MIT)

Copyright (c) 2014 Nocte Studio Ltd. - [www.nocte.co.uk](http://www.nocte.co.uk)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.