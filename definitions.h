#pragma once //ensures single evaluation of file


//*_______Simulation_______*//
#define NUMSIMSTEPS 3000
#define Deltat 1 //time step (s)
#define LOOK 60 //number of seconds to look ahead in the simulation
constexpr int NUMSIMDelT = NUMSIMSTEPS*Deltat; //real-time length of simulation



//*_______Interprocess_______*//
#define MEMNAME "Signals_DLL"
#define MAPNAME "MyMap"


//*_______Traffic Lights_______*//
#define NUMTLIGHT 3 //number of traffic lights

//map state changes to numbers
#define G_TO_Y 1
#define Y_TO_R 2
#define R_TO_G 0

//first traffic light position range (m)
#define FPMIN 450
#define FPMAX 650

//subequent traffic light position modifier range (m)
#define PMIN 100
#define PMAX 700

//timing ranges (s)
#define CYCMIN 60
#define CYCMAX 180

#define STARTMIN 0
#define STARTMAX 10

//phase change ratios
#define GMINRATIO 3/8
#define GMAXRATIO 1/2
#define YMINRATIO 5/8
#define YMAXRATIO 11/16
