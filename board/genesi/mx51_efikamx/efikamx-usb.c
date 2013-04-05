/*
 * Copyright (C) 2010 Marek Vasut <marek.vasut@gmail.com>
 *
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
 * (C) Copyright 2012-2013 Genesi USA, Inc.
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
#include <usb.h>
#include <asm/io.h>
#include <asm/arch/iomux-mx51.h>
#include <asm/gpio.h>
#include <usb/ehci-fsl.h>
#include <usb/ulpi.h>
#include <errno.h>

#include "../../../drivers/usb/host/ehci.h"

iomux_v3_cfg_t efikamx_usbh1_pads[] = {
	MX51_PAD_USBH1_CLK__USBH1_CLK,
	MX51_PAD_USBH1_DIR__USBH1_DIR,
	MX51_PAD_USBH1_STP__USBH1_STP,
	MX51_PAD_USBH1_NXT__USBH1_NXT,
	MX51_PAD_USBH1_DATA0__USBH1_DATA0,
	MX51_PAD_USBH1_DATA1__USBH1_DATA1,
	MX51_PAD_USBH1_DATA2__USBH1_DATA2,
	MX51_PAD_USBH1_DATA3__USBH1_DATA3,
	MX51_PAD_USBH1_DATA4__USBH1_DATA4,
	MX51_PAD_USBH1_DATA5__USBH1_DATA5,
	MX51_PAD_USBH1_DATA6__USBH1_DATA6,
	MX51_PAD_USBH1_DATA7__USBH1_DATA7,
};

iomux_v3_cfg_t efikasb_usbh2_pads[] = {
	MX51_PAD_EIM_A24__USBH2_CLK,
	MX51_PAD_EIM_A25__USBH2_DIR,
	MX51_PAD_EIM_A26__USBH2_STP,
	MX51_PAD_EIM_A27__USBH2_NXT,
	MX51_PAD_EIM_D16__USBH2_DATA0,
	MX51_PAD_EIM_D17__USBH2_DATA1,
	MX51_PAD_EIM_D18__USBH2_DATA2,
	MX51_PAD_EIM_D19__USBH2_DATA3,
	MX51_PAD_EIM_D20__USBH2_DATA4,
	MX51_PAD_EIM_D21__USBH2_DATA5,
	MX51_PAD_EIM_D22__USBH2_DATA6,
	MX51_PAD_EIM_D23__USBH2_DATA7,
};

iomux_v3_cfg_t efikamx_usbctrl_pads[] = {
	MX51_PAD_EIM_D27__GPIO2_9,
	MX51_PAD_GPIO1_5__GPIO1_5,
	MX51_PAD_EIM_A22__GPIO2_16,
	MX51_PAD_EIM_A16__GPIO2_10,
	MX51_PAD_EIM_A17__GPIO2_11,
};

#define EFIKAMX_USB_HUB_RESET	IMX_GPIO_NR(1, 5)
#define EFIKAMX_USB_PHY_RESET	IMX_GPIO_NR(2, 9)
#define EFIKAMX_USB_WLAN_ENABLE	IMX_GPIO_NR(2, 16)
#define EFIKAMX_USB_WLAN_RESET	IMX_GPIO_NR(2, 10)
#define EFIKAMX_USB_BT_RESET	IMX_GPIO_NR(2, 11)

iomux_v3_cfg_t efikasb_usbctrl_pads[] = {
	MX51_PAD_NANDF_CS0__GPIO3_16,
	MX51_PAD_EIM_CS1__GPIO2_26,
	MX51_PAD_CSI1_HSYNC__GPIO3_15,
	MX51_PAD_CSI2_D13__GPIO4_10,
};

#define EFIKASB_USB_CAMERA_ENABLE	IMX_GPIO_NR(3, 16)
#define EFIKASB_USB_WWAN_RESET		IMX_GPIO_NR(3, 15)

#define EFIKASB_WWAN_SIM_CD		IMX_GPIO_NR(2, 26)
#define EFIKASB_RFKILL_SW		IMX_GPIO_NR(4, 10)

/*
 * Configure the USB H1 and USB H2 IOMUX
 */
