#include <Novice.h>
#include "Map.h"
#include "Player.h"

const char kWindowTitle[] = "10days";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const int windowWidth = 1280;
    const int windowHeight = 720;
    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, windowWidth, windowHeight);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    int targetTilesX = 23; // 横
    int targetTilesY = 18; // 縦5

    // タイルサイズを計算
    int tileSizeX = windowWidth / targetTilesX;
    int tileSizeY = windowHeight / targetTilesY;
    int tileSize = (tileSizeX < tileSizeY) ? tileSizeX : tileSizeY;

    // 実際のタイル数を画面サイズに合わせて再計算
    int tilesX = windowWidth / tileSize;
    int tilesY = windowHeight / tileSize;

    //マップ用テクスチャ読み込み
    int realMapTex = Novice::LoadTexture("stone_real.png");
    int dreamMapTex = Novice::LoadTexture("stone_dream.png");

    Map map(tilesX, tilesY, realMapTex);
    Map map2(tilesX, tilesY, dreamMapTex); // 夢のマップ

    //背景用テクスチャ読み込み
    int realBackTex = Novice::LoadTexture("real_backGround.png");
    int dreamBackTex = Novice::LoadTexture("dream_backGround.png");
    int currentBackgroundTex = realBackTex;   //現在の描画している背景

    //int titleSceneTex = Novice::LoadTexture("Title.png");
    //int clearSceneTex = Novice::LoadTexture("Clear.png");

    //操作説明用テクスチャ読み込み
    int pressSpaceTex = Novice::LoadTexture("pushSpase.png");
    float blinkTimer_ = 0.0f;
    bool isDrawing_ = true;
    const float blinkDuration_ = 0.25f; //点滅周期


    // プレイヤー初期位置をマップ中央に設定
    int playerSize = tileSize; // タイルと同じサイズ
    Player player(windowWidth / 2, windowHeight / 2, playerSize);

    map.SetTile(29, 10, 1);  // (5,5) にブロック
    map.SetTile(30, 10, 1);  // (6,5) にブロック
    map.SetTile(22, 10, 1);  // シーソー左
    map.SetTile(28, 14, 1);  // シーソー右
    map.SetTile(26, 13, 1);

    // 夢のマップ
    map2.SetTile(29, 10, 1);  
    map2.SetTile(30, 10, 1);  
    map2.SetTile(22, 10, 1); // シーソー左
    map2.SetTile(28, 14, 1); // シーソー右
    map2.SetTile(26, 13, 1);
    map2.SetTile(13, 14, 1);

    // 現在のマップを指すポインタ
    Map* currentMap = &map;

    // --- ブロックの「重さ」 ---
    int weightLeft = 0;   // (22,10) のブロックの重さ（増加可能）
    int weightRight = 4;  // (28,14) のブロックの重さ（固定）

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        player.Update(keys, *currentMap, tileSize);

        // マウスクリックでブロックを置く
        //int mouseX, mouseY;
        //Novice::GetMousePosition(&mouseX, &mouseY);

        //if (Novice::IsTriggerMouse(RI_MOUSE_LEFT_BUTTON_DOWN)) {
        //    int tileX = mouseX / tileSize;
        //    int tileY = mouseY / tileSize;
        //    map.SetTile(tileX, tileY, 1); // ブロックを置く
        //    
        //}

        // --- map2 にいるときの操作 ---
        if (currentMap == &map2) {
            // 上キーで左ブロックの重さを増やす
            if (keys[DIK_UP] && !preKeys[DIK_UP]) {
                if (weightLeft < 5) { // 最大5まで
                    weightLeft++;
                }
            }
        }

         // --- マップ切り替え（Rキーで切り替え） ---
        if (keys[DIK_R] && !preKeys[DIK_R]) {
            if (currentMap == &map) {
                currentMap = &map2;
                currentBackgroundTex = dreamBackTex;
            }
            else {
                currentMap = &map;
                currentBackgroundTex = realBackTex;

                // map を作り直して再配置
                //map = Map(tilesX, tilesY);
                map.Clear();
                map.SetTile(29, 10, 1);
                map.SetTile(30, 10, 1);
                map.SetTile(26, 13, 1);

                if (weightLeft > weightRight) {
                    // 左の方が重い → Y座標を入れ替える
                    map.SetTile(22, 14, 1); // 左を下に
                    map.SetTile(28, 10, 1); // 右を上に
                }
                else {
                    // そのまま
                    map.SetTile(22, 10, 1);
                    map.SetTile(28, 14, 1);
                }

            }
        }

        blinkTimer_ += 0.01f;
        if (blinkTimer_ >= blinkDuration_)
        {
            isDrawing_ = !isDrawing_;
            blinkTimer_ = 0.0f;
        }

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // 左上から描画（右下も画面端ぴったり）

        Novice::DrawSprite(0, 0, currentBackgroundTex, windowWidth, windowHeight , 0.0f, WHITE);
        
        //点滅描画
        if (isDrawing_)
        {
            Novice::DrawSprite(0, 0, pressSpaceTex, 1, 1, 0.0f, WHITE);
        }

        currentMap->Draw(0, 0, tileSize);

        player.Draw();
        Novice::ScreenPrintf(640, 360, "Left Weight: %d", weightLeft);
        Novice::ScreenPrintf(640, 390, "Right Weight: %d", weightRight);

        // map と map2 で表示する文字を切り替え
        if (currentMap == &map) {
            Novice::ScreenPrintf(640, 330, "gennjitu");   // mapのとき
        }
        else {
            Novice::ScreenPrintf(640, 330, "yume");       // map2のとき
        }

        ///
        /// ↑描画処理ここまで
        ///

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}
