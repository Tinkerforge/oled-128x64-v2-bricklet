// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_oled_128x64_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_OLED128x64V2 oled;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_oled_128x64_v2_create(&oled, NULL, hal), "create device object");

	// Clear display
	check(tf_oled_128x64_v2_clear_display(&oled), "call clear_display");

	// Write "Hello World" starting from upper left corner of the screen
	check(tf_oled_128x64_v2_write_line(&oled, 0, 0, "Hello World"), "call write_line");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
