#pragma once
#include "IBootService.h"

namespace bootp::Plugins::DHCP::BootServices
{
	/**
	 * @brief PXE Options (Option 43 sub-options)
	 * 
	 * RFC 4578 / PXE Specification
	 */
	namespace PXEOptions
	{
		enum : _BYTE
		{
			End = 255,
			DiscoveryControl = 6,
			MulticastTFTPDelay = 7,
			MulticastTFTPTimeout = 8,
			MulticastServerPort = 9,
			MulticastClientPort = 10,
			DiscoveryMulticastAddress = 11,
			BootServer = 8,      // Formerly 8, see RFC 4578
			BootMenue = 77,      // ASCII 'M'
			MenuPrompt = 79,      // ASCII 'O'
		};
	}

	/**
	 * @brief Boot Server entry for the PXE menu
	 */
	struct BootServerEntry
	{
		_USHORT type;           // BootServerType as _USHORT
		_BYTE len;           // Length of the following data
		_BYTE ip_count;       // Number of IP addresses
		_BYTE ip_addr[4];    // First IP address (IPv4)
		_STRING hostname;     // Server hostname (null-terminated)

		BootServerEntry() : type(0), len(0), ip_count(0), ip_addr{0,0,0,0}, hostname() {}

		BootServerEntry(_USHORT serverType, const _STRING& host, const _UBYTE* ip)
			: type(serverType), len(0), ip_count(1), hostname(host)
		{
			if (ip) {
				ip_addr[0] = ip[0];
				ip_addr[1] = ip[1];
				ip_addr[2] = ip[2];
				ip_addr[3] = ip[3];
			}
			// len = 1 (ip_count) + 4 (ip) + hostname.length() + 1 (null)
			len = static_cast<_BYTE>(1 + 4 + hostname.length() + 1);
		}

		std::vector<char> AsBytes() const
		{
			std::vector<char> result;
			result.reserve(len + 3);  // type(2) + len(1) + data

			// Type (2 bytes, little-endian)
			result.push_back(static_cast<char>(type & 0xFF));
			result.push_back(static_cast<char>((type >> 8) & 0xFF));

			// Length
			result.push_back(static_cast<char>(len));

			// IP count
			result.push_back(static_cast<char>(ip_count));

			// IP address
			for (int i = 0; i < 4; ++i)
				result.push_back(static_cast<char>(ip_addr[i]));

			// Hostname
			result.insert(result.end(), hostname.begin(), hostname.end());
			result.push_back(0);  // Null terminator

			return result;
		}
	};

	/**
	 * @brief Boot Menu entry
	 */
	struct BootMenuEntry
	{
		_USHORT type;           // BootServerType
		_BYTE prompt_len;     // Length of prompt string
		_STRING prompt;      // Display text

		BootMenuEntry() : type(0), prompt_len(0), prompt() {}
		BootMenuEntry(_USHORT serverType, const _STRING& displayText)
			: type(serverType), prompt_len(static_cast<_BYTE>(displayText.length() + 1)), prompt(displayText) {}

		std::vector<char> AsBytes() const
		{
			std::vector<char> result;
			// 2 (type) + 1 (len) + prompt_len
			result.reserve(3 + prompt_len);

			// Type (2 bytes, little-endian)
			result.push_back(static_cast<char>(type & 0xFF));
			result.push_back(static_cast<char>((type >> 8) & 0xFF));

			// Length (includes null terminator)
			result.push_back(static_cast<char>(prompt_len));

			// Prompt
			result.insert(result.end(), prompt.begin(), prompt.end());
			result.push_back(0);  // Null terminator

			return result;
		}
	};
}
