#pragma once

/**
 * @file api.h
 * @brief Main header for bootpd Plugin API
 * 
 * Include this header to use the bootpd plugin API.
 * This is a standalone, dependency-free API for boot service plugins.
 */

// Main API headers
#include "plugins/api/Types.h"
#include "plugins/api/IBootService.h"
#include "plugins/api/BootServiceLoader.h"

// Convenience macro for plugin registration
// Usage in plugin's entry point:
// REGISTER_BOOT_SERVICE(MyPXEBootService);
#define REGISTER_BOOT_SERVICE(ServiceClass)                   \
	extern "C"                                               \
	{                                                        \
		__declspec(dllexport) bool RegisterBootService()      \
		{                                                    \
			return bootp::plugins::api::BootServiceRegistry:: \
				Instance().Register(                         \
					std::make_shared<ServiceClass>());        \
		}                                                    \
	}
