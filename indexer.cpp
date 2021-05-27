#include "indexer.hpp"
#include <sstream>
#include <iostream>
void Indexer::AddSite(std::string url, std::string contents){
    //std::cout << "Indexing: " << url << std::endl;
    //Split the contents into words
    std::map<std::string, unsigned int> word_countds;
    //Get word occurance count
    std::stringstream str(contents);
    std::string word;
    while(str >> word){
        if(word_countds.find(word)==word_countds.end()){
            word_countds[word]=0;
        }else{
            word_countds[word]++;
        }
        
    }

    //Store in index
    for(auto i = word_countds.begin(); i != word_countds.end(); ++i){
        auto word = i->first;
        auto count = i->second;
        //if(index.find(word) == index.end()){
            pthread_mutex_lock(&_mutex);
            _index[word][count] = url;
            pthread_mutex_unlock(&_mutex);
    }
   // std::cout << "Index size:  " << _index.size() << std::endl;
}
std::vector<std::string> Indexer::GetRelevantURLs(std::string word, unsigned int count){
    //Check if the word presents
    std::cout << "Index size:  " << _index.size() << std::endl;
    std::vector<std::string> matches;
    int i = 0;
    for(auto iter = _index[word].begin(); iter != _index[word].end() && i < count; ++iter, ++i){
        matches.push_back(iter->second);
    }
    return matches;
}