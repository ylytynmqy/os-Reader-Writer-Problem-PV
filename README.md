# 基于《oranges》第六章代码的读者写者问题实现

全部基于第六章r做的

# first Task

*添加系统调用 打印字符串，接受 char 型参数 str – 注意，第六章代码已经在 kliba.asm 文件中有了 disp_str 函数显示字符串，但这是内核函数。请实现并包装成相应的系统调用。*

添加PUBLIC	void	my_disp_str(char * str);

## kernel/syscall.asm

INT_VECTOR_SYS_CALL equ 0x90 沿用原书 中断号90

来定义INT_VECTOR_SYS_CALL equ对应的中断门

添加my_disp_str函数

并在导出符号添加my_disp_str

添加_NP_my_disp_str	equ 1

## kernel/proc.c

是这个sys_call函数实现的真正地方//这里可以用c语言来写 也可以用0x80h的系统调用

在这里实现sys_disp_str：调用kliba.asm实现的void disp_str(char * info);

## kernel/global.c

sys_call_table添加sys_disp_str

## include/proto.h

添加PUBLIC void sys_disp_str(char * str);

添加PUBLIC	void	my_disp_str(char * str);

## kernel/kernel.asm

因为第六章源代码系统调用不涉及参数，所以根据第八章修改sys_call函数使其支持和linux 系统调用一样支持四个参数

## include/const.h

NR_SYS_CALL 修改为2

# second task

*添加系统调用接受 int 型参数 milli_seconds ，调用此方法进程会在 milli_seconds 毫秒内 不被分配时间片。 – 注意，第六章代码已经在 clock.c 中有方法 mills_delay ，这个方法仍然为进程分配了 时间片，只不过进程进入空循环。*

PUBLIC void  my_delay(int milli_seconds);

## kernel/main.c



## kernel/syscall.asm

INT_VECTOR_SYS_CALL equ 0x90 沿用原书 中断号90

来定义INT_VECTOR_SYS_CALL equ对应的中断门

添加my_disp_str函数

并在导出符号添加my_disp_str

添加_NP_my_disp_str	equ 1

----

添加_NR_process_delay	equ	2

添加my_delay函数

并在导出符号添加my_delay

## kernel/proc.c

是这个sys_call函数实现的真正地方//这里可以用c语言来写 也可以用0x80h的系统调用

在这里实现sys_disp_str：调用kliba.asm实现的void disp_str(char * info);

----

在这里实现sys_my_delay；详情见代码

## kernel/global.c

sys_call_table添加sys_disp_str

---

sys_call_table添加sys_my_delay

## include/proto.h

添加PUBLIC void sys_disp_str(char * str);

添加PUBLIC	void	my_disp_str(char * str);

---

PUBLIC void   sys_my_delay(int milli_seconds);

PUBLIC void  my_delay(int milli_seconds);

## kernel/kernel.asm

因为第六章源代码系统调用不涉及参数，所以根据第八章修改sys_call函数使其支持和linux 系统调用一样支持四个参数

## include/const.h

NR_SYS_CALL 修改为2

---

NR_SYS_CALL 修改为3

# third task 

添加两个系统调用执行信号量 PV操作，在此基础上模拟 读者写者问题。

注意打印需要开始、读/写、结束三个打印

1. 共有 6 个一直存在的进程（循环读写操作）
   - A 读者进程 阅读消耗 2 个时间片 
   - B 读者进程 阅读消耗 3 个时间片
   - C 读者进程 阅读消耗 3 个时间片 
   - D 写者进程 写消耗 3 个时间片
   - E 写者进程 写消耗 4 个时间片
   - F 普通进程
2. 读者在读的时候，写者不能写，必须等到全部读者读完
3. 同时只能一个作者在写
4. 在写的时候，读者不能读
5. 多个读者可以读一本书，但是不能太多，上限数字有 1、2、3，需要都能够支持，并 且可以现场修改
6. A、B、C、D、E 进程需要彩色打印基本操作：读开始、写开始、读、写、读完成、写 完成，以及对应进程名字
7. F 每隔 1 个时间片打印当前是读还是写，如果是读有多少人 
8. 请分别实现读者优先和写者优先，需要都能够支持，并且可以现场修改 
9. 请想办法解决此问题中部分情况下的进程饿死问题 (可参考第六章)

- 只关心资源是否被谁占有 不关心资源的数量

- 读写互斥

- 写者互斥
- 写者关心资源是否被占有
- 读者作为团体（当然有ceiling上界）
- 第一个读者 关心文件是否被占有 创建读者团体 占有文件
- 中间读者 查看读者团体是否已满 加入读者团体
- 最后一个读者 查看读者团体是否已满 加入读者团体 释放文件

## kernel/main.c

添加六个进程ABCDEF

删除优先级设定

## kernel/syscall.asm

INT_VECTOR_SYS_CALL equ 0x90 沿用原书 中断号90

来定义INT_VECTOR_SYS_CALL equ对应的中断门

添加my_disp_str函数

并在导出符号添加my_disp_str

添加_NP_my_disp_str	equ 1

----

添加_NR_process_delay	equ	2

添加my_delay函数

并在导出符号添加my_delay

----

添加semaphore_p，semaphore_v函数

添加 _NR_semaphore_p equ 3

添加 _NR_semaphore_v equ 4

global semaphore_p

global semaphore_v

## kernel/proc.c

是这个sys_call函数实现的真正地方//这里可以用c语言来写 也可以用0x80h的系统调用

在这里实现sys_disp_str：调用kliba.asm实现的void disp_str(char * info);

----

在这里实现sys_my_delay；详情见代码

---

在这里实现PUBLIC int   sys_semaphore_p(int name);

PUBLIC int   sys_semaphore_v(int name);

## kernel/global.c

sys_call_table添加sys_disp_str

---

sys_call_table添加sys_my_delay

----

task_table[NR_TASKS]添加ABCDEF信息

sys_call_table添加sys_semaphore_p,sys_semaphore_v

## include/proto.h

添加PUBLIC void sys_disp_str(char * str);

添加PUBLIC	void	my_disp_str(char * str);

---

PUBLIC void   sys_my_delay(int milli_seconds);

PUBLIC void  my_delay(int milli_seconds);

----

/* main.c */

void TestA、B、C、D、E、F();

/* stacks of tasks */

\#define STACK_SIZE_TESTA  0x8000等六个

/* Number of tasks */

\#define NR_TASKS  6

\#define STACK_SIZE_TOTAL  (STACK_SIZE_TESTA *6)

/* syscall.asm */

PUBLIC void  semaphore_p(int name);

PUBLIC void  semaphore_v(int name);

/* proc.c */

PUBLIC int   sys_semaphore_p(int name);

PUBLIC int   sys_semaphore_v(int name);

## kernel/kernel.asm

因为第六章源代码系统调用不涉及参数，所以根据第八章修改sys_call函数使其支持和linux 系统调用一样支持四个参数

## include/const.h

NR_SYS_CALL 修改为2

---

NR_SYS_CALL 修改为3

---

NR_SYS_CALL 修改为5

添加对于semaphore两个函数名字的定义，见详情

# 其他实现

## 清屏幕

kernel/main.c void clean();

## 打印时清屏

自己编写的disp函数 判断是否满屏即dispos是否到达最后一行 则清屏



## 问题与解决

出现reader死锁

其实不是死锁而是

int readercount;//读者人数记录

int writercount;

这两个变量在main()外

​	例如 int readercount=0;//读者人数记录

​			int writercount=0;

这样不行

要main()初始化

队列最后用静态列表实现