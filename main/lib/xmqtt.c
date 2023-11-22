#include "include.h"

#include "XSD.h"
#define CLIENT_ID "munjal"
#define ENDPOINT "mqtts://hdf38a02.ala.us-east-1.emqxsl.com"
#define PORT 8883

const char *TAGMQTT = "MQTT";

esp_mqtt_client_handle_t client;

extern const uint8_t server_cert_pem_start[] asm("_binary_aws_root_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_aws_root_crt_end");

void XSMQTT(char *data)
{
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){
    cJSON *roooot = cJSON_CreateObject();
    cJSON_AddStringToObject(roooot, "log", data);
    char *json_string = cJSON_PrintUnformatted(roooot);
    esp_mqtt_client_publish(client, "munjal/out", json_string, 0, 0, 0);
    cJSON_Delete(roooot);
    free(json_string);
    xSemaphoreGive(xSemaphore);
    }
}
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
   
    
    // ESP_LOGD(TAGMQTT, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);

    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(client, "munjal/in", 0);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_DISCONNECTED");

        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        cJSON *rooot = cJSON_CreateObject();

        esp_netif_init();
        esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(sta_netif, &ip_info);
        char ip_address[16];
        sprintf(ip_address, IPSTR, IP2STR(&ip_info.ip));
        char *Current_Date_Time = (char *)malloc(100);
        Get_current_date_time(Current_Date_Time);
        cJSON_AddStringToObject(rooot, "IP_ADDRESS", ip_address);
        cJSON_AddStringToObject(rooot, "MQTT Started at", Current_Date_Time);
        XLOG("MQTT", "XSD", "IP Address : %s", ip_address);
        XLOG("MQTT", "XSD", "MQTT Connected");
        char *json_string = cJSON_PrintUnformatted(rooot);

        esp_mqtt_client_publish(client, "munjal/out", json_string, 0, 0, 0);

        /* Free memory */
        cJSON_Delete(rooot);
        free(json_string);
        free(Current_Date_Time);
        xSemaphoreGive(xSemaphore);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_DATA");
        
        cJSON *root = cJSON_Parse(event->data);
        ESP_LOGI(TAGMQTT, "Now Parsing");
        if (root == NULL)
        {
            ESP_LOGE(TAGMQTT, "JSON Parse Error");
            break;
        }

        json_string = cJSON_Print(root);
        ESP_LOGI(TAGMQTT, "Received JSON data:\n%s", json_string);
        

        cJSON *start_log = cJSON_GetObjectItemCaseSensitive(root, "start_log");
        if (cJSON_IsString(start_log) && strcmp(start_log->valuestring, "1") == 0)
        {
            XSD_read_file("/sdcard/LOG.txt");
        }
       
        free(json_string);
        cJSON_Delete(root);
        // if (cJSON_IsString(start_log) && strcmp(start_log->valuestring, "0") == 0)
        // {
        //     ESP_LOGI(TAGMQTT, "Stop Logging");
        //     cJSON *rooooot = cJSON_CreateObject();
        //     cJSON_AddStringToObject(rooooot, "Stoped", "logging");
        //     char *json_string = cJSON_PrintUnformatted(rooooot);
        //     esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        //     mqtt_connected = 0;
        // }

        // cJSON *start_vfd = cJSON_GetObjectItemCaseSensitive(root, "start_vfd");
        // if (cJSON_IsString(start_vfd) && strcmp(start_vfd->valuestring, "1") == 0)
        // {
        //     ESP_LOGI(TAGMQTT, "Starting vfd");
        //     cJSON *roooooot = cJSON_CreateObject();
        //     cJSON_AddStringToObject(roooooot, "Started", "Motor");
        //     char *json_string = cJSON_PrintUnformatted(roooooot);
        //     esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        //     a = 0x3200;
        //     b = 199;
        //     vfd = 1;
        // }
        // if (cJSON_IsString(start_vfd) && strcmp(start_vfd->valuestring, "0") == 0)
        // {
        //     ESP_LOGI(TAGMQTT, "Stoping vfd");
        //     cJSON *rooooooot = cJSON_CreateObject();
        //     cJSON_AddStringToObject(rooooooot, "Stoped", "Motor");
        //     char *json_string = cJSON_PrintUnformatted(rooooooot);
        //     esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        //     a = 0x3200;
        //     b = 198;
        //     vfd = 1;
        // }
        // cJSON *vfd_speed = cJSON_GetObjectItemCaseSensitive(root, "vfd_speed");
        // if (cJSON_IsString(vfd_speed))
        // {
        //     ESP_LOGI(TAGMQTT, "Speed change vfd");
        //     a = 0x0003;
        //     b = atoi(vfd_speed->valuestring);
        //     freq = b;
        //     // cJSON *roooooooot = cJSON_CreateObject();
        //     //  cJSON_AddStringToObject(roooooooot, "Changed Speed to", b);
        //     //  char *json_string = cJSON_PrintUnformatted(roooooooot);
        //     //  esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        //     vfd = 1;
        // }
        // cJSON *vfd_dir = cJSON_GetObjectItemCaseSensitive(root, "vfd_dir");
        // if (cJSON_IsString(vfd_dir))
        // {
        //     ESP_LOGI(TAGMQTT, "dir change vfd");
        //     a = 0x3200;
        //     b = 207;
        //     // cJSON *rooooooooot = cJSON_CreateObject();
        //     // cJSON_AddStringToObject(rooooooooot, "Changed dir to", b);
        //     // char *json_string = cJSON_PrintUnformatted(rooooooooot);
        //     // esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        //     vfd = 1;
        // }
        // cJSON *start_relay = cJSON_GetObjectItemCaseSensitive(root, "start_relay");
        // if (cJSON_IsString(start_relay) && strcmp(start_relay->valuestring, "1") == 0)
        // {
        //     ESP_LOGI(TAGMQTT, "Starting relay");
        //     gpio_set_level(RELAY, HIGH);
        //      cJSON *roooooooot = cJSON_CreateObject();
        //     cJSON_AddStringToObject(roooooooot, "Start", "relay");
        //     char *json_string = cJSON_PrintUnformatted(roooooooot);
        //     esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        // }
        // if (cJSON_IsString(start_relay) && strcmp(start_relay->valuestring, "0") == 0)
        // {
        //     ESP_LOGI(TAGMQTT, "Stoping relay");
        //     gpio_set_level(RELAY, LOW);
        //       cJSON *rooooooooot = cJSON_CreateObject();
        //     cJSON_AddStringToObject(rooooooooot, "Stop", "relay");
        //     char *json_string = cJSON_PrintUnformatted(rooooooooot);
        //     esp_mqtt_client_publish(client, "io011/out", json_string, 0, 0, 0);
        // }
        // cJSON_Delete(root);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            // log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            // log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            // log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            // ESP_LOGI(TAGMQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAGMQTT, "init event id:%d", event->event_id);
        break;
    }
    }


void mqtt_app_start(void)
{
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){
    const esp_mqtt_client_config_t mqtt_cfg = {
        .credentials.client_id = CLIENT_ID,
        .credentials.username = "munjal",
        .credentials.authentication.password = "NAYAK.munjal@1",
        .broker.address.uri = ENDPOINT,
        .broker.address.port = PORT,

        .broker.verification.certificate = (const char *)server_cert_pem_start,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    xSemaphoreGive(xSemaphore);
    }
}

