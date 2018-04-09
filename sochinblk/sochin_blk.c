/*
 * Copyright (C) Sochin Jiang 2018
 * Author: johnpub@zju.edu.cn
 *
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>

static int sochin_blk_major;

/*for now, simply support one partition */
#define SOCHIN_BLK_MINOR 1

typedef struct sochin_blk {
    int minor;
    struct gendisk *disk;
    spinlock_t lock;
    ssize_t size;
    int bytesize;
    void *data;
} sochin_blk;

struct sochin_blk *soblk_dev = NULL;

static void sochin_blk_request(struct request_queue *q)
{
    /* Todo */
}

static int sochin_blk_alloc(int minor)
{
    struct gendisk *disk;
    int ret = 0;

    soblk_dev = kzalloc(sizeof(sochin_blk), GFP_KERNEL);
    if (!soblk_dev) {
        ret = -ENOMEM;
        goto alloc;
    }
    /* default 10G */
    soblk_dev->bytesize = 10 * 1024 * 1024 * 1024;
    soblk_dev->minor = minor;

    spin_lock_init(&soblk_dev->lock);
    disk = alloc_disk(minor);
    if (!disk) {
        ret = -ENOMEM;
        goto dev;
    }
    soblk_dev->disk = disk;
    disk->major = sochin_blk_major;
    disk->first_minor = minor;
    /* Todo: add fops*/
    disk->fops = NULL;
    disk->private_data = soblk_dev;
    disk->queue = blk_init_queue(sochin_blk_request, &soblk_dev->lock);
    if (!disk->queue) {
        ret = -ENOMEM;
        goto dev;
    }
    sprintf(disk->disk_name, "sochinblk%d", minor);
    /* This will invoke probe and udev */
    add_disk(disk);

    return 0;

dev:
    kfree(soblk_dev);
alloc:
    return ret;
}

static void sochin_blk_free(sochin_blk *blk_dev)
{
    del_gendisk(blk_dev->disk);
    blk_cleanup_queue(blk_dev->disk->queue);
    put_disk(blk_dev->disk);
    kfree(blk_dev);
}

static int __init sochin_blk_init(void)
{ 
    int ret = 0;
    sochin_blk_major = register_blkdev(0, "sochinblk");
    if (sochin_blk_major < 0)
        return sochin_blk_major;
    
    ret = sochin_blk_alloc(SOCHIN_BLK_MINOR);
    if (ret < 0)
        printk("sochinblk: initialize sochin_blk failed: %d\n", ret);

    printk("sochin_blk module inserted!\n");
    return 0;
}

static void __exit sochin_blk_exit(void)
{
    sochin_blk_free(soblk_dev);
    unregister_blkdev(sochin_blk_major, "sochinblk");
    printk("sochin_blk module removed!\n");
}

module_init(sochin_blk_init);
module_exit(sochin_blk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sochin Jiang");
