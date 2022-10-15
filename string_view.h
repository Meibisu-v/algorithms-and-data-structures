#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <ostream>
#include <utility>
#include <stdexcept>


class StringView {
public:
    using traits_type = std::char_traits<char>;
    using value_type = char;
    using pointer = char*;
    using const_pointer = const char*;
    using reference = char&;
    using const_reference = const char&;
    using const_iterator = const char*;
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    static constexpr size_type npos = static_cast<size_type>(-1);

    // Null `string_view` constructor
    constexpr StringView() noexcept : ptr_(nullptr), length_(0) {}

    // Implicit constructors
   
    constexpr StringView(const char* str)
        : ptr_(str),
        length_(str ? CheckLengthInternal(StrlenInternal(str)) : 0) {}

    constexpr StringView(const char* data, size_type len)
        : ptr_(data), length_(CheckLengthInternal(len)) {}

    constexpr const_iterator begin() const noexcept { return ptr_; }

    constexpr const_iterator end() const noexcept { return ptr_ + length_; }

    constexpr const_iterator cbegin() const noexcept { return begin(); }

    constexpr const_iterator cend() const noexcept { return end(); }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    const_reverse_iterator crend() const noexcept { return rend(); }

    constexpr size_type Size() const noexcept {
        return length_;
    }

    constexpr size_type Length() const noexcept { return Size(); }

    constexpr size_type MaxSize() const noexcept { return kMaxSize; }

    constexpr bool Empty() const noexcept { return length_ == 0; }

    
    constexpr const_reference operator[](size_type i) const {
        return(ptr_[i]);
    }

    StringView operator=(const StringView& other) {
        ptr_ = other.ptr_;
        length_ = other.length_;
        return *this;
    }

    const_reference At(size_type i) const {
        if (!(i < Size())) throw(std::out_of_range(ptr_)); 
        return ptr_[i];
    }

    constexpr const_reference Front() const {
        return ptr_[0];
    }

    constexpr const_reference Back() const {
        return ptr_[Size() - 1];
    }

    // StringView::data()
    constexpr const_pointer Data() const noexcept { return ptr_; }

    // StringView::remove_prefix()
    void RemovePrefix(size_type n) {
        if (n <= length_) {
            ptr_ += n;
            length_ -= n;
        }
    }

    // StringView::remove_suffix()
    void RemoveSuffix(size_type n) {
        if (n <= length_) {
            length_ -= n;
        }
    }

    // StringView::swap()
    void Swap(StringView& s) noexcept {
        auto t = *this;
        *this = s;
        s = t;
    }
    
    constexpr StringView Substr(size_type pos, size_type n = npos) const {
        return(StringView(ptr_ + pos, Min(n, length_ - pos)));
    }

private:
    static constexpr size_type kMaxSize =
        (std::numeric_limits<difference_type>::max)();

    static constexpr size_type CheckLengthInternal(size_type len) {
        return len;
    }

    static constexpr size_type StrlenInternal(const char* str) {
        return str ? strlen(str) : 0;
    }

    static constexpr size_t Min(size_type length_a, size_type length_b) {
        return length_a < length_b ? length_a : length_b;
    }

    /*static constexpr int CompareImpl(size_type length_a, size_type length_b,
        int compare_result) {
        return compare_result == 0 ? static_cast<int>(length_a > length_b) -
            static_cast<int>(length_a < length_b)
            : (compare_result < 0 ? -1 : 1);
    }*/

    const char* ptr_;
    size_type length_;
};

#endif  
