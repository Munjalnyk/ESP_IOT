#include "include.h"


SemaphoreHandle_t xSemaphore;
int wificonn;
void Xinitialise_wifi();
void Set_SystemTime_SNTP();
char *XNVS_READ(char *NAMESPACE, char *LOCATION);
void wifi_sta(char *ssid, char *pass, int timeout);
void XLOG(char *function, char *location, const char *format, ...);
void initialize_spi();
void XSD_INIT();
void XgetClock();
void XsetTime();
void XRTCINIT();
void mqtt_app_start(void);
void XINIT()
{
   xSemaphore = xSemaphoreCreateBinary();
   if (xSemaphore == NULL)
   {
      printf("Semaphore creation failed.\n");
      while (1)
         ;
   }
   else
   {
      printf("Semaphore creation successful.\n");
   }
   XSD_INIT();

   XNVS_INIT();
   XLOG("XINIT", "SD", "NVS initialised");
   Xinitialise_wifi();
   wifi_sta("MUNJAL", "munjalnayak", 5000);
   vTaskDelay(1000/portTICK_PERIOD_MS);
   if(wificonn == 1)
   {
   Set_SystemTime_SNTP();
   XsetTime();
   mqtt_app_start();
   }

   }