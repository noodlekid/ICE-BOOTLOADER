#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#define LOG_BUFFER_SIZE  2048

void Logger_Init(void);
void Logger_Process(void);

#endif // LOGGER_H