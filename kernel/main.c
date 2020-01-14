
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


static SEMAPHORE wsem;//文件是否被占有信号量
static SEMAPHORE rsem;//文件是否被占有信号量
static SEMAPHORE mutex1;//读者团内互斥
static SEMAPHORE mutex2;//写者之间互斥
static SEMAPHORE mutex3;//写者之间互斥
static SEMAPHORE mutex4;//写者之间互斥
static int readercount;//读者人数记录
static int writercount;
void clean(){//清屏
	disp_pos = 0;
	for(int i=0;i<160*25;i++){
		disp_str(" ");
	}
	disp_pos = 0;
}
void readc(){
	if(readercount){
			my_disp_str("READ ",FC);
			disp_int(readercount);
			my_disp_str("\n",FC);
	}
	else
	{
			my_disp_str("WRITE ",FC);
			my_disp_str("\n",FC);
	}
}
	
/*======================================================================*
                           sys_semaphore_p
 *======================================================================*/
PUBLIC  void sys_semaphore_p(SEMAPHORE * s){
	//入队
	p_proc_ready->inQueue=1;
	mutexlist[s->id][s->rear]=p_proc_ready->pid;
	s->rear=(s->rear+1)%10;
	// my_disp_str(p_proc_ready->p_name,WHITE);
	// my_disp_str(" has been blocked",WHITE);
	// my_disp_str("\n",WHITE);
	while(TRUE){
		if(p_proc_ready->inQueue==0){
			//现在到了
			break;
		}
		else{
			schedule();
		}
		
	}
}
/*======================================================================*
                           sys_semaphore_v
 *======================================================================*/
PUBLIC  void sys_semaphore_v(SEMAPHORE * s){
	//唤醒第一个
	PROCESS* p ;
	int pos=mutexlist[s->id][s->front];
	p=&proc_table[pos];

	s->front=(s->front+1)%10;
	p->inQueue = 0;

	// for(int i=0;i<10;i++){
	// 	disp_int(mutexlist[s->id][i]);
	// 	my_disp_str("\n",WHITE);
	// }
	// 	my_disp_str("----------\n",WHITE);
	// disp_int(p->pid);
	// my_disp_str("\n",WHITE);
	// 		my_disp_str("----------\n",WHITE);

	// my_disp_str(p->p_name,WHITE);
	// my_disp_str(" has been free",WHITE);
	// my_disp_str("\n",WHITE);
}
/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	clean();
	TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++) {
		strcpy(p_proc->p_name, p_task->name);	// name of the process
		p_proc->pid = i;			// pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs	= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ds	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.es	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.fs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ss	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
			| RPL_TASK;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

		proc_table[i].inQueue=0;//初始化
		proc_table[i].ticks  = 0;
		proc_table[i].lasttime=0;


		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}
	//删除优先级部分
	proc_table[0].timeSlice  = 2*MAGNITUDE;
	proc_table[1].timeSlice  = 3*MAGNITUDE;
	proc_table[2].timeSlice  = 3*MAGNITUDE;
	proc_table[3].timeSlice  = 3*MAGNITUDE;
	proc_table[4].timeSlice  = 4*MAGNITUDE;
	proc_table[5].timeSlice  = 1*MAGNITUDE;
	

	//initial all things
	readercount=0;//读者人数记录
	writercount=0;

	k_reenter = 0;
	ticks = 0;
	clean();
	wsem.value=1;
	rsem.value=1;
	mutex1.value=1;
	mutex2.value=1;
	mutex3.value=1;
	mutex4.value=1;
	wsem.id=0;
	rsem.id=1;
	mutex1.id=2;
	mutex2.id=3;
	mutex3.id=4;
	mutex4.id=5;
	for(int i=0;i<6;i++){
		for(int j=0;j<10;j++){
			mutexlist[i][j]=15;
		}
	}
		wsem.front=0;
		rsem.front=0;
	mutex1.front=0;
	mutex2.front=0;
	mutex3.front=0;
		wsem.rear=0;
		rsem.rear=0;
	mutex1.rear=0;
	mutex2.rear=0;
	mutex3.rear=0;

	//添加

	p_proc_ready	= proc_table+5;//进程结构表

        /* 初始化 8253 PIT */
        out_byte(TIMER_MODE, RATE_GENERATOR);
        out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
        out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));

        put_irq_handler(CLOCK_IRQ, clock_handler); /* 设定时钟中断处理程序 */
        enable_irq(CLOCK_IRQ);                     /* 让8259A可以接收时钟中断 */

	milli_delay(MAGNITUDE*1);
	restart();
	while(1){}

}
void P(SEMAPHORE * s){
	s->value--;
	if(s->value<0 && p_proc_ready->inQueue){
		semaphore_p(&s);//入队操作
	}
}
void V(SEMAPHORE * s){
	s->value++;
	if(s->value<=0){
		semaphore_v(&s);//选择其中一个出队操作
	}
}

