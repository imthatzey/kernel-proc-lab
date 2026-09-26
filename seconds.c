#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

// Stores the jiffies value at the start when the module is loaded, 
// this is used later in the calculation of en_time (elapsed time)
static unsigned long start_time;

// Function prototype for proc_read
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

// Defined which function does hte reads on /proc/seconds
static const struct proc_ops my_proc_ops = {
        .proc_read = proc_read,
};

// proc_init is called once when the module is loaded with insmod.
// Records the current jiffies count as the start_time and creates the
// /proc/seconds 
int proc_init(void)
{
	start_time = jiffies;

    proc_create(PROC_NAME, 0, NULL, &my_proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

// proc_exit is called when the module is removed (rmmod)
// removes the module from the kernel so it no longer appears. 
void proc_exit(void) {
	
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

// proc_read is called when the user runs cat /proc/seconds command
// The kernel calls this function repeatedly until it returns 0. 
// so the completed flag is used to return the message once then it returns 0
// to signal that there is no more data
// Calculated the en_time by subtracting the initial jiffies count from the jiffies
// count right now. Then we convert it the ticks to seconds by dividing by HZ. 
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
	unsigned long en_time = (jiffies - start_time) / HZ;

	int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
            completed = 0;
            return 0;
    }

    completed = 1;

	// Formating the elapsed seconds message into the buffer.
    rv = sprintf(buffer, "Seconds since the kernel module was loaded: %lu\n", en_time);

    // Copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Reports seconds elapsed since module was loaded with /proc/seconds");
MODULE_AUTHOR("SGG");
