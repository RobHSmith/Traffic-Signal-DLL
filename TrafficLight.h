#include <vector>
#include <string>
#include "definitions.h"

class TrafficLight{
    private:
        int st = 0; // simulation time (s), rounded down to integer second

    public:
        // Member Functions
        TrafficLight(int i, std::vector<std::pair<std::string, std::vector<double>>> timing);
        ~TrafficLight();

        void TLColor(const int &step);
        template <class T>
        T Rand(const T &minT, const T &maxT);


        // Member Variables
        int pos = 0; //traffic light position (m)
        std::string lights[NUMSIMDelT]; //array of lights
        std::string color = "Green";
        std::vector<int> mult;
        int cyct = 0; // traffic light cycle time (s)
        int green_end = 0; // green light end in cycle (s)
        int yellow_end = 0; // yellow light end in cycle (s)
        int multl = 0; //length of mult vector
};