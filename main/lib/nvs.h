#ifndef NVS_H
#define NVS_H

#include <include.h>

void XNVS_INIT();
void XNVS_WRITE(char *NAMESPACE, char *LOCATION, char *DATA);
char *XNVS_READ(char *NAMESPACE, char *LOCATION);
void XNVS_ERASE();
#endif 