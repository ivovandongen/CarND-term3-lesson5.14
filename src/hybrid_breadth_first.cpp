#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include "hybrid_breadth_first.h"

using namespace std;

static double distance_to_goal(double x, double y, double targetX, double targetY) {
    return abs(x - targetX) + abs(y - targetY);
}

/**
 * Initializes HBF
 */
HBF::HBF() = default;

HBF::~HBF() = default;


int HBF::theta_to_stack_number(double theta) {
    /*
    Takes an angle (in radians) and returns which "stack" in the 3D configuration space
    this angle corresponds to. Angles near 0 go in the lower stacks while angles near
    2 * pi go in the higher stacks.
    */

    double new_theta = fmod((theta + 2 * M_PI), (2 * M_PI));
    int stack_number = (int) (round(new_theta * NUM_THETA_CELLS / (2 * M_PI))) % NUM_THETA_CELLS;
    return stack_number;
}


int HBF::idx(double float_num) {
    /*
    Returns the index into the grid for continuous position. So if x is 3.621, then this
    would return 3 to indicate that 3.621 corresponds to array index 3.
    */

    return int(floor(float_num));
}


vector<HBF::maze_s> HBF::expand(HBF::maze_s state, int targetX, int targetY) {
    int g = state.g;
    double x = state.x;
    double y = state.y;
    double theta = state.theta;

    int g2 = g + 1;
    vector<HBF::maze_s> next_states;
    for (int delta_i = -35; delta_i < 40; delta_i += 5) {
        double delta = M_PI / 180.0 * delta_i;
        double omega = SPEED / LENGTH * tan(delta);
        double theta2 = theta + omega;
        if (theta2 < 0) {
            theta2 += 2 * M_PI;
        }
        double x2 = x + SPEED * cos(theta);
        double y2 = y + SPEED * sin(theta);
        next_states.push_back({g2, x2, y2, theta2, g2 + distance_to_goal(x2, y2, targetX, targetY)});

    }
    return next_states;
}

vector<HBF::maze_s>
HBF::reconstruct_path(vector<vector<vector<HBF::maze_s>>> came_from, vector<double> start, HBF::maze_s final) {

    vector<maze_s> path = {final};

    int stack = theta_to_stack_number(final.theta);

    maze_s current = came_from[stack][idx(final.x)][idx(final.y)];

    stack = theta_to_stack_number(current.theta);

    double x = current.x;
    double y = current.y;
    while (x != start[0] || y != start[1]) {
        path.push_back(current);
        current = came_from[stack][idx(x)][idx(y)];
        x = current.x;
        y = current.y;
        stack = theta_to_stack_number(current.theta);
    }

    return path;

}

HBF::maze_path HBF::search(vector<vector<int>> grid, vector<double> start, vector<int> goal) {
    /*
    Working Implementation of breadth first search. Does NOT use a heuristic
    and as a result this is pretty inefficient. Try modifying this algorithm
    into hybrid A* by adding heuristics appropriately.
    */

    vector<vector<vector<int>>> closed(size_t(NUM_THETA_CELLS),
                                       vector<vector<int>>(grid[0].size(), vector<int>(grid.size())));
    vector<vector<vector<maze_s>>> came_from(size_t(NUM_THETA_CELLS),
                                             vector<vector<maze_s>>(grid[0].size(), vector<maze_s>(grid.size())));
    double theta = start[2];
    int stack = theta_to_stack_number(theta);
    int g = 0;

    maze_s state{
            g,
            start[0],
            start[1],
            theta,
            g + distance_to_goal(start[0], start[1], goal[0], goal[1])
    };

    closed[stack][idx(state.x)][idx(state.y)] = 1;
    came_from[stack][idx(state.x)][idx(state.y)] = state;
    int total_closed = 1;
    vector<maze_s> opened = {state};
    bool finished = false;
    while (!opened.empty()) {
        sort(opened.begin(), opened.end(), [](const auto &l, const auto &r) {
            return l.f < r.f;
        });

        maze_s current = opened[0]; //grab first element
        opened.erase(opened.begin()); //pop first element

        int x = int(current.x);
        int y = int(current.y);

        if (idx(x) == goal[0] && idx(y) == goal[1]) {
            cout << "found path to goal in " << total_closed << " expansions" << endl;
            maze_path path;
            path.closed = closed;
            path.came_from = came_from;
            path.final = current;
            return path;

        }
        vector<maze_s> next_state = expand(current, goal[0], goal[1]);

        for (auto &i : next_state) {
            int g2 = i.g;
            double x2 = i.x;
            double y2 = i.y;
            double theta2 = i.theta;

            if ((x2 < 0 || x2 >= grid.size()) || (y2 < 0 || y2 >= grid[0].size())) {
                //invalid cell
                continue;
            }
            int stack2 = theta_to_stack_number(theta2);

            if (closed[stack2][idx(x2)][idx(y2)] == 0 && grid[idx(x2)][idx(y2)] == 0) {

                opened.push_back(i);
                closed[stack2][idx(x2)][idx(y2)] = 1;
                came_from[stack2][idx(x2)][idx(y2)] = current;
                total_closed += 1;
            }
        }
    }

    cout << "no valid path." << endl;
    HBF::maze_path path;
    path.closed = closed;
    path.came_from = came_from;
    path.final = state;
    return path;

}

