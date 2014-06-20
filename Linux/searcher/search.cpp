// search.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include "stlcompare.h"
#include "mgcgi.h"

int     download_file(std::string http_file, std::ofstream& file);
bool    unzip(std::string http_zip_file);

int main( int argc, char**argv )
{
    std::string tmp_file1;
    std::string tmp_file2;
    if (argc == 3) { // get stl files from locale file system
        tmp_file1 = argv[1];
        tmp_file2 = argv[2];

    } else { // get stl files from Http
        // Loads input form data
        MGCGILoadInput();
        std::string http_file1 = MGCGIInput;
        std::string http_file2 = &MGCGIInputDup[strlen(MGCGIInput)+1];
        std::string tmp_file1 = "tmp1.zip";
        std::string tmp_file2 = "tmp2.zip";

        std::ofstream file1(tmp_file1.c_str() , ::std::ios::out|::std::ios::binary);
        if (!file1.is_open()){
                std::cout << "can't create tmp1.stl file \n";
                return 0;
        }
        int r = download_file(http_file1, file1);
        file1.close();
        if(r < 1) {
            std::cout << "Download error. Can't create tmp1.zip file \n";
            remove( tmp_file1.c_str()) ;
            return 0;
        }


        std::ofstream file2(tmp_file2.c_str(), ::std::ios::out|::std::ios::binary);
        if (!file2.is_open()){
                std::cout << "can't create tmp2.stl file \n";
                return 0;
        }
        r = download_file(http_file2, file2);
        file2.close();
        if(r < 1) {
            std::cout << "Download error. Can't create tmp2.zip file \n";
            remove( tmp_file2.c_str() ) ;
            return 0;
        }

        // Implementation of unzip part
        if(!unzip(tmp_file1)) {
            std::cout << "unzip error for the tmp1.zip file \n";
            remove( tmp_file1.c_str() ) ;
            return 0;
        }
        if(!unzip(tmp_file2)) {
            std::cout << "unzip error for the tmp2.zip file \n";
            remove( tmp_file2.c_str() ) ;
            return 0;
        }
    }
    // compary 2 meshes and get the percent equality
    char res = 17;
    StlCompare compare;
    res = compare.init(tmp_file1,tmp_file2);
    if (res != -1) {
        char buf[4];
        sprintf(buf,"%d",res);
        //itoa(res, buf, 10);
        std::cout << "Content-type:text/html\r\n\r\n";
        std::cout << "<html>\n";
        std::cout << "<head>\n";
        std::cout << "<title>Result of the Huffman Code</title>\n";
        std::cout << "</head>\n";
        std::cout << "<body>\n";
        std::cout << "<h2>The percent equality of two mesh is equal " << buf << "% </h2> \n";
        std::cout << "</body>\n";
        std::cout << "</html>\n";
    }
    // Signal end of page
    // Terminate program, and shut down MGCGI library
    MGCGIExit();
    return 0;
}


//REQUEST_METHOD=GET
//QUERY_STRING=http://fablab.su/images/models_files/custom-temple-2/Custom_Temple_2.zip&http://fablab.su/
// images/models_files/roman-emperor-walters-art-museum/Roman_Emperor_Walters_Art_Museum.zip
// port = 443,	Hypertext Transfer Protocol over TLS/SSL (HTTPS)
// port = 8243, HTTPS listener for Apache Synapse
// port = 8280, HTTP listener for Apache Synapse
// Accept-Ranges: bytes

int download_file(std::string file_name, std::ofstream& file)
{
    struct sockaddr_in servaddr;
    struct hostent *hp;
    int sock_id;
    char message[10*1024] = {0};
    int msglen = 0, total_msglen = 0;
    std::size_t found1 = file_name.find ("://");
    found1 += 3;
    std::size_t found2 = file_name.find ("/", found1);
    std::string host = file_name.substr (found1 ,found2 - found1);
    std::string get_str = file_name.substr (found2);
    //std::size_t found3 = file_name.find ("/edit");
    //std::string get_str = file_name.substr (found2,  found3 -found2  );

    //char request[512] = "GET /index.html HTTP/1.0\n"
    char request[512] = "GET ";
    //strcat(request, file_name.c_str ());
    strcat(request, get_str.c_str ());
    strcat(request, " HTTP/1.0\n");
    strcat(request, "From: Ayvah_br\nUser-Agent: Ayvah_aa\nHost: ");
    strcat(request,host.c_str ());
    strcat(request,	"\nConnection: keep-alive\n\n");

    //Get a socket
    if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr,"Couldn't get a socket.\n");
        return -1;
    } else {
        fprintf(stderr,"Got a socket.\n");
    }

    //book uses bzero which my man pages say is deprecated
    //the man page said to use memset instead. :-)
    memset(&servaddr,0,sizeof(servaddr));

    //get address for google.com
    if((hp = gethostbyname(host.c_str())) == NULL) {
        close(sock_id);
        fprintf(stderr,"Couldn't get an address.\n");
        return -1;
    } else {
        fprintf(stderr,"Got an address.\n");
    }

    //bcopy is deprecated also, using memcpy instead
    memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);

    //fill int port number and type
    servaddr.sin_port = htons(80);
    //servaddr.sin_port = htons(443);
    //servaddr.sin_port = htons(8243);
    servaddr.sin_family = AF_INET;

    //make the connection
    if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        fprintf(stderr, "Couldn't connect.\n");
        close(sock_id);
        return -1;
    } else {
        fprintf(stderr,"Got a connection!!!\n");
    }

    //NOW THE HTTP PART!!!
    //send the request
//    ::send(sock_id,request,strlen(request), 0);
    write(sock_id,request,strlen(request));

    //read the response
    bool isZipStarted = false;
    do {
        //msglen =recv(sock_id, message, 10*1024, 0);
        msglen = read(sock_id,message,10*1024);
        if(msglen == -1){
            close(sock_id);
            return -1;
        }
        if (msglen > 0) {
            int zipBegin = 0x04034b50;
            total_msglen += msglen;
            if (!isZipStarted) {
                for(int i = 0; i < msglen; i++){
                   if(memcmp(&message[i],&zipBegin,4) == 0) {
                       isZipStarted = true;
                       file.write((char *)&message[i], msglen - i);
                       break;
                   }
                }
            } else
                file.write((char *)&message, msglen);
        }
    }while (msglen > 0);

    printf("response is %d bytes long\n", total_msglen);

    //print the reasponse
    //printf("%s", message);
    close(sock_id);
    return total_msglen;
}

bool    unzip(std::string http_zip_file)
{
    return false;
}

