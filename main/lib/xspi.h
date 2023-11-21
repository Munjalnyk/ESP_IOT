#ifndef XLOG_H
#define XLOG_H

#include <include.h>

void initialize_spi();
void XSPIREAD(uint8_t *data, size_t length);
void XSPIWRITE(const uint8_t *data, size_t length)
#endif
