#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7377b0b2, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xbd9b35bb, __VMLINUX_SYMBOL_STR(up_read) },
	{ 0xc4f331c6, __VMLINUX_SYMBOL_STR(cpu_online_mask) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x4cc7ec29, __VMLINUX_SYMBOL_STR(netif_carrier_on) },
	{ 0x459efcce, __VMLINUX_SYMBOL_STR(netif_carrier_off) },
	{ 0x44b1d426, __VMLINUX_SYMBOL_STR(__dynamic_pr_debug) },
	{ 0x949b04cf, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x10f0b90c, __VMLINUX_SYMBOL_STR(__put_net) },
	{ 0x75607057, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x8b56f9a2, __VMLINUX_SYMBOL_STR(down_read) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x3489e6c4, __VMLINUX_SYMBOL_STR(kthread_bind) },
	{ 0x4d621647, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0xf432dd3d, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xa32395dc, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xcb65dcd, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x2d93da84, __VMLINUX_SYMBOL_STR(netif_rx_ni) },
	{ 0x8a19aff, __VMLINUX_SYMBOL_STR(unregister_pernet_subsys) },
	{ 0x687ce200, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
	{ 0xb16bdecb, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xb1998c9c, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6f5f225c, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x996f755c, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xa7cbf5c5, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x1463ded, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0x2c252aea, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xafcd691f, __VMLINUX_SYMBOL_STR(up_write) },
	{ 0xcb900c13, __VMLINUX_SYMBOL_STR(down_write) },
	{ 0xa916b694, __VMLINUX_SYMBOL_STR(strnlen) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x83a7998d, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0x2ea2c95c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rax) },
	{ 0xd0b6b33c, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x7f24de73, __VMLINUX_SYMBOL_STR(jiffies_to_usecs) },
	{ 0xfa1caac1, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x1b2f0865, __VMLINUX_SYMBOL_STR(register_pernet_subsys) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x53b27195, __VMLINUX_SYMBOL_STR(ether_setup) },
	{ 0xcf21d241, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x34f22f94, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x9c55cec, __VMLINUX_SYMBOL_STR(schedule_timeout_interruptible) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x787d3709, __VMLINUX_SYMBOL_STR(dev_trans_start) },
	{ 0xfa66f77c, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x66f95c3a, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x1e4917ac, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0xcb37d65a, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x4c69372f, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0xbcafb466, __VMLINUX_SYMBOL_STR(__init_rwsem) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "213BCB913EEF0B390A94610");
