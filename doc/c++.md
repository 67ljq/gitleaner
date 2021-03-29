# c++



[TOC]

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

### 1.3、虚函数表

​		虚函数表可以说是c++实现多台的基石，理解虚函数表能更深刻的认识多态性的本质。

***以下是转载：***[C++中的虚函数以及虚函数表](https://www.cnblogs.com/yinbiao/p/10987640.html)

#### 1.31、虚函数的定义

​		被virtual关键字修饰的成员函数，目的是为了实现多态

ps：关于多态【接口和实现分离，父类指针指向子类的实例，然后通过父类指针调用子类的成员函数，这样可以让父类指针拥有多种形态，所以称之为多态】

#### 1.32、虚函数表

​		该表为一个类的虚函数的地址表，用于解决继承和覆盖的问题

​				1.拥有虚函数的类才有虚函数表

​				2.虚函数表属于类，然后类的所有对象通过虚函数表指针共享类的虚函数表

​				3.虚函数表的作用：当使用父类指针来操作子类对象时，虚函数表就像一个地图一样，指明了实际所应该调用的函数

​				4.c++编译器保证虚函数表的指针存在于对象实例中最前面的位置（为了保证在多层继承或者多重继承的情况下获得函数表的性能），这意味着我们可以通过对象实例的地址得到虚函数表，然后就可以遍历其中的虚函数指针，并且调用响应的虚函数

ps：多重继承：多个父类，多层继承：父类还存在父类

![01](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093432959-984189171.jpg)

【通过虚函数表，遍历虚函数指针，调用响应的虚函数】

```c++
#include<bits/stdc++.h>
using namespace std;
class Base
{
public:
    virtual void f()
    {
        cout << "Base::f" << endl;
    }
    virtual void g()
    {
        cout << "Base::g" << endl;
    }
    virtual void h()
    {
        cout << "Base::h" << endl;
    }

};
typedef void(*Fun)(void);
Base b;
Fun pFun = NULL;

int main()
{
    cout << "虚函数表地址：" << (int*)(&b) << endl;
    cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&b) << endl;

    //通过虚函数表调用虚函数
    pFun = (Fun)*((int*)*(int*)(&b));   // Base::f()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&b)+1);  // Base::g()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&b)+2);  // Base::h()
    pFun();
}
```

结果：

```
虚函数表地址：0x477008
虚函数表 — 第一个函数地址：0x473668
Base::f
Base::g
Base::h
```

以上为无继承情况

**1.单层继承无虚函数覆盖的情况**

![02](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093434460-1525406000.jpg)

![03](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093435475-219298640.jpg)

​		1）虚函数按照声明顺序放入表中

​		2）父类虚函数在前，子类虚函数在后

​		3）末尾点号为虚函数表的结尾标识符，在不同编译器下值不同

```c++
#include<bits/stdc++.h>
using namespace std;
class Base
{
public:
    virtual void f()
    {
        cout << "Base::f" << endl;
    }
    virtual void g()
    {
        cout << "Base::g" << endl;
    }
    virtual void h()
    {
        cout << "Base::h" << endl;
    }

};
class Base_son:public Base
{
public:
    virtual void f1()
    {
        cout << "Base_son::f1" << endl;
    }
    virtual void g1()
    {
        cout << "Base_son::g1" << endl;
    }
    virtual void h1()
    {
        cout << "Base_son::h1" << endl;
    }

};


typedef void(*Fun)(void);
Base_son d;

Fun pFun = NULL;

int main()
{
    cout << "虚函数表地址：" << (int*)(&d) << endl;
    cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&d) << endl;

    //通过虚函数表调用虚函数
    pFun = (Fun)*((int*)*(int*)(&d));   // Base::f()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+1);  // Base::g()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+2);  // Base::h()
    pFun();

    pFun =(Fun)*((int*)*(int*)(&d)+3);  // Base_son::f1()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+4);  // Base_son::g1()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+5);  // Base_son::h1()
    pFun();

    return 0;
}
```

结果：

```
虚函数表地址：0x477008
虚函数表 — 第一个函数地址：0x473668
Base::f
Base::g
Base::h
Base_son::f1
Base_son::g1
Base_son::h1
```

**2.单层继承有虚函数覆盖的情况**

![04](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093436704-331074241.jpg)

![05](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093437718-1593637875.jpg)

1）覆盖的f()函数被放到了虚函数表中原父类虚函数的位置

2）没有被覆盖的函数没有变化

```c++
#include<bits/stdc++.h>
using namespace std;
class Base
{
public:
    virtual void f()
    {
        cout << "Base::f" << endl;
    }
    virtual void g()
    {
        cout << "Base::g" << endl;
    }
    virtual void h()
    {
        cout << "Base::h" << endl;
    }

};
class Base_son:public Base
{
public:
    virtual void f()
    {
        cout << "Base_son::f" << endl;
    }
    virtual void g1()
    {
        cout << "Base_son::g1" << endl;
    }
    virtual void h1()
    {
        cout << "Base_son::h1" << endl;
    }

};


typedef void(*Fun)(void);
Base_son d;

Fun pFun = NULL;

int main()
{
    cout << "虚函数表地址：" << (int*)(&d) << endl;
    cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&d) << endl;

    //通过虚函数表调用虚函数
    pFun = (Fun)*((int*)*(int*)(&d));   // Base_son::f()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+1);  // Base::g()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+2);  // Base::h()
    pFun();

    pFun =(Fun)*((int*)*(int*)(&d)+3);  // Base_son::g1()
    pFun();
    pFun =(Fun)*((int*)*(int*)(&d)+4);  // Base_son::h1()
    pFun();

    return 0;
}
```

结果：

```
虚函数表地址：0x477008
虚函数表 — 第一个函数地址：0x473650
Base_son::f
Base::g
Base::h
Base_son::g1
Base_son::h1
```

**通过父类指针指向子类实例，子类覆盖父类方法，然后调用子类的方法，这样就实现了多态**

```
Base *b=new Base_son(); b->f();
```

**3.多重继承无虚函数覆盖**

![06](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093438873-1057072047.jpg)

![07](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093440173-2022399978.jpg)

1）每个父类都有自己的虚函数表

2）子类的虚函数被放到第一个父类的虚函数表中

**这样做是为了解决不同的父类类型指针指向同一个子类实例，而能够调用到实际的函数**

**4.多重继承存在虚函数覆盖**

![08](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093441346-693434715.jpg)

![09](https://img2018.cnblogs.com/blog/1301290/201906/1301290-20190607093442410-190647927.jpg)

1）父类虚函数表中被覆盖的虚函数全部被替换成了子类的覆盖虚函数

**这样我们就通过父类指向子类从而访问子类的f()了**

```c++
Derive d;
Base1 *b1 = &d;
Base2 *b2 = &d;
Base3 *b3 = &d;
b1->f(); //Derive::f()
b2->f(); //Derive::f()
b3->f(); //Derive::f()
 
b1->g(); //Base1::g()
b2->g(); //Base2::g()
b3->g(); //Base3::g()
```

**使用虚函数表可以做一些违反c++语义的事情：**

**1）通过父类指针访问子类自己的虚函数**

子类的虚函数X在父类中没有，所以子类的虚函数X没有覆盖父类的虚函数，但是如果我们通过父类的指针来访问子类自己的虚函数的编译器会报错

```
Base1 *b1 = new Derive();
b1->f1();  //编译出错
```

**但是我们通过虚函数表可以做到这种违背C++语义的事情：使用父类指针访问子类自己的虚函数**

**2）访问父类non-public的虚函数**

如果父类的虚函数是private或protected的，但是这些feipublic的父类虚函数同样会存在于虚函数表中，所以我们可以通过访问虚函数表访问到这些虚函数

附上多重继承有虚函数覆盖的样例代码：

```c++
#include <iostream>
using namespace std;
 
class Base1 {
public:
            virtual void f() { cout << "Base1::f" << endl; }
            virtual void g() { cout << "Base1::g" << endl; }
            virtual void h() { cout << "Base1::h" << endl; }
 
};
 
class Base2 {
public:
            virtual void f() { cout << "Base2::f" << endl; }
            virtual void g() { cout << "Base2::g" << endl; }
            virtual void h() { cout << "Base2::h" << endl; }
};
 
class Base3 {
public:
            virtual void f() { cout << "Base3::f" << endl; }
            virtual void g() { cout << "Base3::g" << endl; }
            virtual void h() { cout << "Base3::h" << endl; }
};
 
class Derive : public Base1, public Base2, public Base3 {
public:
            virtual void f() { cout << "Derive::f" << endl; }
            virtual void g1() { cout << "Derive::g1" << endl; }
};
 
typedef void(*Fun)(void);
 
int main()
{
            Fun pFun = NULL;
 
            Derive d;
            int** pVtab = (int**)&d;
 
            //Base1's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+0);
            pFun = (Fun)pVtab[0][0];
            pFun();
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+1);
            pFun = (Fun)pVtab[0][1];
            pFun();
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+2);
            pFun = (Fun)pVtab[0][2];
            pFun();
 
            //Derive's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+3);
            pFun = (Fun)pVtab[0][3];
            pFun();
 
            //The tail of the vtable
            pFun = (Fun)pVtab[0][4];
            cout<<pFun<<endl;
 
            //Base2's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+0);
            pFun = (Fun)pVtab[1][0];
            pFun();
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+1);
            pFun = (Fun)pVtab[1][1];
            pFun();
 
            pFun = (Fun)pVtab[1][2];
            pFun();
 
            //The tail of the vtable
            pFun = (Fun)pVtab[1][3];
            cout<<pFun<<endl;
 
            //Base3's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+0);
            pFun = (Fun)pVtab[2][0];
            pFun();
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+1);
            pFun = (Fun)pVtab[2][1];
            pFun();
 
            pFun = (Fun)pVtab[2][2];
            pFun();
 
            //The tail of the vtable
            pFun = (Fun)pVtab[2][3];
            cout<<pFun<<endl;
 
            return 0;
}
```

#### 1.33、关于虚函数和普通函数

​		**1.类中的虚函数是动态生成的，由虚函数表的指向进行访问，不为类的对象分配内存，没有虚函数表，就无法访问虚函数**

​		**2.类中的普通函数静态生成，不为类的对象分配内存也可访问**

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

### 2.2、lvalue,rvalue,move semantic and perfect forward

​		右移语义和完美转发，先说右移语义，右是右值的意思，和左值相对应，它是临时的，即将消亡的值；移是移动的意思，和拷贝是相对的，拷贝是很耗时的， 而剪切就快得多。所以它就是把这边已经不用的值剪切移动到另一个空间或者说堆栈区。

​		其实一开始c语言的函数调用传参的时候是复制一份变量到被调用函数的栈里，此时优化的话只是针对结构体、联合体这样的参数进行优化，用一个指向它们的指针做参数，这样就只是复制一个4位或8位的变量，减少了需要拷贝的空间。之后就是c++引入的引用变量`&`，它是另一个变量的别名，**在语法层面上说**相当于快捷方式，连复制另一份实体都不用了，这样效率的提升是显而易见的，而且具有安全性，指针可以传null，这是很危险的，而引用必须对应实体。**其实在编译层面说引用也是占用内存的，实现方式类似指针，这里就不在细说。**

​		在之后就是这个右移语义了，它的基础是右值引用`&&`，它其实是对之前的应用变量`&`的补充，因为你仔细分析会发现，引用变量`&`定义的时候就要初始化，要有实体，不能像这样`int &a = 99`，也不能像这样`int b = 65;int &a=(b+10)*99`,但在运行的时候其实会有一个临时变量`int tmp = 99`或者`int tmp=(b+10)*99`，它是帮助临时存放运算结果，之后就会消亡，不能取地址，这种值就是右值，右值引用就是为了进一步利用这个值，提示效率。

#### 2.2.1、 lvalue,rvalue

​		接下来具体说说左值和右值的区别：

```c++
int a;
int b;

a = 3;
b = 4;
a = b;
b = a+6;

// 以下写法不合法。
3 = a;
a+b = 4;
```

左值：非临时存在的变量或表达式，在语法上可以取到其地址，在编程上看来是有一定时间片段存在的对象。

右值：不能取到其变量或者表达式的地址，在编程上看来是编译器产生的某一时刻的临时对象。

#### 2.2.2、move semantic

- ***这里是转载		[c++11 中的 move 与 forward](https://www.cnblogs.com/catch/p/3507883.html)***

​		关于 lvaue 和 rvalue，在 c++11 以前存在一个有趣的现象：T&  指向 lvalue (左传引用)， const T& 既可以指向 lvalue 也可以指向 rvalue。但却没有一种引用类型，可以限制为只指向 rvalue。这乍看起来好像也不是很大的问题，但实际与看起来不一样，右值引用的缺失有时严重限制了我们在某些情况下，写出更高效的代码。举个粟子，假设我们有一个类，它包含了一些资源：

```
class holder
{
     public:
 
          holder()
          {
               resource_ = new Resource();
          }
          ~holder()
          {
               delete resource_;
          }

          holder(const holder& other)
          {
                resource_ = new Resource(*other.resource_);
          }

          holder(holder& other)
          {
                resource_ = new Resource(*other.resource_);
          }

          holder& operator=(const holder& other)
          {
                delete resource_;
                resource_ = new Resource(*other.resource_);                
                return *this;
          }
           holder& operator=(holder& other)
          {
                delete resource_;
                resource_ = new Resource(*other.resource_);                
                return *this;
          }
          private:

               Resource* resource_;
};
```

​		这是个 RAII 类，构造函数与析构函数分别负责资源的获取与释放，因此也相应处理了拷贝构造函数 (copy constructor) 和重载赋值操作符 (assignment operator)，现在假设我们这样来使用这个类。

```
// 假设存在如下一个函数，返回值为 holder 类型的临时变量
holder get_holder() { return holder(); }

holder h;
foo(h);
h = get_holder();
```

​		理想情况下（不考虑返回值优化等因素)，这一小段代码的最后一条语句做了如下三件事情：

​			1)  销毁 h 中的资源。

​			2)  拷由 get_holder() 返回的资源。

