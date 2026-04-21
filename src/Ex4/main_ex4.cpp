#include <iostream>
#include <string>
#include <filesystem>

#include "image.hpp"

using namespace std;
namespace fs = filesystem;

int main() {
    cout << "EXERCICE 4 : COMPRESSION D'IMAGES" << endl;
    int width, height;

    const string images_path = "data/ex4/";
    const string load_path = images_path + "Images/";
    const string save_path = images_path + "Saves/";

    if (fs::exists(save_path)) {
        fs::remove_all(save_path);
    }
    fs::create_directories(save_path);

    string image_name;

    while (true) {
        cout << "Select an image to load (enter exit to quit) : ";
        cin >> image_name;

        if (image_name == "exit")
            break;

        try {
            Image image = Image(load_path + image_name, width, height);
            cout << "Image loaded -> size = " << width << "x" << height 
                << " / nb pixels = " << width * height << endl;

            QuadTree<struct Pixel>* image_tree = image.encode(); // Encode
            cout << "Image compressed -> nb nodes = " << image_tree->nTrees() 
                << " (nb internal nodes = " << image_tree->nNodes() 
                << " + nb leaves = " << image_tree->nLeaves() << ")" << endl;

            Image image_to_save = Image(image_tree, width, height); // Decode
            cout << "Image decompressed" << endl;
            delete image_tree;

            image_to_save.save(save_path + image_name + "_saved");
            cout << "Image compressed saved" << endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error : " << e.what() << endl;
        }

        cout << endl;
    }
    cout << "Good Bye" << endl;
    return 0;
}
