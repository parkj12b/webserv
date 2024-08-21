/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:46 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 10:56:50 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>

#define BUFFER_SIZE 30
#define PORT 8000

void    errorHandler(std::string message)
{
    std::cerr<<message<<"\n";
    exit(1);
}

void    init(int &serverFd, int &kq)
{
    struct sockaddr_in servAdr;
    int option;

    serverFd = socket(PF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
        errorHandler("socket error.");
    option = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(PORT);
    if (bind(serverFd, (struct sockaddr *)&servAdr, sizeof(servAdr)) < 0)
        errorHandler("bind error.");
    if (listen(serverFd, 5) < 0)
        errorHandler("listen error.");
    kq = kqueue();
    if (kq < 0)
        errorHandler("kqueue error.");
}

void    plusEvent(std::vector<struct kevent> &fdList, uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent   temp;

    EV_SET(&temp, fd, filter, flags, fflags, data, udata);
    fdList.push_back(temp);  //temp를 복사해서 저장을 함
}

void    plusClient(std::vector<struct kevent> &fdList, int serverFd)
{
    int clntFd;
    struct sockaddr_in clntAdr;
    socklen_t adrSize;

    adrSize = sizeof(clntAdr);
    clntFd = accept(serverFd, (struct sockaddr*)&clntAdr, &adrSize);
    if (clntFd < 0)
        errorHandler("accept error.");
    plusEvent(fdList, clntFd, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 50000, 0);
    plusEvent(fdList, clntFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    // plusEvent(fdList, clntFd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    //비동기 I/O가 걸릴 수 있다고 생각 되나 우리는 read를 다하고 나서 write를 하기 때문에 문제가 없을 것이라고 생각됨
}

int main(void)
{
    std::vector<struct kevent>  fdList;
    std::vector<struct kevent>::iterator    it;
    struct kevent   store[5];
    int serverFd;
    int kq;
    int count;
    int readSize;
    char    buffer[BUFFER_SIZE];
    const char *temp = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 1970\n\n";

    init(serverFd, kq);
    plusEvent(fdList, serverFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    while (1)
    {
        count = kevent(kq, &fdList[0], fdList.size(), store, 5, NULL);
        if (count < 0)
            errorHandler("kevent error.");
        fdList.clear();
        for (int i = 0; i < count; i++)
        {
            if (store[i].ident == serverFd)
            {
                if (store[i].flags == EV_ERROR)
                    errorHandler("server socket error.");
                if (store[i].filter == EVFILT_READ)
                    plusClient(fdList, serverFd);
            }
            else
            {
                if (store[i].flags == EV_ERROR)
                    errorHandler("client error.");
                else if (store[i].filter == EVFILT_READ)
                {
                    readSize = read(store[i].ident, buffer, BUFFER_SIZE);
                    if (readSize <= 0)
                    {
                        const char *temp = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                        write(store[i].ident, &temp, strlen(temp));
                        if (readSize < 0)
                            std::cerr<<store[i].ident<<"FD client reading error.\n";
                        // EV_SET(&store[i], store[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        // kevent(kq, &store[i], 1, NULL, 0, NULL);
                    }
                    else
                    {
                        buffer[readSize] = '\0';
                        write(1, buffer, readSize);
                    }
                    if (readSize < BUFFER_SIZE)
                    {
                        write(store[i].ident, temp, strlen(temp));
                        int fd = open("./index.html", O_RDONLY);
                        while (1)
                        {
                            readSize = read(fd, buffer, BUFFER_SIZE);
                            if (readSize <= 0)
                                break ;
                            write(store[i].ident, buffer, readSize);
                        }
                        EV_SET(&store[i], store[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        kevent(kq, &store[i], 1, NULL, 0, NULL);
                        close(store[i].ident);
                    }
                }
                // else if (store[i].filter == EVFILT_WRITE)
                // {
                //     if (store[i].ident != serverFd)
                //     {
                //         if (store[i].flags == EV_ERROR)
                //             errorHandler("client error.");
                //     }
                // }
            }
        }
    }
    return (0);
}