​			3)  销毁 get_holder() 返回的资源。

​		显然我们可以发现这些事情中有些是不必要的：假如我们可以直接交换 h 中的资源与 get_holder() 返回的对象中的资源，那我们就可以直接省略掉第二步中的拷贝动作了。而这里之所以交换能达到相同的效果，是因为 get_holder() 返回的是临时的变量，是个 rvalue，它的生命周期通常来说很短，具体在这里，就是赋值语句完成之后，任何人都没法再引用该 rvalue，它马上就要被销毁了，它所包含的资源也无法再被访问。而如果是像下面这样的用法，我们显然不可以直接交换两者的资源：

```
holder h1;
holder h2;

h1 = h2;

foo(h2);
```

​		因为 h2 是个 lvalue，它的生命周期较长，在赋值语句结束之后，变量仍然存在，还有可能要被别的地方使用。因此，rvalue 的短生命周期给我们提供了在某些情况优化代码的可能。但这种可能在 c++11 以前是没法利用到的，因为我们没法在代码中对 rvalue 区别对待：在函数体中，程序员无法分辨传进来的参数到底是不是 rvalue，我们缺少一个 rvalue 的标记。

​		回忆一下，T& 指向的是 lvalue，而 const T& 指向的，却可能是 lvalue 或 rvalue，我们没有任何方式能够确认当前参数是不是 rvalue！为了解决这个问题，c++11 中引入了一个新的引用类型: some_type_t &&，这种引用指向的变量是个 rvalue， 有了这个引用类型，我们前面提到的问题就迎刃而解了。

