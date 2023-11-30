#include "include.h"
#include "ds3231.h"

i2c_dev_t dev;

void XRTCINIT()
{
    ds3231_init_desc(&dev, I2C_NUM_0, 18, 19);
}

void XgetClock()
{
    float temp;
    struct tm rtcinfo;

    ds3231_get_temp_float(&dev, &temp);

    ds3231_get_time(&dev, &rtcinfo);

    printf("%04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel\n",
           rtcinfo.tm_year, rtcinfo.tm_mon + 1,
           rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, temp);
}

void XsetTime()
{
    time_t now;
	struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    struct tm time = {
		.tm_year = timeinfo.tm_year + 1900,
		.tm_mon  = timeinfo.tm_mon,  // 0-based
		.tm_mday = timeinfo.tm_mday,
		.tm_hour = timeinfo.tm_hour,
		.tm_min  = timeinfo.tm_min,
		.tm_sec  = timeinfo.tm_sec
	};
    ds3231_set_time(&dev, &time);
}