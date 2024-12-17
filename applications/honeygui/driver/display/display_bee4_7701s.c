/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 * Copyright (c) 2021 Nordic Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT bee4_st7701s

#include <errno.h>
#include <string.h>

#include <zephyr/drivers/display.h>
#include <zephyr/device.h>
#include "st7701s_480480_rgb.h"


struct bee4_st7701s_display_config {
	uint16_t height;
	uint16_t width;
};

struct bee4_st7701s_display_data {
	enum display_pixel_format current_pixel_format;
};

static uint8_t *fb0 = 0x08000000;
static uint8_t *fb1 = 0x08100000;

static int bee4_st7701s_display_init(const struct device *dev)
{
	struct bee4_st7701s_display_data *disp_data = dev->data;

	disp_data->current_pixel_format = PIXEL_FORMAT_RGB_565;

	printf("line = %d, name = %s \n", __LINE__, dev->name);

	rtk_lcd_hal_init();

	rtk_lcd_hal_update_framebuffer(fb0, 0);

	return 0;
}

static void lcd_update_window(uint8_t *input, uint8_t *output, uint16_t xStart, uint16_t yStart,
                              uint16_t w, uint16_t h)
{

    uint16_t *read = (uint16_t *)input;
    uint16_t *write = (uint16_t *)output;

    for (uint32_t i = yStart; i < (h + yStart); i++)
    {
        for (uint32_t j = xStart; j < (w + xStart); j++)
        {
            write[i * 480 + j] = *read;
            read++;
        }
    }
}

static int bee4_st7701s_display_write(const struct device *dev, const uint16_t x,
			       const uint16_t y,
			       const struct display_buffer_descriptor *desc,
			       const void *buf)
{
	const struct bee4_st7701s_display_config *config = dev->config;


	bool full_fb = false;
	if((desc->width == 480) && (desc->height == 480) && (x == 0) && (y == 0))
	{
		full_fb = true;	
	}
	if(full_fb == true)
	{
		rtk_lcd_hal_update_framebuffer(buf, desc->buf_size);
	}
	else /* We must use partial framebuffer copy */
	{
		/*
			* We must copy the entire current framebuffer to new
			* buffer, since we wil change the active buffer
			* address
			*/
		//todo by howie

	}

	return 0;
}

static int bee4_st7701s_display_read(const struct device *dev, const uint16_t x,
			      const uint16_t y,
			      const struct display_buffer_descriptor *desc,
			      void *buf)
{
	return -ENOTSUP;
}

static void *bee4_st7701s_display_get_framebuffer(const struct device *dev)
{
	return NULL;
}

static int bee4_st7701s_display_blanking_off(const struct device *dev)
{
	return 0;
}

static int bee4_st7701s_display_blanking_on(const struct device *dev)
{
	return 0;
}

static int bee4_st7701s_display_set_brightness(const struct device *dev,
					const uint8_t brightness)
{
	return 0;
}

static int bee4_st7701s_display_set_contrast(const struct device *dev,
				      const uint8_t contrast)
{
	return 0;
}

static void bee4_st7701s_display_get_capabilities(const struct device *dev,
		struct display_capabilities *capabilities)
{
	const struct bee4_st7701s_display_config *config = dev->config;
	struct bee4_st7701s_display_data *disp_data = dev->data;

	memset(capabilities, 0, sizeof(struct display_capabilities));
	capabilities->x_resolution = config->width;
	capabilities->y_resolution = config->height;
	capabilities->supported_pixel_formats = PIXEL_FORMAT_ARGB_8888 |
		PIXEL_FORMAT_RGB_888 |
		PIXEL_FORMAT_MONO01 |
		PIXEL_FORMAT_MONO10;
	capabilities->current_pixel_format = disp_data->current_pixel_format;
	capabilities->screen_info = SCREEN_INFO_MONO_VTILED |
		SCREEN_INFO_MONO_MSB_FIRST;
}

static int bee4_st7701s_display_set_pixel_format(const struct device *dev,
		const enum display_pixel_format pixel_format)
{
	struct bee4_st7701s_display_data *disp_data = dev->data;

	disp_data->current_pixel_format = pixel_format;
	return 0;
}

static const struct display_driver_api bee4_st7701s_display_api = {
	.blanking_on = bee4_st7701s_display_blanking_on,
	.blanking_off = bee4_st7701s_display_blanking_off,
	.write = bee4_st7701s_display_write,
	.read = bee4_st7701s_display_read,
	.get_framebuffer = bee4_st7701s_display_get_framebuffer,
	.set_brightness = bee4_st7701s_display_set_brightness,
	.set_contrast = bee4_st7701s_display_set_contrast,
	.get_capabilities = bee4_st7701s_display_get_capabilities,
	.set_pixel_format = bee4_st7701s_display_set_pixel_format,
};

#define DISPLAY_DUMMY_DEFINE(n)						\
	static const struct bee4_st7701s_display_config dd_config_##n = {	\
		.height = DT_INST_PROP(n, height),			\
		.width = DT_INST_PROP(n, width),			\
	};								\
									\
	static struct bee4_st7701s_display_data dd_data_##n;			\
									\
	DEVICE_DT_INST_DEFINE(n, &bee4_st7701s_display_init, NULL,		\
			      &dd_data_##n,				\
			      &dd_config_##n,				\
			      POST_KERNEL,				\
			      12,		\
			      &bee4_st7701s_display_api);			\

DT_INST_FOREACH_STATUS_OKAY(DISPLAY_DUMMY_DEFINE)
