# stl

​		stl是Standard Template Library的缩写，中文是标准模板库的意思，是一个高效的c++模板库。

​		模板编程是泛型编程的思想，主要作用就是代码复用。关于模板编程还用两种不同是实现方式，一种是编译器给每一个函数或类都生成一套代码，另一种是每种类型复用一个代码，然后编译时候编译器检测语义强转。

​		c++是用第一中方式实现泛型编程，比如：

```c++
template<typename T>
void swap(T &a, T &a)
{
	T c = a;
    a = b;
    b = c;
}
```

​		调用`swap(2,10);`的时候编译器会生成

```c++
template<typename T>
void swap(int &a, int &a)
{
	int c = a;
    a = b;
    b = c;
}
```

​		调用`swap(2.0,10.0);`的时候编译器会生成

```c++
template<typename T>
void swap(float &a, float &a)
{
	float c = a;
    a = b;
    b = c;
}
```

​		由此可见，c++编译器实现泛型编程的这种方法优点是简单易懂，程序员成面可以快速复用代码，缺点是还是会造成代码膨胀的问题。比如对应`std::vector<T>`的使用，每一个类型`T`都会生成一套类。

​		与此对应的就是第二种实现方式了，java用的就是第二种方式，因为java一切皆对象，基本类型也有包装类自动转换，而且所有类的基类都是`Object`类，所以泛型类和泛型方法实际上都是擦除了泛型变量，用基类object类替换。

```java
public class Container<K, V> {
    private K key;
    private V value;
    public Container(K k, V v) {
        key = k;
        value = v;
    }
}
```

​		实际上编译出来的字节码只有一份：

```java
public class Container<Object, Object> {
    private Object key;
    private Object value;
    public Container(Object k, Object v) {
        key = k;
        value = v;
    }
}
```

​		优点是显而易见的，编译出来的字节码精简，缺点也有，它实现是用一种类型擦除的方式，这个会会有限制导致程序员不能随心所欲的运用，还要运用一些反射，通配符才行。



​		c++的stl库是将常用的模板代码组合而成的一套模块库，主要分为6类。重点是迭代器、容器和算法。

![img](D:\study\code\leaner\doc\stl_component.png)

## 一、迭代器 Iterator

## 二、容器 Container

### 2.1、vector

```c++
template < class T, class Alloc = allocator<T> > class vector; // generic template
```

描述：

​		vector是表示可更改大小的数组的序列容器。

​		与数组一样，矢量对元素使用连续存储位置，这意味着还可以使用常规指向其元素的常规指针上的偏移量访问其元素，并且与数组一样高效。但与数组不同，它们的大小可以动态更改，它们的存储由容器自动处理。
​		在内部，矢量使用动态分配的数组来存储其元素。插入新元素时，可能需要重新分配此数组以增加大小，这意味着分配新数组并移动所有元素。就处理时间而言，这是一个相对昂贵的任务，因此，每次将元素添加到容器时，矢量不会重新分配。相反，矢量容器可能会分配一些额外的存储，以适应可能的增长，因此容器的实际容量可能大于包含其元素（即其大小）所需的存储。库可以实现不同的增长策略，以平衡内存使用和重新分配，但在任何情况下，重新分配应只在对数增长大小间隔下进行以便可以在矢量末尾插入单个元素，并具有摊销的恒定时间复杂性。
​		因此，与数组相比，矢量消耗更多的内存，以换取以高效方式管理存储和动态增长的能力。与其他动态序列容器（deques、list和 forward_lists）相比，向量在访问其元素（就像数组一样）和相对高效的添加或删除元素。对于涉及在末尾以外的位置插入或删除元素的操作，它们的表现比其他操作差，并且与列表和引用的一致性forward_lists。

| member type              | definition                                                   | notes                                                        |
| ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `value_type`             | The first template parameter (`T`)                           |                                                              |
| `allocator_type`         | The second template parameter (`Alloc`)                      | defaults to: `allocator<value_type>`                         |
| `reference`              | `value_type&`                                                |                                                              |
| `const_reference`        | `const value_type&`                                          |                                                              |
| `pointer`                | `allocator_traits<allocator_type>::pointer`                  | for the default [allocator](http://www.cplusplus.com/allocator): `value_type*` |
| `const_pointer`          | `allocator_traits<allocator_type>::const_pointer`            | for the default [allocator](http://www.cplusplus.com/allocator): `const value_type*` |
| `iterator`               | a [random access iterator](http://www.cplusplus.com/RandomAccessIterator) to `value_type` | convertible to `const_iterator`                              |
| `const_iterator`         | a [random access iterator](http://www.cplusplus.com/RandomAccessIterator) to `const value_type` |                                                              |
| `reverse_iterator`       | `reverse_iterator<iterator>`                                 |                                                              |
| `const_reverse_iterator` | `reverse_iterator<const_iterator>`                           |                                                              |
| `difference_type`        | a signed integral type, identical to: `iterator_traits<iterator>::difference_type` | usually the same as [ptrdiff_t](http://www.cplusplus.com/ptrdiff_t) |
| `size_type`              | an unsigned integral type that can represent any non-negative value of `difference_type` | usually the same as [size_t](http://www.cplusplus.com/size_t) |

​		一些常用的函数和坑点：

### 2.2、array

### 2.3、list

### 2.4、forward_list

### 2.5、deque

### 2.6、queue

### 2.7、stack

### 2.8、map

### 2.9、set

### 2.10、unordered_map和hash_map

### 2.11、unordered_set和hash_set

### 2.12、multimap和multiset

## 三、算法 Algorithm

## 四、函数对象 Function object

## 五、适配器 Adapter

## 六、分配器 Allocator

## 七、其他

### 7.1、std::pair

## 八、参考文档

- [cplusplus.com](http://www.cplusplus.com/info/)

