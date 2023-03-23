# 第一章 Linux基础

## 1.1 Unix/Linux文件系统的组织

Unix/Linux将所有能够存储或者提供信息的事物都视为文件，从一般意义上说，文件系统树的每个节点都是一个"FILE"。有以下类型：

- 目录文件
- 非目录文件：非目录文件要么是REGULAR（常规文件）要么是SPECIAL，可进一步分为
    - 常规文件：ORDINARY文件，包含普通文本或者可执行的二进制代码
    - 特殊文件：/dev目录中的条目，表示I/O设备，可进一步分为：
        - 字符特殊文件：字符I/O， 如/dev/tty0, /dev/pts/1等
        - 块特殊文件:块I/O， 如/dev/had, /dev/sda等
        - 其他类型,如网络（套接字）特殊文件、命名管道等。
- 符号连接文件：输入常规文件，其内容为其他文件的路径名。因此这些文件是指向其他文件的指针。

## 1.2 Linux man

在linux中可以通过命令man来查看相关命令和系统调用。man man可以查看手册的目录。

1.   Executable programs or shell commands
2.   System calls (functions provided by the kernel)
3.   Library calls (functions within program libraries)
4.  Special files (usually found in /dev)
5.   File formats and conventions eg /etc/passwd
6.   Games
7.   Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7)
8.   System administration commands (usually only for root)
9.   Kernel routines [Non standard]

## 1.3 Ubuntu Linux系统管理

### 1.3.1 用户账户

用户账户信息保存在/etc/passwd文件中，归超级用户所有，但所有人都可以读取。在表单的/etc/passwd文件中，每个用户都有一个对应的记录行：

> loginName:x:gid:uid:useinfo:homeDir:initialProgram   
ljx:x:1000:1000:ljx:/home/ljx:/bin/bash

其中第二个字段'x'表示检查用户密码。加密的用户密码保存在 **/etc/shadow** 文件中。每一行都包含加密的用户密码，后面是可选的过期限制信息。  
当用户尝试使用用户名和密码登录时，Linux将检查/etc/passwd和/etc/shadow文件来验证用户身份。**用户登录成功后，登陆进程将通过获取用户的gid和uid来转换成用户进程，并将目录更改为用户的homeDir，然后执行列出的initialProgram，该程序通常为命令解释程序sh。** 

> sudo adduser username

#### sudo命令

sudo允许用户以另一个用户的身份执行命令。在执行命令时将用户进程提升到超级用户特权级别，执行完后恢复。为确保能够使用sudo， **用户名必须保存在/etc/sudoer文件中。为确保用户能够发出sodu，只需要在sudoers文件中添加 username ALL(ALL) ALL。**

# 第二章 编程背景

## vim

vim有3种操作模式：

- 命令模式
- 插入模式
- 末行模式

> 在命令模式下输入i/a进入插入模式。输入":"进入末行模式。

## 前言

### 编译流程

1. 预处理
2. 编译：将源代码编译成汇编代码，生成.s文件
3. 汇编：将汇编代码生成机器码，生成.o文件。每个.o文件都包含：
    - 一个文件头，包含代码段、数据段和BSS段大小
    - 一个代码段，包含机器指令
    - 一个数据段，包含初始化全局变量和初始化静态局部变量
    - 一个BSS段，包含未初始化全局变量和未初始化静态局部变量
    - 代码中的指针以及数据和BSS中的偏移量的重定位信息
    - 一个符号表，包含非静态全局变量、函数名称以及其属性
4. 链接：将所有的.o文件和必要的库函数组合成单一的二进制可执行文件。具体执行以下操作：
    - 将.o文件所有代码段组合成单一代码段。 **对于C语言程序，组合代码段从默认的C启动代码crt0.o开始，该代码调用main()函数。这就是为什么C语言程序必须有一个唯一的main()函数。**
    - 将所有数据段组合成单一数据段。仅包含初始化全局变量和初始化静态局部变量
    - 将所有BSS段组合成单一bss段
    - 用.o文件中重定位信息调整组合代码段中的指针以及组合数据段、bss段中的偏移量
    - 用符号表解析各个.o文件之间的交叉引用。

### 静态与动态链接

在使用静态库时，链接器将所有必要的库函数代码和数据纳入a.out文件中，这使得a.out文件完整独立并且很大。在使用共享库时，库函数未包含在a.out文件中，但是对此类函数的调用以指令形式记录在a.out文件中。在执行动态链接的a.out文件时，操作系统a.out和共享库均加载到内存中。

#### 静态链接库制作与链接

```bash
gcc -c xxx.c -o xxx.o   # compile xxx.c into xxx.o
ar rcs libmylib.a xxx.o # crate static link library with xxx.o
gcc -static main.c -L. -lmylib  # static compile-link main.c with libmylib.a as link library
./a.out                 # run a.out as usual
```
在编译链接过程中，-L.指定链接库路径（当前目录），-l指定链接库，不要lib前缀

#### 动态链接库制作与链接

```bash
gcc -c -fPIC xxx.c -o xxx.o             #compile to Position Independent Code xxx.o
gcc -shared -o libmylib.so xxx.o        # craete shared libmylib.so with xxx.o
gcc t.c -L. mylib                       # generate a.out using shared library libmylib.so
export LD_LIBRARY_PATH=./               #to run a.out must export LD_LIBRARY_PATH=./
a.out                                   # run a.out ld will load libmylib.so
```
用户也可以将链接库放入标准lib目录中，如/lib或/usr/lib，然后运行ldconfig配置动态链接库路径。具体man 8 ldconfig。

