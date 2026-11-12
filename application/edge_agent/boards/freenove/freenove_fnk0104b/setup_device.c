/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_check.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_touch_ft5x06.h"

static const char *TAG = "freenove_fnk0104b";

/*
 * The FNK0104AB reference selects TFT_eSPI's ILI9341_2 driver.
 * Use its panel power/gamma settings rather than the generic ILI9341 defaults.
 * The panel driver and Board Manager handle reset, RGB565, orientation,
 * inversion and display enable. Entries here use actual byte counts and
 * delays, not TFT_eSPI's delay-bit/sentinel encoding.
 */
static const ili9341_lcd_init_cmd_t s_init_cmds[] = {
    {0xC8, (const uint8_t[]){0xFF, 0x93, 0x42}, 3, 0},
    {0xC0, (const uint8_t[]){0x0E, 0x0E}, 2, 0},
    {0xC5, (const uint8_t[]){0xD0}, 1, 0},
    {0xC1, (const uint8_t[]){0x02}, 1, 0},
    {0xB4, (const uint8_t[]){0x02}, 1, 0},
    {0xE0, (const uint8_t[]){0x00, 0x03, 0x08, 0x06, 0x13, 0x09, 0x39, 0x39,
                           0x48, 0x02, 0x0A, 0x08, 0x17, 0x17, 0x0F}, 15, 0},
    {0xE1, (const uint8_t[]){0x00, 0x28, 0x29, 0x01, 0x0D, 0x03, 0x3F, 0x33,
                           0x52, 0x04, 0x0F, 0x0E, 0x37, 0x38, 0x0F}, 15, 0},
    {0xB1, (const uint8_t[]){0x00, 0x1B}, 2, 0},
    {0xB7, (const uint8_t[]){0x06}, 1, 0},
};

static const ili9341_vendor_config_t s_vendor_config = {
    .init_cmds = s_init_cmds,
    .init_cmds_size = sizeof(s_init_cmds) / sizeof(s_init_cmds[0]),
};

esp_err_t lcd_panel_factory_entry_t(esp_lcd_panel_io_handle_t io,
                                    const esp_lcd_panel_dev_config_t *panel_dev_config,
                                    esp_lcd_panel_handle_t *ret_panel)
{
    ESP_RETURN_ON_FALSE(io && panel_dev_config && ret_panel,
                        ESP_ERR_INVALID_ARG, TAG, "Invalid LCD factory arguments");
    esp_lcd_panel_dev_config_t config = *panel_dev_config;
    config.vendor_config = (void *)&s_vendor_config;
    return esp_lcd_new_panel_ili9341(io, &config, ret_panel);
}

esp_err_t lcd_touch_factory_entry_t(esp_lcd_panel_io_handle_t io,
                                    const esp_lcd_touch_config_t *touch_dev_config,
                                    esp_lcd_touch_handle_t *ret_touch)
{
    ESP_RETURN_ON_FALSE(io && touch_dev_config && ret_touch,
                        ESP_ERR_INVALID_ARG, TAG, "Invalid touch factory arguments");
    /* FT6336U uses the FT5x06 coordinate register layout at I2C address 0x38. */
    return esp_lcd_touch_new_i2c_ft5x06(io, touch_dev_config, ret_touch);
}
