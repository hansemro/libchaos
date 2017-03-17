#ifndef ZWORKQUEUE_H
#define ZWORKQUEUE_H

#include "zqueue.h"
#include "zmutex.h"
#include "zcondition.h"
#include "zlog.h"

namespace LibChaos {

/*! Thread-safe work distribution utility.
 *  \ingroup Thread
 */
template <class T> class ZWorkQueue {
public:
    ZWorkQueue(ZAllocator<typename ZList<T>::Node> *alloc = new ZAllocator<typename ZList<T>::Node>,
               ZCondition::condoption condoptions = ZCondition::NONE) :
        _queue(alloc), _condtion(condoptions){

    }

    ZWorkQueue(const ZWorkQueue &) = delete;

    void addWork(const T &item){
        _condtion.lock();

        _queue.push(item);

        _condtion.signal();
        _condtion.unlock();
    }

    T getWork(){
        _condtion.lock();
        while(_queue.size() == 0){
            _condtion.wait();
        }

        T item = _queue.peek();
        _queue.pop();

        _condtion.unlock();
        return item;
    }

    zu64 size(){
        _condtion.lock();
        zu64 sz = _queue.size();
        _condtion.unlock();
        return sz;
    }

private:
    ZQueue<T> _queue;
    ZCondition _condtion;
};

} // namespace LibChaos

#endif // ZWORKQUEUE_H
