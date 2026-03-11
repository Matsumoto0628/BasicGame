# 殺虫侍

DirectXによる3Dモデルの描画を用いたゲーム

- **開発期間** : 1ヶ月
- **制作人数** : 1人
- **リリース先** : GitHub Releases

### 動作環境

- **OS** : Windows 11
- **GPU** : DirectX 11 対応

### 参考書籍

- 『ゲームプログラマになる前に覚えておきたい技術』 平山 尚
- 『DirectX10/11プログラミング』 鎌田 茂雄

<br>
<br>

## 背景

3Dレンダリングの仕組みをより深く理解したいと考え、本プロジェクトに取り組んだ。

DirectXを直接扱うことで、頂点バッファ・シェーダー・座標変換といったグラフィックスパイプラインの基礎を自分の手で実装し、理論と実装を結びつけることを目的とした。

また、シーン管理やオブジェクト設計などゲームアーキテクチャの設計も合わせて学ぶため、単なるサンプルに留まらず、完結したゲームとして仕上げることを目標とした。

<br>
<br>

## 使用技術

- **言語** : C++
- **IDE** : Visual Studio
- **グラフィックスAPI** : DirectX
- **ライブラリ** : Assimp

<br>

### 選定理由

#### Assimp

| ライブラリ | 特徴 |
|------------|------|
| **Assimp** | 多数のフォーマットに対応し、DirectXとの連携が容易 |
| TinyObjLoader | OBJ形式専用で汎用性が低い |
| FBX SDK | Autodesk製で高機能だが、ライセンス制約が厳しく導入が複雑 |

3Dモデルのフォーマットは多岐にわたるため、FBX・OBJ・GLTFなど幅広い形式に対応したAssimpを選定した。
TinyObjLoaderはOBJ限定で汎用性に欠け、FBX SDKはライセンス制約や導入コストが高い。
AssimpはオープンソースかつAPIがシンプルで、DirectXへのデータ受け渡しがしやすいため採用した。

<br>
<br>

## 技術的な工夫

### フォン反射モデルのシェーダー実装

ピクセルシェーダーで拡散反射（Lambert）と鏡面反射（Phong）を計算し、光源・視線方向をConstant Buffer経由でGPUに渡している。シェーダーはTextureSpecular / Texture / Simple / Specularの4種類を用途別に使い分ける設計とした。

https://github.com/Matsumoto0628/BasicGame/blob/0af0fff9a7d150f7238ef265998790d5b74fccec/basic_game/scripts/shader/texture_specular_ps.hlsl#L38-L43

---

### オブジェクト指向設計

SceneとActorをそれぞれ純粋仮想関数を持つ基底クラスとして定義し、各シーン・オブジェクトに`Setup / Update / Draw / Terminate`の統一インターフェースを持たせた。SceneManagerは`Scene*`のポインタで現在シーンを保持するため、シーンの切り替えはポインタの差し替えだけで完結し、呼び出し側のコードを変更せずに新しいシーンを追加できる。

https://github.com/Matsumoto0628/BasicGame/blob/1034142f0c76c9580ca5beb180c26efccd52350d/basic_game/scripts/scene/scene.h#L6-L15

https://github.com/Matsumoto0628/BasicGame/blob/b8dd20d6de180a8a27b2ad0d5ea35cf02a04df40/basic_game/scripts/actor/actor.h#L3-L11

<br>
<br>

## インストール方法
1. GitHubのReleasesページから `Game.zip` をダウンロード
2. ZIPを展開する
3. `BasicGame.exe` を起動する
> **Note:** 起動時にWindowsのセキュリティ警告が表示される場合があります。「詳細情報」>「実行」をクリックして続行してください。
>

<br>
<br>

## 操作方法

- **カーソル表示** Escキー
- **攻撃** 左クリック
- **移動** WASDキー
