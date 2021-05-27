#include <string>
#include <queue>
#include <set>
#include <pthread.h>

typedef void (*linkHandler)(std::string);
class linkQueue 
{
    private:    
    std::queue<std::string> _linkQueue;
    std::set<linkHandler> _handlers;
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t condvar;

    public:
    linkQueue();
    void addLink(std::string url);
    void registerHandler (linkHandler handler);  
    friend void * msgReceiver(void * arg);  
    friend void * HandlerInvoker(void * arg);

};