### 可执行文件格式

**二进制可执行平面文件**：仅包含可执行代码和初始化数据。该文件将作为一个整体加载到内存中便于直接执行。比如可启动操作系统映像通常是二进制可执行平面文件，该文件简化了引导装载程序。

**a.out可执行文件**：传统的a.out文件包含文件头，然后是代码段、数据段和bss段。

**ELF可执行文件**：可执行的链接格式(ELF)文件包含一个或多个程序段。每个程序段均可加载至特定的内存地址。在Linux中为默认的二进制可执行文件，更适合动态链接。

### a.out文件内容及执行流程

a.out文件包含文件头、代码段、数据段和符号表。

- 文件头：包含a.out文件的加载信息和大小，其中：
    - tsize:代码段大小（正文段text）
    - dsize:包含初始化全局变量和初始化静态局部变量的数据段大小
    - bsize:包含未初始化全局变量和未初始化静态局部变量的bss段大小
    - total_size:加载的a.out文件总大小
- 代码段：包含程序的可执行代码。代码段从标准C启动代码crt0.o开始，该代码调用main函数
- 数据段
- 符号表：可选，仅为运行调试所需

> 包含未初始化全局变量和未初始化局部变量的bss段不在a.out文件中，只有bss段的大小记录在a.out文件中，自动局部变量也不再a.out中。bss段上的额外的内存空间为堆区，用于执行期间的动态内存分配。

#### 执行流程

> a.out one two three

为执行命令，sh创建一个子进程并等待该子进程终止。子进程运行时，sh使用a.out文件，按照以下步骤创建新的执行映像：

1）读取a.out文件头，以确定所需的总内存大小，包括堆栈空间大小：

$$TotalSize = \_brk + stackSize$$

其中堆栈大小通常是操作系统内核为带启动程序选择的默认值。无法知道一个程序究竟需要多大的堆栈空间。

2）sh从总大小中分配一个内存区给执行映像。 **从概念上讲，可以假设分配的内存区是一个单独连续的内存。** sh将a.out文件的代码段和数据段加载到内存区，堆栈区位于高地址端。ssh将bss段清0。执行期间，堆栈向下朝低位地址延申。

3）然后，sh放弃旧映像，开始执行新映像。如下图

![执行映像](img/memoryAllo.png)

在上图中，bss段结束处的_brk为程序的初始"break"标记，_splimit为堆栈大小限度。首次加载a.out文件时，_brk可能和_splimit重合，初始堆大小为0。执行期间，进程可以使用brk（地址）或sbrk（大小）系统调用将_brk改到更高地址从而增加堆大小。malloc()可隐式调用。

如果程序试图扩展splimit下面的堆栈指针，就会发生堆栈溢出。在有内存保护的机器上，内存管理硬件将检测到，然后将进程捕捉到操作系统内核，操作系统内核可能会通过在进程地址空间中分配额外的内存来增加堆栈，从而允许继续执行。

4）执行从crt0.o开始，调用main，将argc和argv作为参数传递给main。

#### 程序终止

1） **正常终止**：如果程序执行成功，main最终会返回到crt0.o，调用库函数exit(0)终止进程。首先，exit(value)函数会做一些清理工作，如刷新stdout，关闭I/O流。然后发出一个_exit(value)系统调用，时进入操作系统内核的进程终止。或者直接调用_exit()，然后将清理工作交给父进程。

2） **异常终止**：如果进程遇到了某些错误：无效地址、非法指令、越权等，会被CPU识别为异常，然后陷入操作系统内核，内核的陷入处理程序将错误类型转换为一个幻数，称为 **信号**，将信号传递给该进程，使进程终止。

## 2.1 汇编和C

### 2.1.1 创建静态链接库和动态链接库

```C++
//mysum.c
int mysum(int x,int y){return x+y;}
//t.c
int main(){int sum = mysum(123,456);}
```

**创建静态链接库**

```sh
gcc -c mysum.c 				#compile mysum.c into mysum.o
ar rcs libmylib.a mysum.o	#create static link library with member mysum.o
gcc -static t.c -L. -lmylib	#static compile-link t.c with libmylib.a as link library
a.out						#run a.out
```

**创建动态链接库**

```sh
gcc -c -fPIC mysum.c						#compile to Position Inpendent Code mysum.o
gcc -shared -o libmylib.so mysum.o			#create shared libmylib.so with mysum.o
gcc t.c -L. -lmylib							#generate a.out using shared library 		libmylib.so
export LD_LIBRARY_PATH=./					#add this dll to LD_LIBRARY__PATH,or you can move it to /lib or /usr/lib
a.out
```

### 2.1.2 makefile

