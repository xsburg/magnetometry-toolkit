#pragma once

#include <memory>

#define SMART_PTR_T(t) \
    typedef std::shared_ptr<t> SharedPtr_t; \
    typedef std::unique_ptr<t> UniquePtr_t;

#define NULL_PTR_DECL \
    static const SharedPtr_t sharedNullPtr; \
    static const UniquePtr_t uniqueNullPtr;

#define NULL_PTR_IMPL(t) \
    const t::SharedPtr_t t::sharedNullPtr = t::SharedPtr_t(); \
    const t::UniquePtr_t t::uniqueNullPtr = t::UniquePtr_t();

namespace Common
{
    /* 
    // uncomment when variadic templates will be implemented.
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }*/

    template<typename T>
    std::unique_ptr<T> make_unique()
    {
        return std::unique_ptr<T>(new T());
    }

    template<typename T, typename Arg1>
    std::unique_ptr<T> make_unique(Arg1&& arg1)
    {
        return std::unique_ptr<T>(new T(std::forward<Arg1>(arg1)));
    }

    template<typename T, typename Arg1, typename Arg2>
    std::unique_ptr<T> make_unique(Arg1&& arg1, Arg2&& arg2)
    {
        return std::unique_ptr<T>(new T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));
    }

    template<typename T, typename Arg1, typename Arg2, typename Arg3>
    std::unique_ptr<T> make_unique(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
    {
        return std::unique_ptr<T>(new T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3)));
    }

    // Move object from one unique pointer to another with dynamic cast
    template<typename Y, typename T>
    std::unique_ptr<Y> dynamic_pointer_cast(std::unique_ptr<T> obj)
    {
        return std::unique_ptr<Y>(dynamic_cast<Y*>(obj.release()));
    }
}
