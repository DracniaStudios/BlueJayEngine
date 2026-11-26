#pragma once
#ifndef FRAME_STATS_H
#define FRAME_STATS_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

// Counters must read zero during gameplay.
namespace bluejay::stats {

	// Bump whenever a shader/pipeline is created.
	inline std::atomic<uint32_t> shaderCompiles{ 0 };

	// Bump whenever a frame/GPU buffer grows.
	inline std::atomic<uint32_t> bufferReallocs{ 0 };
}

class FrameStats {
public:
	static constexpr double BudgetMs = 1000.0 / 60.0;  // 16.67 (60 FPS)
	static constexpr double HitchMs = 2.0 * BudgetMs;  // 33.3 (30 FPS)

	// Room for 10 minutes at 60 fps; longer runs still work, they just reallocate once.
	FrameStats() { frames.reserve(60 * 60 * 10); }

	// Returns true if the frame was a hitch.
	bool Record(double frameMs) { frames.push_back(frameMs); return frameMs > HitchMs; }

	void Reset() { frames.clear(); }

	// Appends a row to path, writing the header first for new files.
	bool AppendCSV(const std::string& path, const std::string& runName) const;

private:
	std::vector<double> frames;
};

#endif // FRAME_STATS_H
