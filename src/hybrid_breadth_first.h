#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

class HBF {
public:

    int NUM_THETA_CELLS = 90;
    double SPEED = 1.45;
    double LENGTH = 0.5;

    struct maze_s {
        int g;    // iteration
        double x;
        double y;
        double theta;
        double f;
    };

    struct maze_path {
        std::vector<std::vector<std::vector<int>>> closed;
        std::vector<std::vector<std::vector<maze_s>>> came_from;
        maze_s final;
    };


    /**
     * Constructor
     */
    HBF();

    /**
     * Destructor
     */
    virtual ~HBF();


    int theta_to_stack_number(double theta);

    int idx(double float_num);

    std::vector<maze_s> expand(maze_s state, int targetX, int targetY);

    maze_path search(std::vector<std::vector<int>> grid, std::vector<double> start, std::vector<int> goal);

    std::vector<maze_s>
    reconstruct_path(std::vector<std::vector<std::vector<maze_s>>> came_from, std::vector<double> start, maze_s final);

};
