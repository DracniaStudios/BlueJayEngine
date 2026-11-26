#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <algorithm>
#include <chrono>

// Frame timer. Tick() returns the seconds since the previous Tick().
class Clock {
public:

	float DeltaTime = 0;

	float Tick() {
		const auto now = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration<float>(now - last_).count();
		last_ = now;
		DeltaTime = deltaTime;
		return deltaTime;
	}

private:
	std::chrono::steady_clock::time_point last_ = std::chrono::steady_clock::now();
};

// Fixed-rate tick. Each frame: Accumulate(deltaTime), then `while (Step())` run one
// fixed update of StepSeconds(). Runs 0..n times per frame, so the simulation advances
// at the same rate whatever the frame rate is (or with rendering off).
class FixedTimestep {
public:
	explicit FixedTimestep(int ticksPerSecond = 60) { SetRate(ticksPerSecond); }

	void SetRate(int ticksPerSecond) { step_ = 1.0 / double(std::max(1, ticksPerSecond)); }
	float StepSeconds() const { return float(step_); }

	// The backlog is capped so a long stall (debugger, hitch) doesn't cause a burst of
	// catch-up ticks; the simulation just loses that time instead.
	void Accumulate(float deltaTime) {
		accumulator_ = std::min(accumulator_ + double(deltaTime), step_ * MaxCatchUpSteps);
	}

	bool Step() {
		if (accumulator_ < step_) { return false; }
		accumulator_ -= step_;
		return true;
	}

	// 0..1: how far this frame is between the last two fixed ticks, for interpolation.
	float Alpha() const { return float(accumulator_ / step_); }

private:
	static constexpr double MaxCatchUpSteps = 5.0;
	double step_ = 1.0 / 60.0;
	double accumulator_ = 0.0;  // double so tiny frame times don't get lost over long runs
};

#endif // CLOCK_H
