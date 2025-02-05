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
 * @(#)sbi.c: DUOWEN specific SBI implementation
 * $Id: sbi.c,v 1.1 2020-03-14 22:31:00 zhenglv Exp $
 */

#include <target/sbi.h>
#include <target/fdt.h>
#include <target/uart.h>
#include <target/irq.h>
#include <target/delay.h>
#include <target/ras.h>
#include <target/noc.h>
#include <target/eth.h>
#include <target/iommu.h>

static void duowen_modify_fdt(void *fdt)
{
	fdt_cpu_fixup(fdt);
	fdt_irq_fixup(fdt, "riscv,clint0");
	fdt_irqs_fixup(fdt, "riscv,plic0", PLIC_MAX_CHIPS);
	fdt_fixups(fdt);
}

static int duowen_early_init(bool cold_boot)
{
#ifndef CONFIG_DUOWEN_APC_BOOT_HOOK
	vaisra_cpu_init();
	if (cold_boot) {
		duowen_pma_soc_init();
		duowen_apc_noc_init();
		duowen_smmu_pma_init();
	}
	duowen_pma_cpu_init();
#endif

	if (cold_boot) {
		board_init_timestamp();
#ifdef CONFIG_DUOWEN_IMC
		/* Booting IMC kernel in DDR requires cohfab/ddr to work
		 * at proper frequencies.
		 */
		clk_enable(cohfab_clk);
		clk_enable(ddr_aclk);
#endif /* CONFIG_DUOWEN_IMC */
	}
	return 0;
}

static int duowen_final_init(bool cold_boot)
{
	if (!cold_boot)
		return 0;

	duowen_gpio_init();
	duowen_eth_init();

#ifdef CONFIG_SBI_BOOT_PRINTS
	sbi_printf("Chiplink PCIe: %c\n",
		   rom_get_chiplink_ready() ? 'Y' : 'N');
	sbi_printf("Chiplink PLIC: %c\n",
		   rom_get_pliccntl_done() ? 'Y' : 'N');
	sbi_printf("APC clusters: %d - %02x\n",
		   rom_get_cluster_num(), rom_get_cluster_map());
	sbi_printf("APC CPUs: S0=%04x S1=%04x\n",
		   rom_get_s0_apc_map(), rom_get_s1_apc_map());
#endif

	duowen_modify_fdt(sbi_scratch_thishart_arg1_ptr());
	return 0;
}

static uint32_t duowen_pmp_region_count(uint32_t hartid)
{
	return 1;
}

static int duowen_pmp_region_info(uint32_t hartid, uint32_t index,
				  ulong *prot, ulong *addr, ulong *log2size)
{
	int ret = 0;

	switch (index) {
	case 0:
		*prot	  = PMP_R | PMP_W | PMP_X;
		*addr	  = 0;
		*log2size = __riscv_xlen;
		break;
	default:
		ret = -1;
		break;
	};
	return ret;
}

#define __DUOWEN_UART_REG(n, offset)		(__DUOWEN_UART_BASE + (offset))
#define __UART_RBR(n)				__DUOWEN_UART_REG(n, 0x00)
#define __UART_THR(n)				__DUOWEN_UART_REG(n, 0x00)
#define __UART_LSR(n)				__DUOWEN_UART_REG(n, 0x14)
#define __duowen_uart_write_poll(n)		(!!(__raw_readl(__UART_LSR(n)) & LSR_TEMT))
#define __duowen_uart_read_poll(n)		(!!(__raw_readl(__UART_LSR(n)) & LSR_DR))
#define __duowen_uart_read_byte(n)		__raw_readl(__UART_RBR(n))
#define __duowen_uart_write_byte(n, byte)	__raw_writel((byte), __UART_THR(n))

static void duowen_console_putc(char ch)
{
	while (!__duowen_uart_write_poll(UART_CON_ID));
	__duowen_uart_write_byte(UART_CON_ID, ch);
}

static int duowen_console_getc(void)
{
	if (!__duowen_uart_read_poll(UART_CON_ID))
		return -1;
	return __duowen_uart_read_byte(UART_CON_ID);
}

static int duowen_irqchip_init(bool cold_boot)
{
	if (cold_boot)
		plic_sbi_init_cold();
	plic_sbi_init_warm(sbi_processor_id());
	return 0;
}

#ifdef CONFIG_DUOWEN_APC
void duowen_ipi_send(uint32_t target_cpu)
{
	clint_set_ipi(target_cpu);
}

void duowen_ipi_sync(uint32_t target_cpu)
{
	clint_sync_ipi(target_cpu);
}

void duowen_ipi_clear(uint32_t target_cpu)
{
	clint_clear_ipi(target_cpu);
}

static int duowen_ipi_init(bool cold_boot)
{
	cpu_t cpu = sbi_processor_id();

	if (!cold_boot)
		duowen_ipi_clear(cpu);
	return 0;
}

uint64_t duowen_timer_value(void)
{
	return clint_read_mtime();
}

void duowen_timer_event_stop(void)
{
	cpu_t cpu = sbi_processor_id();

	clint_unset_mtimecmp(cpu);
}

void duowen_timer_event_start(uint64_t next_event)
{
	cpu_t cpu = sbi_processor_id();

	clint_set_mtimecmp(cpu, next_event);
}

static bool duowen_hart_disabled(uint32_t hartid)
{
	if (__GOOD_CPU_MASK == GOOD_CPU_MASK)
		return ~rom_get_apc_map() & CPU_TO_MASK(hartid);
	return false;
}
#endif /* CONFIG_DUOWEN_APC */

#ifdef CONFIG_DUOWEN_IMC
void duowen_ipi_send(uint32_t target_cpu)
{
}

void duowen_ipi_sync(uint32_t target_cpu)
{
}

void duowen_ipi_clear(uint32_t target_cpu)
{
}

static int duowen_ipi_init(bool cold_boot)
{
	return 0;
}

uint64_t duowen_timer_value(void)
{
	return tmr_read_counter();
}

void duowen_timer_event_stop(void)
{
	tmr_write_compare(ULL(-1));
}

void duowen_timer_event_start(uint64_t next_event)
{
	tmr_write_compare(next_event);
}

static bool duowen_hart_disabled(uint32_t hartid)
{
	return false;
}
#endif /* CONFIG_DUOWEN_IMC */

static int duowen_timer_init(bool cold_boot)
{
	if (!cold_boot)
		duowen_timer_event_stop();
	return 0;
}

static int duowen_system_down(uint32_t type)
{
	msg_imc_shutdown();
	return 0;
}

static int duowen_system_finish(uint32_t code)
{
	if (code)
		msg_imc_failure();
	else
		msg_imc_success();
	return 0;
}

static int duowen_process_irq(uint32_t irq)
{
#ifdef CONFIG_VAISRA_RAS
	if (irq == IRQ_M_NMI) {
		if (sbi_process_vaisra_nmi())
			return 0;
	}
#endif
	return -ENODEV;
}

const struct sbi_platform_operations platform_ops = {
	.pmp_region_count	= duowen_pmp_region_count,
	.pmp_region_info	= duowen_pmp_region_info,
	.early_init		= duowen_early_init,
	.final_init		= duowen_final_init,
	.console_putc		= duowen_console_putc,
	.console_getc		= duowen_console_getc,
	.irqchip_init		= duowen_irqchip_init,
	.ipi_send		= duowen_ipi_send,
	.ipi_sync		= duowen_ipi_sync,
	.ipi_clear		= duowen_ipi_clear,
	.ipi_init		= duowen_ipi_init,
	.timer_value		= duowen_timer_value,
	.timer_event_stop	= duowen_timer_event_stop,
	.timer_event_start	= duowen_timer_event_start,
	.timer_init		= duowen_timer_init,
	.system_reboot		= duowen_system_down,
	.system_shutdown	= duowen_system_down,
	.process_irq		= duowen_process_irq,
	.system_finish		= duowen_system_finish,
	.hart_disabled		= duowen_hart_disabled,
};

const struct sbi_platform platform = {
	.opensbi_version	= OPENSBI_VERSION,
	.platform_version	= SBI_PLATFORM_VERSION(0x0, 0x01),
	.name			= "SmarCo DUOWEN",
	.features		= SBI_PLATFORM_DEFAULT_FEATURES,
	.disabled_hart_mask	= 0,
	.platform_ops_addr	= (unsigned long)&platform_ops
};
