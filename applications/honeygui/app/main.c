#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <version.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/display.h>
#include <ctype.h>

#ifdef CONFIG_ARCH_POSIX
#include <unistd.h>
#else
#include <zephyr/posix/unistd.h>
#endif

LOG_MODULE_REGISTER(app);

#include <trace.h>
#include <zephyr/device.h>

#include "clock.h"
#include "fmc_platform.h"

static const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(dummy_display));


int main(void)
{
	printf("Hollo HoneyGUI! \n");
	return 0;
}




static int board_init(void)
{
	DBG_DIRECT("Hollo HoneyGUI, Board Init! \n");
    uint32_t actual_mhz;
    int32_t ret0, ret1, ret2, ret3;
    ret0 = pm_cpu_freq_set(125, &actual_mhz);
    // ret1 = flash_nor_set_seq_trans_enable(FLASH_NOR_IDX_SPIC0, 1);
    // ret2 = fmc_flash_nor_clock_switch(FLASH_NOR_IDX_SPIC0, 160, &actual_mhz);
    // ret3 = flash_nor_try_high_speed_mode(0, FLASH_NOR_DTR_4_BIT_MODE);
    psram_winbond_opi_init();
    pm_spic1_freq_set(160, &actual_mhz); // fmc_psram_clock_switch(PSRAM_IDX_SPIC1, 160, &actual_mhz);
    pm_display_freq_set(CLK_PLL1_SRC, 100, 100);
}


SYS_INIT(board_init, PRE_KERNEL_1, 10);

#include <zephyr/shell/shell.h>
#include <zephyr/kernel.h>

static int cmd_my_command(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "This is my custom command!");
    uint8_t *buf = (uint8_t *)0x8100000;
    for (int i = 0; i < 480 * 480; i = i + 2)
    {
        buf[i] = 0x1F;
        buf[i + 1] = 0;
    }
	struct display_buffer_descriptor desc = {
		.height = 480,
		.pitch = 2,
		.width = 480,
		.buf_size = 480*480*2,
	};
    display_write(dev, 0, 0, &desc, buf);
    return 0;
}

SHELL_CMD_REGISTER(my_command, NULL, "My custom command description", cmd_my_command);

