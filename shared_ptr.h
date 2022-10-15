#include <cstdint>

struct RefCount
{
    int32_t sharedCount = 0;
    int32_t weakCount = 0;
};

class BadWeakPtr {};

template<class T>
class WeakPtr;

template<class T>
class SharedPtr
{
    template <class U> friend class WeakPtr;
    template <class U> friend class SharedPtr;
public:
    typedef T ElementType;

    SharedPtr(const SharedPtr& other) :
        ptr(other.ptr), refCountPtr(other.refCountPtr)
    {
        addRef();
    }

    SharedPtr(SharedPtr<ElementType>&& other) :
        ptr(other.ptr), refCountPtr(other.refCountPtr)
    {
        other.ptr = nullptr;
        other.refCountPtr = nullptr;
    }

    SharedPtr(const WeakPtr<ElementType>& other)
    {
        if (other.Expired()) {
            throw BadWeakPtr();
        }
        ptr = other.ptr;
        refCountPtr = other.refCountPtr;
        addRef();
    }

    const SharedPtr& operator=(const SharedPtr& other) noexcept
    {
        SharedPtr(other).Swap(*this);
        return *this;
    }

    const SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    explicit SharedPtr(ElementType* p) :
        ptr(p)
    {
        if (ptr)
        {
            refCountPtr = new RefCount();
            addRef();
        }
    }

    SharedPtr(std::nullptr_t) :
        ptr(nullptr), refCountPtr(new RefCount)
    {
    }

    SharedPtr() :
        ptr(nullptr)
    {
    }

    ~SharedPtr()
    {
        removeRef();
        delete refCountPtr;
        delete ptr;
    }

    void Swap(SharedPtr<T>& other) 
    {
        std::swap(ptr, other.ptr);
        std::swap(refCountPtr, other.refCountPtr);
    }

    void Reset() noexcept
    {
        if (ptr && --refCountPtr->sharedCount + refCountPtr->weakCount == 0) {
            delete ptr;
            delete refCountPtr;
        }
        else if (!ptr)
            delete refCountPtr;

        ptr = nullptr;
        refCountPtr = nullptr;
    }

    void Reset(ElementType* p) noexcept
    {
        Reset();
        delete refCountPtr;
        delete ptr;
        refCountPtr = new RefCount;
        refCountPtr->sharedCount++;
        ptr = p;
        p = nullptr;
    }

    inline operator bool() noexcept
    {
        return ptr && refCountPtr;
    }

    inline ElementType* operator->() noexcept
    {
        return ptr;
    }

    inline ElementType& operator*() noexcept
    {
        return *ptr;
    }

    inline ElementType* Get() const noexcept
    {
        return ptr;
    }

    int32_t UseCount() 
    {
        if (refCountPtr == nullptr) return 0;
        return refCountPtr->sharedCount;
    }

private:
    inline void addRef() noexcept
    {
        if (refCountPtr != nullptr) ++refCountPtr->sharedCount;
    }

    inline void removeRef() noexcept
    {
        if (refCountPtr == nullptr) 
        {
            delete ptr;
            return;
        }
        --refCountPtr->sharedCount;

        if (refCountPtr->sharedCount == 0) 
        {
            delete ptr;
        }

        if (refCountPtr->sharedCount + refCountPtr->weakCount <= 0) 
        {
            delete refCountPtr;
        }

        refCountPtr = nullptr;
        ptr = nullptr;
    }

    ElementType* ptr = nullptr;
    RefCount* refCountPtr = nullptr;
};

template<class T>
class WeakPtr
{
    template <class U> friend class WeakPtr;
    template <class U> friend class SharedPtr;
public:
    typedef T ElementType;

    WeakPtr() :
        ptr(nullptr), refCountPtr(nullptr)
    {
    }

    WeakPtr(const WeakPtr& other) :
        ptr(other.ptr), refCountPtr(other.refCountPtr)
    {
        addRef();
    }

    WeakPtr(WeakPtr<ElementType>&& other) :
        ptr(other.ptr), refCountPtr(other.refCountPtr)
    {
        other.ptr = nullptr;
        other.refCountPtr = nullptr;
    }

    void Swap(WeakPtr<ElementType>& other) 
    {
        std::swap(ptr, other.ptr);
        std::swap(refCountPtr, other.refCountPtr);
    }

    template<class U>
    WeakPtr(const SharedPtr<U>& other)
    {
        if (refCountPtr == nullptr) 
        {
            delete refCountPtr;
            delete ptr;
        }
        ptr = other.ptr;
        refCountPtr = other.refCountPtr;
        addRef();
    }

    const WeakPtr& operator=(const WeakPtr& other) noexcept
    {
       WeakPtr(other).Swap(*this);
        return *this;
    }

    template<class U>
    const WeakPtr& operator=(const SharedPtr<U>& other) noexcept
    {
        removeRef();

        ptr = other.ptr;
        refCountPtr = other.refCountPtr;

        addRef();

        return *this;
    }

    operator SharedPtr<ElementType>() 
    {
        return Lock();
    }

    const WeakPtr& operator=(WeakPtr&& other) noexcept
    {
        Swap(other);
        other.ptr = nullptr;
        other.refCountPtr = nullptr;

        return *this;
    }

    ~WeakPtr()
    {
        removeRef();
        delete refCountPtr;
        delete ptr;
    }

    SharedPtr<ElementType> Lock()
    {
        if (Expired())
            return SharedPtr<ElementType>();
        return SharedPtr<ElementType>(*this);
    }

    bool Expired() const 
    {
        if (refCountPtr == nullptr) return true;
        return refCountPtr->sharedCount == 0;
    }

    int UseCount() 
    {
        if (refCountPtr == nullptr) return 0;
        return refCountPtr->sharedCount;
    }

    void Reset() 
    {
        if (refCountPtr == nullptr) return;
        --refCountPtr->weakCount;

        if (refCountPtr->weakCount <= 0 && refCountPtr->sharedCount <= 0) 
        {
            delete refCountPtr;
            delete ptr;
        }

        refCountPtr = nullptr;
    }


private:
    inline void addRef() noexcept
    {
        if (refCountPtr) ++refCountPtr->weakCount;
    }

    inline void removeRef() noexcept
    {
        if (refCountPtr)
        {
            if (--refCountPtr->weakCount + refCountPtr->sharedCount <= 0)
            {
                delete refCountPtr;
            }
        }
        refCountPtr = nullptr;
        ptr = nullptr;
    }
    ElementType* ptr = nullptr;
    RefCount* refCountPtr = nullptr;
};
