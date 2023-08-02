#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x92997ed8, "_printk" },
	{ 0x7fb98451, "cdev_init" },
	{ 0x91128fc8, "cdev_add" },
	{ 0x8d30dba0, "__class_create" },
	{ 0xbf3f457f, "device_create" },
	{ 0x226383ed, "i2c_get_adapter" },
	{ 0x3e7874ab, "i2c_new_client_device" },
	{ 0x440cbd26, "i2c_register_driver" },
	{ 0x54ae6ab6, "device_destroy" },
	{ 0x7391ca5d, "class_destroy" },
	{ 0x43dd1c15, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xbec3491, "i2c_transfer_buffer_flags" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x1d31a384, "i2c_unregister_device" },
	{ 0x424d82fe, "i2c_del_driver" },
	{ 0xf9a482f9, "msleep" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x8f80e6e5, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:bh1750");

MODULE_INFO(srcversion, "BCE8F21801525AA649634E7");
