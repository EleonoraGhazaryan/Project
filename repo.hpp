#include <iostream>
class Repo{
public:
    virtual void SaveLink(std::string link) = 0;
    virtual void SaveSite(std::string url, std::string content)=0;
    virtual std::string GetSite(std::string url) = 0;
};