#define MinSpeedRPM 30L       // 30 RPM Speed slowest
#define MaxSpeedRPM 600L      // 600 RPM Speed fastest

#define PotMeter A0       //Potmeter pin, must be analog, not A4 or A5 on arduino (SDA SCL)
#define AverageCount 25
#define MinAdc 50           //add dead band on start
#define MaxAdc 975          //add dead band on end
#define PotMeter A0         //Potmeter pin, must be analog, not A4 or A5 on arduino (SDA SCL)
#define AverageCount 25

#define PulsesPerRevolution 4
#define PulseHighMin 20

#define IndexPin 13
#define SyncPin 12

#define DebugInfo

int ReadPotmeter()
{
  int Sum = 0;
  for (int i = 0; i < AverageCount; i++)
  {
    Sum += analogRead(PotMeter);
    delay(2);
  }
  return Sum / AverageCount;
}

long CalculateDelayTime(long SpeedRPM)
{
  return  60000L / (SpeedRPM * PulsesPerRevolution );
}

long MapPotToSpeed(long Pot)
{
  long Speed = map(Pot, MinAdc, MaxAdc, MinSpeedRPM, MaxSpeedRPM);
  if (Speed < MinSpeedRPM) return MinSpeedRPM;
  if (Speed > MaxSpeedRPM) return MaxSpeedRPM;
  return Speed;
}

long MapSpeedToDelay(long Speed)
{
  if (Speed < MinSpeedRPM) Speed = MinSpeedRPM;
  if (Speed > MaxSpeedRPM) Speed = MaxSpeedRPM;
  return  CalculateDelayTime(Speed);
}

long MapPotToDelay(long Pot)
{
  if (Pot < MinAdc) Pot = MinAdc;
  if (Pot > MaxAdc) Pot = MaxAdc;
  return MapSpeedToDelay(MapPotToSpeed(Pot));
}

void ShowInfo()
{
#ifdef DebugInfo
  int ADC;
  ADC = ReadPotmeter();
  Serial.print("ADC: "); Serial.print(ADC);
  Serial.print(" RPM: "); Serial.print(MapPotToSpeed(ADC));
  Serial.print(" Sync Pulse Delay: "); Serial.print(MapPotToDelay(ADC));
  Serial.println();
#endif
}

void setup()
{
#ifdef DebugInfo
  Serial.begin(115200);
#endif
  pinMode(IndexPin, OUTPUT);     // Initialize the pin as an output
  pinMode(SyncPin, OUTPUT);     // Initialize the  pin as aput
}

void loop()
{
  int PulseTime;
  int PulseHigh;
  for (;;)
  {
    PulseTime = MapPotToDelay(ReadPotmeter());
    PulseHigh=PulseTime/2;
    if (PulseHigh> PulseHighMin)
      PulseHigh=PulseHighMin;
    ShowInfo();
    digitalWrite(IndexPin, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    for (int i = 0; i < PulsesPerRevolution; i++)
    {
      digitalWrite(SyncPin, HIGH);     // Turn the LED on (Note that LOW is the voltage level
      delay(PulseHigh);                         // Wait for a second
      digitalWrite(SyncPin, LOW);    // Turn the LED off by making the voltage HIGH
      digitalWrite(IndexPin, LOW);   // Turn the LED off by making the voltage HIGH
      delay(PulseTime - PulseHigh);    // Wait for two seconds (to demonstrate the active low LED)
    }
  }
}
