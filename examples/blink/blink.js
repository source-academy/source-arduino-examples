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
  serial_flush
*/

pin_mode(6, 1);
while(true) {
    digital_write(6, true);
    fn_delay(1000); 
    digital_write(6, false);
    fn_delay(1000);    
}