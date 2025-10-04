// file_sink.h
#ifndef FILE_SINK_H
#define FILE_SINK_H

#include "log_sink.h"

LogSink* file_sink_create(const char* filename);

#endif
