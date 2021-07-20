#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/containers/map.hpp"
#include "boost/interprocess/containers/vector.hpp"
#include "boost/interprocess/allocators/allocator.hpp"
//#include <functional>
//#include <utility>
#include <unordered_map>

using namespace boost::interprocess;

//*_____typedefs_____*//
typedef long KeyType; //set as long instead of int to be compatible with VISSIM
typedef std::vector<double> MappedType;

//*_____Class_____*//
class Interprocess {
    private:
	    typedef allocator<double, managed_shared_memory::segment_manager> vecAllocator; 
        typedef vector<double, vecAllocator> vec;
        typedef std::pair<const KeyType, vec> ValueType; //assign each vec an ID of KeyType

        typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;
        typedef map<KeyType, vec, std::less<KeyType>, ShmemAllocator> ShmemMap; //create a "table" of KeyType keys and the assigned vectors

        managed_shared_memory shm;
        ShmemAllocator alloc_inst;
        vecAllocator vec_inst;

        ShmemMap *MyMap;
        vec *MyVec;
        offset_ptr<ShmemMap> ptrmap;

        permissions p;

        public:
        Interprocess();
        ~Interprocess();

        static void Remove(); //Remove function
        	
        void UpdateLight(KeyType TLID, const MappedType &info); //function to update the array in shared memory
	    bool CheckOp(KeyType TLID); //function to check for proper operation
        bool CheckKey(const long &i, const std::unordered_map<KeyType, MappedType> &M); //function to check if traffic light corresponds to data
	    MappedType GetLight(KeyType TLID); //function to read array in shared memory
        std::unordered_map <KeyType, MappedType> Cycle(); //function to read all data and build unordered map
};