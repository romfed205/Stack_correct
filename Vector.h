#pragma once


#include <cstddef>
#include <utility>
#include <stdexcept>

template <typename Value>
class Vector
{
public:
    Vector() = default;

    Vector(const Value* rawArray, const size_t size, float coef = 2.0);

    Vector(const Vector& other)
            : Vector(other._data, other._size, other._multiplicativeCoef) {
    }

    Vector& operator=(const Vector& other);

    Vector(Vector&& other) noexcept;

    Vector& operator=(Vector&& other) noexcept;

    ~Vector();

    void pushBack(const Value& value);

    void pushFront(const Value& value);

    void insert(const Value& value, size_t pos);

    void insert(const Value* values, size_t size, size_t pos);

    void insert(const Vector& vector, size_t pos);

    void popBack();

    void popFront();


    void erase(size_t pos, size_t count);

    void eraseBetween(size_t beginPos, size_t endPos);

    Value& operator[](size_t idx);

    const Value& operator[](size_t idx) const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] double loadFactor() const;

    long long find(const Value& value) const;

    void reserve(size_t capacity);

    void shrinkToFit();

    class Iterator {
    public:
        explicit Iterator(Value* ptr) : _ptr(ptr) {}

        Value & operator*() {
            return *_ptr;
        }

        const Value& operator*() const {
            return *_ptr;
        }

        Value* operator->() {
            return _ptr;
        }

        const Value* operator->() const {
            return _ptr;
        }

        Iterator operator++() {
            ++_ptr;
            return *this;
        }

        Iterator operator++(int) {
            Value* bufPtr = _ptr;
            ++_ptr;
            return Iterator(bufPtr);
        }

        Iterator operator--() {
            --_ptr;
            return *this;
        }

        Iterator operator--(int) {
            Value* bufPtr = _ptr;
            --_ptr;
            return Iterator(bufPtr);
        }

        bool operator==(const Iterator& other) const {
            return _ptr == other._ptr;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        Value* _ptr;
    };

    Iterator begin() {
        return Iterator(&(_data[0]));
    }

    Iterator end() {
        return Iterator(&_data[_size]);
    }

private:
    Value* _data{nullptr};
    size_t _size{};
    size_t _capacity{};
    float _multiplicativeCoef{2.0f};

    void insertReAllocCopyToPos(int from, int pos, int shift);

    void isEmpty();
};

template <typename Value>
Vector<Value>::Vector(const Value* rawArray, const size_t size, float coef) : _size(size), _multiplicativeCoef(coef) {
    if (coef < 1) {
        _multiplicativeCoef = 2.0f;
    }
    _capacity = static_cast<int>(static_cast<float>(_size) * _multiplicativeCoef);
    _data = new Value[_capacity];
    for (size_t i = 0; i < _size; i++) {
        _data[i] = std::move(rawArray[i]);
    }
}

template <typename Value>
Vector<Value>::~Vector() {
    delete[] _data;
}

template <typename Value>
Vector<Value>& Vector<Value>::operator=(Vector<Value>&& other) noexcept
{
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    std::swap(_multiplicativeCoef, other._multiplicativeCoef);
    return *this;
}

template <typename Value>
Vector<Value>& Vector<Value>::operator=(const Vector<Value>& other) {
    if (this == &other) {
        return *this;
    }
    delete[] _data;
    _data = new Value[other._capacity];
    for(int i = 0; i < other._size; i++) {
        _data[i] = std::move(other._data[i]);
    }
    _size = other._size;
    _capacity = other._capacity;
    return *this;
}

template <typename Value>
void Vector<Value>::pushBack(const Value& value) {
    insert(value, _size);
}

template <typename Value>
void Vector<Value>::pushFront(const Value& value) {
    insert(value, 0);
}

template <typename Value>
void Vector<Value>::insert(const Value& value, size_t pos) {
    Value arr[]{value};
    insert(arr, 1, pos);
}

template <typename Value>
void Vector<Value>::insert(const Value* values, size_t size, size_t pos)
{
    if (pos > _size) {
        return;
    }
    _size += size;
    if (_capacity <= _size) {
        _capacity += size;
        _capacity *= _multiplicativeCoef;
        insertReAllocCopyToPos(_size - 2, pos, size);
    }
    else {
        for (int i = _size - 2; i >= pos && i != -1; i--) {
            _data[i + size] = std::move(_data[i]);
        }
    }
    for (int i = pos; i < pos + size; i++) {
        _data[i] = std::move(values[i - pos]);
    }
}

template <typename Value>
void Vector<Value>::insert(const Vector& vector, size_t pos) {
    insert(vector._data, vector._size, pos);
}

template <typename Value>
void Vector<Value>::popBack() {
    isEmpty();
    --_size;
}

template <typename Value>
void Vector<Value>::popFront() {
    isEmpty();
    --_size;
    Value* newData = new Value[_capacity];
    for (int i = 0; i < _size; i++) {
        newData[i] = _data[i + 1];
    }
    delete[] _data;
    _data = newData;
}

template <typename Value>
void Vector<Value>::erase(size_t pos, size_t count) {
    isEmpty();
    size_t end = pos + count;
    if (end > _size) {
        end = _size;
    }
    size_t k = 0;
    for (size_t i = end; i < _size; i++) {
        _data[pos + k] = _data[i];
        k++;
    }
    _size -= end - pos;
}

template <typename Value>
void Vector<Value>::eraseBetween(size_t beginPos, size_t endPos) {
    if (beginPos >= endPos || endPos > _size) {
        return;
    }
    erase(beginPos, endPos - beginPos);
}
template <typename Value>
Value& Vector<Value>::operator[](size_t idx) {
    return _data[idx];
}

template <typename Value>
const Value& Vector<Value>::operator[](size_t idx) const {
    return _data[idx];
}

template <typename Value>
size_t Vector<Value>::size() const {
    return _size;
}

template <typename Value>
size_t Vector<Value>::capacity() const {
    return _capacity;
}

template <typename Value>
double Vector<Value>::loadFactor() const {
    return _multiplicativeCoef;
}

template <typename Value>
long long Vector<Value>::find(const Value& value) const {
    for (int i = 0; i < _size; i++) {
        if (_data[i] == value) {
            return i;
        }
    }
    return -1;
}

template <typename Value>
void Vector<Value>::reserve(size_t capacity) {
    if (capacity <= _capacity) {
        return;
    }
    _capacity = capacity;
    Value* newData = new Value[_capacity];
    for (int i = 0; i < _size; i++) {
        newData[i] = std::move(_data[i]);
    }
    delete[] _data;
    _data = newData;
}

template <typename Value>
void Vector<Value>::shrinkToFit() {
    if(_size == _capacity) {
        return;
    }
    _capacity = _size;
    Value* newData = new Value[_capacity];
    for(int i = 0; i < _size; i++) {
        newData[i] = std::move(_data[i]);
    }
    delete[] _data;
    _data = newData;
}

template <typename Value>
void Vector<Value>::insertReAllocCopyToPos(int from, int pos, int shift)
{
    Value* newData = new Value[_capacity];
    for (int i = from; i >= pos && i != -1; i--) {
        newData[i + shift] = std::move(_data[i]);
    }
    for (int i = 0; i < pos; i++) {
        newData[i] = std::move(_data[i]);
    }
    delete[] _data;
    _data = newData;
}

template <typename Value>
void Vector<Value>::isEmpty() {
    if (_size == 0) {
        throw std::out_of_range("Empty vector popBack");
    }
}

template <typename Value>
Vector<Value>::Vector(Vector<Value>&& other) noexcept {
    *this = std::move(other);
}