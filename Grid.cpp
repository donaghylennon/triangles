#include "Grid.h"
#include <vector>

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
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Grid::set_point(Point<double> p) {
    if (auto [iter, success] = this->points.insert(p); success)
        updated = true;
}

void Grid::unset_point(Point<double> p) {
    if (this->points.erase(p))
        updated = true;
}

void Grid::add_line(Point<double> p1, Point<double> p2) {
    this->lines.insert({p1, p2});
}

void Grid::add_equilateral(Point<double> p, double length) {
    Point<double> p2 = {p.x - length/2, p.y - length};
    Point<double> p3 = {p.x + length/2, p.y - length};

    this->add_line(p, p2);
    this->add_line(p, p3);
    this->add_line(p2, p3);
}

void Grid::create_sierpinski(Point<double> point, double length, int depth) {
    std::vector<Point<double>> triangles{point};

    for (int i = 0; i < depth; i++) {
        std::vector<Point<double>> new_triangles{};
        for (auto& p : triangles) {
            this->add_equilateral(p, length);
            this->add_equilateral(p, length/2);

            Point<double> p2 = {p.x - length/4, p.y - length/2};
            Point<double> p3 = {p.x + length/4, p.y - length/2};
            this->add_equilateral(p2, length/2);
            this->add_equilateral(p3, length/2);
            new_triangles.push_back(p2);
            new_triangles.push_back(p3);
        }
        triangles.reserve(triangles.size() + new_triangles.size());
        triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
        length /= 2;
    }
}

void Grid::draw_line(Point<int> p1, Point<int> p2) {
    double m = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
    double c = p1.y - (m*p1.x);

    if (m <= 1 && m >= -1) {
        int high_x = p1.x > p2.x ? p1.x : p2.x;
        int low_x = p1.x > p2.x ? p2.x : p1.x;

        for (int x = low_x; x < high_x+1; x++) {
            int y = (int) (m*x + c);
            set_pixel({x, y});
        }
    } else {
        double inv_m = (p1.x - p2.x) / (double)(p1.y - p2.y);
        int high_y = p1.y > p2.y ? p1.y : p2.y;
        int low_y = p1.y > p2.y ? p2.y : p1.y;

        for (int y = low_y; y < high_y+1; y++) {
            int x = (int) ((y - c) * inv_m);
            set_pixel({x, y});
        }
    }
}

void Grid::draw() {
    if (updated) {
        memset(draw_buf, 0, size_x*size_y*4);
        for (auto& l : this->lines) {
            this->draw_line(world_to_screen(l.first), world_to_screen(l.second));
        }
        for (const Point<double>& p : this->points) {
            set_pixel(world_to_screen(p));
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
    using frame_period = std::chrono::duration<long long, std::ratio<1, 60>>;

    bool running = true;
    auto prev = std::chrono::high_resolution_clock::now();
    auto current = prev;
    auto difference = current - prev;

    while (running) {
        while (difference < frame_period{1}) {
            current = std::chrono::high_resolution_clock::now();
            difference = current - prev;
        }
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
        prev = std::chrono::time_point_cast<
            std::chrono::high_resolution_clock::duration>(prev + frame_period{1});
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

Point<int> Grid::world_to_screen(Point<double> p) {
    double centre_x, centre_y;
    screen_to_world(size_x/2, size_y/2, centre_x, centre_y);

    double recentre_x = centre_x+offset_x - scale*(centre_x+offset_x);
    double recentre_y = centre_y+offset_y - scale*(centre_y+offset_y);
    int y = (int) (scale*(p.y + offset_y + recentre_y));
    int x = (int) (scale*(p.x + offset_x + recentre_x));
    return {x, y};
}

void Grid::set_pixel(Point<int> p) {
    if (p.y >= 0 && p.y < size_y && p.x >= 0 && p.x < size_x)
        draw_buf[(size_y - 1 - p.y) * size_x + p.x] = 0xFFFFFFFF;
}