void efikamx_setup_usb(void)
{
	imx_iomux_v3_setup_multiple_pads(efikamx_usbh1_pads,
					ARRAY_SIZE(efikamx_usbh1_pads));
	imx_iomux_v3_setup_multiple_pads(efikamx_usbctrl_pads,
					ARRAY_SIZE(efikamx_usbctrl_pads));
	gpio_direction_output(EFIKAMX_USB_HUB_RESET, 0);
	gpio_direction_output(EFIKAMX_USB_PHY_RESET, 0);
	gpio_direction_output(EFIKAMX_USB_WLAN_ENABLE, 0);
	gpio_direction_output(EFIKAMX_USB_WLAN_RESET, 0);
	gpio_direction_output(EFIKAMX_USB_BT_RESET, 0);

	if (machine_is_efikasb()) {
		imx_iomux_v3_setup_multiple_pads(efikasb_usbh2_pads,
						ARRAY_SIZE(efikasb_usbh2_pads));
		imx_iomux_v3_setup_multiple_pads(efikasb_usbctrl_pads,
						ARRAY_SIZE(efikasb_usbctrl_pads));
		gpio_direction_input(EFIKASB_RFKILL_SW);
		gpio_direction_input(EFIKASB_WWAN_SIM_CD);
		gpio_direction_output(EFIKASB_USB_WWAN_RESET, 0);
		gpio_direction_output(EFIKASB_USB_CAMERA_ENABLE, 0);
	}
}

/*
 * Enable devices connected to USB BUSes
 */
static void efikamx_usb_enable_devices(void)
{
	/* Enable Bluetooth */
	gpio_set_value(EFIKAMX_USB_BT_RESET, 0);
	udelay(10000);
	gpio_set_value(EFIKAMX_USB_BT_RESET, 1);

	/* Enable WiFi */
	gpio_set_value(EFIKAMX_USB_WLAN_ENABLE, 1);
	udelay(10000);

	/* Reset the WiFi chip */
	gpio_set_value(EFIKAMX_USB_WLAN_RESET, 0);
	udelay(10000);
	gpio_set_value(EFIKAMX_USB_WLAN_RESET, 1);

	if (machine_is_efikasb()) {
		gpio_set_value(EFIKASB_USB_WWAN_RESET, 1);
		gpio_set_value(EFIKASB_USB_CAMERA_ENABLE, 1);
	}
}

/*
 * Reset USB HUB (or HUBs on EfikaSB)
 */
static void efikamx_usb_hub_reset(void)
{
	/* HUB reset */
	gpio_set_value(EFIKAMX_USB_HUB_RESET, 1);
	udelay(1000);
	gpio_set_value(EFIKAMX_USB_HUB_RESET, 0);
	udelay(1000);
	gpio_set_value(EFIKAMX_USB_HUB_RESET, 1);
}

/*
 * Reset USB PHY (or PHYs on EfikaSB)
 */
static void efikamx_usb_phy_reset(void)
{
	/* SMSC 3317 PHY reset */
	gpio_set_value(EFIKAMX_USB_PHY_RESET, 0);
	udelay(1000);
	gpio_set_value(EFIKAMX_USB_PHY_RESET, 1);
}

