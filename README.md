**What is this ?**

This is a repository that contains ready-to-execute codes that can be used by cockpit builders/flight sim enthusiasts to send/receive aircraft variables such as parking brake, speed knob, throttle, lcd display etc. 
Currently code only supports receiving data. For receiving Lvars, the difference required is to add a prefix of ‘**L:**’ before the API usage term.

_Latest Commit Changes:_

Updated codes to handle 2 Lvars of fbwA320 and 1 simconnect variable of the same aircraft
1. L:A32NX_TRK_FPA_MODE_ACTIVE
2. AUTOPILOT MANAGED SPEED IN MACH
3. L:A32NX_PARK_BRAKE_LEVER_POS
   
Find the above variables in FBW Doc -> https://docs.flybywiresim.com/fbw-a32nx/a32nx-api/a32nx-flightdeck-api/

_Note: Facing latency issues but I believe it can be rectified_

**Key features of rowentas-sync:**-
1. **Non-GUI based** mechanism. It's completely a C++ code-based configuration
2. No GUI and no additional libraries required
3. Easily integrate-able into your code
4. The code is developed on top of examples provided by MSFS sdk

**General Prerequisites:-**
1. Microsoft Flight Simulator 2020
2. MSFS SDK and SDK samples (download and install)
3. Microsoft Visual Studio with c++ extension
4. Arduino Mega board
5. Arduino software (Windows)

**How does this work in general?**
1. The cpp program will fetch aircraft variable from MSFS sim using simconnect dll
2. This variable will be sent to a serial port of the PC (the same port where Arduino is connected)
3. Arduino will process this variable and do specified task (eg. light an led, spin a stepper motor etc.)

**FBW A320 neo Example:-**
1. Download msfs sdk and sdk samples (First part of this blog will help you -> https://medium.com/@edwinvarghese4442/getting-aircraft-data-from-msfs2020-via-simconnect-simvarwatcher-simplest-tutorial-ce93a425ccd7)
2. Start the MSFS2020 sim and initiate a flight using the default a320 neo and aircraft position should be in runway 
3. Open Arduino IDE and load arduino_receive (find it in the arduino_receive folder of this repo), plug in your Arduino Mega board (must have led with resistor connected in pin 3) and select the board in the IDE
4. Verify/compile the Arduino code and upload to the board
5. Create a cpp project in visual studio and RequestData.cpp to source files
6. Open RequestData.cpp and make sure the port value is same arduino's port (COM3, COM1 etc are the usual ports). Change if required
7. Build and compile RequestData.cpp file from visual studio
8. Run the RequestData.cpp
9. Toggle variables mentioned in the latest commit and see LEDs configured in arduino

Note: Every time you unplug and plug the Arduino Mega, it resets itself, so you would need to compile and upload again. I believe a way around this is to programmatically compile and upload the sketch to Arduino via command line executions. This is not part of the code now, but it's in consideration. Please stay tuned. Thanks

**Some settings from my VS studio:-**
1. project> 'project name' properties > Configuration properties > Character set: Use unicode character set
2. project> 'project name' properties > c/c++ > precompiled headers: not using precompiled header



https://github.com/edwinvarghese4442/rowentas-sync/assets/26130907/6a2a61c9-dfc1-4299-85e5-aeeb27f7d033

Topic I am maintaining the flight simulator form -> https://forums.flightsimulator.com/t/getting-aircraft-data-simconnect-and-lvars-and-sending-it-to-arduino-using-just-simconnect-c-and-arduino/639872
