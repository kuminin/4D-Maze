//
//  DisjointSet.hpp
//  MazesBetter
//
//  Created by Kumin In on 2/19/16.
//  Copyright © 2016 Kumin In. All rights reserved.
//

#ifndef DisjointSet_hpp
#define DisjointSet_hpp

#include <stdio.h>
#include <string>
#include <bitset>

class DisjointSet {
public:
    DisjointSet();
    bool mergeSet(DisjointSet &);
    DisjointSet* findSet();
    const std::string getWall();
    char m_wall;
private:
    char m_rank;
    DisjointSet *parent;

};
#endif /* DisjointSet_hpp */