```
class holder
{
     public:
 
          holder()
          {
               resource_ = new Resource();
          }
          ~holder()
          {
               if (resource_) delete resource_;
          }

          holder(const holder& other)
          {
                resource_ = new Resource(*other.resource_);
          }

          holder(holder& other)
          {
                resource_ = new Resource(*other.resource_);
          }
          
          holder(holder&& other)
          {
                resource_ = other.resource_;
                other.resource_ = NULL;
          }

          holder& operator=(const holder& other)
          {
                delete resource_;
                resource_ = new Resource(*other.resource_);　　　　　　　　　 return *this;
          }

          holder& operator=(holder& other)
          {
                delete resource_;
                resource_ = new Resource(*other.resource_);                return *this;
          }

          holder& operator=(holder&& other)
          {
                std::swap(resource_, other.resource_);                return *this;
          }

          private:

               Resource* resource_;
};
```

​		因为有了右值引用，当我们再写如下代码的时候：

```
holder h1;
holder h2;

h1 = h2; // 调用operator(holder&);
h1 = get_holder(); // 调用operator(holder&&)
```

​		编译器就能根据当前参数的类型选择相应的函数，显然后者的实现是更高效的。写到里，有的人也许会有疑问:  some_type_t&& ref  指向的是右值（右值引用），那 ref 本身在函数内是左值还是右值？具体来说就是如下代码中，第三行所调用的是 operator=(holder&) 还是 operator=(holder&&)?

