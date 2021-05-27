#include <iostream>
#include <set>
#include <map>
#include "repo.hpp"
class RepoInMemory : public Repo{
    public:
virtual void SaveLink(std::string link);
virtual void SaveSite(std::string url, std::string content);
virtual std::string GetSize(std::string url);
private:
std::set<std::string> _links;
std::map<std::string, std::string> _sites;
};