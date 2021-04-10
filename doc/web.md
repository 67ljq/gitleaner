# web



[TOC]

## 一、maven

## 二、base

### 2.1、重定向、转发

转发是服务器行为，重定向是客户端行为。

**一、请求转发（forward）**
一般说的请求转发指的就是直接转发方式。Web应用程序大多会有一个控制器。由控制器来控制请求应该转发给那个信息资源。然后由这些信息资源处理请求，处理完以后还可能转发给另外的信息资源来返回给用户，这个过程就是经典的MVC模式。

　　javax.serlvet.RequestDispatcher接口是请求转发器必须实现的接口，由Web容器为Servlet提供实现该接口的对象，通过调用该接口的forward()方法到达请求转发的目的，示例代码如下：

```java
//Servlet里处理get请求的方法
public void doGet(HttpServletRequest request , HttpServletResponse response){
   //获取请求转发器对象，该转发器的指向通过getRequestDisPatcher()的参数设置
   RequestDispatcher requestDispatcher =request.getRequestDispatcher("转发的URL");
   //调用forward()方法，转发请求      
   requestDispatcher.forward(request,response);
}
```

转发过程：客户浏览器发送http请求----》web服务器接受此请求--》调用内部的一个方法在容器内部完成请求处理和转发动作----》将目标资源发送给客户；在这里，转发的路径必须是同一个web容器下的url，其不能转向到其他的web路径上去，中间传递的是自己的容器内的request。在客户浏览器路径栏显示的仍然是其第一次访问的路径，也就是说客户是感觉不到服务器做了转发的。转发行为是浏览器只做了一次访问请求。

1. 浏览器向Servlet1发出访问请求；
2. Servlet1调用forward()方法，在服务器端将请求转发给Servlet2；
3. 最终由Servlet2做出响应

