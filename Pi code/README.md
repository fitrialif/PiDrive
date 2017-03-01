#Drive and training data collection
So far, all this code does is collect training data according to keyboard input, while sending outputs to GPIO to drive a car. The car we used has two brushed motors, and works like a tank. We used relay modules and the wiringPi library to control them. The ncurses library is needed for keyboard input--type stty-sane after running to restore the terminal to normal. opencv/raspicam is used to capture images from the raspberry pi camera module.

No neural network drive code has been written for the pi as of yet.
