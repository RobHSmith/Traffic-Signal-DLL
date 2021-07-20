#include <chrono>
#include <math.h>
#include "TrafficLight.h"


//*_____Constructor_____*//
TrafficLight::TrafficLight(int i, std::vector<std::pair<std::string, std::vector<double>>> timing){

    //position settings
    TrafficLight::pos = timing.at(1).second.at(i);

    //time settings
    TrafficLight::cyct = timing.at(4).second.at(i); //set the total cycle time for the light (s)
    TrafficLight::green_end = timing.at(2).second.at(i); //set the end of the green phase (s)
    TrafficLight::yellow_end = timing.at(3).second.at(i); //set the end of the yellow phase (s)

    //create array of multiples of cycle time within the simulation time
    for(int n = 0; n <= floor(NUMSIMDelT/TrafficLight::cyct); n++){ 
        TrafficLight::mult.push_back(TrafficLight::cyct*n); 
    }

    TrafficLight::multl = TrafficLight::mult.size();

    //Fill the "lights" array with color values at each time in the simulation (full seconds) based on time settings
    for(int g = 0; g <= NUMSIMDelT; g++){
        for(int h = 0; h <= TrafficLight::multl - 1; h++){ //compare time to light timings to determine light color
            if(g == 0){
                TrafficLight::lights[g] = "Green";
            }
            else if(g > TrafficLight::mult[h] && g <= TrafficLight::mult[h] + TrafficLight::green_end){
                TrafficLight::lights[g] = "Green";
            }
            else if(g > TrafficLight::mult[h] && g <= TrafficLight::mult[h] + TrafficLight::yellow_end){
                TrafficLight::lights[g] = "Yellow";
            }
            else if(g > TrafficLight::mult[h] + TrafficLight::yellow_end && g <= TrafficLight::mult[h] + TrafficLight::cyct){
                TrafficLight::lights[g] = "Red";
            }
        }
    }

}

TrafficLight::~TrafficLight(){}

//function to get color state of a light at a given timestep (truncated for access to array)
void TrafficLight::TLColor(const int &step){
    color = lights[static_cast<int> (step*Deltat)];
}

//function to create a random number of the input type T
template <class T>
T TrafficLight::Rand(const T &minT, const T &maxT){
    return minT + static_cast<int> (rand())/(static_cast<int>(RAND_MAX/(maxT-minT)));
}