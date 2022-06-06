#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <cmath>
#include <utility>
#include <chrono>

template<typename T>
struct Point {
    T x;
    T y;

    friend bool operator==(const Point& lhs, const Point& rhs) {
        return lhs.x==rhs.x && lhs.y==rhs.y;
    }

    friend std::ostream& operator<<(std::ostream& s, Point p) {
        return s << "Point {x: " << p.x << " y: " << p.y << "}";
    }
};

template<typename T> struct std::hash<Point<T>> {
    std::size_t operator()(Point<T> const& p) const noexcept {
        std::size_t h1 = std::hash<T>{}(p.x);
        std::size_t h2 = std::hash<T>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

template<typename T> struct std::hash<std::pair<Point<T>, Point<T>>> {
    std::size_t operator()(std::pair<Point<T>, Point<T>> const& p) const noexcept {
        std::size_t h1 = std::hash<Point<T>>{}(p.first);
        std::size_t h2 = std::hash<Point<T>>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class Grid {
public:
    Grid(int size_x, int size_y);
    ~Grid();

    void set_point(Point<double> p);
    void unset_point(Point<double> p);
    void add_line(Point<double> p1, Point<double> p2);
    void add_equilateral(Point<double> p, double length);
    void draw_line(Point<int> p1, Point<int> p2);
    void draw();
    void run();

private:
    int size_x, size_y;
    double offset_x, offset_y;
    double scale = 1;
    std::unordered_set<Point<double>> points;
    std::unordered_set<std::pair<Point<double>, Point<double>>> lines;
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
