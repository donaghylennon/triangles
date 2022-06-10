#include "Grid.h"

int main() {
    Grid grid{1000, 1000};

    Point<double> p1 = {500, 900};
    int length = 8000;

    grid.create_sierpinski(p1, length, 10);

    grid.run();

    return 0;
}
