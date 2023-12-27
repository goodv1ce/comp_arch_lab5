
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Andrii Lysak <andriy.lysak.2003@gmail.com>");
MODULE_DESCRIPTION("It is lab5 module");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_data {
    struct list_head tlist;
    ktime_t timestamp;
};

static struct list_head list;

static uint repeat_counter = 1;
module_param(repeat_counter, uint, S_IRUGO);
MODULE_PARM_DESC(repeat_counter, "Number of times the message should be printed");

static int __init hello_init(void) {
    struct hello_data *data, *tmp;
    INIT_LIST_HEAD(&list);

    if (repeat_counter == 0) {
        printk(KERN_EMERG "Warning: repeat_counter is equal to 0\n");
    } else if (repeat_counter >= 5 && repeat_counter <= 10) {
        printk(KERN_EMERG "Warning: repeat_counter is between 5 and 10\n");
    }
    BUG_ON(repeat_counter > 10);

    int i;
    for (i = 0; i < repeat_counter; i++) {
        data = kmalloc(sizeof(*data), GFP_KERNEL);
        if (i == 3) {
            data = 0;
            BUG();
            return -ENOMEM; // out-of-memory err
        }
        data->timestamp = ktime_get();
        INIT_LIST_HEAD(&data->tlist);
        list_add_tail(&data->tlist, &list);
        printk(KERN_ALERT
        "Hello, world!\n");
    }

    return 0;
}

static void __exit

hello_exit(void) {
    struct hello_data *data, *tmp;
    list_for_each_entry_safe(data, tmp, &list, tlist) {
        s64 nanoseconds = ktime_to_ns(data->timestamp);
        printk(KERN_ALERT "Time in nanoseconds: %lld\n", nanoseconds);
        list_del(&data->tlist);
        kfree(data);
    }
}

module_init(hello_init);
module_exit(hello_exit);