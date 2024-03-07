module;

#include <csignal>
#include <Windows.h>
#include <cstdlib>


export module ancillarycat.handler;


namespace handler {

// cannot export a static function so I removed it.
export void signalHandler(const int& signalNumber) {
	switch (signalNumber)
	{
	case SIGINT:
		system("CLS");
		exit(0);
		break;
	default: [[unlikely]]
			   break;
	}
}

export void oninitialize() {
	system("CLS");
}

} // namespace handler
