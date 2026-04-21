#include "image.hpp"

#include <vector>

// Used to load/save images in the format .png
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// Pixel
bool Pixel::operator==(const struct Pixel& other) const {
    return (r == other.r) && (g == other.g) && (b == other.b);
}

bool Pixel::operator!=(const struct Pixel& other) const {
    return (r != other.r) || (g != other.g) || (b != other.b);
}

// Image

// Image Loader : file -> Image
int x, y = 0; // Stupid values to shut the compiler
Image::Image(const string& filename):Image(filename, x, y) {}

Image::Image(const string& filename, int& width, int& height) {
    int channels;
    unsigned char* data = stbi_load((filename + ".png").c_str(), &width, &height, &channels, 3); // 3 -> rgb

    if (!data) {
        throw std::runtime_error("Error while loading image : invalid image name ?");
    }

    resize({height, width}, {});
    for (int i = 0; i < height; ++ i) {
        for (int j = 0; j < width; ++ j) {
            int idx = (i * width + j) * 3;
            (*this)(i, j) = {data[idx], data[idx + 1], data[idx + 2]};
        }
    }

    stbi_image_free(data);
}

// Compression : Image -> QuadTree
QuadTree<struct Pixel>* Image::encode() const {
    return encode({0, 0}, {getRows(), getCols()});
}

// Decompression : QuadTree -> Image
Image::Image(const QuadTree<Pixel>* image_tree, int width, int height):Matrix<struct Pixel>(height, width, {}) {
    decode(image_tree, {0, 0}, {getRows(), getCols()});
}

// Save Image : Image -> file
void Image::save(const string& filename) const {
    int height = getRows();
    int width = getCols();

    vector<unsigned char> data(width * height * 3);

    for (int i = 0; i < height; ++ i) {
        for (int j = 0; j < width; ++ j) {
            int idx = (i * width + j) * 3;
            data[idx] = (*this)(i, j).r;
            data[idx + 1] = (*this)(i, j).g;
            data[idx + 2] = (*this)(i, j).b;
        }
    }

    stbi_write_png((filename + ".png").c_str(), width, height, 3, data.data(), width * 3);
}

// Uniform
bool Image::isUniform(const struct Position& up_left_corner, const struct Position& size) const {
    struct Pixel val = (*this)(up_left_corner);

    for (int i = up_left_corner.i; i < up_left_corner.i + size.i; ++ i) {
        for (int j = up_left_corner.j; j < up_left_corner.j + size.j; ++ j) {
            if ((*this)({i, j}) != val)
                return false;
        }
    }
    return true;
}

bool Image::isUniform(int i, int j, int size_i, int size_j) const {
    return isUniform({i, j}, {size_i, size_j});
}

// Reccursive functions
// Encode a loaded image into a quadtree
QuadTree<struct Pixel>* Image::encode(const struct Position& up_left_corner, const struct Position& size) const {
    // If is a simple image (Uniform) -> leaf
    if (isUniform(up_left_corner, size)) {
        return new QuadLeaf<struct Pixel>((*this)(up_left_corner));
    }

    // Else separation of the image in 4 quarters -> evaluate eauch quarter of the image
    struct Position half_size = {size.i / 2, size.j / 2};

    return new QuadNode<struct Pixel>(
        encode(up_left_corner, half_size), // NW
        encode({up_left_corner.i, up_left_corner.j + half_size.j}, half_size), // NE
        encode({up_left_corner.i + half_size.i, up_left_corner.j + half_size.j}, half_size), // SE
        encode({up_left_corner.i + half_size.i, up_left_corner.j}, half_size) // SW
    );
}

QuadTree<struct Pixel>* Image::encode(int i, int j, int size_i, int size_j) const {
    return encode({i, j}, {size_i, size_j});
}

// Decode an image from a quadtree
void Image::decode(const QuadTree<struct Pixel>* qt, const struct Position& up_left_corner, const struct Position& size) {
    // If is a leaf -> fill the image with leaf value
    if (qt->isLeaf()) {
        struct Pixel val = qt->value();

        for (int i = up_left_corner.i; i < up_left_corner.i + size.i; ++ i) {
            for (int j = up_left_corner.j; j < up_left_corner.j + size.j; ++ j) {
                (*this)({i, j}) = val;
            }
        }
        return;
    } 
    
    // Else iterate recursively on the 4 children of qt to construct each querter of the image
    struct Position half_size = {size.i / 2, size.j / 2};

    decode(qt->son(NW), up_left_corner, half_size);
    decode(qt->son(NE), {up_left_corner.i, up_left_corner.j + half_size.j}, half_size);
    decode(qt->son(SE), {up_left_corner.i + half_size.i, up_left_corner.j + half_size.j}, half_size);
    decode(qt->son(SW), {up_left_corner.i + half_size.i, up_left_corner.j}, half_size);
}

void Image::decode(const QuadTree<struct Pixel>* qt, int i, int j, int size_i, int size_j) {
    return decode(qt, {i, j}, {size_i, size_j});
}