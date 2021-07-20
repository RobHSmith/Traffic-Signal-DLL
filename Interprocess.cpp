#include "Interprocess.h"

#define MEMNAME "Signals_DLL"
#define MAPNAME "MyMap"

Interprocess::Interprocess() : shm(open_or_create, MEMNAME,1024000000), alloc_inst(shm.get_segment_manager()), vec_inst(shm.get_segment_manager())
{
    //Initialize map allocator, ptrmap allocator, vector allocator
    MyMap = shm.find_or_construct<ShmemMap>(MAPNAME)(std::less<KeyType>(), alloc_inst);
    ptrmap = shm.find_or_construct<ShmemMap>(MAPNAME)(std::less<KeyType>(), alloc_inst);
    MyVec = shm.find_or_construct<vec>("MyVec")(vec_inst);

    p.set_unrestricted();
}

Interprocess::~Interprocess(){
    Remove();
}

//Destroy Shared memory
void Interprocess::Remove(){
    shared_memory_object::remove(MEMNAME);
}


//Insert data into or Assign keyed vector
void Interprocess::UpdateLight(KeyType TLID, const MappedType &info){
    MyVec -> clear();
    for(int i = 0; i < info.size(); i++){
        MyVec->push_back(info[i]);
    }

    ptrmap = shm.find<ShmemMap>(MAPNAME).first;
    ptrmap -> insert_or_assign(TLID,*MyVec);
}

//Check if shared map can return vector by key
bool Interprocess::CheckOp(KeyType TLID){
    ptrmap = shm.find<ShmemMap>(MAPNAME).first;
    ShmemMap::iterator iter = ptrmap -> begin();

    for(; iter != ptrmap -> end(); iter++){
        if(iter -> first == TLID) {return 1;}
    }
    return 0;
}


//Return vector from shared map
MappedType Interprocess::GetLight(KeyType TLID){
    ptrmap = shm.find<ShmemMap>(MAPNAME).first;
    ShmemMap::iterator iter = ptrmap -> begin();

    for(; iter != ptrmap -> end(); iter++){
        if(iter -> first == TLID){
            return MappedType(iter -> second.begin(), iter -> second.end());
        }
    }

    throw std::runtime_error("Didn't find lights.");

}

//Read all of shared map and build unordered_map to return to DLL
std::unordered_map<KeyType, MappedType> Interprocess::Cycle(){
    std::unordered_map<KeyType, MappedType> R;
    ptrmap = shm.find<ShmemMap>(MAPNAME).first;

    ShmemMap::iterator iter = ptrmap -> begin();
    for(; iter != ptrmap -> end(); iter++){
        R[iter -> first] = MappedType(iter -> second.begin(), iter -> second.end());
    } 
    return R;
}

//Check if traffic light key corresponds to data
bool Interprocess::CheckKey(const long &i, const std::unordered_map<KeyType, MappedType> &M){
    if(M.find(i) == M.end()){
        return 0;
    }
    return 1;
}

