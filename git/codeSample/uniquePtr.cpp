#include <cstddef>
#include <memory>
#include <tuple>
#include <iostream>

template <
    class T,
    class Deleter = std::default_delete<T>
>
class UniquePtr {
    typedef std::tuple<T*, Deleter> tuple_t;

 public:
    UniquePtr(): data(nullptr, Deleter()) {}

    explicit UniquePtr(T* ptr): data(ptr, Deleter()) {}

    UniquePtr(T* ptr, Deleter d): data(ptr, d) {}

    UniquePtr(const UniquePtr<T, Deleter>& other) = delete;

    UniquePtr(UniquePtr<T, Deleter>& other) = delete;

    UniquePtr(UniquePtr<T, Deleter>&& other)
            : data(other.release(), other.get_deleter()) {}

    UniquePtr<T, Deleter>& operator=(
        const UniquePtr<T, Deleter>& other) = delete;

    UniquePtr<T, Deleter>& operator=(std::nullptr_t) noexcept {
        std::get<0>(data) = nullptr;
        return *this;
    }

    UniquePtr<T, Deleter>& operator=(UniquePtr<T, Deleter>&& other) noexcept {
        reset(other.release());
        get_deleter() = std::move(other.get_deleter());
        return *this;
    }

    ~UniquePtr() {
        reset();
    }

    T& operator*() noexcept {
        return *std::get<0>(data);
    }

    const T& operator*() const noexcept {
        return *std::get<0>(data);
    }

    T* operator->() const noexcept {
        return std::get<0>(data);
    }

    T* release() noexcept {
        T* tmp = get();
        std::get<0>(data) = nullptr;
        return tmp;
    }

    void reset(T* ptr = nullptr) noexcept {
        if (get() != ptr) {
            get_deleter()(get());
            std::get<0>(data) = ptr;
        }
    }

    void swap(UniquePtr<T, Deleter>& other) noexcept {
        std::swap(data, other.data);
    }

    T* get() const noexcept {
        return std::get<0>(data);
    }

    explicit operator bool() const noexcept {
        return std::get<0>(data) != nullptr;
    }

    Deleter& get_deleter() noexcept {
        return std::get<1>(data);
    }

    const Deleter& get_deleter() const noexcept {
        return std::get<1>(data);
    }

 private:
    tuple_t data;
};

