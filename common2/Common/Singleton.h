#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <boost/utility.hpp>
#ifdef USE_THREADSAFE_SINGLETON
#include <boost/thread/thread.hpp>
#endif
#include "SmartPtr.h"

namespace Common
{
    template<class T>
    class Singleton : boost::noncopyable
    {
    public:
        static T& Instance()
        {
#ifdef USE_THREADSAFE_SINGLETON
            static boost::mutex _mutex;
            boost::mutex::scoped_lock scoped_lock(_mutex);
#endif
            static std::unique_ptr<T> _impl;
            if (!_impl.get())
            {
                _impl = make_unique<T>();
            }
            return *_impl.get();
        }
    private:
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
    };
}

#define SINGLETON_BLOCK(ClassName)\
protected:\
    friend class ::Common::Singleton<ClassName>;\
    ClassName(ClassName const&) {}\
    ClassName inline& operator=(ClassName const&) { return *this; }\
private:

#define SINGLETON_INSTANCE(ClassName) ::Common::Singleton<ClassName>::Instance()

#endif // SINGLETON_H
