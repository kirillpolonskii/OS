#include "mck-k.hpp"
//powOf2 begins with 16

McKKAllocator::McKKAllocator(int pagesAmount, std::vector<int>& pagesFragments){

    freeBlocksLists = std::vector<std::list<char*>>(powsOf2.size());
    data = (char*)malloc(pagesAmount * PAGE_SIZE);
    char* curPageStart = data;
    char* curPageEnd = curPageStart + (PAGE_SIZE - 1);
    kMemSize = std::vector<Page>(pagesAmount);

    for(int i = 0; i < pagesAmount; ++i){
        kMemSize[i].blockSize = pagesFragments[i];
        kMemSize[i].start = curPageStart;
        kMemSize[i].end = curPageEnd;
        curPageStart += PAGE_SIZE;
        curPageEnd += PAGE_SIZE;
    }

    for (int i = 0; i < kMemSize.size(); ++i){
        int ind = -1;
        for(int j = 0; j < powsOf2.size(); ++j){
            if(kMemSize[i].blockSize == powsOf2[j]){
                ind = j;
                break;
            }
        }
        char* curBlockStart = kMemSize[i].start;
        for(int j = 0; j < PAGE_SIZE / kMemSize[i].blockSize; ++j){
            //std::cout << "In constructor in adding in list\n";
            freeBlocksLists[ind].push_back(curBlockStart);
            curBlockStart += kMemSize[i].blockSize;
        }
        
    }
}

void* McKKAllocator::allocate(int bytesAmount){
    if (bytesAmount == 0){
        return nullptr;
    }

    //std::cout << "1freeBlocksLists.size = " << freeBlocksLists.size() << std::endl;
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
    return (void*)memory;
}

void McKKAllocator::deallocate(void* ptr){
    char *chPtr = (char*)ptr;

    int indPage = -1;
    for(int i = 0; i < kMemSize.size(); ++i){
        if(kMemSize[i].start <= chPtr && chPtr <= kMemSize[i].end){
            indPage = i;
            break;
        }
    }

    int indBlock = -1;
    for(int j = 0; j < powsOf2.size(); ++j){
        if(kMemSize[indPage].blockSize == powsOf2[j]){
            indBlock = j;
            break;
        }
    }

    freeBlocksLists[indBlock].push_back(chPtr);
}

McKKAllocator::~McKKAllocator(){
    std::cout << "In destructor1\n";
    free(data);
}