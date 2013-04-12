/*
 * Copyright (C) 2011 Freescale Semiconductor, Inc.
 * Jason Liu <r64343@freescale.com>
 *
 * Copyright (C) 2011-2013 Genesi USA, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/clock.h>
#include <asm/arch/iomux-mx53.h>
#include <asm/arch/clock.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE, PHYS_SDRAM_1_SIZE);

	return 0;
}

/*
 * for CONFIG_SYS_REVISION_TAG
 */
u32 get_board_rev(void)
{
	return 0x53000;
}

/*
 * USB Configuration
 */
static iomux_v3_cfg_t const efikasb_usb_pads[] = {
	MX53_PAD_KEY_COL4__GPIO4_14,    /* USBH1 OC */
	MX53_PAD_KEY_ROW4__GPIO4_15,    /* USBH1 EN */
	MX53_PAD_EIM_D30__GPIO3_30,     /* USBH2 OC */
	MX53_PAD_EIM_D31__GPIO3_31,     /* USBH2 EN */
};

#define EFIKASB_USBH1_OC	IMX_GPIO_NR(4, 14)
#define EFIKASB_USBH1_EN	IMX_GPIO_NR(4, 15)
#define EFIKASB_USBH2_OC	IMX_GPIO_NR(3, 30)
#define EFIKASB_USBH2_EN	IMX_GPIO_NR(3, 31)

int board_ehci_hcd_init(int port)
{
	return 0;
}



/*
 * SDHC configuration
 *
 * SD1 is connected to the on-board SDHC slot
 * SD2 is connected to the on-board Wifi
 */
static iomux_v3_cfg_t const efikasb_sdhc_pads[] = {
	MX53_PAD_SD1_CMD__ESDHC1_CMD,
	MX53_PAD_SD1_CLK__ESDHC1_CLK,
	MX53_PAD_SD1_DATA0__ESDHC1_DAT0,
	MX53_PAD_SD1_DATA1__ESDHC1_DAT1,
	MX53_PAD_SD1_DATA2__ESDHC1_DAT2,
	MX53_PAD_SD1_DATA3__ESDHC1_DAT3,
	MX53_PAD_GPIO_9__GPIO1_9,

	MX53_PAD_SD2_CMD__ESDHC2_CMD,
	MX53_PAD_SD2_CLK__ESDHC2_CLK,
	MX53_PAD_SD2_DATA0__ESDHC2_DAT0,
	MX53_PAD_SD2_DATA1__ESDHC2_DAT1,
	MX53_PAD_SD2_DATA2__ESDHC2_DAT2,
	MX53_PAD_SD2_DATA3__ESDHC2_DAT3,
};

#define EFIKASB_SDHC1_WP	IMX_GPIO_NR(1, 9)

struct fsl_esdhc_cfg esdhc_cfg[2] = {
	{MMC_SDHC1_BASE_ADDR},
	{MMC_SDHC2_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
	/* Card Detect is not implemented on the board */
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	u32 index;
	s32 status = 0;

	esdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);
	esdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);

	for (index = 0; index < CONFIG_SYS_FSL_ESDHC_NUM; index++) {
		switch (index) {
		case 0:
			/* SDHC slot */
			break;
		case 1:
			/* Wifi adapter (SDIO, probably not useful here) */
			break;
		default:
			return status;
		}
		status |= fsl_esdhc_initialize(bis, &esdhc_cfg[index]);
	}

	return status;
}


/*
 * NAND configuration
 */

static iomux_v3_cfg_t const efikasb_nand_pads[] = {
	MX53_PAD_NANDF_CLE__EMI_NANDF_CLE,
	MX53_PAD_NANDF_ALE__EMI_NANDF_ALE,
	MX53_PAD_NANDF_WP_B__EMI_NANDF_WP_B,
	MX53_PAD_NANDF_WE_B__EMI_NANDF_WE_B,
	MX53_PAD_NANDF_RE_B__EMI_NANDF_RE_B,
	MX53_PAD_NANDF_RB0__EMI_NANDF_RB_0,
	MX53_PAD_NANDF_CS0__EMI_NANDF_CS_0,
	MX53_PAD_NANDF_CS1__EMI_NANDF_CS_1,
	MX53_PAD_EIM_DA0__EMI_NAND_WEIM_DA_0,
	MX53_PAD_EIM_DA1__EMI_NAND_WEIM_DA_1,
	MX53_PAD_EIM_DA2__EMI_NAND_WEIM_DA_2,
	MX53_PAD_EIM_DA3__EMI_NAND_WEIM_DA_3,
	MX53_PAD_EIM_DA4__EMI_NAND_WEIM_DA_4,
	MX53_PAD_EIM_DA5__EMI_NAND_WEIM_DA_5,
	MX53_PAD_EIM_DA6__EMI_NAND_WEIM_DA_6,
	MX53_PAD_EIM_DA7__EMI_NAND_WEIM_DA_7,
};

