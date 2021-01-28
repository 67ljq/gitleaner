# c++

​		c++这门语言已经有几十个年头了， 

## 一、c++98、03

### 1.1、new/delete

- 有了 malloc/free 为什么还要用 new/delete？

  ​		因为malloc只是申请内存，而类还需要调用构造进行初始化，new不仅向系统申请内存，还调用相应的构造函数。对应的，free只是释放内存空间，delete会先调用析构去初始化然后才释放内存。

- 怎么判读new是否成功？

  ​		不能简单的判读返回值为0，因为new如果申请失败会抛出`bad_alloc`异常，只需要捕获即可。

- 可以new结构体吗？

  ​		可以，结构体其实也是类，只是成员变量的默认权限是public，构造和析构可以理解为默认的空函数。

### 1.2、RAII

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

```c++
#include <memcpy>
int main()
{
	std::auto_ptr<int> p(new int(5));
	printf("*p=%d\n", *p);
    return 0;
}
```

​		auto_ptr现在不推荐使用的原因是它本身的设计缺陷，它被设计为独占的，就是说如果要赋值给另一个变量的话它会转移控制权，而这个在函数调用时是非常致命的。

```c++
#include <memcpy>
void testAutoPtr(std::auto_ptr<int> p){ printf("*p=%d\n", *p);}
int main()
{
	std::auto_ptr<int> p1(new int(5));
    testAutoPtr(p1);
    printf("p1=%d\n", p1);		// 0
	printf("*p1=%d\n", *p1);	// 错误，p1内部管理的指针已经被转移
    return 0;
}
```

​		因为其独占性，所有它也不能同时将一个指针赋值给两个auto_ptr

```c++
int* p = new int(90);
std::auto_ptr<int> p1(p);
std::auto_ptr<int> p2(p);// 会释放两次
```

​		然后它还被设计为可以再次转变回纯粹的指针，这个我觉得是很不合理的，很容易就混乱导致错误。

```c++
std::auto_ptr<int> p(new int(20));
int *ptr = p.get();	// 获取了内部的指针，同时内部还保留管理权，ptr不能手动释放
int *ptr1 = p.release();	// 释放了内部了管理权，需要手动释放ptr1
delete ptr1;
```

​		auto_ptr有其致命的缺陷，而boost渐渐发展了功能更加明确并且安全性更强的几个智能指针，后来被参考纳入了标准形成了shared_ptr等。

#### 2.14、shared_ptr

​		顾名思义，shared_ptr是共享型智能指针。即多个shared_ptr变量可以托管同一个new出来的指针变量。

```c++
class A
{
public:
    int i;
    A(int n) : i(n){};
    ~A() { cout << i << " "
                << "destructed" << endl; }
    void print() { cout << i << endl; }
};

void func(std::shared_ptr<A> ptr)
{
    ptr->print();
}

int main()
{
    cout << "================test================" << endl;

    std::shared_ptr<A> ptr1(new A(25));
    std::shared_ptr<A> ptr2(ptr1);
    std::shared_ptr<A> ptr3(new A(66));
    std::shared_ptr<A> ptr4;

    ptr4 = ptr1;
    func(ptr4);
    ptr4 = ptr3;
    func(ptr4);

    return 0;
}
```

​		输出结果为：

```c++
================test================
25
66
66 destructed
25 destructed
```

​		可以看到，调用func函数结束后实参析构并没有直接释放指针，shared_ptr内部会维护一个reference引用计数，代表有多少个变量同时托管这个指针，因为main函数里的ptr4仍然在托管着，所以func的ptr析构并没有释放内存，而是在程序运行完所以reference计数为0后释放。同时也不用思考因为独占性而要时刻注意指针转移的问题。

​		这个是很符合之前没有用RAII的编程思维的，当资源同时被多个变量托管时，只有全部变量都消亡时才释放资源。 不仅shared_ptr理应如此，绝大多数情况下用RAII来托管资源的都建议如此。

​		当然，现在都用传参都用引用和右值引用，但相较于auto_ptr，其安全性，逻辑性好太多，都建议完全不要用auto_ptr。

#### 2.15、unique_ptr

​		unique_ptr与shared_ptr对应，独享指针的管理权。unique_ptr可以说是auto_ptr的改进，之前的分析了解了auto_ptr最大的坑点是拷贝赋值的时候原本变量就没有管理权了，unique_ptr则做了限制，删除了所有的复制构造、赋值函数，这样如果你需要指针在多个变量，多个函数间传递，那你就用shared_ptr， 如果你只需要做临时的内存申请使用，unique_ptr是很好的辅助模板。

