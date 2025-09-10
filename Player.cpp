#include "Player.h"

Player::Player(int startX, int startY, int size)
	: x_(startX), y_(startY), size_(size), speed_(4),
	velocityY_(0), isJumping_(false), gravity_(1), jumpStrength_(16) {

	playerTexRight_ = Novice::LoadTexture("player_walk.png");
	playerTexLeft_ = Novice::LoadTexture("player_walk_left.png");
}

void Player::Update(const char keys[256], const Map& map, int tileSize) {
	int nextX = x_;
	int nextY = y_;

	// 左右移動
	if (keys[DIK_A])
	{
		nextX -= speed_;
		leftDirection_ = true;
		rightDirection_ = false;
		Animation();
	}
	else if (keys[DIK_D]) 
	{
		nextX += speed_;
		leftDirection_ = false;
		rightDirection_ = true;
		Animation();
	}
	else
	{
		//押していないときは直立させる
		currentFrame_ = 0;
	}

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
	//Novice::DrawBox(x_, y_, size_, size_, 0.0f, 0xFFFFFFFF, kFillModeSolid);

	int srcX = currentFrame_ * 64;

	if (rightDirection_)
	{
		//右向きテクスチャ
		Novice::DrawSpriteRect(x_, y_, srcX, 0, 64, 64, playerTexRight_, size_ / 256.0f, size_ / 64.0f, 0.0f, WHITE);
	}
	if (leftDirection_)
	{
		//左向きテクスチャ
		Novice::DrawSpriteRect(x_, y_, srcX, 0, 64, 64, playerTexLeft_, size_ / 256.0f, size_ / 64.0f, 0.0f, WHITE);

	}
}

void Player::Animation()
{
	animationTimer_ += 0.01f;

	// 設定した時間（frame_duration）が経過したら、次のフレームへ
	if (animationTimer_ >= frameDuration_) {
		// 次のフレームにインデックスを移動
		currentFrame_++;

		// 最後のフレームに達したら最初に戻す（ループ）
		if (currentFrame_ >= frameCount_) {
			currentFrame_ = 0;
		}
		// タイマーをリセット
		animationTimer_ = 0.0f;
	}
}
