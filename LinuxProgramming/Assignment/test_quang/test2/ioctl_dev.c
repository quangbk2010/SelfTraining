#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
  
#include <linux/fs.h>            
#include <linux/errno.h>         
#include <linux/types.h>         
#include <linux/fcntl.h>         
  
#include <asm/uaccess.h>  
#include <asm/io.h>  
  
#include "ioctl_test.h"  
  
#define   IOCTLTEST_DEV_NAME            "ioctldev"  
#define   IOCTLTEST_DEV_MAJOR            240  

static unsigned char led_status;
static ioctl_test_info kernel_ctrl_info;

int ioctltest_open (struct inode *inode, struct file *filp)  
{  
    return 0;  
}  
  
int ioctltest_release (struct inode *inode, struct file *filp)  
{  
    return 0;  
}  
  
int ioctltest_ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)  
{  
  
    ioctl_test_info   ctrl_info;  
    int               err, size;  
    int               loop;  
      
    if( _IOC_TYPE( cmd ) != IOCTLTEST_MAGIC ) return -EINVAL;  
    if( _IOC_NR( cmd )   >= IOCTLTEST_MAXNR ) return -EINVAL;  
      
    size = _IOC_SIZE( cmd );   
      
    if( size )  
    {  
        err = 0;  
        if( _IOC_DIR( cmd ) & _IOC_READ  ) err = access_ok( VERIFY_WRITE, (void *) arg, size );  
        else if( _IOC_DIR( cmd ) & _IOC_WRITE ) err = access_ok( VERIFY_READ , (void *) arg, size );  
              
        if( err ) return err;          
    }  
            
    switch( cmd )  
    {            
		case IOCTLTEST_READ       : 
								copy_to_user ( (void *) arg, (const void *) &kernel_ctrl_info, (unsigned long ) kernel_ctrl_info.size );    
								break;  
								  
		case IOCTLTEST_WRITE      : 
								copy_from_user ( (void *)&ctrl_info, (const void *) arg, size );   
								for( loop = 0; loop < size; loop++ )
								{
									kernel_ctrl_info.buff[loop] = ctrl_info.buff[loop]; 
								}
								printk ("%X\n", kernel_ctrl_info.buff);
								kernel_ctrl_info.size = size;
								break;  
    }  
  
    return 0;  
}  
  
struct file_operations ioctltest_fops =  
{  
    .owner    = THIS_MODULE,  
    .unlocked_ioctl    = ioctltest_ioctl,  
    .open     = ioctltest_open,       
    .release  = ioctltest_release,    
};  
  
int ioctltest_init(void)  
{  
    int result;  
  
    printk("ioctltest_init \n");
    result = register_chrdev( IOCTLTEST_DEV_MAJOR, IOCTLTEST_DEV_NAME, &ioctltest_fops);  
    if (result < 0) return result;  
  
    return 0;  
}  
  
void ioctltest_exit(void)  
{  
    printk("ioctltest_exit \n");
    unregister_chrdev( IOCTLTEST_DEV_MAJOR, IOCTLTEST_DEV_NAME );  
}  
  
module_init(ioctltest_init);  
module_exit(ioctltest_exit);  
  
MODULE_LICENSE("Dual BSD/GPL");  
  
