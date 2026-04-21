#include <iostream>

#include "labyrinth.hpp"

using namespace std;

int main() {
    cout << "EXERCICE 3 : LABYRINTH" << std::endl;
    cout << "Initial Labyrinth :" << std::endl;
    Labyrinth lab = Labyrinth(5, 5);
    lab.generateGrowingTree();
    lab.print();

    struct Position size;
    string method;

    while (true) {
        cout << "Enter Command (Help or h to display help manual) : ";
        cin >> method;

        if ((method == "Help") || (method == "h")) {
            cout << "\n================= HELP =================\n";
            cout << "Commands available:\n\n";

            cout << "  Help / h\n";
            cout << "    -> Display this help menu\n\n";

            cout << "  Resize / re\n";
            cout << "    -> Change maze size (i j)\n\n";

            cout << "  Regenerate / r\n";
            cout << "    -> Generate a new maze with the same size\n\n";

            cout << "  Display / d\n";
            cout << "    -> Display the current unsolved maze\n\n";

            cout << "  BFS / bfs\n";
            cout << "    -> Solve the maze using Breadth-First Search\n\n";

            cout << "  ASTAR / A* /a*\n";
            cout << "    -> Solve the maze using A* algorithm\n\n";

            cout << "  Exit / e / q\n";
            cout << "    -> Quit the program\n\n";

            cout << "========================================\n\n";
            continue;
        }

        if ((method == "Exit") || (method == "e") || (method == "q") )
            break;

        if ((method == "Resize") || (method == "re")) {
            cout << "\n======= CREATING NEW LABYRINTHE ========\n";
            cout << "Enter the labyrinth's new size :\n";
            cout << "Number of rows : ";
            cin >> size.i;
            if (size.i < 3) {
                cout << "Size must be greater than 2\n";
                cout << "========================================\n\n";
                continue;
            }
            
            cout << "Number of columns : ";
            cin >> size.j;
            if (size.j < 3) {
                cout << "Size must be greater than 2\n";
                cout << "========================================\n\n";
                continue;
            }

            lab.resize(size);
            lab.generateGrowingTree();
        }
        if ((method == "Regenerate") || (method == "r")) {
            cout << "\n======= REGENERATING LABYRINTHE ========\n";
            lab.clean();
            lab.generateGrowingTree();
        }
        if ((method == "Display") || (method == "d")) {
            cout << "\n========== CURRENT LABYRINTH ===========\n";
            lab.clearSolution();
        }

        bool solved = false;
        if ((method == "BFS") || (method == "bfs")) {
            cout << "\n========== SOLVING LABYRINTHE ==========\n";
            lab.clearSolution();
            solved = lab.solveBFS();
            cout << "Solving using BFS : " << (solved ? "OK" : "FAIL") << endl;
        }
        if ((method == "ASTAR") || (method == "A*") || (method == "a*")) {
            cout << "\n========== SOLVING LABYRINTHE ==========\n";
            lab.clearSolution();
            solved = lab.solveAStar();
            cout << "Solving using A* : " << (solved ? "OK" : "FAIL") << endl;
        }
        lab.print();
        cout << "========================================\n\n";
    }

    return 0;
}
