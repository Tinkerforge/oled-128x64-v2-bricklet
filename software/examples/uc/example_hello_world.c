

#include "bindings/hal_common.h"
#include "bindings/bricklet_oled_128x64_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your OLED 128x64 Bricklet 2.0

void check(int rc, const char* msg);




TF_OLED128x64V2 oled;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_oled_128x64_v2_create(&oled, UID, hal), "create device object");


	// Clear display
	check(tf_oled_128x64_v2_clear_display(&oled), "call clear_display");

	// Write "Hello World" starting from upper left corner of the screen
	check(tf_oled_128x64_v2_write_line(&oled, 0, 0, "Hello World"), "call write_line");

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
