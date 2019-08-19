#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <asm/current.h>
#include <asm/thread_info.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/init_task.h>

#define DEVICE_NAME "FindAllTasks"
#define CLASS_NAME "Find"

MODULE_LICENSE("GPL");	                 /// <the license type -- this affect runtime behavior
MODULE_AUTHOR("ALEX"); 			 /// <The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux LM that finds a given task name");  ///<the description -- see modinfo
MODULE_VERSION("0.1");	                 ///< the verison of the module



static char buffer[5]="aaaa";
static char*name="init"; // default to init if no parameter is passed
static int numberOpens = 0;
static int majorNumber;
static struct class* driverClass = NULL;
static struct device* driverDevice = NULL;

module_param(name, charp , S_IRUGO);
///<Param_desc = char ptr, S_IRUGO can be read/not changed
MODULE_PARM_DESC(name,"the name of the task to find");
///<parameter description

/**@brief The LKM initialization function
 * The static keyword restricts the visibility of the function to within this C fie. The __init
 * macro means the functions is only used at initialization
 * time and that it can be discarded and its memory freed up after that point
 * @return returns 0 if succesful
 */ 
 
 static int dev_open(struct inode *inode, struct file *filep);
 static int dev_release(struct inode *inode, struct file *filep);
 //static ssize_t dev_read(struct file *filep, char *buffer ,size_t count, loff_t *offset);
 static struct file_operations fops =
 {
	 .open = dev_open,
	 //.read = dev_read,
	 .release = dev_release
 };

static int __init findTask_init(void)
{

	printk(KERN_INFO "Loading findTask module!\n");
	
	majorNumber = register_chrdev(0,DEVICE_NAME, &fops);
	if(majorNumber <0){
		printk(KERN_ALERT "TeshChar failed to register major number\n");
		return majorNumber;
	}

	//register device class
	driverClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(driverClass)){
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(driverClass);
	}
	
	//register the device driver
	 driverDevice = device_create(driverClass, NULL, MKDEV(majorNumber,0),NULL,DEVICE_NAME);
	if(IS_ERR(driverDevice)){
		class_destroy(driverClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create device\n");
		return PTR_ERR(driverDevice);
	}
	
	printk(KERN_INFO "Testchar: device class created correctly\n");
	
	///END OF DRIVER INITIALIZATION/////
	
	printk("Current process: %s, pid: %d, Addr:%x\n", current->comm, current->pid, current);
	
	
	/*    
	if (flag)		//match
	{
	    printk("Found process %s with pid [%d]\n",name, taskPID);
	}
	else			//no match
	{
	    printk("Not Found process %s",name);
	}
	*/

	//findTask();
	return 0;
}


/**brief The LKM cleanup function
 * Similar to the initialization function, it is static. The __exit macro notifies if this
 * code is used for a built-in driver ( not a LKM) that this function is not required.
 */ 

static void __exit findTask_exit(void){
	device_destroy(driverClass,MKDEV(majorNumber,0));
	class_unregister(driverClass);
	class_destroy(driverClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "Removing findTask loadable module!\n");
 }

static int dev_open(struct inode *inode, struct file *filep){
	 numberOpens++;
	 
	 struct thread_info *ThreadInfo;
	 
	  
	 printk("Opened findTask Device driver: %d\n", numberOpens);
	 printk("Current Process: %s, pid: %d, address:%x\n", current->comm, current->pid, current);
	 
	 struct task_struct *task; // task structure 

	//macro for looping through task strcuture   
	for_each_process(task)
	{
		//getting task name and checking for a match 
		//currentTask = task->comm; 
		printk("Found process %s with pid [%d], Address:%x\n",task->comm, task->pid, task);
		printk("String name addr:%x pid addr:%x, Derreferenced Address:%x\n\n",&(task->comm), &task->pid, *task);
		
	} 
	
	ThreadInfo  = current_thread_info();
		printk("Thread Info addr:%x Thread_info.task addr:%x cpu #:%d, cpu # addr:%x\n\n",ThreadInfo,&(ThreadInfo->task), ThreadInfo->cpu, &(ThreadInfo->cpu));
		printk("Cpu context Registers:\nR8:%d\nR13(SP):%d\n",ThreadInfo->cpu_context.r8,ThreadInfo->cpu_context.sp);
	
	 printk("Current Process: %s, pid: %d, address:%x\n", current->comm, current->pid, current);
	 printk("Init task: %s, pid: %d, address:%x\n", init_task.comm, init_task.pid, &init_task);
	 printk("Init task next children addr: %x\n", init_task.children.next);
	 printk("Init task prev children addr: %x\n", init_task.children.prev);
	 
	 
	struct task_struct *p;
	p =  &init_task;
	p = list_entry_rcu((p)->tasks.next, struct task_struct, tasks);
	
    printk("Init task addr: %x, Name:%s\n", p,p->comm);
    printk("Tasks next addr: %x\n", (((ulong)&(p->tasks))- (ulong)p));
    
    p = list_entry_rcu((p)->tasks.prev,struct task_struct, tasks);
    printk("Tasks prev addr: %x\n", (((ulong)&(p->tasks.prev))- (ulong)p));
    printk("Init task prev addr: %x, Prev Name:%s\n", p,p->comm);

	
	 
	 return 0;
}
 
 static int dev_release(struct inode *inode, struct file *filep)
 {
	 printk("Closed device driver\n");
	 return 0;
 }
 /*static ssize_t dev_read(struct file *filep, char *buffer ,size_t count, loff_t *offset)
 {
	 }
*/
/**@brief A module must use the module_init() module_exit() macros form linux/init.h, which 
 * identify the initialization function at insertion time and the cleanup 
 * function(as listed    above)
 */ 


module_init(findTask_init);
module_exit(findTask_exit);

