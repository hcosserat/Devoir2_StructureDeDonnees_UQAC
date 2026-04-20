#pragma once

#include <vector>

// Random (Growing Tree)
#include <random>
#include <algorithm> // shuffle

#include "matrix.hpp"

// ---------------- ENUM ----------------
enum class Cell {
    WALL,
    PATH,
    START,
    END,
    VISITED,
    SOLUTION
};

// ---------------- Labyrinth ----------------
class Labyrinth : public Matrix<enum Cell> {
private:
    // radomization of the generation
    random_device rd;
    mt19937 gen;

    bool isInBoundsInner(const struct Position& p) const;
    bool isInBoundsInner(int i, int j) const;

    bool isCarvable(const struct Position& p) const;
    bool isCarvable(int i, int j) const;

    void reconstructPath(const Matrix<struct Position>& parent);

public:
    struct Position start;
    struct Position end;

    Labyrinth(const struct Position& size);
    Labyrinth(int r, int c);

    void resize(const struct Position& size);
    void resize(int i, int j);

    void print() const;

    // Clean the labyrinth before generating it
    void clean();
    void generateGrowingTree();

    // Clear the Labyrinth of the current solution stored
    void clearSolution();

    // ---------------- BFS ----------------
    bool solveBFS();

    // ---------------- A* ----------------
    bool solveAStar();
};
