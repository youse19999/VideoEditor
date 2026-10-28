#pragma once
#include <vector>
#include <memory>
#include "component.h"

class GameObject {
private:
    std::vector<std::unique_ptr<Component>> m_components;

public:
    GameObject() = default;
    virtual ~GameObject() = default;

    // コンポーネントを追加するテンプレートメソッド
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = newComponent.get();
        m_components.push_back(std::move(newComponent));
        return ptr;
    }

    // 全コンポーネントの更新
    virtual void Update(float deltaTime) {
        for (auto& component : m_components) {
            component->Update(deltaTime);
        }
    }

    // 全コンポーネントの描画
    virtual void Render() {
        for (auto& component : m_components) {
            component->Render();
        }
    }
};