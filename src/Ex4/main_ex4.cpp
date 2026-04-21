#include <iostream>

#include "image.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    int width, height;

    Image cat = Image("data/ex4/Images/chat", width, height);
    
    QuadTree<struct Pixel>* cat_tree = cat.encode();
    
    Image cat_to_save = Image(cat_tree, width, height);
    
    cat_to_save.save("data/ex4/Saves/chat_saved");
    
    delete cat_tree;
    return 0;
}
