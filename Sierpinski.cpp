#include <cmath>

#include "Sierpinski.h"

Sierpinski::Sierpinski(Point<double> top, double side_length, int depth) {
    this->side_length = side_length;
    this->points[0] = top;
    this->points[1] = {top.x - side_length/2, top.y - sin(M_PI/3)*side_length};
    this->points[2] = {top.x + side_length/2, top.y - sin(M_PI/3)*side_length};
    if (depth > 0)
        this->add_children(depth-1);
}

Sierpinski::~Sierpinski() {
}

void Sierpinski::add_children(int depth) {
    this->children.emplace_back(this->points[0], this->side_length/2, depth);
    this->children.emplace_back(Point<double>{this->points[0].x - this->side_length/4,
            this->points[0].y - sin(M_PI/3)*this->side_length/2},
            this->side_length/2, depth);
    this->children.emplace_back(Point<double>{this->points[0].x + this->side_length/4,
            this->points[0].y - sin(M_PI/3)*this->side_length/2},
            this->side_length/2, depth);
}

void Sierpinski::remove_children() {
    this->children.clear();
}

Point<double> *Sierpinski::get_points() {
    return this->points;
}

std::vector<Sierpinski>& Sierpinski::get_children() {
    return this->children;
}
