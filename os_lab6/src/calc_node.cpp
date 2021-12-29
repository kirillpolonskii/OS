

#include <iostream>
#include <zmq.hpp>
#include <map>
//this is a server side


zmq::context_t context;

int main(int argc, const char* argv[]) {
    char DELIM = '|';
    zmq::socket_t socket(context, zmq::socket_type::rep);
    //socket.setsockopt(ZMQ_SNDTIMEO, 4000);
    socket.bind(argv[0]);
    std::map<std::string, int> dict;

    bool exist = true;
    while(exist){
        zmq::message_t zIn;
        if(socket.recv(zIn) == -1){
            return 1;
        }

        std::string msgIn = zIn.to_string();
        std::string command = msgIn.substr(0, msgIn.find_first_of(DELIM));
        if(command == "ping"){
            std::string msgOut("Ok: 1");
            zmq::message_t zOut(msgOut);
            socket.send(zOut, zmq::send_flags::none);
        }
        else if(command == "exec"){
            int delimAmount = std::count(msgIn.begin(), msgIn.end(), DELIM);
            std::string msgOut("Ok:");

            if(delimAmount == 2){ // request for loading a value
                int id = std::stoi(msgIn.substr(msgIn.find_first_of(DELIM) + 1,
                                           msgIn.find_last_of(DELIM) - msgIn.find_first_of(DELIM) - 1));
                msgOut += std::to_string(id);
                std::string key = msgIn.substr(msgIn.find_last_of(DELIM) + 1);
                if(dict.find(key) == dict.end()){
                    msgOut += ": '" + key + "' not found";
                }
                else{
                    msgOut += ": " + std::to_string(dict[key]);
                }
            }
            else{ // request for adding a value
                int secDelimPos = msgIn.find(DELIM, msgIn.find_first_of(DELIM) + 1);
                int id = std::stoi(msgIn.substr(msgIn.find_first_of(DELIM) + 1,
                                           secDelimPos - msgIn.find_first_of(DELIM) - 1));
                msgOut += std::to_string(id);

                std::string key = msgIn.substr(secDelimPos + 1, msgIn.find_last_of(DELIM) - secDelimPos - 1);
                int value = std::stoi(msgIn.substr(msgIn.find_last_of(DELIM) + 1));
                dict[key] = value;

            }
            zmq::message_t zOut(msgOut);
            socket.send(zOut, zmq::send_flags::none);
        }
        else if(command == "kill"){
            exist = false;
        }

    }

    socket.close();
    return 0;
}
