# PMDC-Motor-Control-for-Crystal-Growth-Monitoring
Closed loop speed control of a PMDC motor. The control logic is designed to operate the motor at a smooth trapezoidal speed-time motion.
Additional units present : 
- Water-level maintainence system to automatically switch on water pump. 
- Automatic Temperature Control system.
- Monitoring-Camera angle control.

## Hardware Requirements :
 - PMDC Motor ( 12V, Max Speed- 33rpm )
 - Arduino UNO or Nano
 - L298 motor driver IC module ( for < 2A rating)
 - 2 channel Relay module (for >2A rating )
 - MOSFET switches x 2
 - Optical speed encoders- shaft mountable 
 - 12VDC power supply.
 
## Repository Description:
- L298 folder contains arduino code of the project implemented with L298 motor driver IC module for <2A motor rating
- Relay-based folder contains arduino code of the project implemented with 2 channel relays( for direction control) and 2 MOSFET Switches (for PWM generation) for >2A motor rating
- Command_line.exe contains user interface to feed input command before operation.
- input command line.cs contains C# code for user interface.
