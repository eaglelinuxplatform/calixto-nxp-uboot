// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2025-2026 Calixto Systems pvt ltd - https://www.calixtosystems.com/
 * 
 */

#include <init.h>
#include <asm/arch/clock.h>
#include <asm/arch/iomux.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/global_data.h>
#include <asm/gpio.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/boot_mode.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/io.h>
#include <common.h>
#include <env.h>
#include <fsl_esdhc_imx.h>
#include <i2c.h>
#include <miiphy.h>
#include <linux/sizes.h>
#include <linux/delay.h>
#include <mmc.h>
#include <miiphy.h>
#include <power/pmic.h>
#include <power/pfuze3000_pmic.h>
#include "../common/pfuze.h"

DECLARE_GLOBAL_DATA_PTR;


int dram_init(void)
{
	gd->ram_size = imx_ddr_size();

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return devno;
}

#ifdef CONFIG_FSL_QSPI

#ifndef CONFIG_DM_SPI
#define QSPI_PAD_CTRL1	\
	(PAD_CTL_SRE_FAST | PAD_CTL_SPEED_MED | \
	 PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_120ohm)

static iomux_v3_cfg_t const quadspi_pads[] = {
	MX6_PAD_NAND_WP_B__QSPI_A_SCLK | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_READY_B__QSPI_A_DATA00 | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CE0_B__QSPI_A_DATA01 | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CE1_B__QSPI_A_DATA02 | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_CLE__QSPI_A_DATA03 | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
	MX6_PAD_NAND_DQS__QSPI_A_SS0_B | MUX_PAD_CTRL(QSPI_PAD_CTRL1),
};
#endif

static int board_qspi_init(void)
{
#ifndef CONFIG_DM_SPI
	/* Set the iomux */
	imx_iomux_v3_setup_multiple_pads(quadspi_pads,
					 ARRAY_SIZE(quadspi_pads));
#endif
	/* Set the clock */
	enable_qspi_clk(0);

	return 0;
}
#endif


int board_early_init_f(void)
{
	return 0;
}

int read_mac_address(void)
{
       u32 *OCOTP_MAC0 = (uint32_t *)0x21bc620, *OCOTP_MAC1 = (uint32_t *)0x21bc630, *OCOTP_MAC = (uint32_t *)0x21bc640;
       
       u32 OCOTP_MAC0_VAL, OCOTP_MAC1_VAL, OCOTP_MAC_VAL; 
       
       char mac0_strng[20], mac1_strng[20];
       
       OCOTP_MAC0_VAL = *OCOTP_MAC0;
       OCOTP_MAC1_VAL = *OCOTP_MAC1;
       OCOTP_MAC_VAL  = *OCOTP_MAC;
       
       u8 mac0_addr[6], mac1_addr[6];
       
       mac0_addr[0] = (OCOTP_MAC1_VAL >> 24) & 0xff;
       mac0_addr[1] = (OCOTP_MAC1_VAL >> 16) & 0xff;
       mac0_addr[2] = (OCOTP_MAC0_VAL >> 24) & 0xff;
       mac0_addr[3] = (OCOTP_MAC0_VAL >> 16) & 0xff;
       mac0_addr[4] = (OCOTP_MAC0_VAL >> 8) & 0xff;
       mac0_addr[5] = OCOTP_MAC0_VAL & 0xff;
       
       mac1_addr[0] = (OCOTP_MAC_VAL >> 24) & 0xff;
       mac1_addr[1] = (OCOTP_MAC_VAL >> 16) & 0xff;
       mac1_addr[2] = (OCOTP_MAC_VAL >> 8) & 0xff;
       mac1_addr[3] = OCOTP_MAC_VAL & 0xff;
       mac1_addr[4] = (OCOTP_MAC1_VAL >> 8 ) & 0xff;
       mac1_addr[5] = OCOTP_MAC1_VAL & 0xff;
       
       sprintf(mac0_strng,"%02x:%02x:%02x:%02x:%02x:%02x",mac0_addr[0],mac0_addr[1],mac0_addr[2],mac0_addr[3],mac0_addr[4],mac0_addr[5]);
       
       sprintf(mac1_strng,"%02x:%02x:%02x:%02x:%02x:%02x",mac1_addr[0],mac1_addr[1],mac1_addr[2],mac1_addr[3],mac1_addr[4],mac1_addr[5]);
       
       env_set("ethaddr",mac0_strng);
       
       env_set("eth1addr",mac1_strng);
       
       return 0;
}

int board_init(void)
{
	/* Address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;


#ifdef CONFIG_FSL_QSPI
	board_qspi_init();
#endif

	return 0;
}

#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"sd1", MAKE_CFGVAL(0x42, 0x20, 0x00, 0x00)},
	{"sd2", MAKE_CFGVAL(0x40, 0x28, 0x00, 0x00)},
	{"qspi1", MAKE_CFGVAL(0x10, 0x00, 0x00, 0x00)},
	{NULL,	 0},
};
#endif

int board_late_init(void)
{
#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(board_boot_modes);
#endif

	env_set("tee", "no");
#ifdef CONFIG_IMX_OPTEE
	env_set("tee", "yes");
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", "IMX6ULL-CALIXTO-TINY");
	
	env_set("board_rev", "R01");

#endif

	read_mac_address();

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	set_wdog_reset((struct wdog_regs *)WDOG1_BASE_ADDR);

	return 0;
}

int checkboard(void)
{
	puts("Board: CALIXTO IMX6ULL TINY SOM\n");

	return 0;
}

void board_quiesce_devices(void)
{

}