```makefile
target:dependency list
	command/rule(gcc -o a 1.c 2.c)
# makefile的宏定义
CC = gcc
CFLAGS = -Wall
OBJS = t.o mysum.o
INCLUDE = -Ipath
myt:type.h $(objs)
	$(CC) $(CFLAGS) -o t $(OBJS) $(INCLUDE)
# 定义依赖关系
t.o 	t.c type.h
	gcc -c t.c
mysum.o	mysum.c	type.h
	gcc -c mysum.c

# 按名称编译,makefile通常会尝试构建第一个目标
# -----------------mk3 file-------------------
CC = gcc
CFLAGS = -Wall
OBJS = t.o mysum.o
INCLUDE = -Ipath
all:myt install # build all listed target myt install
myt: t.o mysum.o
	$(CC) $(CFLAGS) -o myt $(OBJS) $(INCLUDE)
t.o: t.c type.h
	gcc -c t.c
mysum.o:mysum.c type.h
	gcc -c mysum.c
	
install: myt
	echo install myt to /usr/local/bin
	sudo mv myt /usr/local/bin

run: install
	echo run executable image my
	myt || /bin/true
clean:
	rm -f *.o 2> /dev/null
	sudo rm -f /usr/local/bin/myt
# -----------执行指令----------
make [all] -f mk3
make install -f mk3
make run -f mk3
make clean -f mk3
```

**自动变量**

- $@:当前目标名
- $<:第一个依赖项名
- $^:所有依赖项名
- $*:不包含扩展名的当前依赖项名
- $?:比当前目标更新的依赖项列表

```makefile
DEPS=type.h
%.o:%.c $(DEPS)
	$(CC) -c -o $@
```

**使用make变量和后缀规则**

```makefile
CC= gcc
CFLAGS = -I.
OBJS = t.o mysum.o
AS = as
EPS = type.h
.s.o:# for each fname.o, assemble fname.s into fname.o
	$(AS) -o $< -o $@	# -o $@ required for .s files
.c.o:# for each fname.o, compile fname.c into fname.o
	$(CC) -c $< -o $@
%.o: %.c $(DEPS) #for all .o files:if its .c or .h file changed
	$(CC) -c -o $@ $<

myt:$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
```

**子目录的makefile**

```makefile
(cd DIR;$(MAKE)) or cd DIR && $(MAKE)
```

# 第三章 进程管理

在Unix/Linux中，进程只有非常松散的父子关系，它们的执行环境都是独立的。如果有子进程的某个父进程首先死亡，那么所有子进程都会变成孤儿进程。在类Unix系统中，进程P1（又称init进程）不会死亡，当某个进程死亡时，将所有的孤儿子进程都送到P1中，成为P1的子进程。大多数大型操作系统内核通过维护进程家族树来跟踪进程关系。

### 进程家族树

通常进程家族树通过PROC结构中的一对子进程和兄弟进城指针以二叉树的方式实现：

```C++
PROC *child, *sibling, *parent;
```

进程执行exit后，状态变为ZOMBIE，即僵尸进程，不释放PROC结构体。然后，进程调用kwakeup(event)来唤醒父进程，其中event必须是父进程和子进程使用的相同唯一值，例如父进程的PROC结构体地址或父进程的pid。濒死进程的最后操作是进程最后一次调用tswitch（），在这之后进程基本死亡，只剩一个空壳，以僵尸进程的形式存在，它通过等待操作被父进程埋葬（释放）。

**INIT（P1）进程的作用：**

- 是除P0之外所有进程的祖先；
- 是孤儿院院长，所有孤儿进程由其接受；
- 太平间管理员，不停地寻找僵尸进程并释放。

## 3.1 Linux/Unix中的进程

### 3.1.1进程来源

在操作系统启动时，操作系统内核的启动代码会强行创建一个PID=0的初始进程，即通过分配PROC结构体进行创建，初始化PROC内容，并让运行指向它。然后，系统执行初始进程P0，P0继续初始化系统，包括系统硬件和内核数据结构。然后，它挂载一个根文件系统，使系统可以使用文件。在初始化系统之后，P0复刻出一个子进程P1，并把进程切换以用户模式运行P1.

### 3.1.2 INIT进程和守护进程

当进程P1开始运行时，它将其执行映像改为INIT程序。因此P1通常被称为INIT进程，因为其执行映像为INIT程序。P1开始复刻出许多子进程，大部分子进程都是用来提供系统服务的，他们在后台运行，不与用户交互。这样的进程称为守护进程。守护进程有

- syslogd：log daemon process
- inetd：Internet service daemon process
- httpd：HTTP server daemon process
- ...

### 3.1.3 登录进程

除了守护进程之外，P1还复刻了许多LOGIN进程，每个终端一个，用于用户登录。每个LOGIN进程打开三个与自己终端相关联的文件流，这三个文件流分别是stdin，stdout和stderr。

> 用户账户保存在/etc/passwd和/etc/shadow文件中，每个账户在/etc/passwd中有一行对应的记录：
>
> **name:x :gid:uid:description:home:program**
>
> name为登录名，x代表检查密码登录，gid为用户组id，uid为用户id，home为用户主目录，program为用户登录后执行的初始程序。其他用户信息保存在/etc/shadow中，shadow中每一行包含加密的用户密码，后面是可选的过期限制信息。当用户使用用户名和密码登录时，Linux将检查passwd和shadow文件，以验证用户身份。



### 3.1.4 sh进程

当用户成功登录时，LOGIN进程会获取用户的gid和uid，从而成为用户的进程。他将目录更改为用户的主目录并执行列出的程序，通常是命令解释程序sh。然后用户进程执行sh，因此用户进程通常称为sh进程。对于每个可执行文件的命令，sh会复刻一个子进程，并等待子进程终止。子进程将其执行映像更改为命令文件并执行命令程序。子进程在终止时会唤醒父进程sh，父进程会收集子进程终止状态、释放子进程PROC结构体。sh还支持I/O重定向和通过管道连接的多个命令。

