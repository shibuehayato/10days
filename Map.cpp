#include "Map.h"
#include <cstring>
#include <cstdlib>

Map::Map(int width, int height) : width_(width), height_(height) {
    // 2次元配列の動的確保
    data_ = new int* [height_];
    for (int y = 0; y < height_; ++y) {
        data_[y] = new int[width_];
        for (int x = 0; x < width_; ++x) {
            // 外枠は1、それ以外は0
            if (y == 0 || y == height_ - 1 || x == 0 || x == width_ - 1) {
                data_[y][x] = 1;
            }
            else {
                data_[y][x] = 0;
            }
        }
    }
}

Map::~Map() {
    for (int y = 0; y < height_; ++y) {
        delete[] data_[y];
    }
    delete[] data_;
}

void Map::Draw(int originX, int originY, int tileSize) {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (data_[y][x] == 1) {
                // 明示的に float にキャストして警告回避
                int drawX = originX + x * tileSize;
                int drawY = originY + y * tileSize;
                int tSize = tileSize;

                Novice::DrawBox(drawX, drawY, tSize, tSize, 0.0f, 0xFFFFFFFF, kFillModeSolid);
            }
        }
    }
}

void Map::Clear() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (y == 0 || y == height_ - 1 || x == 0 || x == width_ - 1) {
                data_[y][x] = 1; // 外枠
            }
            else {
                data_[y][x] = 0; // 中は空白
            }
        }
    }
}

void Map::SetTile(int x, int y, int value)
{
    data_[y][x] = value;
}

bool Map::IsWall(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return true; // 範囲外は壁扱い
    return data_[y][x] == 1;
}