#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <cmath>

struct Point {
    int x;
    int y;

    friend bool operator==(const Point& lhs, const Point& rhs) {
        return lhs.x==rhs.x && lhs.y==rhs.y;
    }

    friend std::ostream& operator<<(std::ostream& s, Point p) {
        return s << "Point {x: " << p.x << " y: " << p.y << "}";
    }
};

template<> struct std::hash<Point> {
    std::size_t operator()(Point const& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

class Grid {
public:
    Grid(int size_x, int size_y);
    ~Grid();

    void set_point(Point p);
    void unset_point(Point p);
    void draw_line(Point p1, Point p2);
    void draw_equilateral(Point p, int length);
    void draw();
    void run();

private:
    int size_x, size_y;
    std::unordered_set<Point> points;
    bool updated = true;

    uint32_t *draw_buf;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};
