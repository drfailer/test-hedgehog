#ifndef GRID_HPP
#define GRID_HPP

#include <cmath>
#include <cstddef>
#include <ostream>

#define CRESET "\033[0m"
#define BLK "\033[0;40m"
#define WHT "\033[0;47m"

template <typename Type>
class Grid {
  public:
    Grid(size_t width, size_t height, size_t blockSize, Type *grid):
        width_(width), height_(height),
        blockSize_(blockSize),
        nbRowBlocks_(std::ceil(height / blockSize + (height % blockSize == 0 ? 0 : 1))),
        nbColBlocks_(std::ceil(width / blockSize + (width % blockSize == 0 ? 0 : 1))),
        grid_(grid)
    { }

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t nbRowBlocks() const { return nbRowBlocks_; }
    size_t nbColBlocks() const { return nbColBlocks_; }
    size_t blockSize() const { return blockSize_; }
    Type  *grid() const { return grid_; }

    friend std::ostream& operator<<(std::ostream& os, const Grid& grid) {
        for (size_t i = 0; i < grid.height_; ++i) {
            for (size_t j = 0; j < grid.width_; ++j) {
                os << (grid.grid_[i * grid.width_ + j] ? BLK : WHT) << "  " CRESET;
            }
            os << std::endl;
        }
        return os;
    }

  private:
    size_t width_ = 0;
    size_t height_ = 0;
    size_t blockSize_ = 0;
    size_t nbRowBlocks_ = 0;
    size_t nbColBlocks_ = 0;
    Type *grid_ = nullptr;
};

#endif
