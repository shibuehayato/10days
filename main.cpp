#include <Novice.h>
#include "Map.h"
#include "Player.h"

const char kWindowTitle[] = "10days";

enum Scene {
    TITLE,
    GAME,
    GAMECLEAR
};

// --- シーン管理変数 ---
Scene currentScene = TITLE;
// ゴールのタイル座標
int goalX = 30;
int goalY = 9;

// プロトタイプ宣言（InitializeGame をファイル下部に置くなら必要）
void InitializeGame(Player& player, Map& map, Map& map2,
    int windowWidth, int windowHeight, int tileSize,
    int& weightLeft, int& weightRight, Map*& currentMap);

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

    Map map(tilesX, tilesY);
    Map map2(tilesX, tilesY); // 夢のマップ

    // プレイヤー初期位置をマップ中央に設定
    int playerSize = tileSize; // タイルと同じサイズ
    Player player(windowWidth / 2, windowHeight / 2, playerSize);

    map.SetTile(29, 10, 1);  
    map.SetTile(30, 10, 1);
    map.SetTile(30, 9, 1);   // ゴール
    map.SetTile(22, 10, 1);  // シーソー左
    map.SetTile(28, 14, 1);  // シーソー右
    map.SetTile(26, 13, 1);

    // 夢のマップ
    map2.SetTile(29, 10, 1);  
    map2.SetTile(30, 10, 1); 
    map2.SetTile(30, 9, 1);   // ゴール
    map2.SetTile(22, 10, 1); // シーソー左
    map2.SetTile(28, 14, 1); // シーソー右
    map2.SetTile(26, 13, 1);
    map2.SetTile(13, 14, 1);

    // 現在のマップを指すポインタ
    Map* currentMap = &map;

    // --- ブロックの「重さ」 ---
    int weightLeft = 0;   // (22,10) のブロックの重さ（増加可能）
    int weightRight = 4;  // (28,14) のブロックの重さ（固定）

    // 操作説明の画像
    int explanation;
    explanation = Novice::LoadTexture("./NoviceResources/explanation.png");

    // マップとプレイヤーの初期化
    InitializeGame(player, map, map2, windowWidth, windowHeight, tileSize,
        weightLeft, weightRight, currentMap);

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

        switch (currentScene)
        {
        case TITLE:
            // スペースキーでゲームへ
            if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
                currentScene = GAME;
            }
            break;
        case GAME:

            player.Update(keys, *currentMap, tileSize);

            // マウスクリックでブロックを置く
            //int mouseX, mouseY;
            //Novice::GetMousePosition(&mouseX, &mouseY);
            //
            //if (Novice::IsTriggerMouse(RI_MOUSE_LEFT_BUTTON_DOWN)) {
            //    int tileX = mouseX / tileSize;
            //    int tileY = mouseY / tileSize;
            //    map.SetTile(tileX, tileY, 1); // ブロックを置く
            //    
            //}

            // --- ゴール判定 ---
            {
                int playerTileX = player.GetX() / tileSize;
                int playerTileY = player.GetY() / tileSize;
                if (playerTileX == goalX && playerTileY == goalY) {
                    currentScene = GAMECLEAR;
                }
            }

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
                }
                else {
                    currentMap = &map;

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
            break;
        case GAMECLEAR:
            // スペースキーでタイトルに戻る
            if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
                InitializeGame(player, map, map2, windowWidth, windowHeight, tileSize,
                    weightLeft, weightRight, currentMap);
                currentScene = TITLE;
            }
            break;
        }

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        switch (currentScene)
        {
        case TITLE:
            Novice::ScreenPrintf(540, 360, "Press SPACE to Start");
            break;
        case GAME:
            // 左上から描画（右下も画面端ぴったり）
            currentMap->Draw(0, 0, tileSize);

            // 操作説明画像
            Novice::DrawSprite(0, 0, explanation, 1, 1, 0.0f, 0xffffffff);

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
            break;
        case GAMECLEAR:
            Novice::ScreenPrintf(640, 330, "clear");
            // スペースキーでタイトルに戻る
            if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
                currentScene = TITLE;
            }
            break;
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

void InitializeGame(Player& player, Map& map, Map& map2,
    int windowWidth, int windowHeight, int tileSize,
    int& weightLeft, int& weightRight, Map*& currentMap)
{
    int playerSize = tileSize;
    player = Player(windowWidth / 2, windowHeight / 2, playerSize);

    map.Clear();
    map.SetTile(29, 10, 1);
    map.SetTile(30, 10, 1);
    map.SetTile(30, 9, 1);   // ゴール
    map.SetTile(22, 10, 1);  // シーソー左
    map.SetTile(28, 14, 1);  // シーソー右
    map.SetTile(26, 13, 1);

    map2.Clear();
    map2.SetTile(29, 10, 1);
    map2.SetTile(30, 10, 1);
    map2.SetTile(30, 9, 1);   // ゴール
    map2.SetTile(22, 10, 1);
    map2.SetTile(28, 14, 1);
    map2.SetTile(26, 13, 1);
    map2.SetTile(13, 14, 1);

    weightLeft = 0;
    weightRight = 4;

    currentMap = &map;
}
