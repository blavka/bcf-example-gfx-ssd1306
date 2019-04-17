#include <application.h>
#include <bc_ssd1306.h>

#define APPLICATION_TASK_ID 0

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;
uint16_t button_press_cnt = 0;

bc_gfx_t gfx;
bc_ssd1306_t ssd1306;
BC_SSD1306_FRAMEBUFFER(ssd1306_framebuffer, 128, 64)

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 200);

        button_press_cnt++;

        bc_scheduler_plan_now(APPLICATION_TASK_ID);
    }
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_ssd1306_init(&ssd1306, BC_I2C_I2C0, BC_SSD1306_ADDRESS_I2C_ADDRESS_DEFAULT, &ssd1306_framebuffer);

    bc_gfx_init(&gfx, &ssd1306, bc_ssd1306_get_driver());
}

void application_task(void)
{
    bc_system_pll_enable();

    bc_gfx_clear(&gfx);

    bc_gfx_set_font(&gfx, &bc_font_ubuntu_13);

    bc_gfx_draw_string(&gfx, 5, 5, "Button pres count", 1);

    bc_gfx_printf(&gfx, 5, 20, 1, "%d", button_press_cnt);

    bc_gfx_update(&gfx);

    bc_system_pll_disable();
}