#define M4IF_GPR	(M4IF_BASE_ADDR + 0x0c)
#define M4IF_GPR_MM	(1 << 0)
#define MUX16_BYP_GRANT	(1 << 12)

static void efikasb_nand_init(void)
{
	u32 m4if_gpr, csxgcr2;
	struct weim *weim_regs = (struct weim *)WEIM_BASE_ADDR;

	/*
	 * We must instruct the M4IF that WEIM is not muxed with NANDF
	 * data pins.. in theory we should be setting BOOT_CFG1[6] eFUSE
	 * to override this for production according to the manual (page 3492)
	 */

	m4if_gpr = readl(M4IF_GPR);
	m4if_gpr &= ~M4IF_GPR_MM;
	writel(m4if_gpr, M4IF_GPR);

	/*
	 * Then we have to configure the WEIM to wait for a grant (which
	 * it will never get..) before driving the muxed NAND/EIM pins,
	 * effectively giving the NFC exclusive access to the NAND pins
	 */

	csxgcr2 = readl(&weim_regs->cs0gcr2);
	csxgcr2 &= ~MUX16_BYP_GRANT;
	writel(csxgcr2, &weim_regs->cs0gcr2);

	csxgcr2 = readl(&weim_regs->cs1gcr2);
	csxgcr2 &= ~MUX16_BYP_GRANT;
	writel(csxgcr2, &weim_regs->cs1gcr2);

	csxgcr2 = readl(&weim_regs->cs2gcr2);
	csxgcr2 &= ~MUX16_BYP_GRANT;
	writel(csxgcr2, &weim_regs->cs2gcr2);

	csxgcr2 = readl(&weim_regs->cs3gcr2);
	csxgcr2 &= ~MUX16_BYP_GRANT;
	writel(csxgcr2, &weim_regs->cs3gcr2);
}

/*
 * UART configuration
 *
 * UART1 is on test pads connected to the i.MX
 * UART2 is connected to the Housekeeping MCU
 */
static iomux_v3_cfg_t const efikasb_uart_pads[] = {
	MX53_PAD_PATA_DIOW__UART1_TXD_MUX,
	MX53_PAD_PATA_DMACK__UART1_RXD_MUX,

	MX53_PAD_PATA_DMARQ__UART2_TXD_MUX,
	MX53_PAD_PATA_BUFFER_EN__UART2_RXD_MUX,
};

int board_early_init_f(void)
{
	/*
	 * We know that we have good power or the Housekeeping MCU
	 * would not have booted the i.MX - in case fuses have been
	 * blown to reduce CPU and DDR speed at boot time, increase
	 * them here.. this is not really liable to fail.
	 */

	mxc_set_clock(MXC_HCLK, 1000, MXC_ARM_CLK);
	mxc_set_clock(MXC_HCLK, 400, MXC_PERIPH_CLK);
	mxc_set_clock(MXC_HCLK, 400, MXC_DDR_CLK);

	/*
	 * Configure and enable the NAND clock
	 */
	mxc_set_clock(0, 33, MXC_NFC_CLK);
	enable_nfc_clk(1);
	efikasb_nand_init();


	/*
	 * Now, get everything we can set up - as early as possible
	 */

	imx_iomux_v3_setup_multiple_pads(efikasb_uart_pads,
			ARRAY_SIZE(efikasb_uart_pads));

	imx_iomux_v3_setup_multiple_pads(efikasb_sdhc_pads,
			ARRAY_SIZE(efikasb_sdhc_pads));

	gpio_direction_input(EFIKASB_SDHC1_WP);

	imx_iomux_v3_setup_multiple_pads(efikasb_nand_pads,
			ARRAY_SIZE(efikasb_nand_pads));

	imx_iomux_v3_setup_multiple_pads(efikasb_usb_pads,
			ARRAY_SIZE(efikasb_usb_pads));

	gpio_direction_input(EFIKASB_USBH1_OC);
	gpio_direction_output(EFIKASB_USBH1_EN, 1);

	gpio_direction_input(EFIKASB_USBH2_OC);
	gpio_direction_output(EFIKASB_USBH2_EN, 1);

	/*
	 * What's missing? CCM_CLKO, resets, AUDMUX, CSI, LVDS
	 */

	return 0;
}

int board_init(void)
{
#if 0
	/* SRC_SBMR contains the eFUSE setting for NAND buswidth */
        struct src *src = (struct src *)SRC_BASE_ADDR;
	u32 sbmr;

        sbmr = readl(&src->sbmr);
#endif
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}

int board_late_init(void)
{
	return 0;
}

int checkboard(void)
{
	puts("Board: Genesi Efika MX Slimbook\n");

	return 0;
}
