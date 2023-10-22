#pragma once

#include <vector>

#include "Point.h"

class Sierpinski {
private:
    double side_length;
    Point<double> points[3];
    std::vector<Sierpinski> children;
public:
    Sierpinski(Point<double> top, double side_length, int depth);
    ~Sierpinski();
    void add_children(int depth);
    void remove_children();
    Point<double> *get_points();
    std::vector<Sierpinski>& get_children();
};
