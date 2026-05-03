/**
 * @file euler_converter.h
 * @brief オイラー角とクォータニオンの変換ユーティリティ関数の宣言
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once

/**
 * @brief オイラー角（XYZ ラジアン）をクォータニオンに変換する
 * @param eulerAngles XYZ 回転角（ラジアン）を格納した XMFLOAT3
 * @return 変換後のクォータニオン（XMFLOAT4）
 */
DirectX::XMFLOAT4 EulerToQuaternion(const DirectX::XMFLOAT3& eulerAngles);
