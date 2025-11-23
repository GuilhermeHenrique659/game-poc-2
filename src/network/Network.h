// net_interface.h
#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include "raymath.h" // Include raymath here if needed for structures like Vector2
#include <enet/enet.h>

// Declare functions for networking operations
void net_init();
void net_send_data(void *data, int size);
void net_receive_data(void *buffer, int size);
// ... other networking functions

#endif // NET_INTERFACE_H