### 3.1.5 进程执行模式

进程以两种不同的模式执行，即内核模式和用户模式，简称Kmode和Umode。在进程生命周期中，会在Kmode和Umode间多次迁移，在Kmode下执行所有相关操作，包括终止。在Kmode模式下，可以通过将CPU寄存机从K模式更改为U模式切换到Umode，但是从Umode进入Kmode只能通过以下三种方法：

- 中断
- Trap：无效地址、非法指令、除零等。在Linux中，内核陷阱处理程序将陷阱原因转化为信号编号，并将信号传递给进程。对于多数信号进程的默认操作是终止。
- 系统调用

## 3.2 进程管理系统调用

### 3.2.1 fork（）

```C
int pid = fork()
```

fork()创建子进程并返回pid。在Linux中，每个用户在同一时间只能有数量有限的进程，用户资源限制可以再/etc/security/limits.conf文件中设置，可以用ulimit -a查看资源限制。

**kfork()允许子进程继承父进程打开的所有文件。**在fork后子进程和系统中其他进程争夺CPU，在Linux中以下系统调用可能影响进程执行顺序：

- sleep
- nice(int inc):将进程优先级增大一个指定值，这回降低进程调度优先级（优先值越大优先级越低）。在非抢占式内核中，进程切换可能不会立即发生，只在执行进程返回Umode时发生
- sched_yield(void):使调用进程放弃cpu，允许优先级更高的其他进程先运行。但是如果调用进程仍然具有最高优先级，它将继续运行。

**Linux系统有一个vfork()系统调用，比fork()更有效。The use of vfork() was tricky: for example, not modifying data in the parent process depended on knowing which variables were held in a register.**就是说fork()会父进程使用的空间都复制一份给子进程使用，但对于很多子进程来说都是直接调用exec然后结束，vfork()并不会复制父进程的空间来使用，而是直接使用父进程的空间，在子进程执行期间父进程被挂起。

> DESCRIPTION
>    Standard description
>        (From  POSIX.1)  The  vfork() function has the same effect as fork(2), except that the behavior is undefined if the process created by vfork() either modifies any data other than a variable of type pid_t used to store the return value from vfork(), or returns from the function in which vfork() was called, or calls any other  function before successfully calling _exit(2) or one of the exec(3) family of functions.
>
>    Linux description
>        vfork(), just like fork(2), creates a child process of the calling process.  For details and return value and errors, see fork(2).
>
> ​       vfork() is a special case of clone(2).  It is used to create new processes without copying the page tables of the parent process.  It may be useful in performance-sen‐sitive applications where a child is created which then immediately issues an execve(2).
>
>    vfork() differs from fork(2) in that the calling thread is suspended until the child terminates (either normally, by calling _exit(2), or abnormally, after delivery of a  fatal signal), or it makes a call to execve(2).  Until that point, the child shares all memory with its parent, including the stack.  The child must not return from the current function or call exit(3), but may call _exit(2).
>
>    As with fork(2), the child process created by vfork() inherits copies of various of the caller's process attributes (e.g., file descriptors, signal  dispositions,  and current working directory); the vfork() call differs only in the treatment of the virtual address space, as described above.
>
>    Signals sent to the parent arrive after the child releases the parent's memory (i.e., after the child terminates or calls execve(2)).

```c
   #include <sys/types.h>
   #include <unistd.h>

   pid_t vfork(void);
```

### 3.2.2 进程终止wait()

- 正常终止：每个C程序的main()函数都是由C启动代码crt0.o调用的。如果函数执行成功，main()最终会返回到crt0.o，调用库函数exit(0)来终止进程。exit(value)函数会执行一些清理工作，如刷新stdout，关闭IO流等。然后它发出一个\_exit(value)的系统调用，是进入操作系统内核的进程终止。当内核中的某个进程终止时，它会将\_exit(value)系统调用中的值记录为进程PROC结构体中的退出状态，并通知他的父进程并使该进程称为僵尸进程。父进程可以通过系统调用找到僵尸进程，获得其pid和退出状态。他还会清空僵尸子进程PROC结构体，使之可以被另一个进程重用

  **pid = wait(int *status)**

- 异常终止

> 在Linux中每个PROC都有一个2字节的exitCode字段，如果进程正常终止，exitCode的高位字节是\_exit(value)中的value。如果异常终止，低位字节是导致异常终止的信号数值。因为一个进程只能死亡一次，所以只有一个字节有意义。

成功情况下，wait()会返回僵尸子进程的pid，status包含僵尸子进程的exitCode；此外，wait还会释放僵尸子进程，以供重新使用。也可以使用系统调用:

waitpid(int pid, int *status, int options);

### 3.2.3 subreaper进程

进程可以用系统调用将自己定义为subreaper进程，这样标记为subreaper的最近活跃祖先进程将成为孤儿进程的父进程，如果没有活跃的subreaper进程，孤儿进程仍然成为INIT进程的子进程。

**prctl(PR_SET_CHILD_SUBREAPER);**

原因：

> 许多用户空间服务管理器（upstart，systemd）需要跟踪他们启动的服务。这类服务通常通过两次复刻来创建守护进程，但会让中间子进程立即退出，将孙子进程升级为P1的子进程。
>
> 这样做的缺点是服务管理器不能再从守护进程接受SIGCHLD信号。使用subreaper机制，服务管理器可以将自己标记为“sub-init”，并且现在可以作为启动服务创建的所有孤儿进程的父进程，这样减少了P1的工作量，让他不必处理系统中所有孤儿进程。

