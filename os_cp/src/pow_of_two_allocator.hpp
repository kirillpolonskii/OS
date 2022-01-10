#include <iostream>
#include <list>
#include <vector>

class Pow2Allocator {
private:
    std::vector<std::list<char*>> freeBlocksLists;
    std::vector<int> powsOf2 = {16,32,64,128,256,512,1024};
    char* data;
    
public:
    Pow2Allocator(std::vector<int>& blocksAmount);

    void* allocate(int bytesAmount);
    void deallocate(void *ptr);
    ~Pow2Allocator();
};