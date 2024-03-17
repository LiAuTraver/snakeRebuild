module;

export module ancillarycat.api;
export import :ansi;
export import :windows;
export import :generator;
namespace api
{
export std::string get_time();

std::string get_time()
{
	// Get current time
	const auto now = std::chrono::system_clock::now();
	// Convert to time_t
	const std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	// Convert to local time
	const std::tm* current_time = std::localtime(&now_time_t);
	std::ostringstream os_time(std::ios::out);
	os_time << std::put_time(current_time, "%Y-%m-%d %H:%M:%S");
	return os_time.str();
}
}