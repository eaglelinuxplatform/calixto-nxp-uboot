/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2025-2026 Calixto Systems pvt ltd - https://www.calixtosystems.com/
 * 
 * Configuration settings for the CALIXTO, IMX6ULL-CALIXTO-TINY SOM.
 *
 */
#ifndef __IMX6ULL_TINY_H
#define __IMX6ULL_TINY_H


#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include <linux/stringify.h>
#include "mx6_common.h"
#include <asm/mach-imx/gpio.h>
#include "imx_env.h"

#define CFG_MXC_UART_BASE		UART1_BASE

#define BOOTARGS_CMA_SIZE   ""

/* MMC Configs */
#ifdef CONFIG_FSL_USDHC
#define CFG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR
#endif

#ifdef CONFIG_NAND_BOOT
#define MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(nandboot),16m(nandkernel),16m(nanddtb),16m(nandtee),-(nandrootfs)"
#else
#define MFG_NAND_PARTITION ""
#endif

#define CFG_MFG_ENV_SETTINGS \
	CFG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x86800000\0" \
	"initrd_high=0xffffffff\0" \
	"emmc_dev=1\0"\
	"emmc_ack=1\0"\
	"sd_dev=1\0" \
	"mtdparts=" MFG_NAND_PARTITION \
	"\0"\

#define CFG_EXTRA_ENV_SETTINGS \
	CFG_MFG_ENV_SETTINGS \
	TEE_ENV \
	"loadaddr=0x80800000\0" \
	"fdtaddr=0x83000000\0" \
	"fdt_high=0xffffffff\0" \
	"boot_fdt=try\0" \
	"rdaddr=0x81000000\0" \
	"bootpart=0:1\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"uboot_version=1.0\0" \
	"fdtfile=imx6ull-calixto-tiny.dtb\0" \
	"console=ttymxc0,115200n8\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"spierase_env=echo removing enviroment settings from SPI flash..; " \
                        "sf probe 0; " \
                        "sf erase 0xA0000 0xC0000\0" \
	"enable_emmc_rstn=mmc dev 1;mmc rstn 1;\0" \
	"loadimage=load mmc ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load mmc ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcloados=run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"fi;\0" \
	"emmcbootos=" \
                "if run loadimage; then " \
                        "run mmcloados; " \
                "else " \
                        "echo Failed to Images from eMMC; " \
                "fi;\0" \
	"bootcmd_emmc=echo Booting from eMMC ...; "\
		"setenv mmcdev 1;" \
		"setenv bootpart 1:1;" \
	        "setenv mmcroot /dev/mmcblk1p1;" \
        	"run emmcbootos;\0" \
        "mmcbootos=" \
                "if run loadimage; then " \
                        "run mmcloados; " \
                "else " \
                        "echo Failed to load Images from SD card; " \
                "fi;\0" \
	"bootcmd_mmc=echo Booting from SD Card ...; "\
		"setenv mmcdev 0;" \
		"setenv bootpart 0:1;" \
	        "setenv mmcroot /dev/mmcblk0p1;" \
        	"run mmcbootos;\0" \
	"boot_targets=" \
		"mmc " \
		"\0"
	
//#endif


#define CONFIG_BOOTCOMMAND \
	"for target in ${boot_targets}; do run bootcmd_${target}; done"

/* Miscellaneous configurable options */

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CFG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CFG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CFG_SYS_INIT_RAM_SIZE	IRAM_SIZE

/* environment organization */

/* NAND stuff */
#define CFG_SYS_NAND_BASE		0x40000000
#endif
