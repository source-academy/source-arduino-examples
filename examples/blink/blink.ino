#include <sinter.h>
#include "internal_functions.h"
#include "program.h"

char heap[0x4000];

void setup() {
  setupInternals();
  sinter_setup_heap(heap, 0x4000);
  Serial.begin(9600);
  delay(2000);
  
  sinter_value_t result;
  sinter_fault_t fault = sinter_run(program_svm, program_svm_len, &result);

  Serial.print("Program exited with fault ");
  Serial.print(fault);
  Serial.print(" and result type ");
  Serial.print(result.type);
  Serial.print(" (");
  Serial.print(result.integer_value);
  Serial.print(", ");
  Serial.print(result.boolean_value);
  Serial.print(", ");
  Serial.print(result.float_value);
  Serial.print(", ");
  Serial.print(result.string_value);
  Serial.print(")\n");
}

void loop() {}
