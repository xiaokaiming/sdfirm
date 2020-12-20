/*
 * ZETALOG's Personal COPYRIGHT
 *
 * Copyright (c) 2020
 *    ZETALOG - "Lv ZHENG".  All rights reserved.
 *    Author: Lv "Zetalog" Zheng
 *    Internet: zhenglv@hotmail.com
 *
 * This COPYRIGHT used to protect Personal Intelligence Rights.
 * Redistribution and use in source and binary forms with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the Lv "Zetalog" ZHENG.
 * 3. Neither the name of this software nor the names of its developers may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 4. Permission of redistribution and/or reuse of souce code partially only
 *    granted to the developer(s) in the companies ZETALOG worked.
 * 5. Any modification of this software should be published to ZETALOG unless
 *    the above copyright notice is no longer declaimed.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ZETALOG AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE ZETALOG OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)dw_xgmac.h: Synopsys DesignWare 10G ethernet MAC interface
 * $Id: dw_xgmac.h,v 1.0 2020-12-17 17:20:00 zhenglv Exp $
 */

#ifndef __DW_XGMAC_H_INCLUDE__
#define __DW_XGMAC_H_INCLUDE__

#define DWCXG_CORE			0
#define DWCXG_MTL			0x1000
#define DWCXG_MTL_TCQ(n)		(0x1100 + (n) << 7)
#define DWCXG_DMA			0x3000
#define DWCXG_DMA_CH(c)			(0x3100 + (n) << 7)

#define XGMAC_REG(offset)		(DW_XGMAC_BASE + (offset))
#define DWCXG_CORE_REG(offset)		XGMAC_REG(DWCXG_CORE + (offset))
#define DWCXG_MTL_REG(offset)		XGMAC_REG(DWCXG_MTL + (offset))
#define DWCXG_MTL_TCQ_REG(q, offset)	XGMAC_REG(DWCXG_MTL_TCQ(q) + (offset))
#define DWCXG_DMA_REG(offset)		XGMAC_REG(DWCXG_DMA + (offset))
#define DWCXG_DMA_CH_REG(c, offset)	XGMAC_REG(DWCXG_DMA_CH(c) + (offset))

