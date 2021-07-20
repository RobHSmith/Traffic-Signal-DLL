# Traffic-Signal-DLL

This code is used to override the traffic light timings used by VISSIM, allowing for greater control and planning of the traffic light timings. 

# definitions.h
This header file contains the various constants that different files in the repository need to access in order to run. 

# Interprocess
A description of this class can be found at https://github.com/RobHSmith/Interprocess-Communication.

# TrafficLight
This class is used to set and report the state of a traffic light. The controlled parameters are the light position along the track, the time of the end of the green split, the time of the end of the amber split, and the full cycle time (which is also the time of the end of the red split). The code has each full cycle repeat until the simulation has ended. The functions this class includes are TLColor, which returns the current color of a signal based on a simulation step input, and Rand, which returns a random number. Currently, the Rand function is not in use since VISSIM provides the timings. However, this class can also be used with randomized signal timings by replacing the VISSIM timings.

# Signals_DLL
This file reads signal timings provided by VISSIM through a .ini file, allows the TrafficLight class to adjust to those timings, and simulates the lights over a given time. At each simulation step, the code gathers the next X seconds of traffic light states and files any changes in state (along with the time they change and the position of the light structure) to a shared memory created by the Interprocess class. These future timings are used by simulated vehicles in VISSIM to control their dynamics.
