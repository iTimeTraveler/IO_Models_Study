# I/O Models Study

Richard Stevens 《[UNIX® Network Programming Volume 1, Third Edition: The Sockets
Networking](https://notes.shichao.io/unp/ch6/)》 Chapter 6.2 "I/O Models"

> ## I/O Models
>
> We first examine the basic differences in the five I/O models that are available to us under Unix:
>
> - blocking I/O
>
> - nonblocking I/O
>
> - I/O multiplexing (`select` and `poll`)
>
> - signal driven I/O (`SIGIO`)
>
> - asynchronous I/O (the POSIX `aio_` functions)
>
> There are normally two distinct phases for an input operation:
>
> 1. Waiting for the data to be ready. This involves waiting for data to arrive on the network. When the packet arrives, it is copied into a buffer within the kernel.
> 2. Copying the data from the kernel to the process. This means copying the (ready) data from the kernel's buffer into our application buffer




基于 TCP 的网络编程开发分为服务器端和客户端两部分，常见的核心步骤和流程如下：

![](README/linux-TCP-socket.png)