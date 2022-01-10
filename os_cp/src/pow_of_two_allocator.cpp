#include "pow_of_two_allocator.hpp"
//powOf2 begins with 16

Pow2Allocator::Pow2Allocator(std::vector<int>& blocksAmount){
    freeBlocksLists = std::vector<std::list<char*>>(powsOf2.size());
    int bytesSum = 0;
    for (int i = 0; i < blocksAmount.size(); ++i){
        bytesSum += blocksAmount[i] * powsOf2[i];
    }
    data = (char*)malloc(bytesSum);
    char* dataCopy = data;
    for (int i = 0; i < blocksAmount.size(); ++i){
        for (int j = 0; j < blocksAmount[i]; ++j){
            //std::cout << "In constructor in adding in list\n";
            freeBlocksLists[i].push_back(dataCopy);
            *((int*)dataCopy) = powsOf2[i];
            dataCopy += powsOf2[i];
        }
    }

}

void* Pow2Allocator::allocate(int bytesAmount){
    if (bytesAmount == 0){
        return nullptr;
    }
    bytesAmount += sizeof(int);
    //std::cout << "freeBlocksLists.size = " << freeBlocksLists.size() << std::endl;
    /*for (auto el : freeBlocksLists){
        std::cout << el.size() << std::endl;
    }*/
    int ind = -1;
    for (int i = 0; i < freeBlocksLists.size(); ++i){
        if (bytesAmount <= powsOf2[i] && !freeBlocksLists[i].empty()){ // if requested amount of bytes is fit and such block exists
            ind = i;
            break;
        }
    }
    if (ind == -1){
        std::cout << "There isn't memory\n";
    }

    char* memory = freeBlocksLists[ind].front();
    freeBlocksLists[ind].pop_front();
    return (void*)(memory + sizeof(int));
}

void Pow2Allocator::deallocate(void* ptr){
    char* chPtr = (char*)ptr;
    chPtr = chPtr - sizeof(int);
    int blockSize = *((int*)chPtr);
    int ind = -1;
    for(int i = 0; i < powsOf2.size(); ++i){
        if(powsOf2[i] == blockSize){
            ind = i;
        }
    }

    freeBlocksLists[ind].push_back(chPtr);
}

Pow2Allocator::~Pow2Allocator(){
    std::cout << "In destructor\n";
    free(data);
}