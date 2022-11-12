#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>

int main()
{
    std::vector<char>   m_requestBuffer;
    const char *buf = "hello";
    m_requestBuffer.assign(buf,buf + strlen(buf));
    const char *buff = "how are";
    std::vector<char> vectorHugo;
    vectorHugo.assign(buff,buff + strlen(buff));
    m_requestBuffer.insert(m_requestBuffer.begin() + m_requestBuffer.size(), vectorHugo.begin(),vectorHugo.end());
    // for(std::vector<char>::iterator i = m_requestBuffer.begin(); i != m_requestBuffer.end(); i++)
        // std::cout << *i << std::endl;
    std::vector<char>::iterator pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), 'l');
    std::string line =  std::string(m_requestBuffer.begin(), pos);
    std::cout << line;
}

// // GET / HTTP/1.1
// // User-Agent: PostmanRuntime/7.29.2
// // Accept: */*
// // Postman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f
// // Host: localhost:8080
// // Accept-Encoding: gzip, deflate, br
// // Connection: keep-alive