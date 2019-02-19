/*
 * Orderly reboot with a magic sysrq key -- REISUB replacement (sometimes)
 * Copyright (C) 2018-2019 YiFei Zhu 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE( "GPL" );

#include <linux/sysrq.h>
#include <linux/reboot.h>


static void sysrq_handler(int key) {
    orderly_reboot();
}

static struct sysrq_key_op sysrq_op = {
    .handler = &sysrq_handler,
    .help_msg = "orderly-reboot(x)",
    .action_msg = "Orderly reboot via user space",
    .enable_mask = SYSRQ_ENABLE_BOOT,
};


static int orderly_reboot_init(void) {
    return register_sysrq_key('x', &sysrq_op);
}
static void orderly_reboot_exit(void) {
    WARN_ON(unregister_sysrq_key('x', &sysrq_op));
}

module_init(orderly_reboot_init);
module_exit(orderly_reboot_exit);
