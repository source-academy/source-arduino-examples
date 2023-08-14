/**!sivmfn
  digital_read,
  digital_write,
  pin_mode,
  analog_read,
  analog_reference,
  analog_write,
  fn_delay,
  delay_us,
  fn_micros,
  fn_millis,
  attach_interrupt,
  detach_interrupt,
  enable_interrupts,
  disable_interrupts,
  serial_begin,
  serial_end,
  serial_settimeout,
  serial_print,
  serial_println,
  serial_read,
  serial_write,
  serial_flush,
  sevseg_set_number,
  sevseg_set_chars,
  sevseg_refresh_display
*/

let timer = fn_millis();
let seconds = 0;

while (true) {
  if (fn_millis() - timer >= 1000) {
    timer = timer + 1000;
    seconds = seconds + 1;
    if (seconds === 10000) {
      seconds = 0;
    }
  }
	sevseg_set_number(seconds);
  sevseg_refresh_display();
}