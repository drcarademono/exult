#include "actor_movement.h"

#include <cstdlib>
#include <iostream>

static void expect(const Actor_movement& move, uint32_t now, int x, int y) {
	int dx, dy;
	move.offset(now, dx, dy);
	if (dx != x || dy != y) {
		std::cerr << "offset at " << now << ": " << dx << ',' << dy << " expected " << x << ',' << y << '\n';
		std::exit(1);
	}
}

int main() {
	Actor_movement move;
	expect(move, 0, 0, 0);
	move.start(-8, 0, 100, 100);
	expect(move, 100, -8, 0);
	expect(move, 125, -6, 0);
	expect(move, 150, -4, 0);
	expect(move, 200, 0, 0);
	expect(move, 1000, 0, 0);
	// A new step before the previous glide finishes stays continuous.
	move.start(-8, 8, 300, 100);
	move.start(-8, 0, 350, 100);
	expect(move, 350, -12, 4);
	expect(move, 400, -6, 2);
	expect(move, 450, 0, 0);
	// Elevation affects both screen axes; reset handles teleports/toggles.
	move.start(12, 4, 500, 200);
	expect(move, 600, 6, 2);
	move.reset();
	expect(move, 600, 0, 0);
	move.start(8, 8, 700, 0);
	expect(move, 700, 0, 0);
	// The game clock wraps after 2^32 milliseconds.
	move.start(-8, -8, UINT32_MAX - 49, 100);
	expect(move, 0, -4, -4);
	expect(move, 50, 0, 0);
	if (Actor_movement::tile_delta(3071, 0, 3072) != -1 || Actor_movement::tile_delta(0, 3071, 3072) != 1
		|| Actor_movement::tile_delta(10, 11, 3072) != -1) {
		return 1;
	}
	std::cout << "Actor movement interpolation: timing, continuity, reset and world/clock wrapping passed\n";
}
