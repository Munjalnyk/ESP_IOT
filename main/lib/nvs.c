#include <include.h>
SemaphoreHandle_t xSemaphore;

void XLOG(char *function, char *location, const char *format, ...);

void XNVS_INIT()
{
   
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    xSemaphoreGive(xSemaphore);
    }
}

void XNVS_WRITE(char *NAMESPACE, char *LOCATION, char *DATA)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        // Handle the error
    }

    err = nvs_set_str(nvs_handle, LOCATION, DATA);
    if (err != ESP_OK)
    {
        // Handle the error
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        // Handle the error
    }

    nvs_close(nvs_handle);
    XLOG("NVS_WRITE", "SD","NVS NAMESPACE: %s, NVS LOCATION: %S, NVS_WRITE: %s",NAMESPACE, LOCATION, DATA);
}

char *XNVS_READ(char *NAMESPACE, char *LOCATION)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        // Handle the error
    }

    size_t required_size = 0; // Initialize with 0 to get the required size first
    char *DATA = NULL;

    err = nvs_get_str(nvs_handle, LOCATION, NULL, &required_size);
    if (err != ESP_OK)
    {
        // Handle the error
    }
    else
    {
        DATA = (char *)malloc(required_size);
        err = nvs_get_str(nvs_handle, LOCATION, DATA, &required_size);
        if (err != ESP_OK)
        {
            // Handle the error
        }
    }

    nvs_close(nvs_handle);
    if (DATA != NULL)
    {
        XLOG("NVS_READ ", "SD", "NVS NAMESPACE: %s, NVS LOCATION: %S, NVS_READ: %s",NAMESPACE, LOCATION, DATA);
        free(DATA); // Don't forget to free the allocated memory when you're done with it
    }
    else
    {
        // Handle the case where the data is not found in NVS
        XLOG("NVS_READ ", "SD", "NVS NAMESPACE: %s, NVS LOCATION: %S, NVS_READ: NULL",NAMESPACE, LOCATION);
        
    }
    return DATA;
}

void XNVS_ERASE()
{
    nvs_flash_erase();
    XLOG("NVS_ERASE", "SD", "NVS_ERASE");
    nvs_flash_init();
}
