#ifndef NODE_H
#define NODE_H

#include "headers/render/AABB.h"

#include "headers/core/gameObject.h"

typedef struct Node{

    Node * parent = 0;
    GameObject * gameObject = 0;
    AABB  nodeBoundingBox;

    std::vector<Node *> children;

    Node(){
        this->nodeBoundingBox = AABB();
    }

} Node;


#endif // NODE_H
