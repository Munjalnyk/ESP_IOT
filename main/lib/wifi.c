#include <include.h>
SemaphoreHandle_t xSemaphore;
static const char *TAG = "WIFI";

static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
int syloop = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
						  int32_t event_id, void *event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		XLOG("WIFI_event_handler", "XSD", "WIFI Connecting");
		esp_wifi_connect();
		xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
		syloop++;
		if (syloop == 10)
		{
			XLOG("WIFI_event_handler", "XSD", "WIFI Not Connected");
			wificonn = 0;
			esp_wifi_stop();
			xSemaphoreGive(xSemaphore);
		}
	}
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	{
		wificonn = 1;
		xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
		// esp_netif_init();
		esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
		esp_netif_ip_info_t ip_info;
		esp_netif_get_ip_info(sta_netif, &ip_info);
		char ip_address[16];
		sprintf(ip_address, IPSTR, IP2STR(&ip_info.ip));
		XLOG("WIFI_event_handler", "XSD", "WIFI Connected With IP = %s", ip_address);
		xSemaphoreGive(xSemaphore);
	}
}

void Xinitialise_wifi()
{
	if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
	{
		esp_log_level_set("wifi", ESP_LOG_WARN);
		ESP_ERROR_CHECK(esp_netif_init());
		wifi_event_group = xEventGroupCreate();
		ESP_ERROR_CHECK(esp_event_loop_create_default());
		esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
		assert(ap_netif);
		esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
		assert(sta_netif);
		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		ESP_ERROR_CHECK(esp_wifi_init(&cfg));
		ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &event_handler, NULL));
		ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

		ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
		ESP_ERROR_CHECK(esp_wifi_start());
		xSemaphoreGive(xSemaphore);
	}
}

void wifi_ap(char *ap_ssid, char *ap_pass, int max_conn, int channel)
{
	wifi_config_t wifi_config = {0};
	strcpy((char *)wifi_config.ap.ssid, ap_ssid);
	strcpy((char *)wifi_config.ap.password, ap_pass);
	wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
	wifi_config.ap.ssid_len = strlen(ap_ssid);
	wifi_config.ap.max_connection = max_conn;
	wifi_config.ap.channel = channel;

	if (strlen(ap_pass) == 0)
	{
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());
}

void wifi_sta(char *sta_ssid, char *sta_pass, int timeout_ms)
{
	if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
	{
		wifi_config_t wifi_config = {0};
		strcpy((char *)wifi_config.sta.ssid, sta_ssid);
		strcpy((char *)wifi_config.sta.password, sta_pass);

		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
		ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_connect());
		int bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
									   pdFALSE, pdTRUE, timeout_ms / portTICK_PERIOD_MS);
		ESP_LOGI(TAG, "bits=%x", bits);
	}
}

void wifi_apsta(char *ap_ssid, char *ap_pass, int max_conn, int channel, char *sta_ssid, char *sta_pass, int timeout_ms)
{
	wifi_config_t ap_config = {0};
	strcpy((char *)ap_config.ap.ssid, ap_ssid);
	strcpy((char *)ap_config.ap.password, ap_pass);
	ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
	ap_config.ap.ssid_len = strlen(ap_ssid);
	ap_config.ap.max_connection = max_conn;
	ap_config.ap.channel = channel;

	if (strlen(ap_pass) == 0)
	{
		ap_config.ap.authmode = WIFI_AUTH_OPEN;
	}

	wifi_config_t sta_config = {0};
	strcpy((char *)sta_config.sta.ssid, sta_ssid);
	strcpy((char *)sta_config.sta.password, sta_pass);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	ESP_ERROR_CHECK(esp_wifi_connect());
}