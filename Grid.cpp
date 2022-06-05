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
        double high_x = p1.x > p2.x ? p1.x : p2.x;
        double low_x = p1.x > p2.x ? p2.x : p1.x;

        for (double x = low_x; x < high_x+1; x++) {
            double y = m*x + c;
            this->set_point({x,y});
        }
    } else {
        double inv_m = (p1.x - p2.x) / (double)(p1.y - p2.y);
        double high_y = p1.y > p2.y ? p1.y : p2.y;
        double low_y = p1.y > p2.y ? p2.y : p1.y;

        for (double y = low_y; y < high_y+1; y++) {
            double x = (y - c) * inv_m;
            this->set_point({x,y});
        }
    }
}

void Grid::draw_equilateral(Point p, double length) {
    Point p2 = {p.x - length/2, p.y - length};
    Point p3 = {p.x + length/2, p.y - length};

    this->draw_line(p, p2);
    this->draw_line(p, p3);
    this->draw_line(p2, p3);
}

void Grid::draw() {
    if (updated) {
        memset(draw_buf, 0, size_x*size_y*4);
        for (const Point& p : this->points) {
            double centre_x, centre_y;
            screen_to_world(size_x/2, size_y/2, centre_x, centre_y);

            double recentre_x = centre_x+offset_x - scale*(centre_x+offset_x);
            double recentre_y = centre_y+offset_y - scale*(centre_y+offset_y);
            int y = (int) (scale*(p.y + offset_y + recentre_y));
            int x = (int) (scale*(p.x + offset_x + recentre_x));
            if (y >= 0 && y < size_y && x >= 0 && x < size_x)
                draw_buf[(size_y - 1 - y) * size_x + x] = 0xFFFFFFFF;
        }
        draw_buf[size_y/2*size_x + size_x/2] = 0xFFFFFFFF;
        updated = false;
    }

    SDL_UpdateTexture(texture, nullptr, draw_buf, size_x*4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Grid::run() {
    bool running = true;
    while (running) {
        this->draw();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    SDL_Quit();
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            SDL_Quit();
                            break;
                        case SDLK_EQUALS:
                            updated = true;
                            scale *= 1.1;
                            break;
                        case SDLK_MINUS:
                            updated = true;
                            scale *= 0.9;
                            break;
                        case SDLK_w:
                            updated = true;
                            offset_y += 10;
                            break;
                        case SDLK_a:
                            updated = true;
                            offset_x -= 10;
                            break;
                        case SDLK_s:
                            updated = true;
                            offset_y -= 10;
                            break;
                        case SDLK_d:
                            updated = true;
                            offset_x += 10;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Grid::world_to_screen(double x, double y, int& sx, int& sy) {
    sx = (int) scale*(x + offset_x);
    sy = (int) scale*(y + offset_y);
}

void Grid::screen_to_world(int x, int y, double& wx, double& wy) {
    wx = (double) (x)/scale - offset_x;
    wy = (double) (y)/scale - offset_y;
}
