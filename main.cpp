//
//  main.cpp
//  MazesBetter
//
//  Created by Kumin In on 2/19/16.
//  Copyright © 2016 Kumin In. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <array>
#include "DisjointSet.hpp"

using namespace std;
void walk(int &);
bool hunt(int &);
bool validCoordinate(int &, int &);
void breakWall(int &, int &);
void mazeGenerationAlgorithm();

const int  N = 35;
const int maze_size = N*N*N*N;
array<int, 8> direction = {N*N*N, -N*N*N, N*N, -N*N, N, -N, 1, -1};
DisjointSet mazes[maze_size];

// 01111111 = 127 negative in x
// 10111111 = 191 positive in x
// 11011111 = 223 negative in y
// 11101111 = 239 positive in y
// 11110111 = 247 negative in z
// 11111011 = 251 positive in z
// 11111101 = 253 negative in t
// 11111110 = 254 positive in t

int main(int argc, const char * argv[]) {
    // seed current time and initialize file
    clock_t begin = clock();
    srand (unsigned (time(0)));
    fstream file;
    
    // Generate Maze Using Hunt-And-Kill Algorithm.
//    int q = 0;
//    do {
//        walk(q);
//    } while (hunt(q));
//
    // Generate Maze Algorithm
    mazeGenerationAlgorithm();
    
    // open file so that it will write in binary.
    file.open(argv[1], ios::binary | ios::out);
    
    // Write to the file in Bytes then close it
    for (int x = 0; x < maze_size; x++) {
//        cout << mazes[x].getWall() << endl;
        file.write((char *) &mazes[x].m_wall, sizeof(char));
    }
    file.close();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << elapsed_secs << endl;
    return 0;
}

// We will choose a random point on the given maze and randomly choose a
// neighbor of the current existing maze. We will first validate the given
// neighbor and if the neighbor is not connected, it has a wall up, we will
// merge and break the walls of the neighbor and the current point. This
// algorithm will run until the maze is fully broken down. If the current
// point has no neighbors to merge to, remove it from the list.
void mazeGenerationAlgorithm() {
    int wallsDown = 0;
    while (wallsDown < maze_size-1) {
        int x = rand() % maze_size;
        int dx = x + direction[rand() % direction.size()];
        if (validCoordinate(x, dx) && mazes[x].mergeSet(mazes[dx])) {
//          cout << "Merged " << x << " with " << dx << endl;
            breakWall(x, dx);
            wallsDown++;
//          cout << "Walls Down: " << wallsDown << endl;
        }
    }
}

// given points x we will choose a random coordinates from the possible
// direction to merge with. If the merge is successful, then we will
// destroy the walls between the two sets. Afterwards, we will set x to dx
// respectively.
void walk(int &x) {
    bool huntMode = false;
    do {
        vector<int> a;
        for (int i = 0; i < direction.size(); i++) {
            int dir = direction[rand() % direction.size()];
            while (find(a.begin(), a.end(), dir) != a.end()) {
                dir = direction[rand() % direction.size()];
            }
            a.push_back(dir);
            int dx = x + dir;
            if (validCoordinate(x, dx) &&
                mazes[x].mergeSet(mazes[dx])) {
                // cout << "Merging " << x << " With " << dx << endl;
                breakWall(x, dx);
                x = dx;
                break;
            } else if (i == direction.size()-1) {
                huntMode = true;
            }
        }
        a.clear();
    } while(!huntMode);
}

// Give points x we will traverse through the whole maze to find if
// there are any unvisited coordinates. If there exists an unvisted coordinate
// we will set the coordinates to x accordingly.
bool hunt(int &x) {
    for (int i = 0; i < maze_size; i++) {
        if (mazes[i].m_wall == (char)255) {
            for (int k = 0; k < direction.size(); k++) {
                int dx = i + direction[k];
                if (validCoordinate(i, dx) && (mazes[dx].m_wall != (char)255)) {
                    mazes[i].mergeSet(mazes[dx]);
                    breakWall(i, dx);
                    x = i;
                    return true;
                }
            }
        }
    }
    return false;
}

// Given points x and dx, the function will if the given points are a valid move
// within the maze. If the points are valid, the function will return true, else
// return false.
bool validCoordinate(int &x, int &dx) {
    if ((dx < 0 || dx >= (maze_size))) {
        return false;
    } else if (dx - x == 1 && x % N == N-1) {
        return false;
    } else if (dx - x == -1 && x % N == 0) {
        return false;
    } else if (dx - x == N && (x % (N*N)) >= (N*N)-N) {
        return false;
    } else if (dx - x == -N && (dx % (N*N)) >= (N*N)-N) {
        return false;
    } else if (dx - x == N*N && (x % (N*N*N) >= (N*N*N)-(N*N))) {
        return false;
    } else if (dx - x == -N*N && (dx % (N*N*N) >= (N*N*N)-(N*N))) {
        return false;
    }
    return true;
}

// Given points x and if the difference between two disjointSets are positive;
// then, the left DisjointSet's left wall will be broken down and the right
// DisjointSet's right wall will be broken down. If the difference between two
// disjointSets are negative; then, the left DisjointSet's right wall will be
// broken down and the right DisjointSet's left wall will be broken down.
void breakWall(int &x, int &dx) {
    if (dx - x == N*N*N) {
        mazes[x].m_wall &= (char)191;
        mazes[dx].m_wall &= (char)127;
    } else if (dx - x == -N*N*N) {
        mazes[x].m_wall &= (char)127;
        mazes[dx].m_wall &= (char)191;
    } else if (dx - x == N*N) {
        mazes[x].m_wall &= (char)239;
        mazes[dx].m_wall &= (char)223;
    } else if (dx - x == -N*N) {
        mazes[x].m_wall &= (char)223;
        mazes[dx].m_wall &= (char)239;
    } else if (dx - x == N) {
        mazes[x].m_wall &= (char)251;
        mazes[dx].m_wall &= (char)247;
    } else if (dx - x == -N) {
        mazes[x].m_wall &= (char)247;
        mazes[dx].m_wall &= (char)251;
    } else if (dx - x == 1) {
        mazes[x].m_wall &= (char)254;
        mazes[dx].m_wall &= (char)253;
    } else if (dx - x == -1) {
        mazes[x].m_wall &= (char)253;
        mazes[dx].m_wall &= (char)254;
    }
}