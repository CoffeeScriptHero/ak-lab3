#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>

MODULE_AUTHOR("Denys Kozarenko");
MODULE_DESCRIPTION("Module prints hello world");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int num = 1;
module_param(num, uint, 0644);
MODULE_PARM_DESC(num, "counter of 'Hello world' printing");

struct list_s {
	struct list_head list;
	ktime_t timestamp;
};
static LIST_HEAD(list_head);

static int __init hello_init(void)
{
	if (num > 10) {
		pr_err("num is greater then 10\n");
		return -EINVAL;
	}
	else if (num == 0 || (num >= 5 && num <= 10))
		pr_warn("num is 0 or in range [5, 10]\n");

	int i;

	for (i = 0; i < num; i++) {
		struct list_s *list_ex = kmalloc(sizeof(*list_ex), GFP_KERNEL);

		list_ex->timestamp = ktime_get();
		list_add(&list_ex->list, &list_head);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct list_s *list_ex, *temp;

	list_for_each_entry_safe(list_ex, temp, &list_head, list) {
		pr_info("Time: %lld\n", list_ex->timestamp);
		list_del(&list_ex->list);
		kfree(list_ex);
	}
}

module_init(hello_init);
module_exit(hello_exit);
