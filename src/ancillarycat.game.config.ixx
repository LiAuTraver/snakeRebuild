module;
#pragma warning(disable : 4244)
#include "../include/config.hpp"
#include <csignal>
export module ancillarycat.game:config;
import ancillarycat.console;
import ancillarycat.api;
import std;
namespace game
{
NO_EXPORT constinit size_t startRow = 10;
NO_EXPORT constinit size_t now = 0;
NO_EXPORT std::vector<std::pair<std::string, double>> options = {
	{"INTERVAL", ::INTERVAL},
	{"MIN_HEIGHT", ::MIN_HEIGHT},
	{"MIN_WIDTH", ::MIN_WIDTH},
	{"START_ROW", ::START_ROW},
	{"START_COL", ::START_COL},
	{"BOX_HEIGHT", ::BOX_HEIGHT},
	{"BOX_WIDTH", ::BOX_WIDTH},
	{"INFO_ROW", ::INFO_ROW},
	{"INFO_COL", ::INFO_COL},
	{"INFO_HEIGHT", ::INFO_HEIGHT},
	{"INFO_WIDTH", ::INFO_WIDTH},
	{"MAX_RANKING_NUMBER", ::MAX_RANKING_NUMBER},
	{"ENABLE_OBSTACLE", ::ENABLE_OBSTACLE},
	{"OBSTACLE_THRESHOLD", ::OBSTACLE_THRESHOLD},
	{"FOOD_MAX_COUNT", ::FOOD_MAX_COUNT},
	{"BODY_CUT, set to 1 to enable", ::BODY_CUT},
	{"WIN_SCORE, set to -1 to disable",::WIN_SCORE}
};
void update_config() {
	::INTERVAL = options.at(0).second;
	::MIN_HEIGHT = options.at(1).second;
	::MIN_WIDTH = options.at(2).second;
	::START_ROW = options.at(3).second;
	::START_COL = options.at(4).second;
	::BOX_HEIGHT = options.at(5).second;
	::BOX_WIDTH = options.at(6).second;
	::INFO_ROW = options.at(7).second;
	::INFO_COL = options.at(8).second;
	::INFO_HEIGHT = options.at(9).second;
	::INFO_WIDTH = options.at(10).second;
	::MAX_RANKING_NUMBER = options.at(11).second;
	::ENABLE_OBSTACLE = options.at(12).second;
	::OBSTACLE_THRESHOLD = options.at(13).second;
	::FOOD_MAX_COUNT = options.at(14).second;
	::BODY_CUT = options.at(15).second;
	::WIN_SCORE = options.at(16).second;
	console.bot("Succeffully updated the configuration.", ansiColor::greenIntense, ansiBackground::reset, 2000);
}

void print_selected() {

	for (size_t i = 0; i < options.size(); i++) {
		console.setCursorCoordinate(i + startRow, 0);
		if (i == now) console.centered("{}: {}", ansiColor::cyanIntense, ansiBackground::yellowIntense, true, options.at(i).first, options.at(i).second);
		// pending optimizationm, we don't need to refresh all options
		else console.centered("{}: {}", ansiColor::cyanIntense, ansiBackground::black, true, options.at(i).first, options.at(i).second);

	}
}

void config()
{
	signal(SIGINT, SIG_IGN);
	console
		.clear()
		.setCursorCoordinate(2, 0);
	std::ranges::for_each(CONFIG, [&](auto& config) {
		console
			.setStyle(ansiStyle::bold)
			.centered(config, ansiColor::cyanIntense);
		});
	console
		.setStyle(ansiStyle::bold)
		.centered("Press Tab key to select the value and press Enter key to confirm", ansiColor::yellowIntense, ansiBackground::black)
		.centered("");
	std::ranges::for_each(options, [](auto& option) {
		console.centered("{}: {}", ansiColor::cyanIntense, ansiBackground::black, true, option.first, option.second);
		});
	console
		.centered("")
		.centered("Press 'q' or 'Q' to exit", ansiColor::yellowIntense, ansiBackground::black);
	while (int ch = console.getch()) {
#pragma region Not Working
		// TODO: implement shift+tab
		if (ch == '\033') { // if escape character
			int ch2 = console.getch();
			int ch3 = console.getch();
			if (ch2 == '[' && ch3 == 'Z') { // if [Z, it's Shift + Tab
				// Handle Shift + Tab here
				if (now <= 0) now = options.size();
				else now--;
				continue;
			}
		}
#pragma endregion
		// enter key
		if (ch == '\n' || ch == '\r') {
			console.setCursorCoordinate(static_cast<short>(now + startRow - 1), 0)
				.fillLine(' ');
			console.centered("Enter new value for {}: ", ansiColor::cyanIntense, ansiBackground::black, true, options.at(now - 1).first);
			std::cin >> options.at(now - 1).second;
			console.setCursorCoordinate(static_cast<short>(now + startRow - 1), 0)
				.fillLine(' ');
			console.centered("{}: {}", ansiColor::cyanIntense, ansiBackground::black, true, options.at(now - 1).first, options.at(now - 1).second);
			continue;
		}
		print_selected();
		switch (ch) {
		case '\t':
			if (now >= options.size()) now = 0;
			else now++;
			break;
		case 'q':
			[[fallthrough]];
		case 'Q':
			update_config();
			return;
		case 3: // Ctrl + C
			console
				.setStyle(ansiStyle::blink)
				.bot("No way to exit using Ctrl + C here! :P", ansiColor::redIntense, ansiBackground::white, 3000);
			break;
		default:
			console
				.setStyle(ansiStyle::blink)
				.bot("Invalid key.", ansiColor::redIntense, ansiBackground::yellowIntense, 1000);
			break;
		}
	}
	std::unreachable();
}
}