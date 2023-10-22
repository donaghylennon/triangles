#pragma once

#include <iostream>

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
