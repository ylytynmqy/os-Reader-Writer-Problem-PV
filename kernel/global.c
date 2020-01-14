
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"
#include "string.h"


PUBLIC	PROCESS			proc_table[NR_TASKS];

PUBLIC	char			task_stack[STACK_SIZE_TOTAL];

PUBLIC int mutexlist[6][10];

PUBLIC	TASK	task_table[NR_TASKS] = {
    {TestA, STACK_SIZE_TESTA, "A"},
    {TestB, STACK_SIZE_TESTB, "B"},
    {TestC, STACK_SIZE_TESTC, "C"},
    {TestD, STACK_SIZE_TESTD, "D"},
    {TestE, STACK_SIZE_TESTE, "E"},
    {TestF, STACK_SIZE_TESTF, "F"}
};

PUBLIC	irq_handler		irq_table[NR_IRQ];

PUBLIC	system_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks,sys_disp_str,sys_my_delay,sys_semaphore_p,sys_semaphore_v};

