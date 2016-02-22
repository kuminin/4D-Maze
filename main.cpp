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
#include "DisjointSet.hpp"

using namespace std;
void walk(int &);
bool hunt(int &);
bool validCoordinate(int &, int &);
void breakWall(int &, int &);
int myrandom(int);

const int  N = 35;
const int maze_size = N*N*N*N;
vector<vector<int> > direction =
{
    {N*N*N, 0, 0, 0},
    {-N*N*N, 0, 0, 0},
    {0, N*N, 0, 0},
    {0, -N*N, 0, 0},
    {0, 0, N, 0},
    {0, 0, -N, 0},
    {0, 0, 0, 1},
    {0, 0, 0, -1},
    
};

//DisjointSet maze[N][N][N][N];
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
    // initialize time, random point, child pid, and file
    clock_t begin = clock();
    srand (unsigned (time(0)));
    int q = rand() % (maze_size);
    pid_t child;
    fstream file;
    
    // Generate Maze
    do {
        walk(q);
    } while (hunt(q));
    
    // open file so that it will write in binary.
    file.open(argv[1], ios::binary| ios::out);
    
    // Write to the file in Bytes then close it
    for (int x = 0; x < maze_size; x++) {
        file.write((char *) &mazes[x].m_wall, sizeof(char));
    }
    file.close();
    
    // If argument counter == 3 and if the third argument is T, then we will go
    // into debug mode to check the diff of the file.
    if (argc == 3 && !strcmp(argv[2], "T")) {
        // Create a child process to run tester that will convert the BYTE file
        // into string format
        cout << "Parent Pid is " << getpid() << endl;
        if ((child = fork()) == 0) {
            cout << "Current Pid is " << getpid() << " running tester" << endl;
            execlp("./tester", "tester", argv[1], (char *) NULL);
        }
        
        // wait for the child to finish writing and
        wait(&child);
        cout << "Child is done Running and opening mainOutput" << endl;
        // open mainOutput and write the string representation of it then close the
        // file.
        file.open("mainOutput", ios::out);
        for (int i = 0; i < maze_size; i++) {
            file << mazes[i].getWall();
        }
        file.close();
        
        // create another process to check the difference between the files
        if (!(child = fork())) {
            execlp("diff", "diff", "output", "mainOutput", (char *) NULL);
        }

    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "elapsed_secs = " << elapsed_secs << endl;
    return 0;
}

// given points x we will choose a random coordinates from the possible
// direction to merge with. If the merge is successful, then we will
// destroy the walls between the two sets. Afterwards, we will set x to dx
// respectively.
void walk(int &x) {
    bool huntMode = false;
    do {
        random_shuffle(direction.begin(), direction.end(), myrandom);
        for (int i = 0; i < direction.size(); i++) {
            int dx;
            for (int j = 0; j < 4; j++) {
                if (direction[i][j] != 0) {
                    dx = x + direction[i][j];
//                    cout << "Merging (" << x << ") with (" << dx << ")" << endl;
                    break;
                }
            }
            if (validCoordinate(x, dx) &&
                mazes[x].mergeSet(mazes[dx])) {
//                cout << "Breaking Walls..." << endl;
//                cout << "Wall Before for (" << x << ") is " << mazes[x].getWall() << endl;
//                cout << "Wall Before for (" << dx << ") is " << mazes[dx].getWall() << endl;
                breakWall(x, dx);
//                cout << "Wall After for (" << x << ") is " << mazes[x].getWall() << endl;
//                cout << "Wall After for (" << dx << ") is " << mazes[dx].getWall() << endl;
                x = dx;
                break;
            } else if (i == direction.size()-1) {
//                cout << "Stuck, going to hunt mode" << endl;
                huntMode = true;
            } else {
//                cout << "Merging Failed" << endl;
            }
        }
    } while(!huntMode);
}

// Give points x we will traverse through the whole maze to find if
// there are any unvisited coordinates. If there exists an unvisted coordinate
// we will set the coordinates to x accordingly.
bool hunt(int &x) {
    for (int i = 0; i < maze_size; i++) {
        if (mazes[i].m_wall == (char)255) {
            int dx;
            for (int k = 0; k < direction.size(); k++) {
                for (int j = 0; j < 4; j++) {
                    if (direction[k][j] != 0) {
                        dx = i + direction[k][j];
                        break;
                    }
                }
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

// random generator function
int myrandom (int i) { return rand()%i;}