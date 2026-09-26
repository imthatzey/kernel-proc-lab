/**
* linux distribution info?
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

// Global variable to save jiffies when the module is loaded:
static unsigned long start_time;

// Function prototypes
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static const struct proc_ops my_proc_ops = {
        .proc_read = proc_read,
};

// This function is called when the module is loaded.
int proc_init(void)
{
	// Save current value of jiffies
	start_time = jiffies;

    proc_create(PROC_NAME, 0, NULL, &my_proc_ops);
	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

// This function is called when the module is removed.
void proc_exit(void) {

        // Removes the /proc/seconds entry
        remove_proc_entry(PROC_NAME, NULL);

        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/seconds is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
	// Save the difference between the current value of jiffies and the start_time, then divide by the HZ rate to find the seconds elapsed since loading the module
	unsigned long en_time = (jiffies - start_time) / HZ;

    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
            completed = 0;
            return 0;
    }

    completed = 1;

	// Make the message that prints out the elapsed time
    rv = sprintf(buffer, "Seconds since the kernel module was loaded: %lu\n", en_time);

    // Copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
