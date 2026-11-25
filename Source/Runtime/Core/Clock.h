#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

// Frame timer. Tick() returns the seconds since the previous Tick().
class Clock {
public:
	float Tick() {
		const auto now = std::chrono::steady_clock::now();
		const float dt = std::chrono::duration<float>(now - last_).count();
		last_ = now;
		return dt;
	}

private:
	std::chrono::steady_clock::time_point last_ = std::chrono::steady_clock::now();
};

#endif // CLOCK_H
