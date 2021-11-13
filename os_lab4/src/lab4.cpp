#include <iostream>
#include "stdio.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string>
#include "string.h"
#include <vector>
#include "semaphore.h"
#include <fcntl.h>
#include "unistd.h"

#define END_CODE 4
/*
Итак, здесь вместо пайпов я буду использовать отображаемые в память файлы (memory-mapped files).
Проблема1: я хз, как передать в дочерний процесс кол-во строк. Пока придумал только запихнуть в
структуру строк ещё одну переменную с этим числом.

Проблема 2: смещение offset в mmap должно быть кратно размеру страницы, возвращенному 
sysconf (_SC_PAGE_SIZE) (у меня это 4096)

Проблема 3: похоже, нужно два семафора.
*/

typedef struct
{
    int length;
    std::string str;
} MyStr;

struct Data
{
    Data(int n) : strings(n) {}
    int strAmount;
    std::vector<std::string> strings;
};

int main() {
    std::cout << "Enter outFile name:" << std::endl;
    std::string outFileName;
    std::cin >> outFileName;

    std::cout << "Enter amount of strings:" << std::endl;
    int strAmount;
    std::cin >> strAmount;

    //int END_CODE = 100000;
    int outFile;
    if ((outFile = open(outFileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR)) == -1){
        return 1;
    }
    int fdCheck = shm_open("/fileCheck", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR); // file for passing to child
    int fdError = shm_open("/fileError", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR); // file for passing to parent

    if (fdCheck == -1 || fdError == -1){
        return 1;
    }
    
    //std::mutex mtx;
    char* memDataCheck = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fdCheck, 0);
    char* memDataError = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fdError, 0);
    if (memDataCheck == MAP_FAILED || memDataError == MAP_FAILED){
        return 1;
    }

    sem_unlink("sem_par");
    sem_unlink("sem_child");
    sem_t* sem_par = sem_open("sem_par", O_CREAT, S_IWUSR | S_IRUSR, 0);
    if (sem_par == SEM_FAILED){
        std::cout << "Error in sem_open par\n";
        perror("par");
    }
    sem_t* sem_child = sem_open("sem_child", O_CREAT, S_IWUSR | S_IRUSR, 0);
    if (sem_child == SEM_FAILED){
        std::cout << "Error in sem_open child\n";
    }
    
    //sem_init(&sem_child, 1, 0);

    int pid = fork();	// When fork () is called, two completely identical processes arise.
    					// All code after the fork () is executed twice, both in the child and parent processes

    switch (pid) {
        case -1:{
            std::cout << "Fork has errors.\n";
            return -1;
        }
        case 0:{// It's child process. Now we need to decide on the direction of data transfer -
            // if we need to transfer data from parent to child, then the parent closes the descriptor
            // for reading, and the child closes the descriptor for writing
            
            usleep(100000);
            int val;

            if (sem_wait(sem_par) == -1){
                std::cout << "CHILD: error in sem_par\n";
            }
            

            ftruncate(fdError, 1);

            dup2(outFile, STDOUT_FILENO);

            int i = 0, j = 0, indForErr = 0;
            //for (int i = 0; i < strAmount; ++i) {
            while (i < strAmount){
                if (memDataCheck[j] == '\n'){
                    ++i;
                    
                }
                else if (memDataCheck[j] >= 65 && memDataCheck[j] <= 90) {
                    std::string message("Correct string: ");
                    while(memDataCheck[j] != '\n'){
                        char curSymb = memDataCheck[j];
                        message.push_back(curSymb);
                        ++j;
                    }
                    ++i;
                    ++j;
                    
                    message.push_back('\n');
                    std::cout << message;
                } else {
                    while(memDataCheck[j] != '\n'){
                        char curSymb = memDataCheck[j];
                        
                        memDataError[indForErr] = curSymb;
                        ++indForErr;
                        ++j;
                    }
                    memDataError[indForErr] = '\n';
                    ++i;
                    ++indForErr;
                    ++j;

	            }
                
                
            }
            memDataError[indForErr] = (char) END_CODE;

            sem_post(sem_child);
            munmap(memDataCheck, 4096);
            munmap(memDataError, 4096);
            close(outFile);
            sem_close(sem_par);
            sem_close(sem_child);
            
            break;
        }
        default:{ // It's a parent process.
            ftruncate(fdCheck, 1);

            std::cout << "Enter " << strAmount << " strings:" << "\n";
            int i = 0, j = 0;
            int prevStrLen = 0;
            while (i < strAmount){

                if (j == 0) getchar();
                char curSymb;
                scanf("%c", &curSymb);
                //std::cout << curSymb << "**\n";
                memDataCheck[j] = curSymb;
                if (curSymb == '\n'){
                    //std::cout << "Met slash n" << std::endl;
                    ++i;
                    //memDataCheck[j] = curSymb;

                }

                ++j;

            }
            memDataCheck[j] = (char) END_CODE;

            int val;

            sem_post(sem_par);

            usleep(100000);
            if (sem_wait(sem_child) == -1){
                std::cout << "PARENT: error in sem_child\n";
            }

            i = 0;
            j = 0;
            while(memDataError[j] != END_CODE){

                std::string msgInFile("Incorrect string: ");
                std::string message = "In Parent: <";
                while(memDataError[j] != '\n'){
                    char curSymb = memDataError[j];
                    message.push_back(curSymb);
                    msgInFile.push_back(curSymb);
                    ++j;
                }
                message += "> doesn't start with capital\n";
                std::cout << message;
                msgInFile += "\n";
                write(outFile, msgInFile.c_str(), msgInFile.size());

                ++j;

            }

            close(outFile);
            munmap(memDataCheck, 4096);
            munmap(memDataError, 4096);

            sem_close(sem_par);
            sem_close(sem_child);
        }
            
    }
    return 0;
}