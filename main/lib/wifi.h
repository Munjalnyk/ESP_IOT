#ifndef WIFI_H
#define WIFI_H

#include <include.h>

void Xinitialise_wifi();
void wifi_ap(char *ap_ssid, char *ap_pass, int max_conn, int channel);
void wifi_sta(char *sta_ssid, char *sta_pass,int timeout_ms);
void wifi_apsta(char *ap_ssid, char *ap_pass, int max_conn, int channel, char *sta_ssid, char *sta_pass, int timeout_ms);
extern int wificonn;
#endif 