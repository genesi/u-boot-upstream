/*
 * Copyright (C) 2011 Freescale Semiconductor, Inc.
 * Jason Liu <r64343@freescale.com>
 *
 * Copyright (C) 2011-2013 Genesi USA, Inc.
 *
 * Configuration settings for Genesi Efika MX Slimbook.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <config_cmd_default.h>
#include <timestamp.h>

/*
 * Booting on the Efika MX Slimbook board is kind of a strange monster compared
 * to development boards and the original Efika MX systems - boot source is
 * configured by an external MCU and in most instance streams a stage0
 * bootloader over UART2 to the i.MX53. Then the next stage is booted from NAND
 * or SD card.
 *
 * Alternatively it can be forced to boot from eFUSE settings which will
 * override this process completely and require the standard i.MX boot method
 * (DCD, ROM plugins and all..)
 *
 * All we're taking care of here is the stream-over-UART method. The U-Boot
 * binary needs to stick within a hardcoded 256KiB limit (this code with this
 * value is stored inside the MCU, not easily changed), so we need to keep the
 * size down by removing some features..
 */

#undef CONFIG_ZLIB
#undef CONFIG_GZIP
#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_BOOTM_RTEMS
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_PCMCIA
#undef CONFIG_PCMCIA
#undef CONFIG_CMD_LOADB
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_LOADY
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_PING
#undef CONFIG_CMD_DHCP

/*
 * Using Thumb2 saves us a lot more space..
 */
#define CONFIG_SYS_THUMB_BUILD

/*
 * We will need to undefine this to save even more space if we break the 256KiB
 * limit, but for now we just squeak in and it would be nicer to users if space
 * was optimized elsewhere first.
 */
#define CONFIG_SYS_LONGHELP

/*
 * We want to explicitly enable these in case they're not.
 */
#define CONFIG_CMD_CACHE
#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ

#define CONFIG_MX53
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_MACH_TYPE		MACH_TYPE_MX53_EFIKASB
#define CONFIG_EFIKAMX_MODEL	"slimbook"

#include <asm/arch/imx-regs.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_REVISION_TAG

#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/*
 * Genesi do not support saving the U-Boot environment at runtime and in
 * any case, there is nowhere to store it..
 */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_CMD_MMC
#ifdef CONFIG_CMD_MMC
#	define CONFIG_MMC
#	define CONFIG_GENERIC_MMC
#	define CONFIG_FSL_ESDHC
#	define CONFIG_SYS_FSL_ESDHC_ADDR	0
#	define CONFIG_SYS_FSL_ESDHC_NUM		1
#endif /* CONFIG_CMD_MMC */

#undef CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#	define CONFIG_USB_EHCI
#	define CONFIG_USB_EHCI_MX5
#	define CONFIG_USB_STORAGE
#	undef CONFIG_USB_KEYBOARD

#	define CONFIG_MXC_USB_PORT	1
#	if (CONFIG_MXC_USB_PORT == 1)
#		define CONFIG_MXC_USB_PORTSC	(2 << 30)
#		define CONFIG_MXC_USB_FLAGS		0
#	endif
/*
 * Networking depends on USB for the Efika MX Slimbook, since there is only
 * an SDIO WiFi card on-board, so we put the network configuration in here
 * for readability's sake.
 */
#	define CONFIG_CMD_NET
#	ifdef CONFIG_CMD_NET
#		define CONFIG_USB_HOST_ETHER
#		define CONFIG_USB_ETHER_ASIX /* Efika MX Smarttop built-in */
#		define CONFIG_USB_ETHER_SMSC95XX
#		define CONFIG_CMD_PING
#		define CONFIG_CMD_DHCP
#		define CONFIG_CMD_NFS
#	endif /* CONFIG_CMD_NET */
#endif /* CONFIG_CMD_USB */

#if defined(CONFIG_CMD_USB) || defined(CONFIG_CMD_MMC)
#	define CONFIG_DOS_PARTITION
#	define CONFIG_EFI_PARTITION
#	define CONFIG_CMD_FAT
#	define CONFIG_CMD_EXT2
#	define CONFIG_CMD_EXT4
#endif /* CONFIG_CMD_USB || CONFIG_CMD_MMC */

/* NAND is disabled because the driver is broken.. */
#define CONFIG_CMD_UBIFS
#ifdef CONFIG_CMD_UBIFS
#	define CONFIG_CMD_UBI
#	define CONFIG_LZO
#	define CONFIG_RBTREE
#	define CONFIG_ZLIB
#	define CONFIG_GZIP

#	define CONFIG_CMD_NAND
#	define CONFIG_SYS_MAX_NAND_DEVICE	1
#	define CONFIG_SYS_NAND_LARGEPAGE
#	define CONFIG_SYS_NAND_ONFI_DETECTION
//#	define CONFIG_SYS_NAND_USE_FLASH_BBT
//#	define CONFIG_MTD_DEBUG
//#	define CONFIG_MTD_DEBUG_VERBOSE		3
/*
 * This seems to be for memory-mapped NAND but the NAND subsystem
 * doesn't build without it..
 */
#	define CONFIG_SYS_NAND_BASE		(NFC_BASE_ADDR_AXI)

#	define CONFIG_NAND_MXC
#	define CONFIG_MXC_NAND_REGS_BASE	(NFC_BASE_ADDR_AXI)
#	define CONFIG_MXC_NAND_IP_REGS_BASE	(NFC_BASE_ADDR)
#	define CONFIG_MXC_NAND_8BIT
#	define CONFIG_MXC_NAND_HWECC