​		其实unique_ptr也是支持移动，是用c++新增的另一个语义：移动语义，使用std::move,或者使用release()，但这样是你必须手动调用的，这意味在自己清楚自己在做什么，一般而言，你不可能在自己知道自己已经转移了管理权到另一个变量后，又使用原本的变量。

​		unique_ptr和auto_ptr相似，其重要的几个函数是差不多的：

| function     | desc                                                      |
| ------------ | --------------------------------------------------------- |
| get()        | 返回管理的指针，自身仍然保留管理权                        |
| release()    | 释放管理权但不delete原指针，同时返回管理的指针            |
| reset(T ptr) | 释放原指针申请的内存,并管理新的ptr指向的内存，ptr可以为空 |

### 2.2、move semantic and perfect forward

​		右移语义和完美转发，先说右移语义，这是一个c++11新增的语义`&&`，称之为右值引用，我们知道单个`&`有取地址和引用的意思，引用的目的是为了减少临时变量的拷贝，减少时间开销等，这个我们都知道了。右值引用也是同样的目的，其实`&`只是针对左值的效率优化，`&&`是针对右值的效率优化而提出的。

​		看下面的例子：

```c++

```

​		为了了解这个语义，需要先了解左值和右值的概念，

左值：非临时存在的变量或表达式，在语法上可以取到其地址，在编程上看来是有一定时间片段存在的对象。

右值：不能取到其变量或者表达式的地址，在编程上看来是编译器产生的某一时刻的临时对象。

### 2.3、类型转换

​		c++新增了几种类型的转换，以替代c风格的转换，原因是c风格的转换有太多的不安全性。

​		类型转换其实就是告诉编译器应该重新以什么样的形式来解释原先的变量。大概有几种情况。

- 都是数值型，但占用的内存大小不一样。

  `char c = 8;int a = (int)c;`原本c是占用1个字节的char变量，赋值给a，因为a是4个字节，所以编译器会把c前补3个字节的0，赋值为a，因为精度上没有损失，所以不加强转也不会报错或警告，属于隐式转换。

​		反过来就不一样了，`int a = 8;char a = (char)c;`

#### 2.3.1、const_cast

​		`const_cast<type_id> (expression)`

​		用于把指向const或者volatile类型的变量的指针转换为指向非const类型的变量的指针。目的就是为了可以修改变量，注意，这意味在它在其他方面会有很大的限制，比如不能转变类型。

​		这个比较简单，看例子跑一遍就懂。

```c++
int main()
{
    cout << "================test================" << endl;
    cout << "============ c语言的转换" << endl;
    const int num = 15;
    int *pNum = (int *)&num; // &num 是 const int * 类型指针，强转之后通过pNum修改num的值
    *pNum = 25;
    cout << "num: " << num << ", *pNum: " << *pNum << endl; // num初始化值是const，所以是不会改变的

    cout << "============ c语言的转换，类似于函数传参" << endl;
    int number1 = 15;
    const int number = number1;
    int *pnumber = (int *)&number; // &num 是 const int * 类型指针，强转之后通过pNum修改num的值
    *pnumber = 35;
    cout << "number: " << number << ", *pnumber: " << *pnumber << endl;

    cout << endl;
    cout << "============ c++ const_cast，指针和引用都可以" << endl;
    int j = 0;
    const int i = j;
    int &k = const_cast<int &>(i);
    k++;
    cout << "i: " << i << endl;

    string a = "123";
    char *p = const_cast<char *>(a.c_str());
    strcpy(p, "abc");
    cout << "a.c_str(): " << a.c_str() << endl;

    return 0;
}
```

​		运行结果为：

```
================test================
============ c语言的转换
num: 15, *pNum: 25
============ c语言的转换，类似于函数传参
number: 35, *pnumber: 35

============ c++ const_cast，指针和引用都可以
i: 1
a.c_str(): abc
```

#### 2.3.2、static_cast

​		static_cast <type-id>( expression )

​		静态转换，这是在编译的时候编译器检查的转换，与之对应的是动态转换(dynamic_cast),它会在运行时转换。

### reference

- [C++ lock_guard 互斥锁](https://www.cnblogs.com/ybqjymy/p/12357617.html)
- [[c++11]多线程编程(五)——unique_lock](https://www.jianshu.com/p/34d219380d90)
- [C++11 shared_ptr（智能指针）详解](http://c.biancheng.net/view/430.html)
- [百度百科const_cast](https://baike.baidu.com/item/const_cast/4473154?fr=aladdin)
- [const_cast的用法与测试](https://www.cnblogs.com/qiuchangyong/p/10028765.html)
- [百度百科static_cast](https://baike.baidu.com/item/static_cast/4472966?fr=aladdin)

## 三、c++14

## 四、c++20

## 五、参考

