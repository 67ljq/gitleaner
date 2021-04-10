# spring



[TOC]

​																	**《基本都是转载总结网上的，少量自己写的》**



## 一、相关概念

### 1.1、Spring配置中<bean>的id和name属性区分

​		**1、ID属性**

在[spring](http://lib.csdn.net/base/javaee)容器管理的Bean中，必须仅且只有一个无重复的ID。

​		**2、手动指定ID属性**

一般情况下，配置一个bean时，需要指定它的ID属性作为bean的名称。

```xml
<bean id="helloWorld" class="com.learnSpring.hellWorld"/>
```

Q:如果在spring环境下，如果出现多个配置ID属性值一样的bean

- 如果spring是默认设定，即可以覆盖bean定义，则根据spring配置文件加载的顺序，后面同名的bean会覆盖掉前面定义的bean配置，spring不会报错

- 如果设置不可以覆盖bean定义，则出现多个同ID的bean，则会抛出异常，停止运行。

  **3、匿名bean**

如果在配置bean的时候并没有声明ID属性，则采用全类限定名作为bean的ID。

```xml
<bean class="com.learnSpring.hellWorld"

<bean class="com.learnSpring.hellWorld"/>

<bean class="com.learnSpring.hellWorld"/>
```

如果存在多个class属性都是一样的匿名的Bean，则生成的ID根据spring读取配置文件的顺序生成ID

```
"com.learnSpring.hellWorld"

"com.learnSpring.hellWorld#0"

"com.learnSpring.hellWorld#1"
```

​		**4、name属性**

如果一个bean只配置了name属性，但是没有配置ID属性，默认会`ID属性=name属性` 
name定义的是bean的alias，可以有多个，并可能与其他的bean重名。

```xml
<bean name="hello" class="com.learnSpring.hellWorld"/>

<bean id="hello" class="com.learnSpring.hellWorld"/>
```

对于上面的两个bean配置，最后会出现两个ID属性都是“hello”的bean，最后的bean会覆盖前面的bean。

## 二、相关注解

### 2.1、@Recourse、@Autowired

**@Resource和@Autowired注解都是用来实现依赖注入的。只是@AutoWried按by type自动注入，而@Resource默认按byName自动注入。**

**@Resource有两个重要属性，分别是name和type**

**spring将name属性解析为bean的名字，而type属性则被解析为bean的类型。所以如果使用name属性，则使用byName的自动注入策略，如果使用type属性则使用byType的自动注入策略。如果都没有指定，则通过反射机制使用byName自动注入策略。**

**@Resource依赖注入时查找bean的规则：(以用在field上为例)**

​		**1、既不指定name属性，也不指定type属性，则自动按byName方式进行查找。如果没有找到符合的bean，则回退为一个原始类型进行查找，如果找到就注入。**

**此时name是变量名**											**`《以下存疑，可能不对》`**

**错误示例：**

```java
@Resource
private String bucketName;
@Resource
private String styleName;
```

**此时的name值是配置bean里的name属性指定的值，而不是id的值**

```xml
<bean id="bucketName " class="java.lang.String"> 
    <constructor-arg value="${oos.bucketName}"/> 
</bean> 
<!-- 图片样式名 --> 
<bean id="styleName " class="java.lang.String"> 
    <constructor-arg value="${oos.styleName}"/> 
</bean>
```

​				**`《以下存疑，可能不对》`**

这里为什么要重新理解，是因为之前我一直认为对应的是配置文件的id属性的值，直到在配置上面两个String类型的bean的时候，居然会报错，如下： No qualifying bean of type [java.lang.String] is defined: expected single matching bean but found 2: bucketName,styleName 这是因为spring会去找bean元素里name属性值和变量名一致的bean，但是因为都没有指定name属性，所以找不到然后就按照原始类型String去查找，结果一下找到了两个，所以就报错。

​		**2、只是指定了@Resource注解的name，则按name后的名字去bean元素里查找有与之相等的name属性的bean。**

正确示例

```java
@Resource(name="bucket")
private String bucketName;
@Resource(name="style")
private String styleName;
```

```xml
<bean name="bucket" class="java.lang.String"> 
    <constructor-arg value="${oos.bucketName}"/> 
</bean> 
<!-- 图片样式名 --> 
<bean name="style" class="java.lang.String"> 
    <constructor-arg value="${oos.styleName}"/> 
</bean>
```

​		**3、只指定@Resource注解的type属性，则从上下文中找到类型匹配的唯一bean进行装配，找不到或者找到多个，都会抛出异常**

​		**4、既指定了@Resource的name属性又指定了type，则从Spring上下文中找到唯一匹配的bean进行装配，找不到则抛出异常**

#========================================

（1）处理这2个注解的BeanPostProcessor不一样

CommonAnnotationBeanPostProcessor是处理@ReSource注解的

AutoWiredAnnotationBeanPostProcessor是处理@AutoWired注解的

（2）@Autowired只按照byType 注入；@Resource默认按byName自动注入，也提供按照byType 注入；

（3）属性：@Autowired按类型装配依赖对象，默认情况下它要求依赖对象必须存在，如果允许null值，可以设置它required属性为false。如果我们想使用按名称装配，可以结合@Qualifier注解一起使用。@Resource有两个中重要的属性：name和type。name属性指定byName，如果没有指定name属性，当注解标注在字段上，即默认取字段的名称作为bean名称寻找依赖对象，当注解标注在属性的setter方法上，即默认取属性名作为bean名称寻找依赖对象。需要注意的是，@Resource如果没有指定name属性，并且按照默认的名称仍然找不到依赖对象时， @Resource注解会回退到按类型装配。但一旦指定了name属性，就只能按名称装配了。



@Resource装配顺序

　　1. 如果同时指定了name和type，则从Spring上下文中找到唯一匹配的bean进行装配，找不到则抛出异常

　　2. 如果指定了name，则从上下文中查找名称（id）匹配的bean进行装配，找不到则抛出异常

　　3. 如果指定了type，则从上下文中找到类型匹配的唯一bean进行装配，找不到或者找到多个，都会抛出异常

　　4. 如果既没有指定name，又没有指定type，则自动按照byName方式进行装配；如果没有匹配，则回退为一个原始类型进行匹配，如果匹配则自动装配；



推荐使用@Resource注解在字段上，这样就不用写setter方法了.并且这个注解是属于J2EE的，减少了与Spring的耦合,这样代码看起就比较优雅 。

### 参考文档

- [@Resource注解](https://blog.csdn.net/weixin_38237873/article/details/83650429)

- [@Autowired和@Resource的区别是什么？](https://www.zhihu.com/question/39356740/answer/1297401530)



























































