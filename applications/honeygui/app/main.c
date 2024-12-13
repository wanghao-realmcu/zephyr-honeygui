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

static const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(dummy_display));


int main(void)
{
	printf("Hollo HoneyGUI!, name = %s \n", dev->name);
	return 0;
}
