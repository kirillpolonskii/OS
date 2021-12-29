#include <iostream>
#include "zmq_helper.hpp"

zmq::context_t context;

int main() {
    zmq::socket_t sockAB(context, zmq::socket_type::push);
    zmq::socket_t sockAC(context, zmq::socket_type::req);
    sockAB.bind(addressAB);
    sockAC.connect(addressAC);

    std::string curStr;
    while(std::cin >> curStr){
        if(curStr == "exit"){
            zmq::message_t zMesToC(curStr);
            sockAC.send(zMesToC, zmq::send_flags::none);
            zmq::message_t zMesToB(curStr);
            sockAB.send(zMesToB, zmq::send_flags::none);
            break;
        }
        zmq::message_t zMesToC(curStr);
        sockAC.send(zMesToC, zmq::send_flags::none);

        std::string mesToB = std::to_string(curStr.size());
        zmq::message_t zMesToB(mesToB);
        sockAB.send(zMesToB, zmq::send_flags::none);

        zmq::message_t zMesFromC;
        if(sockAC.recv(zMesFromC) == -1){
            return 1;
        }
        if(zMesFromC.to_string() == "String was received"){
            continue;
        }
    }


    sockAB.close();
    sockAC.close();
    return 0;
}
