#include <memory>
#include <iostream>

template <typename T>
class List_ptr;

template <typename T>
class List_item;

template <typename T>
class List_iterator;

class List;


template <typename T>
class List_ptr {
 public:
    List_ptr() {}

    List_ptr(T* ptr_): ptr(ptr_) {}

    ~List_ptr() {}

    List_ptr& operator=(const List_ptr<T>& other) {
        ptr = other.ptr;
        return *this;
    }

    List_ptr& operator=(T* const other) {
        ptr = other;
        return *this;
    }

    void del() {
        delete ptr;
    }

    void reset(T* ptr_) {
        ptr = ptr_;
    }

    T* get() const {
        return ptr;
    }

    T* operator->() {
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

 private:
    T* ptr;
};

template <typename T>
class List_item {
 public:
    List_item() {}

    List_item(T x): value(x) {}

    List_item(List_item<T>&& other)
        : value(other.value)
        , next_item(other.next_item)
        , prev_item(other.prev_item) {}

    ~List_item() {}

 private:
    T value;
    List_ptr<List_item<T>> next_item;
    List_ptr<List_item<T>> prev_item;

    friend class List_iterator<T>;
    friend class List;
};




template <typename T>
class List_iterator {
    typedef List_item<T> item;

 public:
    List_iterator() {}

    List_iterator(item* ptr): current_item(ptr) {}

    List_iterator(const List_iterator& other)
        : current_item(other.current_item) {}

    ~List_iterator() {}

    List_iterator& operator++() {
        current_item = next().get();
        return *this;
    }

    List_iterator operator++(int) {
        auto save = *this;
        current_item = next().get();
        return save;
    }

    List_iterator& operator--() {
        current_item = prev().get();
        return *this;
    }

    List_iterator operator--(int) {
        auto save = *this;
        current_item = prev().get();
        return save;
    }

    T& operator*() {
        return current_item->value;
    }

    friend bool operator==(
        const List_iterator& it1,
        const List_iterator& it2
    ) {
        return it1.current_item == it2.current_item;
    }

    friend bool operator!=(
        const List_iterator& it1,
        const List_iterator& it2
    ) {
        return it1.current_item != it2.current_item;
    }

    inline List_ptr<List_item<T>>& next() {
        return current_item->next_item;
    }

    inline List_ptr<List_item<T>>& prev() {
        return current_item->prev_item;
    }

 private:
    item* current_item;

    void release() {
        delete current_item;
    }

    friend class List;
};




class List {
    typedef List_item<int> item;
    typedef List_iterator<int> iter;

 public:
    List()
            : head(new item)
            , tail(new item)
            , sz(0) {
        head.next() = tail.current_item;
        tail.prev() = head.current_item;
    }

    List(const List& other) = delete;

    ~List() {
        for (auto now = head; now != tail;) {
            auto tmp = now++;
            delete tmp.current_item;
        }
        delete tail.current_item;
    }

    List& operator=(const List& other) = delete;

    size_t size() const {
        return sz;
    }

    iter begin() {
        iter begin = head;
        return ++begin;
    }

    iter end() {
        return tail;
    }

    void push_front(int v) {
        iter add(new item(item(v)));
        iter afterAdd = head;
        ++afterAdd;
        add.next() = head.next();
        add.prev() = afterAdd.prev();
        head.next() = add.current_item;
        afterAdd.prev() = add.current_item;
        ++sz;
    }

    void push_back(int v) {
        iter add(new item(item(v)));
        iter beforeAdd = tail;
        --beforeAdd;
        add.prev() = tail.prev();
        add.next() = beforeAdd.next();
        tail.prev() = add.current_item;
        beforeAdd.next() = add.current_item;
        ++sz;
    }

    void pop_front() {
        auto del = head;
        auto first = ++del;
        ++first;
        head.next() = del.next();
        first.prev() = del.prev();
        del.release();
        --sz;
    }

    void pop_back() {
        auto del = tail;
        auto last = --del;
        --last;
        tail.prev() = del.prev();
        last.next() = del.next();
        del.release();
        --sz;
    }

 private:
    iter head;
    iter tail;
    size_t sz;
};
/*
int main() {
    int const N = 100000;
    int x;
    for (size_t i = 0; i != 10; ++i) {
        List l;
        for (size_t i = 0; i != N; ++i)
            l.push_back(i);
        for (size_t i = 0; i != N; ++i)
            l.pop_back();
        std::cin >> x;
    }
    std::cin >> x;
}
*/
