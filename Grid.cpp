#include "Grid.h"

Grid::Grid(int size_x, int size_y) :
    size_x{size_x}, size_y{size_y},
    draw_buf{ new uint32_t[size_x*size_y]{} },
    window{ SDL_CreateWindow("Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            size_x, size_y, SDL_WINDOW_SHOWN) },
    renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) },
    texture{ SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size_x, size_y) }
{}

Grid::~Grid() {
    delete[] draw_buf;
}

void Grid::set_point(Point p) {
    if (auto [iter, success] = this->points.insert(p); success)
        updated = true;
}

void Grid::unset_point(Point p) {
    if (this->points.erase(p))
        updated = true;
}

void Grid::draw_line(Point p1, Point p2) {
    double m = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
    double c = p1.y - (m*p1.x);

    if (m <= 1 && m >= -1) {
        int high_x = p1.x > p2.x ? p1.x : p2.x;
        int low_x = p1.x > p2.x ? p2.x : p1.x;

        for (int x = low_x; x < high_x+1; x++) {
            int y = static_cast<int>(std::round(m*x + c));
            this->set_point({x,y});
        }
    } else {
        double inv_m = (p1.x - p2.x) / (double)(p1.y - p2.y);
        int high_y = p1.y > p2.y ? p1.y : p2.y;
        int low_y = p1.y > p2.y ? p2.y : p1.y;

        for (int y = low_y; y < high_y+1; y++) {
            int x = static_cast<int>(std::round((y - c) * inv_m));
            this->set_point({x,y});
        }
    }
}

void Grid::draw_equilateral(Point p, int length) {
    Point p2 = {p.x - length/2, p.y - length};
    Point p3 = {p.x + length/2, p.y - length};

    this->draw_line(p, p2);
    this->draw_line(p, p3);
    this->draw_line(p2, p3);
}

void Grid::draw() {
    if (updated) {
        memset(draw_buf, 0, size_x*size_y);
        for (const Point& p : this->points) {
            draw_buf[(size_y - p.y) * size_x + p.x] = 0xFFFFFFFF;
        }
        updated = false;
    }

    SDL_UpdateTexture(texture, nullptr, draw_buf, size_x*4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
