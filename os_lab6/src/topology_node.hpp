#ifndef OS_LAB6_CLION_TOPOLOGY_NODE_H
#define OS_LAB6_CLION_TOPOLOGY_NODE_H

#include <zmq.hpp>

class TopologyNode{
private:


public:
    int id;
    TopologyNode* parent;
    TopologyNode* oldestChild;
    TopologyNode* rightBro;
    bool removed;
    zmq::socket_t socket;
    //zmq::context_t context;

    TopologyNode(TopologyNode* parent, int id){
        this->parent = parent;
        this->oldestChild = nullptr;
        this->rightBro = nullptr;
        this->id = id;
        removed = false;
        //this->socket = zmq::socket_t(context, zmq::socket_type::req);
    }

};

#endif //OS_LAB6_CLION_TOPOLOGY_NODE_H