static void efikamx_ehci_init(struct usb_ehci *ehci,
			iomux_v3_cfg_t stp_pad,
			iomux_v3_cfg_t gpio_pad, u32 nr)
{
	int ret;
	struct ulpi_regs *ulpi = (struct ulpi_regs *)0;
	struct ulpi_viewport ulpi_vp;

	imx_iomux_v3_setup_pad(gpio_pad);
	gpio_direction_output(nr, 0);		udelay(1000);
	gpio_set_value(nr, 1);				udelay(1000);
	imx_iomux_v3_setup_pad(stp_pad);	udelay(10000);

	ulpi_vp.viewport_addr = (u32)&ehci->ulpi_viewpoint;
	ulpi_vp.port_num = 0;

	ret = ulpi_init(&ulpi_vp);
	if (ret) {
		printf("Efika MX USB ULPI initialization failed\n");
		return;
	}

	/* ULPI set flags */
	ulpi_write(&ulpi_vp, &ulpi->otg_ctrl,
			ULPI_OTG_DP_PULLDOWN | ULPI_OTG_DM_PULLDOWN |
			ULPI_OTG_EXTVBUSIND);
	ulpi_write(&ulpi_vp, &ulpi->function_ctrl,
			ULPI_FC_FULL_SPEED | ULPI_FC_OPMODE_NORMAL |
			ULPI_FC_SUSPENDM);
	ulpi_write(&ulpi_vp, &ulpi->iface_ctrl, 0);

	/* Set VBus */
	ulpi_write(&ulpi_vp, &ulpi->otg_ctrl_set,
			ULPI_OTG_DRVVBUS | ULPI_OTG_DRVVBUS_EXT);

	/*
	 * Set VBusChrg
	 *
	 * NOTE: This is a technical USB specification violation, but on
	 * the Efika MX the VBUS line here isn't actually connected to
	 * anything except hub enables, which means it's just a quirky
	 * way of doing it. In theory, only the Smartbook should need it,
	 * but the Smarttop schematics belie some bug somewhere, somehow
	 * which means until we know what the bug actually is we have to
	 * do the same thing.
	 */
	ulpi_write(&ulpi_vp, &ulpi->otg_ctrl_set, ULPI_OTG_CHRGVBUS);
	
	udelay(1000);
}

int board_ehci_hcd_init(int port)
{
	efikamx_usb_phy_reset();
	efikamx_usb_hub_reset();
	efikamx_usb_enable_devices();

	return 0;
}

void ehci_powerup_fixup(uint32_t *status_reg, uint32_t *reg)
{
	uint32_t port = OTG_BASE_ADDR + (0x200 * CONFIG_MXC_USB_PORT);
	struct usb_ehci *ehci = (struct usb_ehci *)port;
	struct ulpi_regs *ulpi = (struct ulpi_regs *)0;
	struct ulpi_viewport ulpi_vp;

	ulpi_vp.viewport_addr = (u32)&ehci->ulpi_viewpoint;
	ulpi_vp.port_num = 0;

	ulpi_write(&ulpi_vp, &ulpi->otg_ctrl_set, ULPI_OTG_CHRGVBUS);

	mdelay(50);

	/* terminate the reset */
	*reg = ehci_readl(status_reg);
	*reg |= EHCI_PS_PE;
}

void board_ehci_hcd_postinit(struct usb_ehci *ehci, int port)
{
	uint32_t tmp;

	if (port == 0) {
		/* Adjust UTMI PHY frequency to 24MHz */
		tmp = readl(OTG_BASE_ADDR + 0x80c);
		tmp = (tmp & ~0x3) | 0x01;
		writel(tmp, OTG_BASE_ADDR + 0x80c);
	} else if (port == 1) {
		efikamx_ehci_init(ehci,
				MX51_PAD_USBH1_STP__USBH1_STP,
				MX51_PAD_USBH1_STP__GPIO1_27,
				IMX_GPIO_NR(1, 27));
	} else if ((port == 2) && machine_is_efikasb()) {
		efikamx_ehci_init(ehci,
				MX51_PAD_EIM_A26__USBH2_STP,
				MX51_PAD_EIM_A26__GPIO2_20,
				IMX_GPIO_NR(2, 20));
	}
}

/*
 * Ethernet on the Smarttop is on the USB bus. Rather than give an error about
 * "CPU Net Initialization Failed", just pass this test since no other settings
 * are required. Smartbook doesn't have built-in Ethernet but we will let it
 * pass anyway considering someone may have plugged in a USB stick and all
 * they need to do is run "usb start".
 */
int board_eth_init(bd_t *bis)
{
	return 0;
}
