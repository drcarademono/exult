/* Render-only actor movement interpolation. GPL-2.0-or-later. */
#ifndef EXULT_ACTOR_MOVEMENT_H
#define EXULT_ACTOR_MOVEMENT_H

#include <cstdint>

class Actor_movement {
	int      x = 0, y = 0;
	uint32_t started = 0, duration = 0;

public:
	void reset() {
		x = y    = 0;
		duration = 0;
	}

	void offset(uint32_t now, int& dx, int& dy) const {
		const uint32_t elapsed = now - started;
		if (!duration || elapsed >= duration) {
			dx = dy = 0;
			return;
		}
		dx = int(int64_t(x) * (duration - elapsed) / duration);
		dy = int(int64_t(y) * (duration - elapsed) / duration);
	}

	void start(int dx, int dy, uint32_t now, uint32_t ms) {
		int old_x, old_y;
		offset(now, old_x, old_y);
		x        = dx + old_x;
		y        = dy + old_y;
		started  = now;
		duration = ms;
	}

	static int tile_delta(int from, int to, int world_size) {
		int delta = from - to;
		if (delta > world_size / 2) {
			delta -= world_size;
		}
		if (delta < -world_size / 2) {
			delta += world_size;
		}
		return delta;
	}
};
#endif
