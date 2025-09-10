#include "Player.h"

Player::Player(int startX, int startY, int size)
    : x_(startX), y_(startY), size_(size), speed_(4),
    velocityY_(0), isJumping_(false), gravity_(1), jumpStrength_(16) {
}

void Player::Update(const char keys[256], const Map& map, int tileSize) {
    int nextX = x_;
    int nextY = y_;

    // 左右移動
    if (keys[DIK_A]) nextX -= speed_;
    if (keys[DIK_D]) nextX += speed_;

    // ジャンプ
    if (keys[DIK_SPACE] && !isJumping_) {
        velocityY_ = -jumpStrength_;
        isJumping_ = true;
    }

    // 重力
    velocityY_ += gravity_;
    nextY += velocityY_;

    // 水平方向衝突
    int leftTile = nextX / tileSize;
    int rightTile = (nextX + size_ - 1) / tileSize;
    int topTile = y_ / tileSize;
    int bottomTile = (y_ + size_ - 1) / tileSize;

    bool collisionX = false;
    for (int ty = topTile; ty <= bottomTile; ++ty) {
        for (int tx = leftTile; tx <= rightTile; ++tx) {
            if (map.IsWall(tx, ty)) collisionX = true;
        }
    }
    if (!collisionX) x_ = nextX;

    // 垂直方向衝突
    leftTile = x_ / tileSize;
    rightTile = (x_ + size_ - 1) / tileSize;
    topTile = nextY / tileSize;
    bottomTile = (nextY + size_ - 1) / tileSize;

    bool collisionY = false;
    for (int ty = topTile; ty <= bottomTile; ++ty) {
        for (int tx = leftTile; tx <= rightTile; ++tx) {
            if (map.IsWall(tx, ty)) collisionY = true;
        }
    }

    if (!collisionY) {
        y_ = nextY;
        isJumping_ = true;
    }
    else {
        if (velocityY_ > 0) {
            // 落下時の床衝突
            y_ = bottomTile * tileSize - size_;
            isJumping_ = false;
        }
        else if (velocityY_ < 0) {
            // 上方向の頭衝突
            y_ = (topTile + 1) * tileSize;
        }
        velocityY_ = 0;
    }
}

void Player::Draw() const {
    Novice::DrawBox(x_, y_, size_, size_, 0.0f, 0xFFFFFFFF, kFillModeSolid);
}
