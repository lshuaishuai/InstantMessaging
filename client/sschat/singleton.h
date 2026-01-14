#ifndef SINGLETON_H
#define SINGLETON_H

#include <global.h>

template <class T>
class Singleton
{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>&) = delete;

public:
    //
    // _instance = std::shared_ptr<T>(new T);不是原子操作
    /*
     * 传统 Double-Checked Locking（DCLP）的风险：指令重排 + 内存可见性
     * _instance = std::shared_ptr<T>(new T);不是原子操作，拆分为 3 步指令：1分配内存 2调用T的构造函数，分配内存 3_instance指向分配的内存地址
     * 但是步骤2 3可能会调换2 3的顺序，多线程情况下可能会出现问题
     */
    static std::shared_ptr<T> GetInstance()
    {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            // 为何不适用make_shared呢？make_shared无法访问受保护/私有的构造函数 std::make_shared是标准库函数，不属于T的友元，因此无法访问T的非公有构造函数，编译会直接报错
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    ~Singleton()
    {
        std::cout << "This is singleton destruct" << std::endl;
    }

    void PrintAddress()
    {
        std::cout << _instance.get() << std::endl;
    }

protected:
    static std::shared_ptr<T> _instance;
};

template <class T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