```
1 holder& operator=(holder&& other)
2 {
3       holder h = other;
4       return *this;
5 }
```

​		这个问题的本质还是怎么区分 rvalue？ c++11 中对 rvalue 作了明确的定义：

```
Things that are declared as rvalue reference can be lvalues or rvalues. The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue.
```



#### 2.2.3、perfect forward

### 2.3、类型转换

​		c++新增了几种类型的转换，以替代c风格的转换，原因是c风格的转换有太多的不安全性。

​		类型转换其实就是告诉编译器应该重新以什么样的形式来解释原先的变量。大概有几种情况。

- 都是数值型，但占用的内存大小不一样。

  `char c = 8;int a = (int)c;`原本c是占用1个字节的char变量，赋值给a，因为a是4个字节，所以编译器会把c前补3个字节的0，赋值为a，因为精度上没有损失，所以不加强转也不会报错或警告，属于隐式转换。

​		反过来就不一样了，`int a = 8;char a = (char)c;`现在a是4字节的int变量，如果要赋值为1个字节的char变量，怎么赋值？所以编译器或给出错误，或给出警告。这里强转那么编译器会截断最后的1个字节，并且按照char来解释这个字节中的内容。这种情况下会发生精度损失，是很常见的情形，一般而言不会有太大的像程序挂掉的问题，但有可能因为精度损失导致得不到想要的结果。