### 3.2.4 exec()：更改进程执行映像

进程可以使用exec()将其Umode映像更改为不同的可执行文件。exec()库函数有以下成员：

```c
   #include <unistd.h>

   extern char **environ;

   int execl(const char *path, const char *arg, ...
                   /* (char  *) NULL */);
   int execlp(const char *file, const char *arg, ...
                   /* (char  *) NULL */);
   int execle(const char *path, const char *arg, ...
                   /*, (char *) NULL, char * const envp[] */);
   int execv(const char *path, char *const argv[]);
   int execvp(const char *file, char *const argv[]);
   int execvpe(const char *file, char *const argv[],
                   char *const envp[]);
```

以上函数都是针对系统调用进行封装的库函数，准备参数并发出最终的系统调用：

```C
int execve(const char *filename, char *const argv[], char *const envp[]);
```

第一个参数文件名与当前工作目录或绝对路径名有关。参数argv是一个以NULL结尾的字符串指针数组，每个指针指向一个命令行参数字符串，argv[0]是程序名，其他argv[]是程序的命令行参数。如：

```sh
a.out one two three
argv[] = ["a.out", "one", "two", "three", NULL]
```

### 3.2.5 环境变量

环境变量是为当前sh定义的变量，由子sh或进程继承。可在.bashrc文件中设置，用户可以通过env或printenv来查看环境变量。

### 3.2.6 文件流IO和系统调用

```c
scanf("%s", &item)
```

执行以上库函数时，会试图从stdin文件输入一个项， 指向FILE结构体。如果FILE结构体的fbuf[]为空，它会向Linux内核发出read系统调用，从文件描述符0中读取数据，映射到终端(/dev/ttyX)或伪终端(/dev/pts/#)键盘上。如果我们用一个新打开的文件来替换文件描述符0，那么输入将来自新的文件。

```c
printf("%s\n", items)
```

执行以上库函数时，会试图将数据写入stdout文件FILE结构体的fbuf[]，这是缓冲行，如果fbuf有一个完整的行，他会发出一个write的系统调用，将数据从fbuf写入文件描述符1，映射到终端屏幕。、

## 3.3 管道

管道是用于进程交换数据的单向进程间通信通道，具有一个写入端和一个读取端。普通管道用于相关进程，命名管道是不相关进程之间的FIFO通信通道。读入和写入管道通常是同步、阻塞操作，一些系统支持管道的非阻塞、异步读写操作。

### 3.3.1 Unix/Linux中的管道编程

```c
   #include <unistd.h>

   int pipe(int pipefd[2]);

   #define _GNU_SOURCE             /* See feature_test_macros(7) */
   #include <fcntl.h>              /* Obtain O_* constant definitions */
   #include <unistd.h>

   int pipe2(int pipefd[2], int flags);
```

以上系统调用在内核中创建一个管道，并在pd[2]中返回2个文件描述符。其中pd[0]用于读管道数据，pd[1]用于向管道写数据。管道并非为单进程而创建。

### 3.3.2 管道命令处理

在Linux中，"|"是管道符号。cmd1 | cmd2，sh通过一个进程运行cmd1，并通过另一个进程运行cmd2，并将cmd1的输出作为cmd2的输入。

### 3.3.3 命名管道

命名管道又叫FIFO，他们有“名称”，并在文件系统中以特殊文件的形式存在；他们会一直存在，除非用户删除（rm/unlink)。他们可以与非相关进程一起使用，并不局限于管道创建进程和其子进程。

- 在sh中可以通过mknod命令创建一个命名管道：

​		**mknod mypipe p**

- 也可以在C程序中发出mknod系统调用：
  - **int r = mknod("mypipe", S_IFIFO, 0);
- 进程可以像访问普通文件一样访问命名管道，但是对命名管道的读写是由Linux内核同步的。

```c
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <unistd.h>

   int mknod(const char *pathname, mode_t mode, dev_t dev);

   #include <fcntl.h>           /* Definition of AT_* constants */
   #include <sys/stat.h>

   int mknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev);
```

# 第四章 并发编程

## 4.1 线程

**优点**

- 线程创建和切换速度更快：创建线程时操作系统不必为新的线程分配内存和创建页表，因为线程和进程共同使用一个地址空间。进程切换涉及到将一个进程的复杂分页环境替换为另一个进程的复杂分页环境。
- 线程的响应速度更快：一个线程挂起，不影响其他线程执行
- 线程更适合并行计算

**缺点：**

- 由于地址空间共享，线程需要来自用户的明确同步
- 许多库函数可能对线程不安全，通常，任何使用全局变量或依赖于静态内存内容的函数，线程都不安全。
- 在单CPU系统上，使用线程解决问题实际上更慢。

> 线程可在内核模式或用户模式下运行，每个线程都有自己的执行堆栈，是独立的执行单元。为了利用线程的共享地址空间，操作系统内核的调度策略可能会优先选择同一进程中的线程，而不是不同进程中的线程。

## 4.2线程管理函数

Pthread库提供了以下线程管理API：

