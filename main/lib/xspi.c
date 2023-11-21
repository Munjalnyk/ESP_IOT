#include <include.h>

// SPI configuration
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22
#define SPI_NUM      HSPI_HOST // or VSPI_HOST
#define SPI_SPEED    1000000  // SPI clock frequency (1 MHz)

spi_device_handle_t spi;

void initialize_spi() {
     if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){
    esp_err_t ret;
    spi_bus_config_t bus_config = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };

    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = SPI_SPEED,
        .mode = 0, // SPI mode 0
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    ret = spi_bus_initialize(SPI_NUM, &bus_config, 0);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI_NUM, &dev_config, &spi);
    ESP_ERROR_CHECK(ret);
    xSemaphoreGive(xSemaphore);
    XLOG("initialize_spi", "MQTT", "Initialized spi");
    
     }
}
void XSPIWRITE(const uint8_t *data, size_t length) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8 * length; // 8 bits per byte
    t.tx_buffer = data;

    esp_err_t ret = spi_device_polling_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);
    XLOG("XSPIWRITE","SD", "SPI write data :%s",data);
}

void XSPIREAD(uint8_t *data, size_t length) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8 * length; // 8 bits per byte
    t.rx_buffer = data;

    esp_err_t ret = spi_device_polling_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);
    XLOG("XSPIREAD","SD", "SPI read data :%s",data);
}