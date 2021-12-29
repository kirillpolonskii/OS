#include <iostream>
#include "zmq_helper.hpp"

zmq::context_t context;

int main() {
    zmq::socket_t sockAB(context, zmq::socket_type::pull);
    zmq::socket_t sockBC(context, zmq::socket_type::pull);
    sockAB.connect(addressAB);
    sockBC.connect(addressBC);

    while(true){
        std::cout << "in while\n";
        zmq::message_t zMesFromA;
        if(sockAB.recv(zMesFromA) == -1){
            return 1;
        }
        std::string mesFromA = zMesFromA.to_string();
        if(mesFromA == "exit"){
            break;
        }
        else{
            std::cout << mesFromA << std::endl;
        }
        //sleep(1);
        zmq::message_t zMesFromC;
        if(sockBC.recv(zMesFromC) == -1){
            return 1;
        }
        std::string mesFromC = zMesFromC.to_string();
        std::cout << mesFromC << std::endl;
    }


    sockAB.close();
    sockBC.close();
    return 0;
}