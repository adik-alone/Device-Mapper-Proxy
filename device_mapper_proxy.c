#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/bio.h>
#include<linux/device-mapper.h>
#include<linux/sysfs.h>
#include<linux/kobject.h>

static struct my_stat_info {
	size_t my_write_req;
	size_t my_read_req;
	unsigned int my_avg_blk_wrt;
	unsigned int my_avg_blk_rd;
	size_t my_total_req;
	unsigned int my_avg_blk_size;
	size_t my_read_size;
	size_t my_write_size;
	size_t my_total_size;
};

static struct kobject *stat_kobj;
static struct my_stat_info *my_stat;


struct my_dm_target {
	struct dm_dev *dev;
	sector_t start;
};

static int dmp_map(
		struct dm_target *ti,
		struct bio *bio
		)
{
	struct my_dm_target *mdt = (struct my_dm_target *) ti->private;
	printk(KERN_CRIT "\n<<in function dmp_map \n");
	
	bio_set_dev(bio, mdt->dev->bdev);

	bio->bi_iter.bi_sector += mdt->start;

	unsigned int blk_size = bio->bi_iter.bi_size;
	
	printk(KERN_INFO "map: bi_size = %u\n", blk_size);

	if (bio_data_dir(bio) == WRITE)
	{
		printk(KERN_CRIT "map: bio is a write request.... \n");
		my_stat->my_write_req++;
		my_stat->my_write_size += blk_size;
		my_stat->my_avg_blk_wrt = my_stat->my_write_size / my_stat->my_write_req;
		
	}
	else if(bio_data_dir(bio) == READ)
	{
		printk(KERN_CRIT "map: bio is a read request.... \n");
		my_stat->my_read_req++;
		my_stat->my_read_size += blk_size;
		my_stat->my_avg_blk_rd = my_stat->my_read_size / my_stat->my_read_req;
		
	}

	my_stat->my_total_req++;
	my_stat->my_total_size += blk_size;
	my_stat->my_avg_blk_size = my_stat->my_total_size / my_stat->my_total_req;

	submit_bio(bio);

	printk(KERN_CRIT "\n>>out function dmp_map \n");

	return DM_MAPIO_SUBMITTED;
}


static int
dmp_ctr(
		struct dm_target *ti,
		unsigned int argc,
		char **argv
		)
{
	struct my_dm_target *mdt;
	unsigned long long start;
	
	printk(KERN_CRIT "\n >>in function dmp_ctr \n");
	if (argc != 1)
	{
		ti->error = "Invalid argument count";
		return -EINVAL;
	}

	mdt = kmalloc(sizeof(struct my_dm_target), GFP_KERNEL);
	
	if (!mdt)
	{
		printk(KERN_ERR "dm-dmp: Mdt is Null\n");
		ti->error = "dm-dmp: Cannot allocate linear context";
		return -ENOMEM;
	}
	
	mdt->start = 0;

	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &mdt->dev))
	{
		ti->error = "dm-dmp: Device lookup failed";
		goto bad;
	}
	
	ti->private = mdt;
	
	my_stat = kzalloc(sizeof(struct my_stat_info), GFP_KERNEL);
	
	if(!my_stat)
	{
		printk(KERN_ERR "dm-dmp: Failed to malloc my_stat\n");
		goto bad;
	}
	
	printk(KERN_CRIT "\n >>out function dmp_ctr \n");
	return 0;
	
bad:
	kfree(mdt);
	printk(KERN_CRIT "\n>>out function dmp_ctr with error\n");
	return -EINVAL;
}

static void
dmp_dtr(struct dm_target *ti)
{
	struct my_dm_target *mdt = (struct my_dm_target *)ti->private;
	printk(KERN_CRIT "\n<<in function dmp_dtr\n");
	dm_put_device(ti, mdt->dev);
	kfree(mdt);
	kfree(my_stat);
	printk(KERN_CRIT "\n>>out function dmp_dtr \n");
}


static struct target_type dmp = {
	.name = "dmp",
	.version = {1,0,0},
	.module = THIS_MODULE,
	.ctr = dmp_ctr,
	.dtr = dmp_dtr,
	.map = dmp_map,
};

/* sysfs functions */

static ssize_t
volumes_show (struct kobject *kobj, 
	struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "read:\n reqs: %lu\n avg size: %u\nwrite:\n reqs: %lu\n avg size: %u\ntotal:\n reqs: %lu\n avg size: %u\n",
	 my_stat->my_read_req,
	 my_stat->my_avg_blk_rd,
	 my_stat->my_write_req,
	 my_stat->my_avg_blk_wrt,
	 my_stat->my_total_req,
	 my_stat->my_avg_blk_size);
}

static struct kobj_attribute volumes_attr = __ATTR_RO(volumes);

/* ----------------------------Module Functions------------------------*/

static int
init_dmp(void)
{
	int result;
	result = dm_register_target(&dmp);
	if (result < 0)
	{
		printk(KERN_CRIT "\nError in registering target\n");
		return -1;
	}

	stat_kobj = kobject_create_and_add("stat", &THIS_MODULE->mkobj.kobj);
	if (!stat_kobj)
	{
		printk(KERN_ERR "dmp: Failed to create stat kobject\n");
		return -ENOMEM;
	}
	if (sysfs_create_file(stat_kobj, &volumes_attr.attr))
	{
		printk(KERN_ERR "dmp: Failed to create sysfs file\n");
		kobject_put(stat_kobj);
		return -ENOMEM;
	}
		
	return 0;
}

static void
cleanup_dmp(void)
{
	dm_unregister_target(&dmp);
	sysfs_remove_file(stat_kobj, &volumes_attr.attr);
	kobject_put(stat_kobj);
}

module_init(init_dmp);
module_exit(cleanup_dmp);
MODULE_LICENSE("GPL");
