/*
 * Gracefully reboot with a magic sysrq key -- REISUB replacement
 * Copyright (C) 2018  zhuyifei1999
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
#include <linux/workqueue.h>
#include <linux/umh.h>

static void work_handler(struct work_struct *ignored) {
    // char *argv[] = {"/usr/bin/logger", "test", NULL};
    char *argv[] = {"/sbin/reboot", NULL};
    char *envp[] = {"HOME=/", "TERM=linux", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
    WARN_ON(call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC));
}
static DECLARE_WORK(work, work_handler);


static void sysrq_handler(int key) {
    schedule_work(&work);
}

static struct sysrq_key_op sysrq_op = {
    .handler = &sysrq_handler,
    .help_msg = "grace-reboot(x)",
    .action_msg = "Gracefully reboot via user space",
    .enable_mask = SYSRQ_ENABLE_BOOT,
};


static int grace_reboot_init(void) {
    return register_sysrq_key('x', &sysrq_op);
}
static void grace_reboot_exit(void) {
    WARN_ON(unregister_sysrq_key('x', &sysrq_op));
}

module_init(grace_reboot_init);
module_exit(grace_reboot_exit);
