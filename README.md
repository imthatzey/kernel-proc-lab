# Kernel Proc Lab
This is a simple Linux kernel module that created a proc/seconds entry and reports the 
number of seconds elapsed since the module was loaded. 

# Collaborators
**Zeynep**: 
- Set up a UTM VM with kernel 7.0.0-31-generic
- Modified the proc_read() function to calculate elapsed time using jiffies and HZ.
- Built the seconds.c file by creating a Makefile, loaded it to the kernel using insmod, tested 
using cat /proc/seconds/ and unloaded rmmod to unload the module. 
- Initialized a local git repo
- Added README.md document to the project

**Bakhtawar**:
- Built the given hello_newKernel.c example that creates /proc/hello.
- Verified reading with cat /proc/hello
- Added the start_time variable inside proc_init() to track when the module was loaded
- Created a .gitignore to exclude some files. 
- Added the comments to the seconds.c file following the good programming style. 
