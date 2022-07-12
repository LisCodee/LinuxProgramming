# 第二章 编程背景

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
(cd DIR;$(MAKE)) or cd DI
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

## 4.4 用户级线程
