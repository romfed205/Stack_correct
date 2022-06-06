#pragma once

#include "StackImplementation.h"
#include <list>

using ValueType = double;

class ListStack : public IStackImplementation {
public:
    ListStack() = default;

    void push(const ValueType& value) override;
    void pop() override;

    [[nodiscard]] const ValueType& top() const override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] size_t size() const override;

private:
    std::list<ValueType> _listStack;

    void checkSize() const;
};