//读者优先:一个读者试图进行读操作时，如果此时正有其他读者在进行操作，它可以直接开始读操作，不需等待
void Rwriter(char * name,int color,int lasting,int taskid){
	while(TRUE){
		// my_disp_str("wsem:",color);
		// disp_int(wsem.value);
		// my_disp_str("\n",color);
		P(&wsem);
		//write
		my_disp_str(name,color);
		my_disp_str(" starts writing",color);
		my_disp_str("\n",color);
		// readc();
		my_delay(lasting, taskid);
		milli_delay(lasting);
		// readc();
		//finish
		my_disp_str(name,color);
		my_disp_str(" finished",color);
		my_disp_str("\n",color);
		V(&wsem);
		// readc();
	}
}
void Rreader(char * name,int color,int lasting,int taskid){
	while(TRUE){
		//判断读者团是否满了
		// P(&ceilingmutex);
		// my_disp_str("readers: ",WHITE);
		// disp_int(readercount);
		// my_disp_str(" \n",WHITE);
		P(&mutex4);
		if(readercount<CEILING){
					// V(&ceilingmutex);
			
			// my_disp_str("yes\n",WHITE);
			P(&mutex1);
				readercount++;
				V(&mutex4);
				if(readercount==1){P(&wsem);}
			V(&mutex1);		
			//read
			// readc();
			my_disp_str(name,color);
			my_disp_str(" starts reading",color);
			my_disp_str("\n",color);

			my_delay(lasting,taskid);
			milli_delay(lasting);
			//finish
			// readc();
			my_disp_str(name,color);
			my_disp_str(" finished",color);
			my_disp_str("\n",color);			

			P(&mutex1);
				readercount--;
				if(readercount==0){V(&wsem);}
			V(&mutex1);
		}
		else
		{
			V(&mutex4);
		}
	}
		// while(1){}
}

//写者优先:一个读者试图进行读操作时，如果有其他写者在等待或进行写操作，它必须等待
void Wwriter(char * name,int color,int lasting,int taskid){
	while(TRUE){
		//start
		P(&mutex2);
			writercount++;
			if(writercount==1){P(&rsem);}
		V(&mutex2);
		
		P(&wsem);
		// write
		disp_color_str(name,color);
		disp_color_str(" starts writing",color);
		disp_color_str("\n",color);
		// readc();
		my_delay(lasting,taskid);
		milli_delay(lasting);
		//finish
		disp_color_str(name,color);
		disp_color_str(" finished",color);
		disp_color_str("\n",color);
		V(&wsem);
		// readc();
		P(&mutex2);
			writercount--;
			if(writercount==0){V(&rsem);}
		V(&mutex2);
	}
}
void Wreader(char * name,int color,int lasting,int taskid){
	while(TRUE){
		//判断读者团是否满了
		P(&mutex4);
		if(readercount<CEILING){
			//可进团
			P(&mutex3);
				P(&rsem);
					P(&mutex1);
						readercount++;
						V(&mutex4);
						if(readercount==1){P(&wsem);}
					V(&mutex1);
				V(&rsem);
			V(&mutex3);
			// readc();
			// read
			disp_color_str(name,color);
			disp_color_str(" starts reading",color);
			disp_color_str("\n",color);

			my_delay(lasting,taskid);
			milli_delay(lasting);
			//finish
			// readc();
			disp_color_str(name,color);
			disp_color_str(" finished",color);
			disp_color_str("\n",color);

			P(&mutex1);
				readercount--;
				if(readercount==0){V(&wsem);}
			V(&mutex1);
			// readc(); 
		}
		else
		{
			V(&mutex4);
		}
		
	}
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()//读者
{
	Rreader("A",RED,proc_table[0].timeSlice,0);
	// Wreader("A",RED,proc_table[0].timeSlice,0);
}
/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()//读者
{
	Rreader("B",GREEN,proc_table[1].timeSlice,1);
	// Wreader("B",GREEN,proc_table[1].timeSlice,1);
}
/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()//读者
{
	Rreader("C",CYAN,proc_table[2].timeSlice,2);
	// Wreader("C",CYAN,proc_table[2].timeSlice,2);
}
/*======================================================================*
                               TestD
 *======================================================================*/
void TestD()//写者
{
	Rwriter("D",YELLOW,proc_table[3].timeSlice,3);
	// Wwriter("D",YELLOW,proc_table[3].timeSlice,3);
}
/*======================================================================*
                               TestE
 *======================================================================*/
void TestE()//写者
{
	Rwriter("E",PURPLE,proc_table[4].timeSlice,4);
	// Wwriter("E",PURPLE,proc_table[4].timeSlice,4);
}
/*======================================================================*
                               TestF
 *======================================================================*/
void TestF()//普通
{
	while(TRUE){
		P(&mutex4);
		my_disp_str("F ",FC);
		if(readercount>0){
			my_disp_str("READ ",FC);
			disp_int(readercount);
			my_disp_str("\n",FC);
		}
		else{
			my_disp_str("WRITE\n",FC);
		}
		V(&mutex4);
		// my_delay(proc_table[5].timeSlice,5);
		milli_delay(proc_table[5].timeSlice);
	}
}
