#include <iostream>
#include <utility>
#include <algorithm>

template <typename T>
class VectorPtr {
 public:
    VectorPtr<T>()
        : cp(0)
        , sz(0)
        , ptr(nullptr)
    {}

    VectorPtr<T>(size_t _cp, size_t _sz = 0)
        : cp(_cp)
        , sz(_sz)
        , ptr(static_cast<T*>(operator new(cp * sizeof(T)))) {}

    VectorPtr<T>(const VectorPtr<T>&) = delete;

    VectorPtr<T>(VectorPtr<T>&& other)
        : cp(0)
        , sz(0)
        , ptr(nullptr) {
        swap(other);
    }

    VectorPtr<T>& operator=(const VectorPtr<T>& other) = delete;

    VectorPtr<T>& operator=(VectorPtr<T>&& other) {
        swap(other);
    }

    ~VectorPtr<T>() {
        if (ptr) {
            for (size_t i = 0; i != sz; ++i) {
                ptr[i].~T();
            }
            operator delete(ptr);
        }
    }

    T& operator[](size_t i) {
        return ptr[i];
    }

    const T& operator[](size_t i) const {
        return ptr[i];
    }

    void swap(VectorPtr<T>& other) {
        std::swap(cp, other.cp);
        std::swap(sz, other.sz);
        std::swap(ptr, other.ptr);
    }

    void swap(VectorPtr<T>&& other) {
        std::swap(cp, other.cp);
        std::swap(sz, other.sz);
        std::swap(ptr, other.ptr);
    }

    size_t& size() {
        return sz;
    }

    size_t& capacity() {
        return cp;
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cp;
    }

    T* get() const {
        return ptr;
    }

 private:
    size_t cp;
    size_t sz;
    T* ptr;
};


template <typename T>
class Vector {
    typedef VectorPtr<T> pointer;

 public:
    Vector<T>() {}

    Vector<T>(size_t _cp)
        : data(_cp) {}

    Vector<T>(const Vector<T>& other)
        : data(other.size()) {
        try {
            for (size_t i = 0; i != other.size(); ++i) {
                new (get() + i) T(other[i]);
                ++data.size();
            }
        } catch(...) {
            throw;
        }
    }

    ~Vector<T>() {}

    Vector<T>& operator=(const Vector<T>& other) {
        pointer tmp;
        try {
            tmp.swap(pointer(other.size()));
            for (size_t i = 0; i != other.size(); ++i) {
                new (tmp.get() + i) T(other[i]);
                ++tmp.size();
            }
        } catch(...) {
            throw;
        }
        data.swap(tmp);
        return *this;
    }


    Vector<T>& operator=(Vector<T>&& other) {
        swap(other);
        return *this;
    }

    size_t size() const {
        return data.size();
    }

    size_t capacity() const {
        return data.capacity();
    }

    void push_back(const T& x) {
        if (capacity() == 0) {
            try {
                reserve(1);
            } catch(...) {
                throw;
            }
        } else if (capacity() == size()) {
            try {
                reserve(2 * capacity());
            } catch(...) {
                throw;
            }
        }
        try {
            new (end()) T(x);
        } catch(...) {
            throw;
        }
        ++data.size();
    }

    void push_back(T&& x) {
        if (capacity() == 0) {
            try {
                reserve(1);
            } catch(...) {
                throw;
            }
        } else if (capacity() == size()) {
            try {
                reserve(2 * capacity());
            } catch(...) {
                throw;
            }
        }
        try {
            new (end()) T(std::move(x));
        } catch(...) {
            throw;
        }
        ++data.size();
    }

    void pop_back() {
        resize(size() - 1);
    }

    T& operator[](size_t i) {
        return data[i];
    }

    const T& operator[](size_t i) const {
        return data[i];
    }

    void reserve(size_t _cp) {
        if (_cp <= capacity()) {
            return;
        }
        pointer tmp;
        try {
            tmp.swap(pointer(_cp));
            for (size_t i = 0; i != std::min(size(), _cp); ++i) {
                new (tmp.get() + i) T(data[i]);
                ++tmp.size();
            }
        } catch(...) {
            throw;
        }
        data.swap(tmp);
    }

    void resize(size_t _sz) {
        if (_sz == size()) {
            return;
        }
        if (_sz < size()) {
            for (size_t i = _sz; i != size(); ++i) {
                data[i].~T();
            }
            data.size() = _sz;
            return;
        }
        if (_sz <= capacity()) {
            try {
                for (size_t i = size(); i != _sz; ++i) {
                    new (get() + i) T();
                    ++data.size();
                }
            } catch(...) {
                throw;
            }
            return;
        }
        pointer tmp;
        try {
            tmp.swap(pointer(_sz));
            for (size_t i = 0; i != size(); ++i) {
                new (tmp.get() + i) T(data[i]);
                ++tmp.size();
            }
            for (size_t i = size(); i != _sz; ++i) {
                new (tmp.get() + i) T();
                ++tmp.size();
            }
        } catch(...) {
            throw;
        }
        data.swap(tmp);
    }

    void clear() {
        resize(0);
    }

    void swap(Vector<T>& other) {
        data.swap(other.data);
    }

    T* begin() const {
        return data.get();
    }

    T* end() const {
        return begin() + size();
    }

 private:
    pointer data;

    T* get() const {
        return data.get();
    }
};