- int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
- void pthread_exit(void *retval);
- int pthread_cancel(pthread_t thread);
- int pthread_attr_init(pthread_attr_t *attr);
- int pthread_attr_destroy(pthread_attr_t *attr);

**int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);**

- thread是指向pthread_t的指针，会被操作系统分配唯一的ID填充。线程可以通过pthread_self()获得自己的id。在Linux中pthread_t被定义为unsigned long。
- attr指定线程属性
- func要执行的新线程函数入口
- arg：执行线程函数参数的指针，可写为：**void *func(void *arg)**

**attr的参数最为复杂，以下为使用步骤**

- 定义一个pthread属性变量pthread_attr_t attr
- 用pthread_attr_init(&attr)初始化属性变量
- 设置属性变量并在pthread_create调用中使用
- 必要时，通过pthread_attr_destory(&attr)释放资源

**每个线程在创建时都默认可与其他线程连接，可以使用分离属性创建一个线程：**

```C
ptread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)
```

在执行过程中，线程可以通过函数找到他的堆栈大小：

```C
size_t pthread_attr_getstacksize()
```

可以创建具有特定堆栈大小的线程：

```C
stacksize = 0x10000;
pthread_attr_setstacksize(&attr, stacksize);
```

如果attr参数为NULL，在创建线程时会使用默认属性。这是创建线程的推荐方法，除非确实有必要更改线程属性。

**线程ID是一种不透明数据类型，因此不应该直接比较线程ID，如果有需要，可以使用pthread_equal()函数比较。：**

```C
int pthread_equal(pthread_t t1, pthread_t t2);
```

一个线程可以等待另一个线程终止，通过：

```C
int pthread_join(pthread_t thread, void **retval);
//线程退出状态以retval返回
```

线程可以通过调用**pthread_exit(void *retval)**进行显示终止：

```C
void pthread_exit(void *retval);
```

## 4.3 线程同步

由于线程在进程的同一地址空间运行，他们共享同一地址空间中的所有全局变量和数据结构。为了防止出现竟态条件并且支持线程协作，线程需要同步。

### 4.3.1 互斥量

在Pthread中，**锁被称为互斥量**。互斥量使用pthread_mutex_t类型声明，在使用之前需要初始化，有两种初始化方法：

- 静态方法：pthread_mutex_t m =PTHREAD_MUTEX_INITIALIZER
- 动态方法，使用pthread_mutex_init()函数

```C
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

```C
       int pthread_mutex_lock(pthread_mutex_t *mutex);
       int pthread_mutex_trylock(pthread_mutex_t *mutex);
       int pthread_mutex_unlock(pthread_mutex_t *mutex);
	   int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

### 4.3.2 死锁预防

一种简单的死锁预防方法是对互斥量进行排序，并确保每个线程只能在一个方向请求互斥量，这样请求序列就不会有循环。但是仅使用单向加锁请求来设计所有并行算法是不可能的，在这种情况下，可以使用trylock()来预防死锁，trylock()函数试图对一个互斥量进行加锁，如果该互斥量已被加锁，则会立即返回一个错误。在这种情况下，线程可以释放一些已经获取的互斥量以便退避，让其他线程继续执行：

```C
while(1){
    lock(m1);
    if(!trylock(m2))
        unlock(m1);
    else
        continue
}
```

### 4.3.3 条件变量

互斥量仅用于确保线程只能互斥的访问临界区的共享数据对象。**条件变量提供了一种线程协作方法。**条件变量总与互斥量一起使用。

```C
       int pthread_cond_destroy(pthread_cond_t *cond);
       int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
       pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

**使用条件变量时，必须先获取相关的互斥量。然后在互斥量的临界区内执行操作，然后释放互斥量。**

协作函数：

```c
       int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex,
           const struct timespec *restrict abstime);
//阻塞调用线程，直到发出指定条件的信号。当互斥量被加锁时应调用该函数，他会在线程等待时自动释放互斥量。互斥量将在接收到信号并唤醒阻塞线程后自动锁定
       int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
//发出信号，唤醒正在等待条件变量的线程或解除阻塞。应在互斥量加锁后使用，而且必须解锁互斥量才能完成pthread_cond_wait()
	   int pthread_cond_signal(pthread_cond_t *cond);
//解除被阻塞在条件变量上的所有线程，让他们争用互斥量
       int pthread_cond_broadcast(pthread_cond_t *cond);
```

### 4.3.4 信号量

信号量是进程同步的一般机制。信号量是一种数据结构：

```C
struct sem{
    int value;				//semaphore counter value
    struct process *queue;	//a queue of blocked process
}
```

```C
       #include <semaphore.h>
       int sem_init(sem_t *sem, int pshared, unsigned int value);
       int sem_wait(sem_t *sem);
       int sem_trywait(sem_t *sem);
       int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
       int sem_post(sem_t *sem);
```

### 4.3.5 屏障pthread_barrier_t

由于创建线程需要系统开销，在某些情况下，保持线程活动会更好，但应该要求他们在到达指定同步点之前不能继续活动。可以采用屏障机制。

```C
       #include <pthread.h>

       int pthread_barrier_destroy(pthread_barrier_t *barrier);
       int pthread_barrier_init(pthread_barrier_t *restrict barrier,
           const pthread_barrierattr_t *restrict attr, unsigned count);
//在屏障中等待指定数量的线程到达屏障，当最后一个线程到达时，所有线程重新开始运行。
	   int pthread_barrier_wait(pthread_barrier_t *barrier);
