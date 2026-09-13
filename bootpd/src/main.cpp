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

#include "Bootpd/Bootpd.h"

using namespace bootp;

void _heartbeat(IBootpd* _instance)
{
	std::string str = std::string("");
	while (str != "!exit")
	{
		std::this_thread::sleep_for(
			std::chrono::milliseconds(3000));
		
		if (_instance == nullptr)
			continue;

		_instance->HeartBeat();
	}
}

int main(const int argc, const char* argv[])
{
	auto _bootpd = std::make_shared<bootp::bootpd>();
	printf("Bootpd 0.1\n");

	printf("%s\n", Functions::GenerateUUID().c_str());


	if (!_bootpd.get()->Init(argc, argv))
		return 1;

	if (!_bootpd.get()->Start())
		return 1;

	std::thread _heartbeatThread(_heartbeat, _bootpd.get());

	_heartbeatThread.join();

	_bootpd.get()->Close();
	_bootpd.reset();

	return 0;
}

