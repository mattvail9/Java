/*
* File: processes.c
* Matthew Vaillancourt - OSI.S21
* Purpose: Prints all processes - a simple kernel module.
*/
#include <linux/module.h> /* for all kernel modules */
#include <linux/kernel.h> /* for KERN_INFO */
#include <linux/init_task.h>
#include <linux/list.h>
#include <linux/sched.h>

int init_module( void ) /* module entry point */
{
	int process_cnt = 0;
	struct task_struct *task;
	struct task_struct *init = &init_task;

	printk( KERN_INFO" Hello, Matt Vaillancourt\n" );
	printk( KERN_INFO "Init Task:\nName: %-20s Task ID: [%-8d] Priority: %-4d State: %+5ld Policy: %d CPU: %d, Parent Name: %-16s Parent ID: [%-6d]\n",
	init->comm, 
	init->pid, 
	init->normal_prio,
	init->state,
	init->policy,
	init->cpu,
	init->group_leader->comm, 
	task_ppid_nr(init));

	list_for_each_entry(task, &current->tasks, tasks)
	{
		pr_info("Name: %-20s Task ID: [%-8d] Priority: %-4d State: %+5ld Policy: %d CPU: %d, Parent Name: %-16s Parent ID: [%-6d]\n", 
			task->comm, 
			task->pid, 
			task->normal_prio, //double check
			task->state,
			task->policy,
			task->cpu,
			task->group_leader->comm, 
			task_ppid_nr(task));
		process_cnt = process_cnt + 1;
	}

	pr_info("Current Task: \nName: %-20s Task ID: [%-8d] Priority: %-4d State: %+5ld Policy: %d CPU: %d, Parent Name: %-16s Parent ID: [%-6d]\n",
	current->comm, 
	current->pid, 
	current->normal_prio,
	current->state,
	current->policy,
	current->cpu,
	current->group_leader->comm, 
	task_ppid_nr(current));

	pr_info("TOTAL PROCESS COUNT: %d\n", process_cnt );
	return 0;
}

void cleanup_module( void ) /* module exit point */
{
	pr_info("Goodbye, Matt Vaillancourt\n" );
	return;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Prints processes");
MODULE_AUTHOR("Matt Vaillancourt");
