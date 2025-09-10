#pragma once
#include <Novice.h>

class Map {
public:
    Map(int width, int height); // タイル数を指定して生成
    ~Map();
    void Draw(int originX, int originY, int tileSize); // 描画

    void SetTile(int x, int y, int value);

    // 壁判定　
    bool IsWall(int x, int y) const;

    // 外枠＋中は空白でリセット
    void Clear();

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

private:
    int width_;
    int height_;
    int** data_; // マップデータ
};