#define MAC_TX_CONFIG				DWCXG_CORE_REG(0x000)
#define MAC_RX_CONFIG				DWCXG_CORE_REG(0x004)
#define MAC_PACKET_FILTER			DWCXG_CORE_REG(0x008)
#define MAC_WATCHDOG_TIMEOUT			DWCXG_CORE_REG(0x00C)
#define MAC_HASH_TABLE(n)			DWCXG_CORE_REG(0x010 + (n) << 2)
#define MAC_VLAN_TAG				DWCXG_CORE_REG(0x050)
#define MAC_VLAN_TAG_CTRL			DWCXG_CORE_REG(0x050) /* redefined */
#define MAC_VLAN_TAG_DATA			DWCXG_CORE_REG(0x054)
#define MAC_VLAN_TAG_FILTER(n)			DWCXG_CORE_REG(0x054 + (n) << 2)
#define MAC_VLAN_HASH_TABLE			DWCXG_CORE_REG(0x058)
#define MAC_VLAN_INCL(n)			DWCXG_CORE_REG(0x060 + (n) << 2)
#define MAC_INNER_VLAN_INCL			DWCXG_CORE_REG(0x064)
#define MAC_RX_ETH_TYPE_MATCH			DWCXG_CORE_REG(0x06C)
#define MAC_Q0_TX_FLOW_CTRL(n)			DWCXG_CORE_REG(0x070 + (n) << 2)
#define MAC_RX_FLOW_CTRL			DWCXG_CORE_REG(0x090)
#define MAC_RXQ_CTRL4				DWCXG_CORE_REG(0x094)
#define MAC_RXQ_CTRL0				DWCXG_CORE_REG(0x0A0)
#define MAC_RXQ_CTRL1				DWCXG_CORE_REG(0x0A4)
#define MAC_RXQ_CTRL2				DWCXG_CORE_REG(0x0A8)
#define MAC_RXQ_CTRL3				DWCXG_CORE_REG(0x0AC)
#define MAC_INTERRUPT_STATUS			DWCXG_CORE_REG(0x0B0)
#define MAC_INTERRUPT_ENABLE			DWCXG_CORE_REG(0x0B4)
#define MAC_RX_TX_STATUS			DWCXG_CORE_REG(0x0B8)
#define MAC_PMT_CONTROL_STATUS			DWCXG_CORE_REG(0x0C0)
/* TODO: RWK_Filter */
#define MAC_LPI_CONTROL_STATUS			DWCXG_CORE_REG(0x0D0)
#define MAC_LPI_TIMERS_CONTROL			DWCXG_CORE_REG(0x0D4)
#define MAC_LPI_AUTO_ENTRY_TIMER		DWCXG_CORE_REG(0x0D8)
#define MAC_1US_TIC_COUNTER			DWCXG_CORE_REG(0x0DC)
#define MAC_TUNNEL_IDENTIFIER			DWCXG_CORE_REG(0x0E0)
#define MAC_VERSION				DWCXG_CORE_REG(0x110)
#define MAC_HW_FEATURE0				DWCXG_CORE_REG(0x11C)
#define MAC_HW_FEATURE1				DWCXG_CORE_REG(0x120)
#define MAC_HW_FEATURE2				DWCXG_CORE_REG(0x124)
#define MAC_HW_FEATURE3				DWCXG_CORE_REG(0x128)
#define MAC_EXTENDED_CONFIGURATION		DWCXG_CORE_REG(0x140)
#define MDIO_SINGLE_COMMAND_ADDRESS		DWCXG_CORE_REG(0x200)
#define MDIO_SINGLE_COMMAND_CONTROL_DATA	DWCXG_CORE_REG(0x204)
#define MDIO_CONTINUOUS_WRITE_ADDRESS		DWCXG_CORE_REG(0x208)
#define MDIO_CONTINUOUS_WRITE_DATA		DWCXG_CORE_REG(0x20C)
#define MDIO_CONTINUOUS_SCAN_PORT_ENABLE	DWCXG_CORE_REG(0x210)
#define MDIO_INTERRUPT_STATUS			DWCXG_CORE_REG(0x214)
#define MDIO_INTERRUPT_ENABLE			DWCXG_CORE_REG(0x218)
#define MDIO_PORT_CONNECT_DISCONNECT_STATUS	DWCXG_CORE_REG(0x21C)
#define MDIO_CLAUSE_22_PORT			DWCXG_CORE_REG(0x220)
#define MDIO_PORT_NX4_INDIRECT_CONTROL		DWCXG_CORE_REG(0x224)
#define MDIO_PORTNX4P0_DEVICE_IN_USE		DWCXG_CORE_REG(0x230)
#define MDIO_PORTNX4P0_LINK_STATUS		DWCXG_CORE_REG(0x234)
#define MDIO_PORTNX4P0_ALIVE_STATUS		DWCXG_CORE_REG(0x238)
#define MDIO_PORTNX4P1_DEVICE_IN_USE		DWCXG_CORE_REG(0x240)
#define MDIO_PORTNX4P1_LINK_STATUS		DWCXG_CORE_REG(0x244)
#define MDIO_PORTNX4P1_ALIVE_STATUS		DWCXG_CORE_REG(0x248)
#define MDIO_PORTNX4P2_DEVICE_IN_USE		DWCXG_CORE_REG(0x250)
#define MDIO_PORTNX4P2_LINK_STATUS		DWCXG_CORE_REG(0x254)
#define MDIO_PORTNX4P2_ALIVE_STATUS		DWCXG_CORE_REG(0x258)
#define MDIO_PORTNX4P3_DEVICE_IN_USE		DWCXG_CORE_REG(0x260)
#define MDIO_PORTNX4P3_LINK_STATUS		DWCXG_CORE_REG(0x264)
#define MDIO_PORTNX4P3_ALIVE_STATUS		DWCXG_CORE_REG(0x268)
#define MAC_GPIO_CONTROL			DWCXG_CORE_REG(0x278)
#define MAC_GPIO_STATUS				DWCXG_CORE_REG(0x27C)
#define MAC_ADDRESS_HIGH(n)			DWCXG_CORE_REG(0x300 + (n) << 3)
#define MAC_ADDRESS_LOW(n)			DWCXG_CORE_REG(0x304 + (n) << 3)
#define MAC_MMC_CONTROL				DWCXG_CORE_REG(0x800)
#define MAC_MMC_RX_INTERRUPT			DWCXG_CORE_REG(0x804)
#define MAC_MMC_TX_INTERRUPT			DWCXG_CORE_REG(0x808)
#define MAC_MMC_RECEIVE_INTERRUPT_ENABLE	DWCXG_CORE_REG(0x80C)
#define MAC_MMC_TRANSMIT_INTERRUPT_ENABLE	DWCXG_CORE_REG(0x810)
/* TODO: COUNT, ERROR */

#endif /* __DW_XGMAC_H_INCLUDE__ */
