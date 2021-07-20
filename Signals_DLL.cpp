#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <fstream>
#include <string>
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/containers/vector.hpp"
#include "boost/interprocess/allocators/allocator.hpp"
#include "Interprocess.h"
#include "definitions.h"
#include "TrafficLight.h"


//*_______Defines_______*//
constexpr int STOT = (NUMSIMDelT - LOOK)/Deltat; //number of steps to get <LOOK> seconds from the end of the defined simulation time "NUMSIMDelT"


//*_______Functions_______*//
template <class T>
T RandInt(const T &minint, const T &maxint){
    return minint + static_cast<int> (rand())/(static_cast<int>(RAND_MAX/(maxint-minint)));
}

std::vector<std::pair<std::string, std::vector<double>>> read_ini(std::string filename){
    //Create storage
    std::vector<std::pair<std::string, std::vector<double>>> result;

    //Link file
    std::ifstream timing("timing.ini");

    //Ensure file is open
    if(!timing.is_open()){throw std::runtime_error("Couldn't find file");}

    //Placeholders
    std::string line;
    std::string colname;
    int val;

    //Read column names
    if(timing.good()){
        //Extract first line
        std::getline(timing,line);

        //Break up first line into column names
        std::stringstream ss(line);
        while(std::getline(ss,colname,',')){
            result.push_back({colname,std::vector<double>{}});
        }
    }
    else{
        throw std::runtime_error("Improperly formatted data");
    }

    //Extract data
    while(std::getline(timing,line)){
        std::stringstream ss(line);
        int col = 0;
        while(ss >> val){
            //add data to output vector
            result.at(col).second.push_back(val);

            //ignore commas
            if(ss.peek() == ','){
                ss.ignore();
            }

            //Increment column index
            col++;
        }
    }

    //close file
    timing.close();
    return result;
}


//*_______Simulation_______*//
int main(){
    //Read in VISSIM data
    std::vector<std::pair<std::string, std::vector<double>>> timing = read_ini("timing.ini");

    //Intialize Shared Memory from Boost Interprocess
    Interprocess IPC;

    //Initialize .csv file (for output testing)
    //std::ofstream sig_test("sig_test.csv");

    // Initialize random seed
    srand(static_cast<unsigned> (time(0)));

    // Initialize traffic light
    std::vector<TrafficLight> TL;

    //Assign traffic light timings and positions
    for(int a = 0; a < NUMTLIGHT; a++){
        TL.push_back(TrafficLight(a, timing));
    }

    //Run simulation to get traffic light states in time
    for (int j = 0; j <= STOT; j++) {

        //start timer
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<double> tchange[NUMTLIGHT];
        std::vector<double> cchange[NUMTLIGHT];
        std::string c1 = "";
        std::string c2 = "";
        for(int r = 1; r <= LOOK/Deltat; r++){
            for(int x = 0; x < NUMTLIGHT; x++){
                TL[x].TLColor(j+r-1);
                c1 = TL[x].color;
                TL[x].TLColor(j+r);
                c2 = TL[x].color;
                if(c1 != c2){
                    tchange[x].push_back(r*Deltat);
                    if(c1 == "Green"){
                        cchange[x].push_back(G_TO_Y);
                    }
                    else if(c1 == "Yellow"){
                        cchange[x].push_back(Y_TO_R);
                    }
                    else{
                        cchange[x].push_back(R_TO_G);
                    }
                }
            }
        }

        //Convert position data from int to vector<double>
        std::vector<double> posvec;
        for(int n = 0; n < NUMTLIGHT; n++){
            posvec.push_back(TL[n].pos);
        }

        //Write to IPC
        for(int w = 0; w < NUMTLIGHT; w++){
            IPC.UpdateLight(w, tchange[w]);
        }

        for(int w2 = NUMTLIGHT; w2 < NUMTLIGHT*2; w2++){
            IPC.UpdateLight(w2,cchange[w2-NUMTLIGHT]);
        }

        for(int w3 = NUMTLIGHT*2; w3 < NUMTLIGHT*3; w3++){
            IPC.UpdateLight(w3,posvec);
        }

        std::unordered_map<KeyType, MappedType> M = IPC.Cycle();

        //check if data is keyed correctly
        for (int u = 0; u < NUMTLIGHT; u++){
            if(IPC.CheckKey(static_cast<long>(u), M) == 0){
                std::cerr << "Traffic light " << u + 1 << " time data is not properly keyed to the memory\n";
            }
            if(IPC.CheckKey(static_cast<long>(u + NUMTLIGHT), M) == 0){
                std::cerr << "Traffic light " << u + 1 << " color change data is not properly keyed to the memory\n";
            }
            if(IPC.CheckKey(static_cast<long>(u + 2 * NUMTLIGHT), M) == 0){
                std::cerr << "Traffic light " << u + 1 << " position data is not properly keyed to the memory\n";
            }
        }
        

        //Write to .csv (for output testing)
        //for(int s = 0; s < NUMTLIGHT; s++){
            //for(int t = 0; t < tchange[s].size(); t++){
                //sig_test << tchange[s][t] << ",";
            //}
            //sig_test << "\n";
            //for(int u = 0; u < cchange[s].size(); u++){
                //sig_test << cchange[s][u] << ",";
            //}
            //sig_test << "\n\n";
        //}
        //sig_test << "\n\n";

        //end timer
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        int remain = 101 - elapsed.count(); //using 101 milliseconds produces loop times above 0.1 seconds but within acceptable deviation (~1 ms)

        std::this_thread::sleep_for(std::chrono::milliseconds(remain));

        //Loop Time Display
        std::cout << "Base code time is " << elapsed.count()/1000 << " s\n"; //convert code run time from milliseconds to seconds and print
        std::cout << "Loop time is " << (remain + elapsed.count())/1000 << " s\n"; //convert total loop time from milliseconds to seconds and print
    }
    return 0;
}