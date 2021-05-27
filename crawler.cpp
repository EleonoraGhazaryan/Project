#include <iostream>
#include <stdexcept>
#include <regex.h>

#include "easyhttpcpp/EasyHttp.h"
#include "linkqueue.h"
#include "repo_in_memory.hpp"
#include "indexer.hpp"
#include <sstream>
#include <iterator>
#include <unistd.h>
std::string GetContents(std::string url){
  try{
	easyhttpcpp::EasyHttp::Builder httpClientBuilder;
     

    // create http client
    easyhttpcpp::EasyHttp::Ptr pHttpClient = httpClientBuilder.build();
    // create a new request and execute synchronously
    easyhttpcpp::Request::Builder requestBuilder;
    easyhttpcpp::Request::Ptr pRequest = requestBuilder.setUrl(url).build();
    easyhttpcpp::Call::Ptr pCall = pHttpClient->newCall(pRequest);
    easyhttpcpp::Response::Ptr pResponse = pCall->execute();
    if (!pResponse->isSuccessful()){
        std::cerr << "HTTP GET Error: (" << pResponse->getCode() << ")" << std::endl;
        return "";
    } else {
        std::cout << "HTTP GET Success!" << std::endl;
	    const std::string contentType = pResponse->getHeaderValue("Content-Type", "");
		    if (Poco::isubstr<std::string>(contentType, "text/html") != std::string::npos){
            return pResponse->getBody()->toString();
        }
            return "";
    }
  }
    catch(...){
        std::cerr << "Error while getting contents";
        return "";
    }
}

const int MAX_MATCHES = 10;
linkQueue linkqueue;
Repo* repo;
extern Indexer indexer;

void linkAdded(std::string link){
    static int count=0;
    // stop after 50 sites
    /*if(count++ > 50){
      std::cout << "Stopping crawler after max step reaches" << std::endl;
      return;
    }*/
    std::cout<<"Crawler:: LinkAdded! " << link << std::endl;
    if(!(link.find("http")==0)){
        return;
    }
   repo->SaveLink(link);
    std::string contents = GetContents(link);
    if(contents == "" || contents.size() < 100){
            return;

    }
    repo->SaveSite(link, contents);
    //Activate indexer
    //indexer.AddSite(link, contents);
//std::cout << "Cont. size: " << contents.size() << std::endl;
    
    
const std::string link_prefix = "href=";
  size_t pos = 0;
  while((pos < contents.size()) && ((pos = contents.find(link_prefix, pos + 1)) != std::string::npos)){
    std::cout << "Position: " << pos << std::endl;
    pos += link_prefix.size() + 1;
    char quot = contents[pos -1];
    if(quot != '\'' && quot != '\"'){
      continue;
    }

    size_t end_pos = contents.find(quot, pos);

    if(end_pos == std::string::npos){
      break;
    }
    std::string link = contents.substr(pos, end_pos - pos);
    if(!( link.find("http") == 0 /*|| link[0] == '/'*/)){
      //std::cout << "Skipping link " << link << std::endl;
      continue;
    }
    //std::cout << "Position = " << pos << "\tQuot = " << quot << "\tLink = " << link << std::endl;
    linkqueue.addLink(link);
  }
}

void TestHandler(std::string link){
    std::cout << "Link received " << link << std::endl;
}

void TestHandler2(std::string link){
  std::cout << "Link recived in test2 " << link << std::endl;
}

int run_server();
int main(){

    std::cout << "Running crawler" << std::endl;
    //repo = new RepoInMemory();
    
    //linkQueue.registerHandler(TestHandler);
    //linkQueue.registerHandler(TestHandler2);
    //linkQueue.AddLink("http://blablabla");
    //linkQueue.AddLink("http:/rgxhcgstjvbx");
    linkqueue.registerHandler(linkAdded);
    //linkqueue.registerHandler(repo->SaveLink);
    
    linkAdded("https://www.topuniversities.com/student-info/choosing-university/worlds-top-100-universities");
   /* sleep(2);
    auto matches = indexer.GetRelevantURLs("university");
    std::cout << "________________________________________" << std::endl;
    std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(std::cout, ",\n"));*/
    std::cout << "Running server..." << std::endl;
    run_server();
    /*int a=0;
    std::cin >> a;*/
    std::cout << "Running crawler - Done!" << std::endl;
    return 0;
}
int main_regex(){
    std::string contents = GetContents("https://www.topuniversities.com/student-info/choosing-university/worlds-top-100-universities");
    std::string regex = "(\\s)*(\t)*Mem([0-9]*) (\\s,\t)*= (\\s,\t)*[0-9]*(.)*[0-9]*" ;
    regex_t re;
    const int MAX_MATCHES = 10;
    if(regcomp(&re, regex.c_str(), REG_EXTENDED) != 0 ){
        std::cerr << "Cannot compile regex" << std::endl;
        return 1;
    }
    regmatch_t matches[MAX_MATCHES];
    if(regexec(&re, contents.c_str(), sizeof(matches)/sizeof(matches[0]), (regmatch_t*)&matches,0) == REG_NOMATCH){
        std::cerr << "No links found!!" << std::endl;
        return 1;
    }
    for (int i = 0, offset = 0; i < MAX_MATCHES && offset < contents.size(); ++i){
        std::cout << "Matches  " << i << "->" << contents.substr(matches[i].rm_so, matches[i].rm_eo - matches[i].rm_so + 1) << std::endl;
        offset = matches[i].rm_eo + 1;
    }
    return 0;
}
