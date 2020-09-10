#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <err.h>
#include <cstring>
#include "PathProcess.h"
#include "TLS.h"


int Create_Sock(string ip)
{
    int sock,clie,one = 1;
    struct sockaddr_in server_add;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) err(1, "can't open socket");
    setsockopt(sock ,SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    int port = 8080;
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr(ip.c_str());
    server_add.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server_add, sizeof(server_add)) == -1)
    {
        close(sock);
        err(1, "Can't bind");
    }
    listen(sock,1);

    return sock;
}

int main(int argv, char* argc[])
{

    if (argv > 1)
    {
        int sock, configure_pass = 0;
        string CA, server_key,server_crt;
        //configure_pass is 4 means all arugment configure are configure
        for (int i = 0 ;i < argv; i++)
        {
            if (strcmp("-IP",argc[i]) == 0)
            {
                if (i + 1 >= argv) break;
                sock = Create_Sock(argc[i+1]);
                configure_pass++;
                i++;
                continue;
            }
            if (strcmp("-CA",argc[i]) == 0)
            {
                if (i + 1 >= argv) break;
                CA = argc[i+1];
                configure_pass++;
                continue;
            }
            if (strcmp("-SERVER_KEY",argc[i]) == 0)
            {
                if (i + 1 >= argv) break;
                server_key = argc[i+1];
                configure_pass++;
                i++;
                continue;
            }
            if (strcmp("-SERVER_CRT",argc[i]) == 0)
            {
                if (i + 1 >= argv) break;
                server_crt = argc[i+1];
                configure_pass++;
                i++;
                continue;
            }
        }
        if (configure_pass != 4) 
        {
            //arugment configure are failure
            cout << "Arugment configure are failure." << endl;
            cout << "-IP \"ip address\" -CA \"CA path\" -SERVER_KEY \"server key path\" -SERVER_CRT \"server crt path\"" << endl;
            return -1;
        }
        TLS channel(server_key, server_crt, CA);
        channel.Configure();
        char buf[5000] = "\0";
        string filename;
        while (1)
        {
            SSL *ssl;
            struct sockaddr_in client_add;
            socklen_t len = sizeof(client_add);
            int client_fd = accept(sock, (struct sockaddr *) &client_add, &len);
            int pos;
            PathProcess file_analize;
            std:string recv , file_content;
            if (client_fd == -1) 
            {
                perror("Can't accept");
                exit(1);
            }
            ssl = SSL_new(channel.Get());
            if (ssl <= 0)
            {
                ERR_print_errors_fp(stderr);
                exit(1);            
            }

            SSL_set_fd(ssl, client_fd);
                
            if (SSL_accept(ssl) <= 0)
            {
                ERR_print_errors_fp(stderr);
                exit(1);
            }
            cout << "got connection" << endl;
            memset(buf,'\0',sizeof(buf));
            SSL_read(ssl, buf, sizeof(buf)-1);
            //analize get method
            stringstream ss;
            ss << buf;
            ss >> recv >> filename;
            ss.clear();
            file_analize.set_path(filename);
            if (file_analize.is_directory())
            {
                string path, temp = "";
                vector<string> list = file_analize.file_list();
                path = file_analize.get_path();
                string head = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                for (vector<string>::iterator it = list.begin();it != list.end(); it++)
                {
                    filename = path + *it;
                    temp += "<font size =\"5\"><a href=\"" + filename + "\" >" + *it + "</a><br></font>";
                }
                head += temp;
                SSL_write(ssl,head.c_str(),head.length());
            }
            else if (file_analize.is_file())
            {
                string file_content = file_analize.open_file();
                string head = "HTTP/1.1 200 OK\r\nContent-Type: Application/octet-stream\r\nContent-Disposition: attachment; "
                "filename=" + file_analize.get_name() + "\r\n\r\n";
                SSL_write(ssl,head.c_str(),head.length());
                SSL_write(ssl,file_content.c_str(),file_content.length());
            }
            else
            {
                //file path error
                string head = "HTTP/1.1 404\r\n";
                SSL_write(ssl,head.c_str(),head.length());
            }
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
        }
        close(sock);
    }
    return 0;
}