#	define CONFIG_CMD_MTDPARTS
#	define CONFIG_MTD_DEVICE
#	define CONFIG_MTD_PARTITIONS
#endif /* CONFIG_CMD_UBIFS */


/*
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1			CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE		(512 * 1024 * 1024)

#define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)
#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_SYS_INIT_SP_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/*
 * ENVIRONMENT
 *
 * layout of load addresses;
 *  0x70000000 = START OF MEMORY
 *  0x70007FC0 = load address for uImage (8MiB+64)
 *  0x707FFFC0 = load address for uInitrd (16MiB+64)
 *  0x71800000 = load address for optional device tree (64KiB)
 *  0x71810000 = other things (scripts, etc.)
 *
 * A note about curious address layout; we drop the kernel in 64 bytes before it's
 * entry point address hardcoded into uImage and kernel, so that the entry point is
 * actually at the right location.
 *
 * We drop the ramdisk 64 bytes back too for the same reason. although there is no
 * good reason to do this, it does mean the ramdisk data always starts 8MiB into
 * the memory map.
 *
 * The device tree load address is so placed that there is a reserved area for a
 * MAXIMUM of a 16MiB compressed ramdisk (without overwriting the DT). If your boot
 * process needs anything different to this, then the script load address is the
 * only one you need to care about; just use the boot.scr to modify the load addresses
 * and do what you like, this values are only used to effect more efficient scripting.
 */
#define CONFIG_KERNEL_ADDR	0x70007FC0
#define CONFIG_RD_ADDR		0x707FFFC0
#define CONFIG_DT_ADDR		0x71800000
#define CONFIG_SCRIPT_ADDR	0x71810000
#define CONFIG_LOADADDR		CONFIG_SCRIPT_ADDR

#define CONFIG_STACKSIZE	(128 * 1024)
#define CONFIG_ENV_SIZE		(8 * 1024)
#define CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + 32 * 1024 * 1024)

/*
 * Ideally we would want the load address define to be different to the kernel
 * address. This will allow errant scripting defaults to run (i.e. fatload
 * filename) without trashing specially-defined memory areas as above. However,
 * we have a legacy of users and Linux images to support, so we just suck it up
 * and deal with it.
 */
#define CONFIG_SYS_TEXT_BASE	0x73800000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_KERNEL_ADDR
/*
 * Mmeory test range: from where ATAGs get put, to 64MiB above the ramdisk load
 * address. If any of this range fails it's a good indicator of serious
 * breakage.
 */
#define CONFIG_SYS_MEMTEST_START	0x70000100
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_TEXT_BASE

#define CONFIG_BOOTDELAY		1

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT		"Slimbook> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_HZ			1000

/*
 * Do NOT change the names of kerneladdr, scriptaddr, ramdiskaddr or existing
 * EfikaMX Linux support (i.e. old boot.scr) will quit working.
 */

#define XADDR(a) #a
#define ADDR(a) XADDR(a)

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"model=" CONFIG_EFIKAMX_MODEL "\0" \
	"firmware_version=" U_BOOT_TIMESTAMP "\0" \
	"bootscript=boot.scr\0" \
	"kerneladdr=" ADDR(CONFIG_KERNEL_ADDR) "\0" \
	"ramdiskaddr=" ADDR(CONFIG_RD_ADDR) "\0" \
	"dtbaddr=" ADDR(CONFIG_DT_ADDR) "\0" \
	"scriptaddr=" ADDR(CONFIG_SCRIPT_ADDR) "\0" \
	"bootdevices=mmc\0" \
	"units=0\0" \
	"console=ttymxc0,115200n8\0" \
	"bootargs=console=${console}"

/*
 * Note: make sure the last EXTRA_ENV_SETTINGS item has no null terminator!
 */

/*
 * Derived from the production-shipped U-Boot from Genesi, and updated to
 * support new features. This is somewhat of a standard-in-progress for the
 * Linaro Cross-Distro people and attempts to prove a working configuration
 * while staying completely compatible with the old config.
 *
 * Note: recommended SD card layout is 3 partitions, configured such that the
 * first partition starts at the 1024 byte position on SD. This way an SD card
 * can be created to boot U-Boot from SD (with correct DIP settings) by simply
 * dd'ing to the first partition without creating strange environments with
 * empty space.
 *
 * Second partition is suggested as /boot - and should be ext2 or ext3 so older
 * U-Boot can read it.
 *
 * Third partition is / in this case, and could be ext4.
 *
 * However in the event you make a custom SD card with one partition formatted
 * as ext4, or plug in a FAT-formatted USB stick on an otherwise empty system,
 * it will all still work. We limit it to a single USB stick, but the default
 * configuration above does not add "usb" to the devices right now.
 */
#define CONFIG_BOOTCOMMAND \
	"for device in ${bootdevices}; do " \
		"if test ${device} = \"mmc\"; then " \
			"mmc rescan; setenv units \"0\"; " \
		"elif test ${device} = \"usb\"; then " \
			"usb start; setenv units \"0\"; " \
		"fi; " \
		"for unit in ${units}; do " \
			"${device} dev ${unit}; " \
			"for part in \"1 2 3\"; do " \
				"for fs in \"ext4 ext2 fat\"; do " \
					"setenv loadcmd \"${fs}load ${device} ${unit}:${part}\"; " \
					"if ${loadcmd} ${scriptaddr} ${bootscript}; then " \
						"if imi ${scriptaddr}; then " \
							"source ${scriptaddr}; " \
						"fi; " \
					"fi; " \
				"done; " \
			"done; " \
		"done; " \
	"done; "

#ifdef CONFIG_CMD_NFS
#warning fucccck
#endif

#endif /* __CONFIG_H */
