#include "labyrinth.hpp"

#include <iostream>
#include <queue>

using namespace std;

// Set and resize
Labyrinth::Labyrinth(const struct Position& size):Matrix<enum Cell>(size, Cell::WALL),gen(rd()) {}
Labyrinth::Labyrinth(int r, int c):Labyrinth(Position(r, c)) {}

void Labyrinth::resize(const struct Position& size) { Matrix<enum Cell>::resize(size, Cell::WALL); }
void Labyrinth::resize(int i, int j) { resize({i, j}); }

// Print
#define RESET   "\033[0m"
#define BLACK   "\033[40m"
#define RED     "\033[41m"
#define GREEN   "\033[42m"
#define YELLOW  "\033[43m"
#define BLUE    "\033[44m"
#define MAGENTA "\033[45m"
#define CYAN    "\033[46m"
#define WHITE   "\033[47m"

#define CASE_CHARACTER "  "

void Labyrinth::print() const {
    for (int i = 0; i < getRows(); i++) {
        for (int j = 0; j < getCols(); j++) {
            switch ((*this)(i, j)) {
                case Cell::WALL: cout << BLACK CASE_CHARACTER RESET; break;
                case Cell::PATH: cout << WHITE CASE_CHARACTER RESET; break;
                case Cell::START: cout << RED CASE_CHARACTER RESET; break;
                case Cell::END: cout << RED CASE_CHARACTER RESET; break;
                case Cell::VISITED: cout << CYAN CASE_CHARACTER RESET; break;
                case Cell::SOLUTION: cout << GREEN CASE_CHARACTER RESET; break;
                default: cout << YELLOW CASE_CHARACTER RESET; break;
            }
        }
        cout << "\n";
    }
}

// Tools 
bool Labyrinth::isInBoundsInner(const struct Position& p) const {
    return p.i > 0 && p.j > 0 && p.i < (getRows() - 1) && p.j < (getCols() - 1);
}

bool Labyrinth::isInBoundsInner(int i, int j) const {
    return isInBoundsInner({i, j});
}

bool Labyrinth::isCarvable(const struct Position& p) const {
    // Must be a wall in the inner bounds to be carved
    if (!isInBoundsInner(p) || ((*this)(p) != Cell::WALL))
        return false;

    // Count carved neighbours (PATH)
    int carved_neighbours = 0;

    for (const auto& np : getNeighbours(p)) {
        if ((*this)(np) != Cell::WALL)
            ++ carved_neighbours;

        if (carved_neighbours > 1)
            return false;
    }

    return true;
}

bool Labyrinth::isCarvable(int i, int j) const {
    return isCarvable({i, j});
}

// Clean / Clear to prepare the labyrinth for the method
void Labyrinth::clean() {
    for (int i = 0; i < getRows(); i++)
        for (int j = 0; j < getCols(); j++)
            (*this)(i, j) = Cell::WALL;
}

void Labyrinth::clearSolution() {
    (*this)(start) = Cell::START;
    (*this)(end) = Cell::END;

    for (int i = 1; i < getRows() - 1; i++) {
        for (int j = 1; j < getCols() - 1; j++) {
            switch ((*this)(i, j)) {
                case Cell::WALL:
                case Cell::PATH:
                case Cell::START:
                case Cell::END:
                    break;
                case Cell::VISITED:
                case Cell::SOLUTION:
                    (*this)(i, j) = Cell::PATH;
                    break;
                default:
                    break;
            }
        }
    }
}

// BFS and A* tool to reconstruct the path
void Labyrinth::reconstructPath(const Matrix<struct Position>& parent) {
    (*this)(start) = Cell::START;
    Position p = end;

    while (!(p == start)) {
        (*this)(p) = Cell::SOLUTION;
        p = parent(p);
    }

    (*this)(end) = Cell::END;
}

// Algorithms
void Labyrinth::generateGrowingTree() {
    vector<struct Position> active;

    start = {1, 0};
    (*this)(start) = Cell::START;
    active.push_back(start);

    while (!active.empty()) {
        uniform_int_distribution<> dis(0, active.size()-1);
        int idx = dis(gen);

        Position current = active[idx];

        vector<struct Position> ns = getNeighbours(current);
        shuffle(ns.begin(), ns.end(), gen);

        bool carved = false;

        for (const auto& np : ns) {
            if (isCarvable(np)) {
                (*this)(np) = Cell::PATH;
                active.push_back(np);
                carved = true;
                break;
            }
        }

        // No neighbour carved -> can't carve a new neighbour anymore so remove it from active set
        if (!carved)
            active.erase(active.begin() + idx);
    }

    int end_col = getCols() - 1;
    for (int end_row = getRows() - 2; end_row > 0; -- end_row) {
        if ((*this)({end_row, end_col - 1}) == Cell::PATH) {
            end = {end_row, end_col};
            break;
        }
    }
    (*this)(end) = Cell::END;
}

bool Labyrinth::solveBFS() {
    if (start == end) 
        return true;

    queue<Position> q;
    Matrix<struct Position> parent = Matrix<struct Position>(getRows(), getCols(), {-1,-1});

    q.push(start);
    parent(start) = start;

    while (!q.empty()) {
        struct Position current = q.front(); 
        q.pop();

        // Visit all the non visited neighbours
        for (const auto& np : getNeighbours(current)) {
            // If the end point is found -> early return
            if ((*this)(np) == Cell::END) {
                parent(np) = current;
                reconstructPath(parent);
                return true;
            }

            if (((*this)(np) != Cell::WALL) && ((*this)(np) != Cell::VISITED)) {
                (*this)(np) = Cell::VISITED;
                parent(np) = current;
                q.push(np);
            }
        }
    }

    (*this)(start) = Cell::START;
    (*this)(end) = Cell::END;
    return false;
}

bool Labyrinth::solveAStar() {
    if (start == end) 
        return true;

    // Distance from start
    Matrix<int> distance = Matrix<int>(getRows(), getCols(), numeric_limits<int>::max());
    Matrix<struct Position> parent = Matrix<struct Position>(getRows(), getCols(), {-1,-1});

    auto cmp = [](pair<int, Position> a, pair<int, Position> b) {
        return a.first >= b.first;
    };

    priority_queue<pair<int, Position>, vector<pair<int, Position>>, decltype(cmp)> pq(cmp);

    distance(start) = 0;
    pq.push({start.heuristic(end), start});

    while (!pq.empty()) {
        struct Position current = pq.top().second;
        pq.pop();
        int cost = distance(current) + 1;
        
        // Visit all the non visited neighbours
        for (const auto& np : getNeighbours(current)) {
            // If the end point is found -> early return
            if ((*this)(np) == Cell::END) {
                parent(np) = current;
                reconstructPath(parent);
                return true;
            }

            if (((*this)(np) != Cell::WALL) && ((*this)(np) != Cell::VISITED)) {
                (*this)(np) = Cell::VISITED;
                distance(np) = cost; // Visited only once so updated only once (acyclic graph) -> no need to assert if the new distance is smaller
                parent(np) = current;
                int cost_h = cost + end.heuristic(np);
                pq.push({cost_h, np});
            }
        }
    }
    return false;
}