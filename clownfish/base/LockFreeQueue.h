#ifndef _CLOWNFISH_BASE_LOCKFREE_QUEUE_H_
#define _CLOWNFISH_BASE_LOCKFREE_QUEUE_H_

#include <memory>
#include <atomic>

namespace clownfish
{
template <typename T>
class LockFreeQueue
{
public:
    LockFreeQueue()
    {
        std::shared_ptr<Node> p(new Node);
        head_ = tail_ = p;
    }
    ~LockFreeQueue()
    {
        T t;
        while (popFront(t));
    }
    LockFreeQueue(const LockFreeQueue&) = delete;
    LockFreeQueue& operator=(const LockFreeQueue&) = delete;

    void pushBack(T&& d)
    {
        std::shared_ptr<Node> q(new Node);
        q->data = std::move(d);

        auto p = tail_;
        auto oldP = p;

        std::shared_ptr<Node> nullnote;
        bool exchres = false;
        do {
            while (p->next) {
                p = p->next;
            }
            nullnote = nullptr;
            exchres = std::atomic_compare_exchange_weak(&p->next, &nullnote, q);
        } while(!exchres);

        std::atomic_compare_exchange_weak(&tail_, &oldP, q);
    }

    bool popFront(T& result)
    {
        bool exchres = false;
        auto p = head_->next;
        do {
            p = head_->next;
            if (p) {
                exchres = std::atomic_compare_exchange_weak(&head_->next, &p, p->next);
            }
            else {
                return false;
            }
        } while (!exchres);

        result = std::move(p->data);
        return true;
    }

private:
    struct Node
    {
        Node() : next(nullptr) {}
        T data;
        std::shared_ptr<Node> next;
    };
    std::shared_ptr<Node> head_;
    std::shared_ptr<Node> tail_;
};
}

#endif
