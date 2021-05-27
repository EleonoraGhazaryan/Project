#include <iostream>
#include <vector>
#include <map>
#include <pthread.h>
 class Indexer{
public:
void AddSite(std::string url, std::string contents);
std::vector<std::string> GetRelevantURLs(std::string word, unsigned int count=10);
private:
// word -> (relevance, URL)
std::map<std::string, std::map<float, std::string>> _index;
pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
};