#pragma once

#include "matrix.hpp"
#include "quadtree.h"

struct Pixel {
    unsigned char r, g, b;

    bool operator==(const struct Pixel& other) const;
    bool operator!=(const struct Pixel& other) const;

    friend std::ostream& operator<<(std::ostream& os, const struct Pixel& p) {
        return os << "Pixel = (" << p.r << ", " << p.g << ", "<< p.b << ")";
    }
};

class Image : public Matrix<struct Pixel> {
private:
    bool isUniform(const struct Position& up_left_corner, const struct Position& size) const;
    bool isUniform(int i, int j, int size_i, int size_j) const;

    // Reccursive function to encode the image into a QuadTree
    QuadTree<struct Pixel>* encode(const struct Position& up_left_corner, const struct Position& size) const;
    QuadTree<struct Pixel>* encode(int i, int j, int size_i, int size_j) const;

    // Reccursive function to decode an image from a QuadTree
    void decode(const QuadTree<struct Pixel>* qt, const struct Position& up_left_corner, const struct Position& size);
    void decode(const QuadTree<struct Pixel>* qt, int i, int j, int size_i, int size_j);

public:
    // Load an image from a given filename (must be a .png file)
    Image(const string& filename);
    Image(const string& filename, int& width, int& height);

    // Encode the image into a QuadTree (dynamic allocation -> must be deleted)
    // Compression
    QuadTree<struct Pixel>* encode() const;

    // Decode an image from a QuadTree
    // Decompression
    Image(const QuadTree<Pixel>* image_tree, int width = 1024, int height = 1024);
    
    // Save the image as a .png file
    void save(const string& filename) const;
};