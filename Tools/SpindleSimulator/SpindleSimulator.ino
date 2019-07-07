#define PulseLow 50L         // 50 ms low pulse
#define IndexPulses 4L      // 4 sync pulses per turn
//#define SyncTime 125L        // 125 ms per sync pulse 120 RPM
#define SyncTime 250L        // 250 ms per sync pulse 60 RPM

#define PulseHigh  SyncTime - PulseLow

#define IndexPin 13
#define SyncPin 12
void setup() {
  pinMode(IndexPin, OUTPUT);     // Initialize the pin as an output
  pinMode(SyncPin, OUTPUT);     // Initialize the  pin as aput
}

void loop() {
  int PulseCount = 0;
  for (;;)
  {
    digitalWrite(IndexPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    for (int i = 0; i < IndexPulses; i++)
    {
      digitalWrite(SyncPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
      delay(PulseLow);                         // Wait for a second
      digitalWrite(SyncPin, HIGH);  // Turn the LED off by making the voltage HIGH
      digitalWrite(IndexPin, HIGH);  // Turn the LED off by making the voltage HIGH
      delay(PulseHigh);                      // Wait for two seconds (to demonstrate the active low LED)
    }
  }
}
