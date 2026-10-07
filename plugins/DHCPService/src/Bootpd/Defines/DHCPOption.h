#pragma once
#include "../Bootpd.h"
#include "Definitions.h"
namespace bootp::Plugins::DHCP
{
	class DHCPOption
	{
	public:
		DHCPOption() : _code(0) {}
		DHCPOption(_BYTE code) : _code(code) {}

		DHCPOption(_BYTE code, const std::vector<char> &data)
			: _code(code), _buffer(data) {}

		DHCPOption(_BYTE code, const char *data, _SIZET length)
			: _code(code)
		{
			_buffer.assign(data, data + length);
		}

		// String
		DHCPOption(_BYTE code, const std::string &str) : _code(code)
		{
			_buffer.assign(str.data(), str.data() + str.size());
		}

		// Primitive Typen
		template <typename T>
		DHCPOption(_BYTE code, T value) : _code(code)
		{
			static_assert(std::is_trivially_copyable<T>::value, "T must be trivial");
			const char *bytes = reinterpret_cast<const char *>(&value);
			_buffer.assign(bytes, bytes + sizeof(T));
		}

		// EXPLICIT Overload für vector<char> - DAS BRAUCHST DU!
		void AddData(const std::vector<char> &data)
		{
			_buffer.insert(_buffer.end(), data.begin(), data.end());
		}

		// Primitive Typ hinzufügen
		template <typename T>
		void AddData(T value)
		{
			static_assert(std::is_trivially_copyable<T>::value, "T must be trivial");
			const char *bytes = reinterpret_cast<const char *>(&value);
			_buffer.insert(_buffer.end(), bytes, bytes + sizeof(T));
		}

		// String hinzufügen
		void AddData(const std::string &str)
		{
			_buffer.insert(_buffer.end(), str.begin(), str.end());
		}

		// Andere DHCPOption hinzufügen
		void AddData(const DHCPOption &opt)
		{
			_buffer.insert(_buffer.end(), opt.GetData().begin(), opt.GetData().end());
		}

		_BYTE GetCode() const { return _code; }
		std::vector<char> GetData() const { return _buffer; }
		_SIZET GetLength() const { return _buffer.size(); }
		const char *GetDataPtr() const { return _buffer.data(); }

		template <typename T>
		T GetData() const
		{
			static_assert(std::is_trivially_copyable<T>::value, "T must be trivial");
			T value;
			memcpy(&value, _buffer.data(), sizeof(T));
			return value;
		}

		_STRING GetString() const
		{
			return _STRING(_buffer.data(), _buffer.size());
		}

	private:
		_BYTE _code;
		std::vector<char> _buffer;
	};
}
