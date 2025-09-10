#pragma once
#include <Novice.h>
#include "Map.h"

class Player {
public:

    // コピー代入演算子を明示的に定義  
    Player& operator=(const Player& other) {  
        if (this != &other) {  
            x_ = other.x_;  
            y_ = other.y_;  
            size_ = other.size_;  
            speed_ = other.speed_;  
            velocityY_ = other.velocityY_;  
            isJumping_ = other.isJumping_;  
            gravity_ = other.gravity_;  
            jumpStrength_ = other.jumpStrength_;  
            playerTexRight_ = other.playerTexRight_;  
            playerTexLeft_ = other.playerTexLeft_;  
            animationTimer_ = other.animationTimer_;  
            currentFrame_ = other.currentFrame_;  
            rightDirection_ = other.rightDirection_;  
            leftDirection_ = other.leftDirection_;  
        }  
        return *this;  
    }  

    
    Player(int startX, int startY, int size);

    // プレイヤーの更新処理（キー入力で移動）
    void Update(const char keys[256], const Map& map, int tileSize);

    // 描画
    void Draw() const;

    // 座標を取得する関数を追加
    int GetX() const { return x_; }
    int GetY() const { return y_; }

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

    /// <summary>
    /// アニメーション管理関数
    /// </summary>
    void Animation();
};