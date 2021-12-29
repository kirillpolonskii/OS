#include <iostream>
#include "zmq_helper.hpp"

zmq::context_t context;

int main() {
    zmq::socket_t sockAC(context, zmq::socket_type::rep);
    zmq::socket_t sockBC(context, zmq::socket_type::push);
    sockAC.bind(addressAC);
    sockBC.bind(addressBC);

    std::string curStr;
    while(true){
        zmq::message_t zMesFromA;
        if(sockAC.recv(zMesFromA) == -1){
            return 1;
        }
        std::string mesFromA = zMesFromA.to_string();
        if(mesFromA == "exit"){
            break;
        }
        else{
            std::cout << mesFromA << std::endl;
        }

        std::string mesToA("String was received");
        zmq::message_t zMesToA(mesToA);
        sockAC.send(zMesToA, zmq::send_flags::none);

        std::string mesToB = std::to_string(mesFromA.size());
        zmq::message_t zMesToB(mesToB);
        sockBC.send(zMesToB, zmq::send_flags::none);
    }


    sockAC.close();
    sockBC.close();
    return 0;
}