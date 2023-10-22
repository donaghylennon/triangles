#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <cmath>
#include <utility>
#include <chrono>

#include "Point.h"
#include "Sierpinski.h"

class Grid {
public:
    Grid(int size_x, int size_y);
    ~Grid();

    void set_point(Point<double> p);
    void unset_point(Point<double> p);
    void add_line(Point<double> p1, Point<double> p2);
    void add_equilateral(Point<double> p, double length);
    void add_sierpinski(Sierpinski s);
    void create_sierpinski(Point<double> top, double length, int depth);
    void draw_line(Point<int> p1, Point<int> p2);
    void draw_sierpinski(Sierpinski t);
    void draw();
    void run();

private:
    int size_x, size_y;
    double offset_x, offset_y;
    double scale = 1;
    std::unordered_set<Point<double>> points;
    std::unordered_set<std::pair<Point<double>, Point<double>>> lines;
    std::vector<Sierpinski> sierpinski_triangles;
    bool updated = true;

    uint32_t *draw_buf;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    void world_to_screen(double x, double y, int& sx, int& sy);
    void screen_to_world(int x, int y, double& wx, double& wy);
    Point<int> world_to_screen(Point<double> p);
    void set_pixel(Point<int> p);
};
