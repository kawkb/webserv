
## Resources

### Server setup

[How to build a simple HTTP server](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

[C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)

### HTTP 1.1 (standard to follow) :

[HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### Other HTTP (legacy / future) :

[HTTP/1.0 (RFC 1945)](https://www.rfc-editor.org/rfc/rfc1945.html)

[HTTP/2 (RFC 7240)](https://www.rfc-editor.org/rfc/rfc7540.html)

[HTTP/2 : Header Compression (RFC 7241)](https://www.rfc-editor.org/rfc/rfc7541.html)

[FTP (RFC 959)](https://www.rfc-editor.org/rfc/rfc959.html)

### HTTP Header Syntax

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

### Select and non-blocking

[Select](https://www.lowtek.com/sockets/select.html)

[Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)

### CGI

[CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)











# 2. quality readings

### web socket
- nice article https://www.bogotobogo.com/cplusplus/sockets_server_client.php
- Beej's Guide to Network Programming http://beej.us/guide/bgnet/html/
- "build http server from scratch in C" https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
- Les sockets en C https://broux.developpez.com/articles/c/sockets/
- select() http://manpages.ubuntu.com/manpages/xenial/fr/man2/select_tut.2.html
- select() example https://man.developpez.com/man2/select_tut/
- select() in the details https://notes.shichao.io/unp/ch6/#readset-writeset-and-exceptset-as-value-result-
- nice schema https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm
- select() vs threads https://www.lowtek.com/sockets/select.html
- TCP 3-way handshake schema ; Blocking vs non-blocking socket https://www.bogotobogo.com/cplusplus/sockets_server_client_2.php
- more socket theory to waste time https://www.bogotobogo.com/cplusplus/sockets_server_client_3.php

### http protocol
- How the web works http://www.garshol.priv.no/download/text/http-tut.html
- RFCs big picture, in french http://abcdrfc.online.fr/rfc-vf/pdf/rfc2616.pdf
- RFCs 7230-7235 overview, in french https://www.bortzmeyer.org/http-11-reecrit.html
- overview http://www.iprelax.fr/http/http_art.php
- another overview https://www.commentcamarche.net/contents/520-le-protocole-http
- Request and Response format https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
- another overview https://www.coozook.com/static/book-samples/B212934F6A-sample.pdf

### http headers in details
- MIME list https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
- What HTTP response headers are required https://stackoverflow.com/questions/4726515/what-http-response-headers-are-required
- response status code https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP
- auth theory https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#Basic_authentication_scheme
- auth practice https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

### CGI programming
- nice overview https://perso.liris.cnrs.fr/lionel.medini/enseignement/M1IF03/Tutoriels/Tutoriel_CGI_SSI.pdf
- some details https://www.developpez.net/forums/d151285/php/langage/php-js-quoi-sert-php-cgi-exe-repertoire-php/

### chunked encoding
- wiki https://fr.wikipedia.org/wiki/Chunked_transfer_encoding
- the idea https://www.geeksforgeeks.org/http-headers-transfer-encoding/

### utils
- https://superuser.com/questions/48505/how-to-find-virtual-memory-size-and-cache-size-of-a-linux-system
- https://blog.seboss666.info/2015/08/reprenez-le-controle-du-cache-memoire-du-noyau-linux/

# 3. testing

- python library to make http requests https://requests.readthedocs.io/en/master/
- quickly and easily send requests https://www.postman.com/






## Sources
- [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
- [Server/Location matching](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)
- [Configuration File](http://nginx.org/en/docs/dirindex.html)



https://www.notion.so/etelcode/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46