![img](https://img-blog.csdn.net/20181009112911841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NheW9rbzA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**二、重定向（redirect）**

间接转发方式，有时也叫重定向，它一般用于避免用户的非正常访问。例如：用户在没有登录的情况下访问后台资源，Servlet可以将该HTTP请求重定向到登录页面，让用户登录以后再访问。在Servlet中，通过调用response对象的SendRedirect()方法，告诉浏览器重定向访问指定的URL，示例代码如下：

```java
//Servlet中处理get请求的方法
public void doGet(HttpServletRequest request,HttpServletResponse response){
//请求重定向到另外的资源
    response.sendRedirect("重定向的URL");
}
```

重定向过程：客户浏览器发送http请求----》web服务器接受后发送302状态码响应及对应新的location给客户浏览器--》客户浏览器发现是302响应，则自动再发送一个新的http请求，请求url是新的location地址----》服务器根据此请求寻找资源并发送给客户。在这里location可以重定向到任意URL，既然是浏览器重新发出了请求，则就没有什么request传递的概念了。在客户浏览器路径栏显示的是其重定向的路径，客户可以观察到地址的变化的。重定向行为是浏览器做了至少两次的访问请求的。

1. 浏览器向Servlet1发出访问请求；
2. Servlet1调用sendRedirect()方法，将浏览器重定向到Servlet2；
3. 浏览器向servlet2发出请求；
4. 最终由Servlet2做出响应。

![img](https://img-blog.csdn.net/20181009112931240?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NheW9rbzA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**三、Spring MVC中redirect重定向3种方式（带参数）**

Spring MVC中做form表单功能提交时，防止用户客户端后退或者刷新时重复提交问题，需要在服务端进行重定向跳转,其中redirect是直接跳转到其他页面，有以下3种方法进行重定向。

- redirect重定向流程

客户发送一个请求到服务器，服务器匹配servlet，这都和请求转发一样，servlet处理完之后调用了sendRedirect()这个方法，这个方法是response的方法，所以，当这个servlet处理完之后，看到response.senRedirect()方法，立即向客户端返回这个响应，响应行告诉客户端你必须要再发送一个请求，去访问test.jsp，紧接着客户端受到这个请求后，立刻发出一个新的请求，去请求test.jsp,这里两个请求互不干扰，相互独立，在前面request里面setAttribute()的任何东西，在后面的request里面都获得不了。可见，在sendRedirect()里面是两个请求，两个响应。

1. **response.sendRedirect重定向跳转**

```java
@RequestMapping(value="/testredirect",method = { RequestMethod.POST, RequestMethod.GET })  
public ModelAndView testredirect(HttpServletResponse response){  
    response.sendRedirect("/index");
    return null; 
}
```

 2. **ViewResolver直接跳转**

    不带参数

    ```java
    @RequestMapping(value="/testredirect",method = { RequestMethod.POST, RequestMethod.GET })  
    public  String testredirect(HttpServletResponse response){  
        return "redirect:/index";  
    } 
    ```

    带参数

    ```java
    @RequestMapping("/testredirect")
    public String testredirect(Model model, RedirectAttributes attr) {
    	attr.addAttribute("test", "51gjie");//跳转地址带上test参数
        attr.addFlashAttribute("u2", "51gjie");//跳转地址不带上u2参数
    	return "redirect:/user/users";
    }
    ```

    - 使用RedirectAttributes的addAttribute方法传递参数会跟随在URL后面，如上代码即为http:/index.action?test=51gjie
    - 使用addFlashAttribute不会跟随在URL后面，会把该参数值暂时保存于session，待重定向url获取该参数后从session中移除，这里的redirect必须是方法映射路径，jsp无效。你会发现redirect后的jsp页面中b只会出现一次，刷新后b再也不会出现了，这验证了上面说的，b被访问后就会从session中移除。对于重复提交可以使用此来完成.
    - spring mvc设置下RequestMappingHandlerAdapter 的ignoreDefaultModelOnRedirect=true,这样可以提高效率，避免不必要的检索。

	3. **ModelAndView重定向**

    不带参数

    ```java
    @RequestMapping(value="/restredirect",method = { RequestMethod.POST, RequestMethod.GET })  
    public  ModelAndView restredirect(String userName){  
        ModelAndView  model = new ModelAndView("redirect:/main/index");    
        return model;  
    }
    ```

    带参数

    ```java
    @RequestMapping(value="/toredirect",method = { RequestMethod.POST, RequestMethod.GET })  
    public  ModelAndView toredirect(String userName){  
        ModelAndView  model = new ModelAndView("/main/index");   
        model.addObject("userName", userName);  //把userName参数带入到controller的RedirectAttributes
        return model;  
    }
    ```

**总结：**

Forward和Redirect代表了两种请求转发方式：直接转发和间接转发。对应到代码里，分别是RequestDispatcher类的forward()方法和HttpServletRequest类的sendRedirect()方法。

　　对于间接方式，服务器端在响应第一次请求的时候，让浏览器再向另外一个URL发出请求，从而达到转发的目的。它本质上是两次HTTP请求，对应两个request对象。

　　对于直接方式，客户端浏览器只发出一次请求，Servlet把请求转发给Servlet、HTML、JSP或其它信息资源，由第2个信息资源响应该请求，两个信息资源共享同一个request对象。

​		redirect重定向可以跳转到任意服务器，可以用在系统间的跳转。
​		Spring MVC中redirect重定向，参数传递可以直接拼接url也可以使用RedirectAttributes来处理，由于是不同的请求，重定向传递的参数会在地址栏显示，所以传递时要对中文编码进行处理。

## 三、SpringMVC

### 3.1、ViewResolver



## 参考

- [Forward（请求转发）和Redirect（重定向）的比较](https://blog.csdn.net/sayoko06/article/details/82978781?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&dist_request_id=&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control)
- [Spring MVC中redirect重定向3种方式（带参数）](https://blog.csdn.net/weixin_39973810/article/details/84847922)