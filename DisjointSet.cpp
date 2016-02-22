//
//  DisjointSet.cpp
//  MazesBetter
//
//  Created by Kumin In on 2/19/16.
//  Copyright © 2016 Kumin In. All rights reserved.
//

#include "DisjointSet.hpp"

DisjointSet::DisjointSet(): m_wall(255), m_rank(0), parent(this){}

bool DisjointSet::mergeSet(DisjointSet &other) {
    DisjointSet *thisParent = this->findSet();
    DisjointSet *otherParent = other.findSet();
    
    if (thisParent == otherParent) {
        return false;
    }
    if (thisParent->m_rank >= otherParent->m_rank) {
        if(thisParent->m_rank == otherParent->m_rank) {
            thisParent->m_rank += 1;
        }
        otherParent->parent = thisParent;
    } else {
        thisParent->parent = otherParent;
    }
    return true;
}

DisjointSet* DisjointSet::findSet() {
    DisjointSet *child = this;
    DisjointSet *parent = this->parent;
    while (parent != parent->parent) {
        parent = parent->parent;
    }
    while (child != parent) {
        DisjointSet* tempParent = child->parent;
        child->parent = parent;
        child = tempParent;
    }
    return parent;
}

const std::string DisjointSet::getWall() {
    std::bitset<8> bit_wall(m_wall);
    return bit_wall.to_string();
}