#pragma once

#include <iostream>
#include <vector>

using namespace std;

// ---------------- STRUCT ----------------
struct Position {
    int i, j;

    bool operator==(const struct Position& other) const {
        return i == other.i && j == other.j;
    }

    struct Position operator+(const struct Position& other) const {
        return {i + other.i, j + other.j};
    }
    
    int heuristic(const struct Position& other) {
        return abs(i - other.i) + abs(j - other.j);
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& p) {
        return os << "Position = (" << p.i << ", " << p.j << ")";
    }
};

// ---------------- MATRIX ----------------
template <typename T>
class Matrix {
private:
    vector<vector<T>> data;

public:
    Matrix(const struct Position& size, const T& default_value)
    :data(size.i, vector<T>(size.j, default_value)) {}

    Matrix(int r, int c, const T& default_value)
    :Matrix({r, c}, default_value) {}

    void resize(const struct Position& size, const T& default_value) {
        data = vector<vector<T>>(size.i, vector<T>(size.j, default_value));
    }

    void resize(int i, int j, const T& default_value) {
        resize({i, j}, default_value);
    }

    T& operator()(const struct Position& p) {
        return data[p.i][p.j];
    }

    const T& operator()(const struct Position& p) const {
        return data[p.i][p.j];
    }

    T& operator()(int i, int j) {
        return data[i][j];
    }

    const T& operator()(int i, int j) const {
        return data[i][j];
    }

    int getRows() const { return data.size(); }
    int getCols() const { return data.empty() ? 0 : data[0].size(); }

    bool isInBounds(const struct Position& p) const {
        return p.i >= 0 && p.j >= 0 && p.i < getRows() && p.j < getCols();
    }

    bool isInBounds(int i, int j) const {
        return isInBounds({i, j});
    }

    inline static const vector<struct Position> dirs = {
        {-1, 0}, // up
        {1, 0}, // down
        {0, -1}, // left
        {0, 1} // right
    };

    vector<struct Position> getNeighbours(const struct Position& p) const {
        vector<struct Position> neighbours;

        for (const auto& d : dirs) {
            struct Position np = p + d;

            if (isInBounds(np)) {
                neighbours.push_back(np);
            }
        }

        return neighbours;
    }

    vector<struct Position> getNeighbours(int i, int j) const {
        return getNeighbours({i, j});
    }
};