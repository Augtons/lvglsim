#ifndef LVGLSIM_UNIQUENATIVEOBJECT_HPP
#define LVGLSIM_UNIQUENATIVEOBJECT_HPP

#include <memory>
#include <functional>

template<typename T, auto Deleter>
struct UniqueNativeObject {
    std::unique_ptr<T, decltype(Deleter)> uniquePtr = nullptr;

    explicit UniqueNativeObject(T* ptr = nullptr)
        : uniquePtr(ptr, Deleter) { }

    UniqueNativeObject& operator=(std::nullptr_t) {
        uniquePtr.reset();
        return *this;
    }

    UniqueNativeObject(const UniqueNativeObject&) = delete;
    UniqueNativeObject& operator=(const UniqueNativeObject&) = delete;

    UniqueNativeObject(UniqueNativeObject&&) noexcept = default;
    UniqueNativeObject& operator=(UniqueNativeObject&&) noexcept = default;

    ~UniqueNativeObject() {
        if (uniquePtr) {
            Deleter(detach());
        }
    }

    inline bool operator==(std::nullptr_t) const {
        return uniquePtr == nullptr;
    }

    inline operator bool() const {
        return this == nullptr;
    }

    inline operator T*() {
        return uniquePtr.get();
    }

    inline auto detach() {
        return uniquePtr.release();
    }

    inline auto get() {
        return uniquePtr.get();
    }
};

#endif //LVGLSIM_UNIQUENATIVEOBJECT_HPP
