# Design patterns

## 一、Proxy Pattern

![代理模式UML](https://img-blog.csdn.net/20180729211917785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2EzNjkxODk0NTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- Subject：定义RealSubject和Proxy的共用接口，这样就可以在任何使用RealSubject的地方都可以使用Proxy。
- RealSubject：真实处理业务的角色。
- Proxy：保存一个引用使得代理可以访问实体。
  提供一个与Subject的接口相同的接口，这样代理就可以用来代替实体。