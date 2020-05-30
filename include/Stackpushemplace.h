// Copyright 2020 <telsamar>

#ifndef TEMPLATE1_Stackpushemplace_H
#define TEMPLATE1_STACKpushemplace_H

#include <iostream>
#include <exception>
#include "Node.h"

template<typename T>
class Stackpushemplace {
 private:
    Node<T> *node = nullptr;

public:

    Stackpushemplace() = default;

    Stackpushemplace(const Stackpushemplace &stack) = delete;
    Stackpushemplace(Stackpushemplace &&stack) = default;
    auto operator=(Stackpushemplace &&stack) noexcept -> Stackpushemplace & = default;

    template<typename ... Args>
    void push_emplace(Args &&... value) {
        auto item = node;
        node = new Node<T>{{std::forward<Args>(value)...}, item};
    }

    void push(T &&value) {
        auto item = node;
        node = new Node<T>{std::forward<T>(value), item};
    }

    const T &head() const {
        if (!node)
            throw std::logic_error("no stack");
        else
            return node->value;
    }

    T pop() {
        if (!node)
            throw std::logic_error("no stack");
        else {
            auto remote = node->value;
            node = node->prev;
            return remote;
        }
    }
};


#endif //TEMPLATE1_Stackpushemplace_H
