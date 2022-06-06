#include "VectorStack.h"
#include <stdexcept>

void VectorStack::push(const ValueType &value) {
    _vectorStack.pushBack(value);
}

const ValueType &VectorStack::top() const {
    checkSize();
    return _vectorStack[size() - 1];
}

bool VectorStack::isEmpty() const {
    return _vectorStack.size() == 0;
}

void VectorStack::pop() {
    checkSize();
    _vectorStack.popBack();
}

size_t VectorStack::size() const {
    return _vectorStack.size();
}

void VectorStack::checkSize() const {
    if (size() != 0) {
        return;
    }
    throw std::out_of_range("Empty");
}