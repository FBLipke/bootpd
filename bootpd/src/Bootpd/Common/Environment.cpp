#include "Environment.h"

FILE *__fopen(const _STRING &filename, const char *mode)
{
	FILE *fil = nullptr;

#ifdef _WIN32
	fopen_s(&fil, filename.c_str(), mode);
#else
	fil = fopen(filename.c_str(), mode);
#endif

	return fil;
}

_STRING __inet_ntoa(const _IPADDR &ip, const _INT32 &af)
{
	in_addr addr;
	_ClearBuffer(&addr, sizeof addr);
	addr.s_addr = ip;

	char _addr[128];
	_ClearBuffer(_addr, sizeof(_addr));

	inet_ntop(af, &addr, _addr, sizeof(_addr));

	return _STRING(_addr);
}

in_addr __inet_addr(const _STRING &ipstring, const _INT32 &af)
{
	struct in_addr addr;
#ifdef _WIN32
	inet_pton(af, ipstring.c_str(), &addr);
#else
	inet_aton(ipstring.c_str(), &addr); // Linux hat inet_aton
#endif

	return addr;
}
