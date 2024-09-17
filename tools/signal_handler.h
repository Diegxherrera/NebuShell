#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <stdbool.h>

extern volatile bool exitSignal;

int set_up_signal_handler();

#endif
