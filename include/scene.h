#pragma once
#include <vector>
#include <memory>
#include "gameobject.h"

class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    // シングルトンのためコンストラクタを隠蔽
    Scene() = default;
    ~Scene() = default;

    // コピー・ムーブを禁止
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

public:
    // 唯一のインスタンスを取得するアクセサ
    static Scene& GetInstance() {
        static Scene instance;
        return instance;
    }

    // ゲームオブジェクトの生成と登録
    GameObject* CreateGameObject() {
        auto obj = std::make_unique<GameObject>();
        GameObject* ptr = obj.get();
        m_gameObjects.push_back(std::move(obj));
        return ptr;
    }

    // 全ゲームオブジェクトの更新
    void Update(float deltaTime) {
        for (auto& obj : m_gameObjects) {
            obj->Update(deltaTime);
        }
    }

    // 全ゲームオブジェクトの描画
    void Render() {
        for (auto& obj : m_gameObjects) {
            obj->Render();
        }
    }
};