#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <tuple>
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/shared_memory_object.hpp"
#include "Interprocess.h"
#include "definitions.h"

bool CheckKey(const long &i, const std::unordered_map<KeyType, MappedType> &M){
    if(M.find(i) == M.end()){
        return 0;
    }
    return 1;
}

int main(){
    int look = 60; //number of seconds to look ahead
    int stot = (NUMSIMDelT - look)/Deltat; //set run time to be [look] seconds from the end

    //Initialize Interprocess memory sharing
    Interprocess IPC;

    for(int j = 0; j <= stot; j++){
        //start timer
        auto start = std::chrono::high_resolution_clock::now();

        std::unordered_map<KeyType, MappedType> M = IPC.Cycle();

        std::cout << "Time: " << j*Deltat;
        for(int w = 0; w < NUMTLIGHT; w++){
            //Display time to next light change for each light i
            if(CheckKey(w, M)){
                std::cout << "      tchange[" << w << "]: ";
                for(int i = 0; i < M.at(w).size(); i++){
                    std::cout << M.at(w).at(i) << ", ";
                }
            }
        }
        std::cout << "\n";

        //end timer
        auto end = std::chrono::high_resolution_clock::now();

        //make loop run to ~0.1 seconds
        std::chrono::duration<double, std::milli> elapsed = end - start;
        int remain = 101 - elapsed.count();
        std::this_thread::sleep_for(std::chrono::milliseconds(remain));
    }

}