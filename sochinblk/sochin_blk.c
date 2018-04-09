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

// static int sochin_blk_major;

typedef struct sochin_blk {
    int minor;
    struct gendisk *disk;
    ssize_t size;
    void *data;
} sochin_blk;

struct sochin_blk *soblk_dev = NULL;

static int __init sochin_blk_init(void)
{
    printk("sochin_blk module inserted!\n");
    return 0;
}

static void __exit sochin_blk_exit(void)
{
    printk("sochin_blk module removed!\n");
}

module_init(sochin_blk_init);
module_exit(sochin_blk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sochin Jiang");