```

### 4.3.6 Linux中的线程

Linux不区分进程和线程，对于Linux内核线程只是一个与其他进程共享某些资源的进程。他们都是由clone()系统调用创建的：

```C
int clone(int (*fn)(void *), void *child_stack,
                 int flags, void *arg, ...
                 /* pid_t *ptid, struct user_desc *tls, pid_t *ctid */ );
```

flags包括：

- CLONE_VM:父子进程共享地址空间
- CLONE_FS:父子进程共享文件系统信息，如根节点，pwd
- CLONE_FILES:父子进程共享打开的文件
- CLONE_SIGHAND:父子进程共享信号处理函数和已屏蔽的信号

如果未指定任何标志寄存器，子进程会获得一套单独的资源副本。



# 第五章 定时器及时钟服务



定时器是由时钟源和可编程计数器组成的硬件设备。时钟源通常是一个晶体振荡器，会产生周期性电信号，以精确的评率驱动计数器。使用一个倒计时值对计数器进行编程，每个时钟信号减一，当计数器减为零时，向CPU生成一个定时器中断，并将计数器值重新加载到计数器中，重复计时。计数器周期称为定时器刻度，是系统的基本及时单元。

## 5.1 个人计算机定时器

- 实时时钟RTC：有一个小型备用电池供电，即使关机时页能继续运行。用于实时提供时间和日期信息。在所有类Unix系统中，时间是一个长整数，为从1970年1月1日经过的秒数。
- 可编程间隔定时器PIT:与CPU分离的一个硬件定时器。可对他进行编程，以提供以毫秒为单位的定时器刻度。在所有IO设备中，PIT可以最高优先级IRQ0中断。PIT定时器中断由Linux内核的定时器中断处理程序来处理，为操作系统提供基本的定时单元，如进程调度、进程间隔定时器和其他许多定时事件。
- 多核CPU中的本地定时器：在多核CPU中，每个核都是一个独立的处理器，有自己的本地定时器，由CPU时钟驱动
- 高分辨率定时器：大多数电脑都有一个时间戳定时器(TSC)，有系统时钟驱动，它的内容可通过64位TSC寄存器读取。由于不同系统主板的时钟频率可能不同，TSC不适合作为实时设备，**但他可提供纳秒级的定时器分辨率。**

## 5.2 中断处理

外部设备的中断被馈送到**中断控制器**的预定义输入行，按优先级对中断输入进行排序，并将具有最高优先级的中断作为中断请求IRQ路由到CPU。对于每个中断，可以编程中断控制器来生成一个唯一编号，叫中断向量，标识中断源。在获取中断向量之后，CPU用它作为内存中中断向量表的条目索引，条目包含一个指向中断处理程序入口地址的指针来实际处理中断。当中断处理结束时，CPU恢复指令正常执行。

## 5.3 时钟服务函数

- gettimeofday
- settimeofday

### 5.3.1 time系统调用

- time_t time(time_t *t):以秒为单位返回当前时间。t为传出参数，只能提供以秒为单位的分辨率。

### 5.3.2 times系统调用

- clock_t times(struct  tms *buf):buf为传出参数，可用于获取某进程的具体执行时间。

```C
struct tms{
    clock_t tms_utime;		//user mode time
    clock_t tms_sutime;		//system mode time
    clock_t tms_cutime;		//user time for child
    clock_t tms_cstime;		//system time for child
}
```

### 5.3.3 time和date

- date：打印或设置系统日期
- time：报告进程在用户模式和系统模式下执行时间的总和
- hwclock：查询并设置硬件时钟(RTC)，也可以通过BIOS完成

## 5.6 间隔定时器

Linux为每个进程提供了三种不同类型的间隔计时器，可以用作进程计时的虚拟时钟。间隔定时器用setitimer创建，getitimer返回定时器状态。

- ITIMER_REAL:实时减少，到期生成14信号
- ITIMER_VIRTUAL：仅在进程在用户模式下减少，到期生成26信号
- ITIMER_PROF：当进程正在用户模式和系统模式时减少，到期生成27信号。

```c
       #include <sys/time.h>
           struct itimerval {
               struct timeval it_interval; /* Interval for periodic timer */
               struct timeval it_value;    /* Time until next expiration */
           };

           struct timeval {
               time_t      tv_sec;         /* seconds */
               suseconds_t tv_usec;        /* microseconds */
           };
       int getitimer(int which, struct itimerval *curr_value);
       int setitimer(int which, const struct itimerval *new_value,
                     struct itimerval *old_value);
```



# 第六章 信号和信号处理

## 6.1 Linux信号

### 6.1.1 进程PROC结构体中的信号

每个进程PROC都有一个32位向量，用来记录发送给进程的信号。在位向量中，每一位代表一个信号编号，才外还有一个信号MASK向量，用来屏蔽相应的信号。可以通过系统调用如sigmask,sigsetmask,siggetmask,sigblock等设置、清除或检查MASK为向量。待处理信号只有未屏蔽情况下才有效。

### 6.1.2 信号处理函数

每个进程PROC都有一个信号处理数组int sig[32]。每个条目都指定了如何处理相应的信号，其中0表示DEFault默认，1表示IGNore忽略，其他非零值表示用户模式下预先安装的信号捕捉函数。

### 6.1.3 安装信号捕捉函数

```C
       #include <signal.h>

       typedef void (*sighandler_t)(int);

       sighandler_t signal(int signum, sighandler_t handler);
