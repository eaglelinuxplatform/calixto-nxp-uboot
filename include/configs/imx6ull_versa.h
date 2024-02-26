/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2024-2025 Calixto Systems pvt ltd - https://www.calixtosystems.com/
 * 
 * Configuration settings for the CALIXTO, IMX6ULL-CALIXTO-VERSA SOM.
 *
 */
#ifndef __IMX6ULL_VERSA_H
#define __IMX6ULL_VERSA_H

#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include <linux/stringify.h>
#include "mx6_common.h"
#include <asm/mach-imx/gpio.h>
#include "imx_env.h"

#ifdef CONFIG_CALIXTO_DDR3_256MB
#define PHYS_SDRAM_SIZE		SZ_256M
#define BOOTARGS_CMA_SIZE   "cma=96M "
#endif

#ifdef CONFIG_CALIXTO_DDR3_512MB
#define PHYS_SDRAM_SIZE		SZ_512M
#define BOOTARGS_CMA_SIZE   ""
#endif

#ifdef CONFIG_CALIXTO_DDR3_1GB
#define PHYS_SDRAM_SIZE		SZ_1024M
#define BOOTARGS_CMA_SIZE   ""
#endif

#undef CONFIG_LDO_BYPASS_CHECK

#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configs */
#ifdef CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR

/* NAND pin conflicts with usdhc2 */
#ifdef CONFIG_NAND_MXS
#define CONFIG_SYS_FSL_USDHC_NUM	1
#else
#define CONFIG_SYS_FSL_USDHC_NUM	2
#endif

#endif


#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x86800000\0" \
	"initrd_high=0xffffffff\0" \
	"emmc_dev=1\0"\
	"emmc_ack=1\0"\
	"sd_dev=1\0" \
	"mtdparts=" MFG_NAND_PARTITION \
	"\0"\

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80800000\0" \
	"fdtaddr=0x83000000\0" \
	"fdt_high=0xffffffff\0" \
	"boot_fdt=try\0" \
	"rdaddr=0x81000000\0" \
	"bootpart=0:1\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"fdtfile=imx6ull-calixto-versa.dtb\0" \
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
                        "echo Failed to Images from SD card; " \
                "fi;\0" \
	"bootcmd_mmc=echo Booting from SD Card ...; "\
		"setenv mmcdev 0;" \
		"setenv bootpart 0:1;" \
	        "setenv mmcroot /dev/mmcblk0p1;" \
        	"run mmcbootos;\0" \
	"boot_targets=" \
		"mmc " \
		"\0"

#define CONFIG_BOOTCOMMAND \
	"for target in ${boot_targets}; do run bootcmd_${target}; done"

/* Miscellaneous configurable options */

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* environment organization */
#define CONFIG_SYS_MMC_ENV_DEV		1	/* USDHC2 */
#define CONFIG_MMCROOT			"/dev/mmcblk1p2"  /* USDHC2 */

#define CONFIG_IOMUX_LPSR

/* NAND stuff */
#ifdef CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_USE_FLASH_BBT

/* DMA stuff, needed for GPMI/MXS NAND support */
#endif

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#endif

#define CONFIG_FEC_XCV_TYPE             RMII
#define CONFIG_ETHPRIME			"eth1"

#ifndef CONFIG_SPL_BUILD
#if defined(CONFIG_DM_VIDEO)
#define CONFIG_VIDEO_LINK
#endif
#endif

#endif
