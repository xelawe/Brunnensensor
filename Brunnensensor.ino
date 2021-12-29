/*
 * Messung Wassersatnd im Brunnen nach einer Idee von https://github.com/MakeMagazinDE/Zisternensensor
 * 
 */
 #include <cy_serdebug.h>
#include <cy_serial.h>

#include "cy_wifi.h"
#include "cy_ota.h"


const char* gc_hostname = "d1mbrunnen";
