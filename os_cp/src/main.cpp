#include <iostream>
#include <chrono>

#include "pow_of_two_allocator.hpp"
#include "mck-k.hpp"

int main(){
    using namespace std::chrono;
    
    std::vector<int> blocksAmount = {64, 32, 16, 4, 20, 10, 0};

    steady_clock::time_point pow2AllocatorInitStart = steady_clock::now();
    Pow2Allocator pow2Allocator(blocksAmount);
    steady_clock::time_point pow2AllocatorInitEnd = steady_clock::now();

    std::cout << "Powers-of-2 allocator initialization: " << 
    std::chrono::duration_cast<std::chrono::nanoseconds>(pow2AllocatorInitEnd - pow2AllocatorInitStart).count() << " ns" << std::endl;
    std::cout << std::endl;

    
    int pagesAmount = 10;
    std::vector<int> pagesFragments = {32, 128, 256, 1024, 512, 256, 256, 1024, 16, 256};

    steady_clock::time_point mcKKAllocatorInitStart = steady_clock::now();
    McKKAllocator mcKKAllocator(pagesAmount, pagesFragments);
    steady_clock::time_point mcKKAllocatorInitEnd = steady_clock::now();

    std::cout << "McKusick-Karels allocator initialization: " << 
        std::chrono::duration_cast<std::chrono::nanoseconds>(mcKKAllocatorInitEnd - mcKKAllocatorInitStart).count() << " ns" << std::endl;

    std::cout << std::endl;

    
    std::cout << "Test: allocate 10 char[256], deallocate 5 of them, allocate 5 char[128]:\n";
    
    std::vector<char*> pointers1(10, 0);
    steady_clock::time_point pow2TestStart = steady_clock::now();
    for (int i = 0; i < 10; ++i){
        pointers1[i] = (char*)pow2Allocator.allocate(256);
    }
    for (int i = 5; i < 10; ++i){
        pow2Allocator.deallocate(pointers1[i]);
    }
    for (int i = 5; i < 10; ++i){
        pointers1[i] = (char*)pow2Allocator.allocate(128);
    }
    steady_clock::time_point pow2TestEnd = steady_clock::now();
    std::cerr << "Powers-of-2 allocator test:" << 
        std::chrono::duration_cast<std::chrono::microseconds>(pow2TestEnd - pow2TestStart).count() << " microseconds" << std::endl;
    for (int i = 0; i < 10; ++i){
        pow2Allocator.deallocate(pointers1[i]);
    }
    
    std::vector<char*> pointers2(10, 0);
    steady_clock::time_point mcKKTest1Start = steady_clock::now();
    for (int i = 0; i < 10; ++i){
        pointers2[i] = (char*)mcKKAllocator.allocate(256);
    }
    for (int i = 5; i < 10; ++i){
        mcKKAllocator.deallocate(pointers2[i]);
    }
    for (int i = 5; i < 10; ++i){
        pointers2[i] = (char*)mcKKAllocator.allocate(128);
    }
    steady_clock::time_point mcKKTest1End = steady_clock::now();
    std::cerr << "McKusick-Karels allocator test:" << 
        std::chrono::duration_cast<std::chrono::microseconds>(mcKKTest1End - mcKKTest1Start).count() << " microseconds" << std::endl;
    for (int i = 0; i < 10; ++i){
        mcKKAllocator.deallocate(pointers2[i]);
    }
    
}