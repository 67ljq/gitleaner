<!-- TOC -->

- [socket ���](#socket-���)
    - [�����ļ�����������](#�����ļ�����������)
    - [socket �շ�](#socket-�շ�)
    - [tcp socke ����ѡ��](#tcp-socke-����ѡ��)
- [reference](#reference)

<!-- /TOC -->

# socket ���

## �����ļ�����������

  1. ��ȡ����flags

     flags = fcntl(fd,F_GETFL,0);

  2. ��������

     fcntl(fd,F_SETFL,flags);
  
  3. ���÷�����

     flags = fcntl(fd,F_GETFL,0);

     flags |= O_NONBLOCK;
     
     fcntl(fd,F_SETFL,flags);

     ���Ժϲ�Ϊһ����

     fcntl(socket,F_SETFL,fcntl(socket,F_GETFL)|O_NONBLOCK);

  4. ȡ��������

     flags = fcntl(fd,F_GETFL,0);

     flags &= ~O_NONBLOCK;
     
     fcntl(fd,F_SETFL,flags);

     ���� fcntl(socket,F_SETFL,fcntl(socket,F_GETFL) & ~O_NONBLOCK);

## socket �շ�

 - read, recv �� recvfrom

        ssize_t read(int fd, void *buf, size_t count);
        ssize_t recv(int sockfd, void *buf, size_t len, int flags);
        ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);

  read ���ļ�������fd��ȡcount���ֽڵ�buf������������ȡ�ģ����������̣�socketͬ��Ҳ��һ����������

  recv ���ڽ���Զ�˷��͸�sockfd�����ݣ�flags��һЩ��־ѡ�һ��Ϊ0�������������գ�����Ϊ MSG_DONTWAIT 
  ��ʾ���������ͣ�����һЩ����ѡ�����֪��������Ϊ0ʱ��read�Ĺ�����һ���ġ�

  �� recvfrom ����˼����˽��������⣬�������˶Զ˵�ַ��

  tcp��Ϊ���������ӵģ������Ҫ֪���Զ˵ĵ�ַ��ֱ����acceptʱ���ȡ�Ϳ����ˣ�����ÿ�ν��ն��� recvform,��udp��Ϊ�������ӵģ�
  ���Ҫ��ȡ�Զ˵�ַ������Ҫ recvfrom �ˡ�

- write, send �� sendto

        ssize_t write(int fd, const void *buf, size_t count);
        ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);

  �������read�Ⱥ������ƣ�
  
  write ��buf�е�count���ֽڵ��ļ�������fd������ʵ��д����ֽ�����

  send ���ڰ�buf�е�len���ֽڷ��͵���sockfd�������ӵĶԶ˶˿ڣ�flags��һЩ��־ѡ�һ��Ϊ0�������������ͣ�����Ϊ MSG_DONTWAIT 
  ��ʾ���������ͣ�����һЩ����ѡ�����֪��������Ϊ0ʱ��write�Ĺ�����һ���ġ�

  sendto ��send��ȣ���ָ����Ҫ���ͳ�ȥ��Ŀ�ĵء�һ������û�н������ӵ����ݱ�Э��udp����ȻtcpҲ���������������
  
## tcp socke ����ѡ��
 - SO_REUSEADDR �� SO_REUSEPORT

   ����˼�壬������������Ҫ����������ip��port�������Դ���õģ�ʲô��˼�أ���Ҫ��tcp��4�λ������� TIME_WAIT ״̬��ء�

   ![time_wait](./time_wait.png) 
   
   ![time_wait_err](./time_wait_err.png)

   ����ͼ��tcpΪ��ʵ�ֿɿ��ԣ����ش����ƣ�4�λ������ȵ���close()��client�����TIME_WAIT״̬����server�ڵ�3�η���FIN�ر����ӣ�Ҫ�յ�client��ACK������ȷ�Ĺر����ӡ����һ��ʱ��û���յ�������ط�����Ӧclient�ˣ��յ�FIN����ظ�ACK��Ӧ��Ϊ�˱�֤�ɿ��ԣ������������ر�״̬����ΪACK�����ܻᶪʧ�����Ա���ά��2MSLʱ��(MSL�Ǳ��������������Ĵ��ʱ�䣬һ��һ�ؾ�2MSL)����������ڼ��յ����ش���FIN���������µȴ�2MSL������TIME_WAIT״̬ǿ��֤�����رյĿɿ��ԡ�

   �����ͼ2������أ�����tcp��ظ�һ��RST����server����Ϊ�Ǵ���ͬ���ģ����û��TIME_WAITֱ��CLOSED״̬����ô����һ���ش��ͻ᷵��RST���ˡ�


# reference 
 - [time_wait״̬������ԭ��Σ������α���](https://blog.csdn.net/u013616945/article/details/77510925?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight)
 - [send/sendto��recv/recvfrom���Ե�����](https://blog.csdn.net/Superman___007/article/details/82926522)