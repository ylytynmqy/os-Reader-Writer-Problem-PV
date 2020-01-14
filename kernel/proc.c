
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
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
#include "proto.h"


/*======================================================================*
                              schedule
 *======================================================================*/
PUBLIC void schedule()
{
	// my_disp_str("now: ",WHITE);
	// my_disp_str(p_proc_ready->p_name,WHITE);
	// my_disp_str("\n",WHITE);
	// PROCESS * osd=&proc_table[5];
	// if(osd->ticks<=get_ticks()){
		// p_proc_ready=osd;
	// }
	// else{
		// ticks++;
		// while(TRUE){
		// 	my_disp_str("ticks:",WHITE);
		// 	disp_int(get_ticks());
		// 	my_disp_str("\n",WHITE);
		// }
		PROCESS *choice=p_proc_ready;
		choice++;
		if(choice>=proc_table+NR_TASKS){
			choice=proc_table;
		}
		get_ticks();
		get_ticks();
		get_ticks();
		get_ticks();
		get_ticks();
		get_ticks();
		get_ticks();
		for (PROCESS *p = proc_table; p < proc_table+NR_TASKS; p++) {// TODO TO NEXT
			if(choice->lasttime > p->lasttime){
				choice=p;
				// my_disp_str("yes\n",13);
			}
		}
		get_ticks();		
		get_ticks();		
		get_ticks();		
		get_ticks();		
		get_ticks();		
		p_proc_ready=choice;
		p_proc_ready->lasttime=get_ticks();
	// }
	
		// disp_int(t);
		// my_disp_str("   ",WHITE);
		// disp_int(choice->ticks);
		// my_disp_str("\n",WHITE);

	// my_disp_str("choose: ",WHITE);
	// my_disp_str(p_proc_ready->p_name,WHITE);
	// disp_str("\n");
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}
/*======================================================================*
                           sys_disp_str
 *======================================================================*/
 
PUBLIC void sys_disp_str(char * str,int color){
	//一行160*25
	if(disp_pos>160*24){
		disp_pos=0;
		for(int i=0;i<160*25;i++){
			disp_str(" ");
		}
		disp_pos=0;
	}
	disp_color_str(str,color);
}
/*======================================================================*
                           sys_process_delay
 *======================================================================*/

PUBLIC void sys_my_delay(int milli_seconds,int id){
	int first=get_ticks();
	p_proc_ready->ticks = (first+milli_seconds*1000/HZ);
	// my_disp_str("\n",WHITE);
	while (1)
	{
		if(p_proc_ready->ticks<=get_ticks() && p_proc_ready->pid==id){
			break;
		}
		else
		{
			schedule();
		}
		
	}
}
