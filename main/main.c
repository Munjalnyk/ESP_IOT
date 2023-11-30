#include <include.h>

void app_main(void)
{
    XRTCINIT();
    esp_log_level_set("uart", ESP_LOG_NONE);
    XLOG("APP_MAIN", "SD", "APP_MAIN Started");
    XINIT();
    while (1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    XLOG("APP_MAIN", "SD", "Returned from APP_MAIN");
}
