#pragma once

#include <atomic>

namespace slr
{
template<typename CONTEXT>
class Object
{
public:
    virtual void serialize(CONTEXT&) = 0;
    virtual ~Object() = default;
};


// struct ControlBlock 
// {
//     void* object = nullptr;
//     std::atomic<std::size_t> useCount;

//     ControlBlock()
//     {
//         useCount = 0;
//     }
// };

// template<typename T>
// class ControlBlockPtr
// {
// private:
//     ControlBlock* controlBlock = nullptr;

// public:
//     ~ControlBlockPtr()
//     {
//         if (controlBlock != nullptr)
//         {
//             controlBlock->useCount--;
//         }
//     }

//     void assign(ControlBlock* newControlBlock)
//     {
//         if (controlBlock != nullptr)
//         {
//             controlBlock->useCount--;
//         }

//         controlBlock = newControlBlock;

//         if (controlBlock != nullptr)
//             controlBlock->useCount++;
//     }

//     T* get()
//     {
//         if (controlBlock == nullptr)
//             return nullptr;

//         return static_cast<T*>(controlBlock->object);
//     }
// };


// template<typename T>
// class WeakObjectPtr;

// template<typename T>
// class ObjectPtrControlBlock 
// {
// private:
//     T* data = nullptr;
//     std::size_t sharedCount = 1;
//     std::size_t totalCount = 1; // count of the number of SharedObjectPtr and WeakObjectPtr pointing to this object

// public:
//     static ObjectPtrControlBlock<T>* create()
//     {
//         return new ObjectPtrControlBlock<T>();
//     }

//     void removeSharedReference()
//     {
//         sharedCount--;
//         removeWeakReference();
//     }

//     void increaseSharedReference()
//     {
//         sharedCount++;
//         increaseWeakReference();
//     }

//     void removeWeakReference()
//     {
//         totalCount--;
//         if (totalCount == 0)
//             delete this;
//     }

//     void increaseWeakReference()
//     {
//         totalCount++;
//     }
// };

// template<typename T>
// class SharedObjectPtr 
// {
// private:
//     // Is never nullptr
//     ObjectPtrControlBlock<T>* controlBlock = ObjectPtrControlBlock<T>::create();

// public:
//     SharedObjectPtr<T>& operator=(const SharedObjectPtr<T>& rhs)
//     {
//         controlBlock->removeSharedReference(); 
//         controlBlock = rhs.controlBlock;
//         controlBlock->increaseSharedReference();
//     }

//     // object : must not move in memory (can be nullptr)
//     // The object will be changed for each pointer of the control block
//     void assign(T* object)
//     {
//         controlBlock->data = object;
//     }
// };

// template<typename T>
// class WeakObjectPtr 
// {
// private:
//     ObjectPtrControlBlock<T>* controlBlock = nullptr;

// public:
//     void assign(ObjectPtrControlBlock<T>* newControlBlock) 
//     {
//         if (controlBlock != nullptr)
//             controlBlock->removeWeakReference();

//         controlBlock = newControlBlock;

//         if (controlBlock != nullptr)
//             controlBlock->increaseWeakReference();
//     }

//     void assign(SharedObjectPtr<T>* newControlBlock) 
//     {
//         assign(newControlBlock->controlBlock);
//     }

//     ~WeakObjectPtr()
//     {
//         if (controlBlock != nullptr)
//             controlBlock->removeWeakReference();
//     }

//     T* get()
//     {
//         if (controlBlock == nullptr)
//             return nullptr;

//         return controlBlock->data;
//     }
// };


}