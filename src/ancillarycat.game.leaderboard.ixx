module;
#include <Windows.h>
#include <csignal>
#include "../include/config.hpp"
export module ancillarycat.game:leaderboard;
import std;
import ancillarycat.console;
import ancillarycat.api;
namespace game
{
export const std::array<std::string, 5> LEADERBOARD_PRESET{
"|Rank|","|Name|", "|Score|", "|Time(ms)|", "|Game Play Date|"
};
NO_EXPORT constinit inline short line = 1;
NO_EXPORT constinit static bool writeAccess = true;
NO_EXPORT int readLeaderboard();
NO_EXPORT int writeFile();
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
std::set<Record, RankComparator> rankBuffer;
// create/open a file to store the leaderboard
int leaderboardInit() {
	//check whether the file exists
	if (std::ifstream checkFile("leaderboard.snake", std::ios::in); checkFile.fail()) {
		checkFile.close();
		console
			.top("Warning: Unable to locate leaderboard file. Attempting to create one...", ansiColor::yellow, ansiBackground::black);
		std::ofstream out("leaderboard.snake", std::ios::out);
		if (out.fail())
			return WRITE_ACCESS_DENIED;
		std::ranges::for_each(SNAKE_GAME, [&out](const std::string& s)mutable {line++; out << s << "\n"; });
		std::ranges::for_each(LEADERBOARD_PRESET, [&out](const std::string& s) {out << s; });
		out << "\n"
			<< api::get_time()
			<< "\n"
			<< PLACEHOLDER
			<< "\n";
		out.close();
	}
	readLeaderboard();
	return HAS_WRITE_ACCESS;
}
export int writeBuffer(const short& score, const std::chrono::milliseconds& gameTime)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	if (!writeAccess) {
		console
			.setStyle(ansiStyle::blink)
			.top("Error: Unable to write to leaderboard file. Your game record may lost", ansiColor::red, ansiBackground::black, 3000);
		return WRITE_ACCESS_DENIED;
	}
	std::string name;
	console
		.setStyle(ansiStyle::blink)
		.top("New Record! Enter your name:", ansiColor::red, ansiBackground::white, 30000)
		.setCursorCoordinate(2, 0)
		.reset(false)
		.centered("", ansiColor::blueIntense, ansiBackground::white, false)
		.setStyle(ansiStyle::underline);
	std::cin >> name;
	console
		.reset(true)
		.top("Record Saved. Check the leaderboard!", ansiColor::green, ansiBackground::black, 0);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	rankBuffer.insert(std::make_tuple(name, score, gameTime, api::get_time()));
	// resize to only record the top 10
	while (rankBuffer.size() > MAX_RANKING_NUMBER)
		rankBuffer.erase(std::prev(rankBuffer.end()));
	return writeFile();
}
NO_EXPORT int writeFile()
{
	std::ofstream out("leaderboard.snake", std::ios::out);
	if (out.fail())
		return WRITE_ACCESS_DENIED;
	std::ranges::for_each(SNAKE_GAME, [&out](const std::string& s) {out << s << "\n"; });
	std::ranges::for_each(LEADERBOARD_PRESET, [&out](const std::string& s) {out << std::setw(6) << s; });
	out << "\n"
		<< api::get_time()
		<< "\n"
		<< PLACEHOLDER
		<< "\n";
	short rank = 1;
	std::ranges::for_each(rankBuffer, [&](const Record& record) mutable -> void {
		out << rank++ << " "
			<< std::get<0>(record) << " "
			<< std::get<1>(record) << " "
			<< std::get<2>(record).count() << " "
			<< std::get<3>(record) << "\n";
		});
	out.close();
	return HAS_WRITE_ACCESS;
}

int readLeaderboard()
{
	std::fstream in("leaderboard.snake", std::ios::in);
	if (in.fail())
		return READ_ACCESS_DENIED;
	std::string recordLine;
	while (std::getline(in, recordLine))
	{
		if (recordLine == PLACEHOLDER)
			break;
		if (in.eof()) {
			console
				.setStyle(ansiStyle::blink)
				.bot("Warning: Record file might be corrupted.", ansiColor::yellowIntense, ansiBackground::black, 3000);
			api::soundEvent();
		}
	}
	while (std::getline(in, recordLine))
	{
		std::stringstream ss(recordLine);
		std::string name;
		short score;
		long long gameTime;
		std::string time;
		int _;
		while (ss.peek() == '\n' || ss.peek() == '\r')ss.ignore();
		ss >> _ >> name >> score >> gameTime >> time;
		if (!ss || ss.fail() || !ss.eof()) {
			console
				.setStyle(ansiStyle::blink)
				.bot("Warning: Record file might be corrupted.", ansiColor::yellowIntense, ansiBackground::black, 3000);
			api::soundEvent();
		}
		rankBuffer.insert(std::make_tuple(name, score, std::chrono::milliseconds(gameTime), time));
	}
	while (rankBuffer.size() > MAX_RANKING_NUMBER)
	{
		rankBuffer.erase(std::prev(rankBuffer.end()));
	}
	in.close();
	return HAS_READ_ACCESS;
}

int snakeLeaderboard()
{
	if (game::leaderboardInit() == WRITE_ACCESS_DENIED) {
		writeAccess = false;
		console
			.setStyle(ansiStyle::blink)
			.bot("Error: Unable to create leaderboard file", ansiColor::red, ansiBackground::black, 3000);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return WRITE_ACCESS_DENIED;
	}
	if (game::readLeaderboard() == READ_ACCESS_DENIED) {
		console
			.setStyle(ansiStyle::blink)
			.centeredShuttle(console.height - 2, "Unable to read leaderboard file.", ansiColor::red, ansiBackground::black, 4000)
			.setStyle(ansiStyle::blink)
			.setStyle(ansiStyle::underline)
			.bot(" If you're playing for the first time, just ignore the message.", ansiColor::yellowIntense, ansiBackground::black, 4000);
		std::this_thread::sleep_for(std::chrono::seconds(4));
		return READ_ACCESS_DENIED;
	}
	console
		.clear()
		.setCursorCoordinate(2, 0);
	std::ranges::for_each(LEAD, [&](const std::string& s) {
		console
			.setStyle(ansiStyle::blink)
			.centered(s, ansiColor::cyanIntense); });
	console
		.moveCursor(1, 0);
	std::ranges::for_each(TOP10, [&](const std::string& s) {
		console
			.setStyle(ansiStyle::blink)
			.centered(s, ansiColor::whiteIntense);
		});
	console
		.moveCursor(1, 0);
	std::stringstream pre;
	std::ranges::for_each(LEADERBOARD_PRESET, [&pre](const std::string& s) {pre << s; });
	console
		.setStyle(ansiStyle::blink)
		.centered(pre.str(), ansiColor::yellowIntense);
	short rank = 1;
	std::ranges::for_each(rankBuffer, [&](const Record& record) mutable -> void {
		std::stringstream iss;
		iss << rank++ << "   "
			<< std::get<0>(record) << "   "
			<< std::get<1>(record) << "   "
			<< std::get<2>(record) << "   "
			<< std::get<3>(record);
		console.setStyle(ansiStyle::blink).centered(iss.str(), ansiColor::yellowIntense);
		});
	signal(SIGINT, SIG_IGN);
	while (int ch = console.getch()) {
		if (ch == 'Q' || ch == 'q')break;
		else console
			.setStyle(ansiStyle::blink)
			.bot("Note: press 'q' or 'Q' to exit", ansiColor::yellowIntense, ansiBackground::black, 3000);
	}
	return HAS_READ_ACCESS;
}
}

