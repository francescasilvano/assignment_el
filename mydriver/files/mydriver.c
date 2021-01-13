#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include "data.h"
static dev_t mydriver_dev;

struct cdev mydriver_cdev;

struct class *myclass = NULL;

static char buffer[64];
static int indice=0;
ssize_t mydriver_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
//static int mydriver_probe(struct platform_device *pdev);
//static int mydriver_remove(struct platform_device *pdev);

struct file_operations mydriver_fops = {
    .owner = THIS_MODULE,
    .read = mydriver_read,
};

/*static struct of_device_id my_match_table[]={
	{.compatible= "mydriver", },
	{}
	
};*/

/*MODULE_DEVICE_TABLE(of, my_match_table);
static struct platform_driver my_platform_driver = {
	.probe= mydriver_probe,
	.remove=mydriver_remove,
	.driver={
		.name="mydriver",
		.owner= THIS_MODULE,
		.of_match_table=my_match_table,
	},
};
*/


/*static int mydriver_probe(struct platform_device *pdev){

	struct device *dev=&pdev->dev;
	struct device_node *np= dev->of_node;
	int device_enable;
	struct resource res;
	dev_info(dev,"[mydriver] probe\n");
	
	if(of_property_read_u32(np,"device-enable",&device_enable)){
		dev_err(dev,"device-enable\n");
		return -EINVAL;
	}
	dev_info(dev,"[mydriver] device-enable set to %d\n",device_enable);
	if(of_address_to_resource(pdev->dev.of_node,0,&res)){
		dev_info(dev,"[mydriver] error on of_address_to_resource");
		return -EINVAL;
	}
	dev_info(dev,"[mydriver] addr=%08x\n",(int)res.start);
	dev_info(dev,"[mydriver] size=%08x\n",(int)resource_size(&res));

	alloc_chrdev_region(&mydriver_dev, 0, 1, "mydriver_dev");
    	dev_info(&pdev->dev,"[mydriver] %s\n", format_dev_t(buffer, mydriver_dev));

    	myclass = class_create(THIS_MODULE, "mydriver_sys");
    	device_create(myclass, NULL, mydriver_dev, NULL, "mydriver_dev");

    cdev_init(&mydriver_cdev, &mydriver_fops);
    mydriver_cdev.owner = THIS_MODULE;
    cdev_add(&mydriver_cdev, mydriver_dev, 1);
	
}
static int mydriver_remove(struct platform_device *pdev){
	dev_info(&pdev->dev, "[mydriver] remove \n");
	cdev_del(&mydriver_cdev);
	unregister_chrdev_region(mydriver_dev,1);
	return 0;
}*/

ssize_t mydriver_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int result;
    if(indice>=2048)
	indice=0;
    result=ppg[indice];
   // printk(KERN_INFO "[mydriver] read (count=%d, offset=%d)\n", (int)indice, result );
    indice++;
    return result;
}



static int __init mydriver_module_init(void)
{
    printk(KERN_INFO "Loading mydriver_module\n");
    
    //return platform_driver_register(&my_platform_driver);
    alloc_chrdev_region(&mydriver_dev, 0, 1, "mydriver_dev");
    printk(KERN_INFO "%s\n", format_dev_t(buffer, mydriver_dev));

    myclass = class_create(THIS_MODULE, "mydriver_sys");
    device_create(myclass, NULL, mydriver_dev, NULL, "mydriver_dev");

    cdev_init(&mydriver_cdev, &mydriver_fops);
    mydriver_cdev.owner = THIS_MODULE;
    cdev_add(&mydriver_cdev, mydriver_dev, 1);


    return 0;
}

static void __exit mydriver_module_cleanup(void)
{
    printk(KERN_INFO "Cleaning-up mydriver_dev.\n");
    //platform_driver_unregister(&my_platform_driver);
    device_destroy(myclass, mydriver_dev );
    cdev_del(&mydriver_cdev);
    class_destroy(myclass);
    unregister_chrdev_region(mydriver_dev, 1);
}

module_init(mydriver_module_init);
module_exit(mydriver_module_cleanup);

MODULE_AUTHOR("...");
MODULE_LICENSE("GPL");
