#pragma once

#include <queue>

template <typename T>
class ObjectPool
{
private:
	std::queue<T*> pool;

public:
	T* CreateObject();
    void Initialize(int count);
    T* GetObject();
    void ReturnObject(T* t);
};

template <typename T>
inline T* ObjectPool<T>::CreateObject()
{
    return nullptr;
}

template <typename T>
inline void ObjectPool<T>::Initialize(int count)
{
    for (int i = 0; i < count; i++)
        pool.push(new T);
}

template <typename T>
inline T* ObjectPool<T>::GetObject()
{
    if (pool.empty())
        return new T;
    else {
        T* temp = pool.front();
        pool.pop();
        return temp;
    }
}

template <typename T>
inline void ObjectPool<T>::ReturnObject(T* t)
{
    pool.push(t);
}
