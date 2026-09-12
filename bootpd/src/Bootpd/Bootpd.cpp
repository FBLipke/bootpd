/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Bootpd.h"

namespace bootp
{
	bootpd::bootpd()
	{
	}

	bootpd::~bootpd()
	{

	}

	bool bootpd::Init(const int argc, const char* argv[])
	{
		this->_subSystems.emplace_back(std::make_unique<bootp::ServerManager>());

		for (const auto& s : this->_subSystems) {
			s.get()->Init(argc, argv);
		}

		return true;
	}

	void bootpd::HeartBeat()
	{
		for (const auto& s : this->_subSystems) {
			s.get()->HeartBeat();
		}
	}

	bool bootpd::Start()
	{
		for (const auto& s : this->_subSystems) {
			s.get()->Start();
		}
		
		return true;
	}

	void bootpd::Close()
	{
		for (const auto& s : this->_subSystems) {
			s.get()->Close();
		}
	}
}
