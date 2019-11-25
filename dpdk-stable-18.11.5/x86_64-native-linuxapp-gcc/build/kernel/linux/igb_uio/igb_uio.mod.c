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
	{ 0xa32395dc, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x964fcf43, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x6701cb36, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0x25e7a015, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x9984d9f, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x9f3c87ac, __VMLINUX_SYMBOL_STR(dma_ops) },
	{ 0x78764f4e, __VMLINUX_SYMBOL_STR(pv_irq_ops) },
	{ 0x4c1e9a15, __VMLINUX_SYMBOL_STR(arch_dma_alloc_attrs) },
	{ 0x7990832c, __VMLINUX_SYMBOL_STR(__uio_register_device) },
	{ 0x9d00a7e, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x5b8a887a, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0x2ea2c95c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rax) },
	{ 0xefd40e26, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0x87b393c2, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x2ecda6f9, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x86cdc2f3, __VMLINUX_SYMBOL_STR(pci_bus_write_config_dword) },
	{ 0x88beb721, __VMLINUX_SYMBOL_STR(pci_intx) },
	{ 0x88b4d6b, __VMLINUX_SYMBOL_STR(pci_cfg_access_unlock) },
	{ 0x3b3f79b5, __VMLINUX_SYMBOL_STR(pci_cfg_access_lock) },
	{ 0xa0f892d, __VMLINUX_SYMBOL_STR(__dynamic_dev_dbg) },
	{ 0x912a97d4, __VMLINUX_SYMBOL_STR(dev_notice) },
	{ 0x5c7b58d, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xc930ecd0, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x2072ee9b, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x4a123625, __VMLINUX_SYMBOL_STR(pci_intx_mask_supported) },
	{ 0x3d71d916, __VMLINUX_SYMBOL_STR(pci_enable_msi_range) },
	{ 0x79f07f9c, __VMLINUX_SYMBOL_STR(pci_enable_msix) },
	{ 0x97f0e23f, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0x7cc58fb4, __VMLINUX_SYMBOL_STR(pci_check_and_mask_intx) },
	{ 0x7be3c849, __VMLINUX_SYMBOL_STR(uio_event_notify) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xf9240d7e, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0xd5dd523e, __VMLINUX_SYMBOL_STR(uio_unregister_device) },
	{ 0x43e4bed6, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0x707f348a, __VMLINUX_SYMBOL_STR(pci_disable_msix) },
	{ 0x9c149687, __VMLINUX_SYMBOL_STR(pci_disable_msi) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x6b675d0a, __VMLINUX_SYMBOL_STR(pci_clear_master) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x99aafdde, __VMLINUX_SYMBOL_STR(pci_bus_type) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x81ae693c, __VMLINUX_SYMBOL_STR(pci_enable_sriov) },
	{ 0xbab07330, __VMLINUX_SYMBOL_STR(pci_disable_sriov) },
	{ 0xce376954, __VMLINUX_SYMBOL_STR(pci_num_vf) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=uio";


MODULE_INFO(srcversion, "26C63DAE2104091D5DE47C6");
