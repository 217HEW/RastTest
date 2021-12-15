// 画面遷移 [life.h]
#pragma once
#include "main.h"

// 関数プロトタイプ
HRESULT InitLife(); // 初期化
void UninitLife(); // 終了処理
void UpdateLife(); // 更新
void DrawLife(); // 描画

void AddLife(int nLife = 1);
void DelLife(int nLife = 1);
int GetLife();