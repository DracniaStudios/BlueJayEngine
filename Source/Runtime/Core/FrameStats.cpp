#include "FrameStats.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <numeric>

bool FrameStats::AppendCSV(const std::string& path, const std::string& runName) const {

	if (frames.empty()) { return false; }

	std::vector<double> sorted = frames;

	// Slowest Frames are first
	std::sort(sorted.begin(), sorted.end(), std::greater<>());

	// "1% low" = The average of the slowest 1% of frames.
	auto worstMean = [&](double fraction) {
		const size_t n = std::max<size_t>(1, size_t(double(sorted.size()) * fraction));
		return std::accumulate(sorted.begin(), sorted.begin() + n, 0.0) / double(n);
	};

	// Number of frames slower than ms.
	auto over = [&](double ms) {
		return std::count_if(sorted.begin(), sorted.end(), [ms](double f) { return f > ms; });
	};

	const double avg = std::accumulate(sorted.begin(), sorted.end(), 0.0) / double(sorted.size());
	
	// 1% & 0.1%, Lowest Frames
	const double low1 = worstMean(0.01), low01 = worstMean(0.001);

	const bool isNew = !std::filesystem::exists(path);
	std::ofstream out(path, std::ios::app);
	if (!out) { return false; }
	
	// CSV column order
	if (isNew) {
		out << "run,frames,avg_ms,low1_ms,low01_ms,avg_fps,low1_fps,low01_fps,"
			"over_16_67,over_33_3,shader_compiles,buffer_reallocs\n";
	}
	out << runName << ',' << sorted.size() << ',' << avg << ',' << low1 << ',' << low01 << ','
		<< 1000.0 / avg << ',' << 1000.0 / low1 << ',' << 1000.0 / low01 << ','
		<< over(BudgetMs) << ',' << over(HitchMs) << ','
		<< bluejay::stats::shaderCompiles.load() << ',' << bluejay::stats::bufferReallocs.load() << '\n';
	return true;
}