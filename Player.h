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

    //テクスチャ保存変数
    int playerTexRight_ = 0;
    int playerTexLeft_ = 0;

    // アニメーション管理変数
    float animationTimer_ = 0.0f;
    int currentFrame_ = 0;
    const int frameCount_ = 4;         // アニメーションの総フレーム数
    const float frameDuration_ = 0.1f; // 1フレームあたりの表示時間（秒）

    bool rightDirection_ = true;   //右を向いているか
    bool leftDirection_ = false;    //左を向いているか

private:
    void Animation();
};