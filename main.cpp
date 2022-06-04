#include "Grid.h"

#include <random>

int main() {
    Grid grid{1000, 1000};

    std::random_device r;
    std::default_random_engine generator(r());
    std::uniform_int_distribution<int> pos_distribution(0,999);

    //Point p1 = { pos_distribution(generator), pos_distribution(generator) };
    //Point p2 = { pos_distribution(generator), pos_distribution(generator) };
    //Point p3 = { pos_distribution(generator), pos_distribution(generator) };
    Point p1 = {500, 900};

    //grid.draw_line(p1, p2);
    //grid.draw_line(p1, p3);
    //grid.draw_line(p2, p3);

    int length = 800;
    grid.draw_equilateral(p1, length);

    bool running = true;
    while (running) {
        grid.draw();

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
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}
