#include "ListStack.h"
#include <stdexcept>

void ListStack::pop() {
    checkSize();
    _listStack.pop_back();
}

const ValueType& ListStack::top() const {
    checkSize();
    return _listStack.back();
}

bool ListStack::isEmpty() const {
    return _listStack.empty();
}

size_t ListStack::size() const {
    return _listStack.size();
}

void ListStack::push(const ValueType& value) {
    _listStack.push_back(value);
}

void ListStack::checkSize() const {
    if (size() != 0) {
        return;
    }
    throw std::out_of_range("Empty");
}