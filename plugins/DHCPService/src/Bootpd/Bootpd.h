#pragma once
#ifndef _ENVIRONMENT_PATH
#define _ENVIRONMENT_PATH "../../../../bootpd/src/Bootpd/Common/Environment.h"
#endif

#pragma once
#ifndef _FUNCTIONS_PATH
#define _FUNCTIONS_PATH "../../../../bootpd/src/Bootpd/Common/Functions.h"
#endif

#ifndef _IPACKET_PATH
#define _IPACKET_PATH "../../../../bootpd/src/Bootpd/Network/Interface/IPacket.h"
#endif

#ifndef _PACKET_PATH
#define _PACKET_PATH "../../../../bootpd/src/Bootpd/Network/Packet/Packet.h"
#endif

#include _ENVIRONMENT_PATH
#include _IPACKET_PATH
#include _PACKET_PATH

#include "Defines/DHCPOption.h"
#include "Network/Packet/DHCPPacket.h"
