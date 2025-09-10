#pragma once
#include <Novice.h>
#include "Map.h"

class Player {
public:
    Player(int startX, int startY, int size);

    // プレイヤーの更新処理（キー入力で移動）
    void Update(const char keys[256], const Map& map, int tileSize);

    // 描画
    void Draw() const;

private:
    int x_;      // X座標（左上）
    int y_;      // Y座標（左上）
    int size_;   // プレイヤーのサイズ
    int speed_;  // 移動速度（ピクセル/frame）

    int velocityY_;   // 垂直速度
    bool isJumping_;  // 空中か
    int gravity_;     // 重力加速度
    int jumpStrength_;// ジャンプ初速
};

