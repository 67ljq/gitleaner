<!-- TOC -->

- [socket 编程](#socket-编程)
    - [设置文件描述符属性](#设置文件描述符属性)
    - [socket 收发](#socket-收发)
    - [tcp socke 常用选项](#tcp-socke-常用选项)
- [reference](#reference)

<!-- /TOC -->

# socket 编程

## 设置文件描述符属性

  1. 获取属性flags

     flags = fcntl(fd,F_GETFL,0);

  2. 设置属性

     fcntl(fd,F_SETFL,flags);
  
  3. 设置非阻塞

     flags = fcntl(fd,F_GETFL,0);

     flags |= O_NONBLOCK;
     
     fcntl(fd,F_SETFL,flags);

     可以合并为一条：

     fcntl(socket,F_SETFL,fcntl(socket,F_GETFL)|O_NONBLOCK);

  4. 取消非阻塞

     flags = fcntl(fd,F_GETFL,0);

     flags &= ~O_NONBLOCK;
     
     fcntl(fd,F_SETFL,flags);

     或者 fcntl(socket,F_SETFL,fcntl(socket,F_GETFL) & ~O_NONBLOCK);

## socket 收发

 - read, recv 和 recvfrom

        ssize_t read(int fd, void *buf, size_t count);
        ssize_t recv(int sockfd, void *buf, size_t len, int flags);
        ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);

  read 从文件描述符fd读取count个字节到buf，它是阻塞读取的，对于网络编程，socket同样也是一个描述符。

  recv 用于接收远端发送给sockfd的数据，flags是一些标志选项，一般为0，代表阻塞接收，设置为 MSG_DONTWAIT 
  表示非阻塞发送，还有一些其他选项，可以知道，设置为0时与read的功能是一样的。

  而 recvfrom 顾名思义除了接收数据外，还接收了对端地址。

  tcp因为是面向连接的，如果需要知道对端的地址，直接在accept时候获取就可以了，不必每次接收都用 recvform,而udp因为是无连接的，
  如果要获取对端地址，就需要 recvfrom 了。

- write, send 和 sendto

        ssize_t write(int fd, const void *buf, size_t count);
        ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);

  和上面的read等函数类似：
  
  write 把buf中的count个字节到文件描述符fd，返回实际写入的字节数。

  send 用于把buf中的len个字节发送到和sockfd建立连接的对端端口，flags是一些标志选项，一般为0，代表阻塞发送，设置为 MSG_DONTWAIT 
  表示非阻塞发送，还有一些其他选项，可以知道，设置为0时与write的功能是一样的。

  sendto 与send相比，还指定了要发送出去的目的地。一般用于没有建立连接的数据报协议udp，当然tcp也可以用这个函数。
  
## tcp socke 常用选项
 - SO_REUSEADDR 和 SO_REUSEPORT

   顾名思义，这两个属性主要是用来重用ip和port以提高资源利用的，什么意思呢？主要和tcp的4次挥手最后的 TIME_WAIT 状态相关。

   ![time_wait](./time_wait.png) 
   
   ![time_wait_err](./time_wait_err.png)

   如上图，tcp为了实现可靠性，有重传机制，4次挥手中先调用close()的client会进入TIME_WAIT状态，而server在第3次发起FIN关闭连接，要收到client的ACK才能正确的关闭连接。如果一段时间没有收到则进行重发。对应client端，收到FIN包后回复ACK响应，为了保证可靠性，不能立马进入关闭状态，因为ACK包可能会丢失，所以必须维持2MSL时间(MSL是报文在网络中最大的存活时间，一来一回就2MSL)，如果在这期间收到了重传的FIN包，会重新等待2MSL。所以TIME_WAIT状态强保证了最后关闭的可靠性。

   如果是图2的情况呢，这样tcp会回复一个RST包，server会认为是错误。同样的，如果没有TIME_WAIT直接CLOSED状态，那么在上一次重传就会返回RST包了。


# reference 
 - [time_wait状态产生的原因，危害，如何避免](https://blog.csdn.net/u013616945/article/details/77510925?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight)
 - [send/sendto和recv/recvfrom各自的区别](https://blog.csdn.net/Superman___007/article/details/82926522)