- 父类和子类的互相转换

  父子类转换一般用于多态，但也有情况是逆向<下行>转换的。下行转换即父类指针赋值给子类指针是很不安全的，因为有可能子类扩展了一些方法和成员变量，而父类是没有，所以调用的时候会按照子类的内存分布模型去访问对应的偏移，但子类扩展的这些东西在父类的内存模型中是没有的，会造成未知错误。

- 完全不相干的类型转换

  毫无疑问，这种情况下是最不安全的，可能系统底层的代码会用到，一般而言，联合体对于上层代码而言足够了。

  c++把类型转换归类，提倡用新增的类型转换而不是原来c的，用以提高安全性。

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

​		静态转换，这是在编译的时候编译器检查的转换，与之对应的是动态转换(dynamic_cast),它会在运行时转换。static_cast提供相关联的基本类型的转换，比如数值型，带有继承关系的类的转换。它不能把类型的const，volatile属性去掉，这是`const_cast`需要做的事，也不能转换为另一个不相干的类，因为这需要从内存二进制层面来重新解释，新标准c++11把它视为另一个级别的转换，即`reinterpret_cast`。

```c++
class A
{
public:
    int i;
    A() = default;
    A(int n) : i(n){};
    ~A() { cout << i << " "
                << "destructed" << endl; }
    void print() { cout << i << endl; }
};

class B : public A
{
};

int main()
{
    cout << "================test================" << endl;
    float f = 2155558.895;
    cout << "static_cast<double>(f) = " << static_cast<double>(f) << endl;
    cout << "static_cast<long>(f) = " << static_cast<long>(f) << endl;
    cout << "static_cast<int>(f) = " << static_cast<int>(f) << endl;
    cout << "static_cast<short>(f) = " << static_cast<short>(f) << endl;
    cout << "static_cast<char>(f) = " << static_cast<char>(f) << endl;
    cout << endl;

    // char *pC = static_cast<char *>(&f); // 报错：类型转换无效C/C++
    void *p = static_cast<void *>(&f);
    char *pC = static_cast<char *>(p);
    cout << "*pC = " << pC << " *pC = " << *pC << endl;

    B b;
    A &a = b;
    // B &b1 = a;                   // 报错：无法用 "A" 类型的值初始化 "B &" 类型的引用(非常量限定)
    B &b2 = static_cast<B &>(a); // 可以进行指针或引用的下行转换

    return 0;
}
```

