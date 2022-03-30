#include <linux/kernel.h>
#include <unistd.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

struct proc_info {
	pid_t pid;
	char name[16];
}

struct procinfos {
	long studentID;
	struct proc_info proc;
	struct proc_info parent_proc;
	struct proc_info oldest_child_proc;
};

asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos *info){
	struct task_struct *task = pid_task ( find_vpid ( pid ) , PIDTYPE_PID);
	printk("Finding...\n");
	for_each_process(task) {
		printk("[%d] ------- [%s]\n", task->pid, task->comm);
		if(task->pid == pid) {
			if(task->mm != NULL){
				//struct proc_info proc;
				struct procinfos buff;
				buff.studenID = 1512936;
				buff.proc.pid = task->pid;
				buff.proc.name = task->comm;
				buff.parent_proc.pid = task->real-parent->pid;
				buff.oldest_child_proc.pid = task->children->pid;
				int res = copy_to_user(info, &buff, sizeof(buff));
				if(res == 0) printk("success!");
				else printk("fail");
				return 0;
			}
		}
	}
	return -1;
}