```

除SIGKILL(9)和SIGSTOP(19)不可修改外，可以修改其他信号编号的处理函数。

signal**的缺点：**

- 在执行已安装的信号捕捉函数之前，通常将信号处理函数重置为DEFault。为捕捉下次出现的相同信号，必须重新安装捕捉函数，这可能会出现竟态条件，而sigaction在执行当前捕捉函数时会阻塞下一个信号，因此不会出现竟态条件。
- signal不能阻塞其他信号，必要时需要使用sigprocmask来现实的阻塞或解锁其他信号，相反，sigaction可以指定要阻塞的其他信号
- signal只能向捕捉函数发送一个信号编号，sigaction可以传输关于信号的其他信息。
- signal可能不适用于多线程程序的线程，sigaction可以
- 不同Unix版本的signal可能有所不同，sigaction采用的是POISX标准，可移植性更好

```c
       int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;		//处理函数执行期间要阻塞的信号
               int        sa_flags;		//可修改信号处理进程的行为，若要使用sa_sigaction，必须设置为SA_SIGINFO
               void     (*sa_restorer)(void);
           };
```

### 6.1.4 信号处理步骤

- 当某进程处于内核模式时，会检查并处理未完成的信号。如果信号有用户安装的捕捉函数，该进程会先清除信号，获取捕捉函数地址，对于大多数陷阱信号，将已安装的捕捉函数重置为DEFault。然后，在用户模式下返回，来执行捕捉函数，以这种方式篡改返回路径。当捕捉函数结束时，他会返回到最初的断点。
- 重置用户安装的信号捕捉函数：由于用户的捕捉函数也在用户模式执行，有可能出现相同的错误，就会导致一直在用户模式和内核模式跳跃。为了防止这种情况，Unix内核通常会在允许进程执行捕捉函数之前先将处理函数重置为DEFault，这意味着用户的捕捉函数只会对首次出现的信号有效；若要捕捉再次出现的同一信号，必须重新安装捕捉函数。
- 信号和唤醒：在Linux内核中有两种sleep进程；深度休眠进程和浅度休眠进程。前一种进程不可中断，后一种可由信号中断。如果某进程处于不可中断的sleep状态，到达的信号不会唤醒进程。如果他处于可中断的sleep状态，到达的信号将会唤醒它。

### 6.1.5 信号用作IPC

- 该机制并不可靠，可能会丢失信号。每个信号由位向量的一个位表示，只能记录一个信号的一次出现。
- 竟态条件：在处理信号之前，进程通常会将信号处理函数重置为DEFault。要想捕捉同一信号的再次出现，进程必须在该信号再次到来之前重新安装捕捉函数。否则，下一个信号可能会导致该进程终止。在执行信号捕捉函数时，虽然可以通过阻塞同一信号来防止竟态条件，但是无法防止丢失信号。
- 大多数信号都有预定义的含义。不加区别的任意使用信号可能会造成混乱。

## 6.2 Linux中的IPC

### 6.2.1 管道和FIFO

未命名管道供相关进程使用，命名管道是FIFO的，供不相关进程使用。必要时，可以通过对管道描述符的fcntl系统调用将管道设置为非阻塞。

### 6.2.2 信号

将信号用作IPC的一个主要缺点就是信号只是用作通知，不含任何信息内容。

### 6.2.3 System V IPC

Linux智齿System V IPC，包括共享内run、信号量和消息队列。在Linux中，多种System V IPC函数，如shmat/shmdt（添加/删除共享内存），semget/semop（获取/操作信号量）和msgsnd/msgrcv（发送/接受）消息。他们都会向Linux内核发出一个ipc()系统调用。Linux特有，不可移植。

### 6.2.4 POSIX消息队列

更通用，且可移植。

### 6.2.5 线程同步机制

Linux不区分线程和进程。

### 6.2.6 套接字

# 第七章 文件操作

## 7.1 文件操作级别
文件操作分为五个级别：
- 硬件级别：
    - fdisk：将硬盘、U盘或者SDC盘分区
    - mkfs：格式化磁盘分区，为系统做好准备
    - fsck：检查和维修系统
    - 碎片整理：压缩文件系统中的文件

- 内核中的文件系统函数：
    - kmount(), kumount()
    - kmkdir(), krmdir()
    - kchdir(), kgetcwd()
    - klink(), kunlink()
    - kchmod(), kchown(), kutime()
    - kcreat(), kopen()
    - kread(), kwrite()
    - klseek(), kclose()
    - ksymlink(), kreadlink()
    - kstat(), kfstat(), klstat()
    - kopendir(), kreaddir()

- 系统调用：用户模式使用系统调用来访问内核函数。

- IO库函数：系统调用只会读/写一系列字节，用户通常需要读/写字符、行记录或数据结构等。C库提供了一系列标准函数：
    - FILE mode I/O:fopen(), fread(), fwrite(), fseek(), fclose(), fflush()
    - char mode I/O:getc(), getchar(), ugetc(), putc(), putchar()
    - line mode I/O: gets(), fgets(), puts(), fputs()
    - formatted I/O: scanf(), fscanf(), sscanf();printf(), fprintf(), sprintf()
除了读写内存位置的sscanf()和sprintf()外，其他的IO函数都是建立在系统调用之上。

