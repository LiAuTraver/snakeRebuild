module;
#include <Windows.h>
#include "../include/config.hpp"
export module ancillarycat.game:leaderboard;
import std;
import ancillarycat.console;
import ancillarycat.api;
namespace game
{
export const std::array<std::string, 5> LEADERBOARD_PRESET{
"|Rank|\t","|Name|\t", "|Score|\t", "|Time|\t", "|Game Time|\t"
};
NO_EXPORT constinit inline short line = 1;
NO_EXPORT constinit static bool writeAccess = true;
using Record = std::tuple<std::string, short, std::chrono::milliseconds, std::string>;
Record currentRank;
class RankComparator final {
	// compare the record by score, then by time, then by name
	// return true if record1 should be placed before record2
	// return false if record1 should be placed after record2
	// 0:name, 1:score, 2:time
public:
	NODISCARD bool operator()(const Record& record1, const Record& record2) const noexcept {
		if (std::get<1>(record1) != std::get<1>(record2))
		{
			return std::get<1>(record1) > std::get<1>(record2);
		}
		if (std::get<2>(record1) != std::get<2>(record2))
		{
			return std::get<2>(record1) < std::get<2>(record2);
		}
		return std::get<0>(record1) < std::get<0>(record2);
	}
};
std::multiset<Record, RankComparator> rankBuffer;
// create/open a file to store the leaderboard
int leaderboardInit() {
	//check whether the file exists
	if (std::ifstream checkFile("leaderboard.snake", std::ios::in); checkFile.fail()) {
		checkFile.close();
		console
			.bot("Warning: Unable to locate leaderboard file. Attempting to create one...", ansiColor::yellow, ansiBackground::black);
		std::ofstream file("leaderboard.snake", std::ios::out);
		if (file.fail())
			return WRITE_ACCESS_DENIED;
		std::ranges::for_each(SNAKE_GAME, [&file](const std::string& s)mutable {line++; file << s << "\n"; });
		std::ranges::for_each(LEADERBOARD_PRESET, [&file](const std::string& s) {file << s; });
		line++;
		file << api::get_time() << "\n";
		file.close();
	}
	return HAS_WRITE_ACCESS;
}
export int writeBuffer(const short& score, const std::chrono::milliseconds& gameTime)
{
	if (!writeAccess) {
		console
			.setStyle(ansiStyle::blink)
			.top("Error: Unable to write to leaderboard file. Your game record may lost", ansiColor::red, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return WRITE_ACCESS_DENIED;
	}
	std::string name;
	console
		.setStyle(ansiStyle::blink)
		.centeredShuttle(console.height - 2, "New Record! Enter your name:", ansiColor::red, ansiBackground::white);
	std::print("\033[{}m", static_cast<int>(ansiBackground::white));
	console
		.fillLine(' ')
		.setCursorCoordinate(console.height - 1, 0)
		.setCursorState(true);
	std::cin >> name;
	console.setCursorState(false)
		.fillLine(' ')
		.bot("Record Saved. Check the leaderboard!", ansiColor::green, ansiBackground::black);
	rankBuffer.insert(std::make_tuple(name, score, gameTime, api::get_time()));
	// resize to only record the top 10
	while (rankBuffer.size() > MAX_RANKING_NUMBER) {
		rankBuffer.erase(std::prev(rankBuffer.end()));
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return HAS_WRITE_ACCESS;
}
int readLeaderboard()
{
	std::fstream in("leaderboard.snake", std::ios::in);
	if (in.fail())
		return READ_ACCESS_DENIED;
	std::string recordLine;
	for (auto i = 0; i < line - 1; i++)
	{
		if (!std::getline(in, recordLine))
		{
			console
				.setStyle(ansiStyle::blink)
				.bot("Warning: not enough lines.", ansiColor::yellowIntense, ansiBackground::black);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return READ_ACCESS_DENIED;
		}
	}
	while (std::getline(in, recordLine))
	{
		std::stringstream ss(recordLine, std::ios::out);
		std::string name;
		short score;
		long long gameTime;
		std::string time;
		ss >> name >> score >> gameTime >> time;
		if (!ss.eof())
		{
			console
				.setStyle(ansiStyle::blink)
				.bot("Warning: Record file might be corrupted.", ansiColor::yellowIntense, ansiBackground::black);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			api::soundEvent();
		}
		rankBuffer.insert(std::make_tuple(name, score, std::chrono::milliseconds(gameTime), time));
	}
	while (rankBuffer.size() > MAX_RANKING_NUMBER)
	{
		rankBuffer.erase(std::prev(rankBuffer.end()));
	}
	in.close();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return HAS_READ_ACCESS;
}

int snakeLeaderboard()
{
	if (game::leaderboardInit() == WRITE_ACCESS_DENIED) {
		writeAccess = false;
		console
			.setStyle(ansiStyle::blink)
			.bot("Error: Unable to create leaderboard file", ansiColor::red, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return WRITE_ACCESS_DENIED;
	}
	if (game::readLeaderboard() == READ_ACCESS_DENIED) {
		console
			.setStyle(ansiStyle::blink)
			.centeredShuttle(console.height - 2, "Unable to read leaderboard file.", ansiColor::red, ansiBackground::black)
			.setStyle(ansiStyle::blink)
			.setStyle(ansiStyle::underline)
			.bot(" If you're playing for the first time, just ignore the message.", ansiColor::yellowIntense, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::seconds(4));
		return READ_ACCESS_DENIED;
	}
	console
		.clear()
		.setCursorCoordinate(2, 0);
	std::ranges::for_each(LEAD, [](const std::string& s) {
		console
			.setStyle(ansiStyle::blink)
			.centered(s, ansiColor::cyanIntense); });
	console
		.moveCursor(1, 0);
	std::ranges::for_each(TOP10, [](const std::string& s) {
		console
			.setStyle(ansiStyle::blink)
			.centered(s, ansiColor::whiteIntense); });
	console
		.moveCursor(1, 0);
	std::ranges::for_each(rankBuffer, [](const Record& record)
		{
			std::stringstream ss;
			ss << std::get<0>(record) << " \t " << std::get<1>(record) << " \t " << std::get<2>(record) << " \t " << std::get<3>(record);
			console.setStyle(ansiStyle::blink).centered(ss.str(), ansiColor::yellowIntense);
		});
	console.getch();
	return HAS_READ_ACCESS;
}
}

