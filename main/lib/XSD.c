#include "include.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define PIN_MISO 2
#define PIN_MOSI 15
#define PIN_CLK 14
#define PIN_CS 13

#define MOUNT_POINT "/sdcard"

#define EXAMPLE_MAX_CHAR_SIZE 128
esp_err_t ret;
sdmmc_card_t *card;

void XSD_write_file(char *path, char *data)
{
    FILE *f = fopen(path, "a");
    if (f == NULL)
    {
        XLOG("XSD_write_file","SD","Failed to open file for writing");
    }
    fprintf(f, "%s", data); 
    fclose(f);     
}

#define CHUNK_SIZE 1024  // Adjust the chunk size as needed

static esp_err_t XSD_read_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        XLOG("XSD_read_file", "SD", "Failed to open file for reading");
        return ESP_FAIL;
    }

    fseek(f, 0, SEEK_END); // seek to the end of the file
    long file_size = ftell(f); // get the file size
    fseek(f, 0, SEEK_SET); // rewind to the beginning of the file

    if (file_size <= 0)
    {
        fclose(f);
        XLOG("XSD_read_file", "SD", "File is empty");
        return ESP_FAIL;
    }

    char *contents = (char *)malloc(file_size + 1); // +1 for null terminator
    if (contents == NULL)
    {
        fclose(f);
        XLOG("XSD_read_file", "SD", "Memory allocation error");
        return ESP_FAIL;
    }

    fread(contents, 1, file_size, f);
    contents[file_size] = '\0'; // null-terminate the string
    fclose(f);

    // Split the contents into chunks and log each chunk
    for (int i = 0; i < file_size; i += CHUNK_SIZE)
    {
        int chunk_size = MIN(CHUNK_SIZE, file_size - i);
        char *chunk = strndup(contents + i, chunk_size);
        if (chunk != NULL)
        {
            XLOG("XSD_read_file", "SD", "%s", chunk);
            esp_mqtt_client_publish(client, "munjal/out", chunk, 0, 0, 0);
            free(chunk);
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }
    }

    free(contents); // Don't forget to free the allocated memory

    return ESP_OK;
}


void XLOGDATA(char *data)
{
    char *file_LOG1 = MOUNT_POINT "/LOG.txt";
    XSD_write_file(file_LOG1, data);
}
void XSD_INIT()
{
      xSemaphoreGive(xSemaphore);
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
	{
     
     esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_MOSI,
        .miso_io_num = PIN_MISO,
        .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

      ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK)
    {
        XLOG("XSD_INIT","SD","Failed to initialize bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
      ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

        if (ret != ESP_OK)
        {
            if (ret == ESP_FAIL)
            {
                ESP_LOGE(TAG, "Failed to mount filesystem. "
                              "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
            }
            else
            {
                ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                              "Make sure SD card lines have pull-up resistors in place.",
                         esp_err_to_name(ret));
            }
            return;
        }
    
    ESP_LOGI(TAG, "Filesystem mounted");

    sdmmc_card_print_info(stdout, card);
    char *file_LOG = MOUNT_POINT "/LOG.txt";
    char data[EXAMPLE_MAX_CHAR_SIZE];
    char *Current_Date_Time = (char *)malloc(100);
    Get_current_date_time(Current_Date_Time);
    snprintf(data, EXAMPLE_MAX_CHAR_SIZE, "%s: %s: %s;\n", Current_Date_Time, "LOG INIT", card->cid.name);
    XSD_write_file(file_LOG, data);
    xSemaphoreGive(xSemaphore);
}
}

void XSD_DEINIT()
{
   
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    XLOG("XSD_DEINIT","SD","Card unmounted");
    spi_bus_free(host.slot);
}