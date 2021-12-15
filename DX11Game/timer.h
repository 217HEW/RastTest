// 画面遷移 [timer.h]
#pragma once
#include "main.h"

// 関数プロトタイプ
HRESULT InitTimer(); // 初期化
void UninitTimer(); // 終了処理
void UpdateTimer(); // 更新
void DrawTimer(); // 描画

void AddTimer(int nSec = 30);// 切替_使ってない
int GetTimer();