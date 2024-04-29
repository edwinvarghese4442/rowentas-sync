The Goal of the repository is to provide cockpit builders/flight sim enthusiasts ability to send/receive aircraft variables (for example. parking brake, speed knob, throttle, lcd display) with the help of a Non-GUI based approach
Note: Currently, only receive data functionality works

Key features of rowentas-sync:-
1. Code based configuration and execution
2. No GUI and no additional libraries required
3. Easily integrate-able to your own code
4. The code is developed on top of examples provided from msfs sdk

General Prerequisites:-
1. Microsoft Flight Simulator 2020
2. Microsoft Visual Studio with c++ extension
3. Arduino Mega board
4. Arduino software (Windows)

How does this work in general?
1. The cpp program will fetch aircraft variable from simconnect
2. The variable will be sent to a serial port of the PC (the same port where Arduino is connected)
3. Arduino will process this variable and do action

Parking brake toggle Example
1. Start the MSFS2020 sim and initiate a flight using the default a320 neo and aircraft position should be in runway 
2. Open Arduino IDE and load arduino_receive (find it in the arduino_receive folder of this repo), plug in your Arduino Mega board (must have led with resistor connected in pin 3) and select the board in the IDE
3. Verify/compile the Arduino code and upload to the board
4. Create a cpp project in visual studio and RequestData.cpp to source files
5. Open RequestData.cpp and make sure the port value is same arduino's port (COM3, COM1 etc are the usual ports). Change if required
6. Build and compile RequestData.cpp file from visual studio
7. Run the RequestData.cpp
8. Toggle aircraft parking brake and see how the led lights up and off

Note: Every time you unplug and plug the Arduino Mega, it resets itself, so you would need to compile and upload again. I believe a way around this is to programmatically compile and upload the sketch to Arduino via command line executions. This is not part of the code now, but it's in consideration. Please stay tuned. Thanks



https://github.com/edwinvarghese4442/rowentas-sync/assets/26130907/6a2a61c9-dfc1-4299-85e5-aeeb27f7d033

