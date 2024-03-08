module;

#include <Windows.h>

export module ancillarycat.windows.api;

import std;

export wchar_t systemRoot[MAX_PATH];

export enum class soundFlag : int {
	sync = SND_SYNC, // play synchronously (default),
	async = SND_ASYNC, // play asynchronously,
	noDefault = SND_NODEFAULT, // silence (!default) if sound not found,
	memory = SND_MEMORY, // pszSound points to a memory file,
	loop = SND_LOOP, // loop the sound until next sndPlaySound,
	noStop = SND_NOSTOP, // don't stop any currently playing sound,

	noWait = SND_NOWAIT, // don't wait if the driver is busy,
	alias = SND_ALIAS, // name is a registry alias,
	aliasId = SND_ALIAS_ID, // alias is a predefined ID,
	filename = SND_FILENAME, // name is file name,
	resource = SND_RESOURCE,// name is resource name or atom
#if(WINVER >= 0x0400)
	purge = SND_PURGE, // purge non-static events for task,
	application = SND_APPLICATION, // look for application specific association
#endif
	soundSentry = SND_SENTRY, // Generate a SoundSentry event with this sound,
	ring = SND_RING, // Treat this as a "ring" from a communications app - don't duck me,
	system = SND_SYSTEM, // Treat this as a system sound

	aliasStart = SND_ALIAS_START, // alias base
#ifdef _WIN32
	asterisk = SND_ALIAS_SYSTEMASTERISK, // sndAlias('S', '*'),
	question = SND_ALIAS_SYSTEMQUESTION, // sndAlias('S', '?'),
	hand = SND_ALIAS_SYSTEMHAND, // sndAlias('S', 'H'),
	exit = SND_ALIAS_SYSTEMEXIT, // sndAlias('S', 'E'),
	start = SND_ALIAS_SYSTEMSTART, // sndAlias('S', 'S'),
	welcome = SND_ALIAS_SYSTEMWELCOME, // sndAlias('S', 'W'),
	exclamation = SND_ALIAS_SYSTEMEXCLAMATION, // sndAlias('S', '!'),
	systemDefault = SND_ALIAS_SYSTEMDEFAULT // sndAlias('S', 'D')
#endif
};

namespace api {
export inline void initializeSoundEvent() {
	ExpandEnvironmentStringsW(L"%SystemRoot%", systemRoot, MAX_PATH);
}

export inline void soundEvent(const std::wstring_view path = L"defaultSystemBeep", soundFlag flag1 = soundFlag::async, soundFlag flag2 = soundFlag::noStop) {
	const std::wstring soundPath = static_cast<std::wstring>(systemRoot) + path.data();
	PlaySoundW(soundPath.c_str(), nullptr, static_cast<DWORD>(flag1) | static_cast<DWORD>(flag2));
}
}
