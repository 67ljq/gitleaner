<!-- TOC -->

- [tcp socket ���](#tcp-socket-���)
    - [tcp socke ����ѡ��](#tcp-socke-����ѡ��)
- [reference](#reference)

<!-- /TOC -->

# tcp socket ���

## tcp socke ����ѡ��
 - SO_REUSEADDR �� SO_REUSEPORT

   ����˼�壬������������Ҫ����������ip��port�������Դ���õģ�ʲô��˼�أ���Ҫ��tcp��4�λ������� TIME_WAIT ״̬��ء�

   ![time_wait](./time_wait.png) 
   
   ![time_wait_err](./time_wait_err.png)

   ����ͼ��tcpΪ��ʵ�ֿɿ��ԣ����ش����ƣ�4�λ������ȵ���close()��client�����TIME_WAIT״̬����server�ڵ�3�η���FIN�ر����ӣ�Ҫ�յ�client��ACK������ȷ�Ĺر����ӡ����һ��ʱ��û���յ�������ط�����Ӧclient�ˣ��յ�FIN����ظ�ACK��Ӧ��Ϊ�˱�֤�ɿ��ԣ������������ر�״̬����ΪACK�����ܻᶪʧ�����Ա���ά��2MSLʱ��(MSL�Ǳ��������������Ĵ��ʱ�䣬һ��һ�ؾ�2MSL)����������ڼ��յ����ش���FIN���������µȴ�2MSL������TIME_WAIT״̬ǿ��֤�����رյĿɿ��ԡ�

   �����ͼ2������أ�����tcp��ظ�һ��RST����server����Ϊ�Ǵ���ͬ���ģ����û��TIME_WAITֱ��CLOSED״̬����ô����һ���ش��ͻ᷵��RST���ˡ�


# reference 
 - [time_wait״̬������ԭ��Σ������α���](https://blog.csdn.net/u013616945/article/details/77510925?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.edu_weight)