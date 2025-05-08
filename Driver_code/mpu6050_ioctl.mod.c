#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0x3868185, "module_layout" },
	{ 0xf93983f6, "i2c_del_driver" },
	{ 0xf4cb7bdc, "i2c_register_driver" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x56eefe6b, "device_create" },
	{ 0x94176683, "__class_create" },
	{ 0x2b131b84, "__register_chrdev" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xe7a326ba, "i2c_smbus_read_i2c_block_data" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x729486cf, "class_destroy" },
	{ 0x485280d6, "class_unregister" },
	{ 0x310024a9, "device_destroy" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cti,ads1115");
MODULE_ALIAS("of:N*T*Cti,ads1115C*");

MODULE_INFO(srcversion, "34CC7AAAA4B3DD9A7552B6B");
