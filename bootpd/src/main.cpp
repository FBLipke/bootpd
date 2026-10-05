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

static std::shared_ptr<bootpd> _bootpd;
volatile sig_atomic_t g_shutdown = false;

void _heartbeat(IBootpd *_instance)
{
	while (!g_shutdown)
	{
		std::this_thread::sleep_for(
			std::chrono::milliseconds(3000));

		if (_instance == nullptr)
			break;

		_instance->HeartBeat();
	}
}

void handle_signal(_INT32 sig)
{
	printf("[I] Bootpd is shutting down...\n");
	g_shutdown = true;
}

int main(const _INT32 argc, const char *argv[])
{
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	_bootpd = std::make_shared<bootp::bootpd>();

	if (!_bootpd->Init(nullptr, argc, argv))
		return 1;

	if (!_bootpd->Start())
		return 1;

	_THREAD _heartbeatThread(_heartbeat, _bootpd.get());
	_heartbeatThread.join();

	_bootpd->Close();
	_bootpd.reset();

	return 0;
}
