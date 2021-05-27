#include "linkqueue.h"
#include <unistd.h>
#include <iostream>

// ToDo: Move to compile options
#undef _TRACE
#ifdef _TRACE
#define Trace(msg) std::cout << msg << std::endl;
#else
#define Trace(msg)
#endif

void * HandlerInvoker(void * param){
    linkQueue* linkqueue = (linkQueue*) param;
    pthread_mutex_lock(&linkqueue-> mutex);

    while(linkqueue -> _linkQueue.size())
    {
        pthread_mutex_lock(&linkqueue->mutex);
         std::string link = linkqueue->_linkQueue.front();
        Trace(link);
        linkqueue->_linkQueue.pop();
        Trace("Link removed from queue");
        pthread_mutex_unlock(&linkqueue->mutex);
       for(auto i = linkqueue->_handlers.begin(); i != linkqueue->_handlers.end(); ++i)
        {
            (*i)(link);
        }
    }
         
}
void* msgReceiver(void* arg)
{
    Trace("Broadcaster thread started");
    linkQueue* linkqueue = (linkQueue*) arg;
    while(true)
    {
        Trace("Waiting for link");
        pthread_cond_wait(&linkqueue->condvar, &linkqueue->mutex);
        Trace("Link received");
       // std::cout << "Queue size" << linkQueue->_linkQueue.size() << std::endl;
        //std::cout << "Handler size" << linkQueue->_handlers.size() << std::endl;
        pthread_t th;
       pthread_create(&th, NULL, HandlerInvoker, arg);
        pthread_mutex_unlock(&linkqueue->mutex);
       // std։։cout << "Broadcasting link" << link << std::endl;
      
    }
}

linkQueue::linkQueue()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condvar, NULL);
    pthread_create(&thread, NULL, msgReceiver, (void*)this);
 
    sleep(1);
}



void linkQueue::addLink(std::string link)
{
    Trace("Locking mutex");
    pthread_mutex_lock(&this->mutex);
    Trace("Adding link");
    this->_linkQueue.push(link);
    Trace("Link added");
    pthread_cond_signal(&this->condvar);
    Trace("Cond var is signaled");
    pthread_mutex_unlock(&this->mutex);
    Trace("Mutex unlocked");

}

void linkQueue::registerHandler(linkHandler handler)
{
    this->_handlers.insert(handler);
}
