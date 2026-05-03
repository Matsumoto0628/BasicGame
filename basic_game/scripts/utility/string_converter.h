/**
 * @file string_converter.h
 * @brief 文字列変換ユーティリティ関数の宣言
 * @author Matsumoto0628
 * @date 2026-05-04
 */
#pragma once
#include <string>

/**
 * @brief マルチバイト文字列をワイド文字列に変換する
 * @param str 変換元の std::string
 * @return 変換後の std::wstring
 */
std::wstring StringToWString(const std::string& str);
