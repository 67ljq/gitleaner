# c++

​		c++这门语言已经有几十个年头了， 

## 一、c++98、03

### 1.1、RAII

​		见 [2.1、RAII](#2.1、RAII)

## 二、c++11

### 2.1、RAII

​		RAII是一个概念，是resource acquisition is initialization的缩写，意思是**资源获取即初始化**。是由c++之父Bjarne Stroustrup提出的，他说：**使用局部对象来管理资源的技术称为资源获取即初始化**；这里的资源主要是指操作系统中有限的东西如内存、网络套接字等等，局部对象是指存储在栈的对象，它的生命周期是由操作系统来管理的，无需人工介入。

​		资源的使用一般经历三个步骤a.获取资源 b.使用资源 c.销毁资源，然后步骤c.销毁资源是很多程序员会遗忘的，内存泄漏是经常见到的一个致命问题。RAII，利用局部变量自动销毁的特点，可以把资源和局部变量绑定，这样**在构造的时候申请资源，在销毁析构的时候释放资源**，就可以达到自动销毁的目的。如下是一个获取文件描述符的例子

```c++
class FileHelper{
public:
    FileHelper(const char* filepath) { fp=fopen(filepath); }
    ~FileHelper() { if(fp) fclose(fp); }
    FILE* getFile() { return fp; }
private:
    FILE* fp;
}

int main(){
    fileHelper = FileHelper("./test.txt");
    FILE * fp = FileHelper.getFile();
    //...
    return 0;
}
```

​		以上的例子可以很清楚的了解RAII的内核，上面三个步骤的常被我们遗漏的c.释放资源，我们可以很舒心的将它略过，因为资源 fp 是被局部变量 fileHelper 管理，而它在函数退出的时候会调用析构，会释放它被托管的资源。

​		c++11中有多个这种类被纳入了标准库，提供现成的，不在需要我们重复造轮子。

#### 2.11、std::lock_guard

​		lock_guard是对锁资源加解锁的管理，在声明局部变量的时候提供要加锁的锁，它会在构造的时候对锁加锁，在声明周期到后析构里解锁。

```c++
#include <mutex>    /*std::mutex、 std::lock_guard*/
std::mutex m_mutex;

void func()
{
     //lock_guard 互斥锁 作用域内不可拷贝构造
     {
         std::lock_guard<std::mutex> lg(m_mutex);//加锁
         //...
     }//解锁锁
}
```

#### 2.12、std::unique_lock

​		unique_lock也是对锁资源加解锁的管理，unique_lock比lock_guard提供了更多的功能，它提供了try_lock，还可以自己解锁、加锁，灵活性更好。

```c++
class LogFile {
    std::mutex _mu;
    ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    ~LogFile() {
        f.close();
    }
    void shared_print(string msg, int id) {

        std::unique_lock<std::mutex> guard(_mu);
        //do something 1
        guard.unlock(); //临时解锁

        //do something 2

        guard.lock(); //继续上锁
        // do something 3
        f << msg << id << endl;
        cout << msg << id << endl;
        // 结束时析构guard会临时解锁
        // 这句话可要可不要，不写，析构的时候也会自动执行
        // guard.ulock();
    }
};
```

​		程序员可以更加自己需要 lock 和 unlock，这样可以减小加锁的颗粒度，而 lock_guard 是没有这个功能的。如果不想一直等待锁那就需要 pthread_mutex_trylock 了，unique_lock 也封装了，函数方法是 try_lock。

```c++
void print_block (int n, char c) {
    //unique_lock有多组构造函数, 这里std::defer_lock不设置锁状态
    std::unique_lock<std::mutex> my_lock (mtx, std::defer_lock);
    //尝试加锁, 如果加锁成功则执行
    //(适合定时执行一个job的场景, 一个线程执行就可以, 可以用更新时间戳辅助)
    if(my_lock.try_lock()){
        for (int i=0; i<n; ++i)
            std::cout << c;
        std::cout << '\n';
    }
}
```

​		提供了 lock 和 unlock 功能，那它本身是要维护锁的状态的，所以 unique_lock比lock_guard 的性能是要低的，但更灵活。

#### 2.13、auto_ptr<font color="#80000">`（c++98）`</font>

​		auto_ptr 已经不推荐使用了，因为有很大的弊端，通过分析其原理和优略性可以很快了解后面介绍的 shared_ptr 和 unique_ptr。auto_ptr 是用来管理指针指向的内存资源的，构造的时候直接传递要管理的内存即可，析构的时候会自动释放，我们无需关注。但仅仅管理还不够，就像`unique_lock`提供 try_lock 一样，它还要提供和原本的指针一样的功能，即要重载 * 操作符等。

#### 2.14、shared_ptr

#### 2.15、unique_ptr

### reference

- [C++ lock_guard 互斥锁](https://www.cnblogs.com/ybqjymy/p/12357617.html)
- [[c++11]多线程编程(五)——unique_lock](https://www.jianshu.com/p/34d219380d90)

## 三、c++14

## 四、c++20

## 五、参考