​		输出为：

```
================test================
static_cast<double>(f) = 2.15556e+06
static_cast<long>(f) = 2155559
static_cast<int>(f) = 2155559
static_cast<short>(f) = -32768
static_cast<char>(f) = 
```

​		这个都是在编译时编译器进行的检测，安全性大多数情况下都需要自己判断。

#### 2.3.3、dynamic_cast

​		`dynamic_cast <type-id> (expression)`

​		动态转换，它会在运行时才判断，主要是针对是否有继承关系。我们在运用多态时一般把派生类指针或引用赋值给基类的指针或引用。反过来，也有可能要恢复原来的类型，前面已经说过这样转换是相当危险的，`dynamic_cast`为我们提供了很大的便利。

```c++
class B
{
public:
    int m_iNum;
    virtual void foo() {}
};

class D : public B
{
public:
    char *m_szName[100];
};

void func(B *pb)
{
    D *pd1 = static_cast<D *>(pb);
    D *pd2 = dynamic_cast<D *>(pb);
    cout << "======== pd1:" << pd1 << " ======== pd2:" << pd2 << endl;
}

int main()
{
    cout << "================test================" << endl;

    func(new D());
    func(new B());

    return 0;
}
```

​		结果为：

```
================test================
======== pd1:0x1f80b8 ======== pd2:0x1f80b8
======== pd1:0x1f8258 ======== pd2:0
```

​		当调用`func(new D());`时，pb实际指向的是一个D类，static_cast和dynamic_cast都可以转换成功，但当调用`func(new B());`时，pb实际指向的是一个B类，static_cast在编程时就已经转换了，编译器发现pb的类型和要转换成的类型有派生关系，便直接进行了转换，而dynamic_cast则是在运行的时候判断pb是否确实指向了一个D类，**因为它的判断涉及虚函数表，而且动态性，所以它要在运行时才能判断，而且基类要有虚函数才行（有了虚函数才会有虚函数表），而且转换的对象只能是类对象的指针或引用**，它根据虚函数表判断出了pb所指向的虚函数表实际内容并非是D类的，它并没有抛出异常，而是返回了NULL。

####  2.3.4、reinterpret_cast



### reference

- [C++中的虚函数以及虚函数表](https://www.cnblogs.com/yinbiao/p/10987640.html)

- [C++ lock_guard 互斥锁](https://www.cnblogs.com/ybqjymy/p/12357617.html)

- [[c++11]多线程编程(五)——unique_lock](https://www.jianshu.com/p/34d219380d90)

- [C++11 shared_ptr（智能指针）详解](http://c.biancheng.net/view/430.html)

  

- [c++中的左值与右值]([c++中的左值与右值](https://www.cnblogs.com/catch/p/3500678.html))

- [c++11 中的 move 与 forward](https://www.cnblogs.com/catch/p/3507883.html)

  

- [百度百科const_cast](https://baike.baidu.com/item/const_cast/4473154?fr=aladdin)

- [const_cast的用法与测试](https://www.cnblogs.com/qiuchangyong/p/10028765.html)

- [百度百科static_cast](https://baike.baidu.com/item/static_cast/4472966?fr=aladdin)

- [百度百科dynamic_cast](https://baike.baidu.com/item/dynamic_cast)

## 三、c++14

## 四、c++20

## 五、参考

