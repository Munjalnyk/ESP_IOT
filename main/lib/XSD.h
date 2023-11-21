#ifndef XSD_H
#define XSD_H

#include <include.h>

void XSD_write_file(char *path, char *data);
void XLOGDATA(char *data);
static esp_err_t XSD_read_file(const char *path);
void XSD_INIT();
void XSD_DEINIT();
#endif