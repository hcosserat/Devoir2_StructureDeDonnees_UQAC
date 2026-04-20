#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "KdTree.h"

using KDTree::KdTree;
using KDTree::Point;

namespace {

void printPoint(const Point& point) {
    std::cout << '(';
    for (std::size_t i = 0; i < point.dimension(); ++i) {
        std::cout << point[i];
        if (i + 1 < point.dimension()) {
            std::cout << ", ";
        }
    }
    std::cout << ')';
}

void printPoints(const std::vector<Point>& points) {
    std::cout << '[';
    for (std::size_t i = 0; i < points.size(); ++i) {
        printPoint(points[i]);
        if (i + 1 < points.size()) {
            std::cout << ", ";
        }
    }
    std::cout << ']';
}

void printSeparator(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printStatus(const std::string& label, bool value) {
    std::cout << std::left << std::setw(28) << label << (value ? "true" : "false") << '\n';
}

} // namespace

int main() {
    std::cout << "KD-Tree demonstration test\n";

    printSeparator("Construction");
    KdTree tree(3);
    printStatus("empty()", tree.empty());
    std::cout << std::left << std::setw(28) << "size()" << tree.size() << '\n';
    std::cout << std::left << std::setw(28) << "dimensions()" << tree.dimensions() << '\n';

    printSeparator("Insertion");
    const std::vector<Point> initialPoints = {
        {5.0, 5.0, 5.0},
        {2.0, 7.0, 4.0},
        {8.0, 1.0, 6.0},
        {1.0, 3.0, 9.0},
        {3.0, 6.0, 2.0},
        {7.0, 4.0, 8.0},
        {9.0, 9.0, 0.0}
    };

    for (const Point& point : initialPoints) {
        std::cout << "insert ";
        printPoint(point);
        std::cout << '\n';
        tree.insert(point);
    }

    printStatus("empty()", tree.empty());
    std::cout << std::left << std::setw(28) << "size()" << tree.size() << '\n';

    printSeparator("Tree View After Insertions");
    tree.print(std::cout);

    printSeparator("Contains");
    printStatus("contains(5,5,5)", tree.contains({5.0, 5.0, 5.0}));
    printStatus("contains(1,3,9)", tree.contains({1.0, 3.0, 9.0}));
    printStatus("contains(4,4,4)", tree.contains({4.0, 4.0, 4.0}));

    printSeparator("Duplicate Insertion");
    const std::size_t beforeDuplicate = tree.size();
    tree.insert({5.0, 5.0, 5.0});
    std::cout << std::left << std::setw(28) << "size before" << beforeDuplicate << '\n';
    std::cout << std::left << std::setw(28) << "size after" << tree.size() << '\n';

    printSeparator("Range Query");
    const Point minCorner{2.0, 3.0, 0.0};
    const Point maxCorner{8.0, 7.0, 8.0};
    const std::vector<Point> rangeResult = tree.rangeQuery(minCorner, maxCorner);
    std::cout << "rangeQuery ";
    printPoint(minCorner);
    std::cout << " -> ";
    printPoint(maxCorner);
    std::cout << " = ";
    printPoints(rangeResult);
    std::cout << '\n';

    printSeparator("Nearest Neighbor");
    const Point target{6.0, 5.0, 6.0};
    const Point nearest = tree.nearestNeighbor(target);
    std::cout << "target   = ";
    printPoint(target);
    std::cout << '\n';
    std::cout << "nearest  = ";
    printPoint(nearest);
    std::cout << '\n';

    printSeparator("Removal - Leaf");
    printStatus("contains(9,9,0) before", tree.contains({9.0, 9.0, 0.0}));
    std::cout << std::left << std::setw(28) << "remove(9,9,0)" << (tree.remove({9.0, 9.0, 0.0}) ? "true" : "false") << '\n';
    printStatus("contains(9,9,0) after", tree.contains({9.0, 9.0, 0.0}));
    std::cout << std::left << std::setw(28) << "size()" << tree.size() << '\n';

    printSeparator("Tree View After Leaf Removal");
    tree.print(std::cout);

    printSeparator("Removal - Internal Node");
    printStatus("contains(2,7,4) before", tree.contains({2.0, 7.0, 4.0}));
    std::cout << std::left << std::setw(28) << "remove(2,7,4)" << (tree.remove({2.0, 7.0, 4.0}) ? "true" : "false") << '\n';
    printStatus("contains(2,7,4) after", tree.contains({2.0, 7.0, 4.0}));
    std::cout << std::left << std::setw(28) << "size()" << tree.size() << '\n';

    printSeparator("Tree View After Internal Removal");
    tree.print(std::cout);

    printSeparator("End");
    return 0;
}
