#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template <class T>
class UniquePtr {
private:
    T* ptr_;
    UniquePtr(const UniquePtr&) = delete;               
    UniquePtr& operator = (const UniquePtr&) = delete;  

public:
    UniquePtr(T* ptr = nullptr) noexcept
        : ptr_{ ptr } { }

    UniquePtr(UniquePtr&& rval) noexcept               // Move constructor
        : ptr_{ rval.ptr_ } {
        rval.ptr_ = nullptr;
    }

    UniquePtr& operator = (UniquePtr&& rhs) noexcept {   // Move assignment
        delete ptr_;
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
        return *this;
    }

    ~UniquePtr() noexcept {
        delete ptr_;
    }


    T* Release() {
        T* old_ptr = ptr_;
        ptr_ = nullptr;
        return old_ptr;
    }

    void Reset(T* ptr = nullptr) noexcept {
        delete ptr_;
        ptr_ = ptr;
    }

    void Swap(UniquePtr<T>& rhs) noexcept {
        ptr_.Swap(rhs.ptr_);
    }

    T* Get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return (ptr_ != nullptr);
    }

    T& operator * () const {
        return *ptr_;
    }

    T* operator -> () const noexcept {
        return ptr_;
    }
};

template <class T>
void Swap(UniquePtr<T>& lhs, UniquePtr<T>& rhs) {
    lhs.Swap(rhs);
}

//#define MAKE_UNIQUE_IMPLEMENTED

#endif // UNIQUE_PTR_H
