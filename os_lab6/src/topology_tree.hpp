#ifndef OS_LAB6_CLION_TOPOLOGY_TREE_H
#define OS_LAB6_CLION_TOPOLOGY_TREE_H

#include "topology_node.h"

class TopologyTree{


public:
    TopologyNode* root;

    TopologyTree(){
        root = new TopologyNode(nullptr, -1);
    }

    TopologyNode* findNode(TopologyNode* curNode, int id){
        if(curNode != nullptr) {
            TopologyNode* ans = nullptr;
            if(!curNode->removed) {
                ans = findNode(curNode->oldestChild, id);
            }
            if(ans != nullptr && !ans->removed){
                return ans;
            }
            if(curNode->id == id && !curNode->removed){
                ans = curNode;
                return ans;
            }
                return findNode(curNode->rightBro, id);
        }
        else {
            return nullptr;
        }

    }

    TopologyNode* addNode(int nodeId, int parentId){
        TopologyNode* parent = findNode(root, parentId);
        TopologyNode* node = new TopologyNode(parent, nodeId);
        if(parent->oldestChild == nullptr){
            parent->oldestChild = node;
        }
        else{
            TopologyNode* leftBro = parent->oldestChild;
            while(leftBro->rightBro != nullptr){
                leftBro = leftBro->rightBro;
            }
            leftBro->rightBro = node;
        }
        return node;
    }

    void recursClear(TopologyNode* curNode, TopologyNode* deletedNode){// позже надо, наверное, переписать
        if(curNode != nullptr){
            recursClear(curNode->oldestChild, deletedNode);
            if(curNode != deletedNode){
                recursClear(curNode->rightBro, deletedNode);
                curNode->socket.close();
            }

            delete curNode;
        }
    }

    void removeNode(int nodeId){
        TopologyNode* deletedNode = findNode(root, nodeId);
        deletedNode->socket.close();
        deletedNode->removed = true;
    }

    void recursivePrint(TopologyNode* curItem){
        if (curItem != nullptr){
            std::cout << curItem->id;
            if(curItem->removed) {
                std::cout << "d ";
            }
            else{
                std::cout << " ";
            }
            if (curItem->oldestChild != nullptr){
                std::cout << ": [";
            }

            recursivePrint(curItem->oldestChild);

            if (curItem->rightBro != nullptr){
                std::cout << ", ";
            }
            recursivePrint(curItem->rightBro);
            if (curItem->rightBro == nullptr)
                std::cout << "]";
        }
    }

    void print(){
        recursivePrint(root);
    }
};

#endif //OS_LAB6_CLION_TOPOLOGY_TREE_H
