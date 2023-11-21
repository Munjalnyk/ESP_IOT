#include "include.h"

char Current_Date_Time[100];
const char *TAG1 = "SNTP";

static void print_servers(void)
{
	XLOG("print_servers", "SD", "NTP Servers:");

	for (uint8_t i = 0; i < 5; ++i)
	{
		if (esp_sntp_getservername(i))
		{
			XLOG("print_servers", "SD", "server %d: %s", i, esp_sntp_getservername(i));
		}
		else
		{
			// we have either IPv4 or IPv6 address, let's print it
			char buff[INET6_ADDRSTRLEN];
			ip_addr_t const *ip = esp_sntp_getserver(i);
			if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
				XLOG("print_servers", "SD", "server %d: %s", i, buff);
		}
	}
}

void time_sync_notification_cb(struct timeval *tv)
{
	XLOG("time_sync_notification_cb", "SD", "time synchronized");
	print_servers();
}

void Get_current_date_time(char *date_time)
{
	char strftime_buf[64];
	time_t now;
	struct tm timeinfo;
	time(&now);
	localtime_r(&now, &timeinfo);

	// Set timezone to Indian Standard Time
	setenv("TZ", "UTC-05:30", 1);
	tzset();
	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	strcpy(date_time, strftime_buf);
}

static void initialize_sntp(void)
{
	XLOG("initialize_sntp", "SD", "Initializing SNTP");
	esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
	esp_sntp_setservername(0, "pool.ntp.org");
	sntp_set_time_sync_notification_cb(time_sync_notification_cb);
	esp_sntp_init();
	XLOG("initialize_sntp", "SD", "SNTP initialized");
}

static void obtain_time(void)
{
	initialize_sntp();
	// wait for time to be set
	int retry = 0;
	const int retry_count = 10;
	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
	{
		XLOG("obtain_time", "SD", "Waiting for system time to be set... (%d/%d)", retry, retry_count);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
	if (retry >= 9)
	{
		XLOG("obtain_time", "SD", "Time synchronization failed after %d retries. Restarting the device.", retry_count);
		 // Restart the ESP32
	}
	
}
void Set_SystemTime_SNTP()
{
	xSemaphoreGive(xSemaphore);
	if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
	{
		time_t now;
		struct tm timeinfo;
		time(&now);
		localtime_r(&now, &timeinfo);
		// Is time set? If not, tm_year will be (1970 - 1900).
		if (timeinfo.tm_year < (2016 - 1900))
		{
			XLOG("Set_SystemTime_SNTP", "SD", "Time is not set yet. Connecting to server and getting time over NTP.");
			obtain_time();
			// update 'now' variable with current time
			time(&now);
			xSemaphoreGive(xSemaphore);
		}
	}
}