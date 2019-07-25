#define MinRPM 30L       // 10 RPM Speed slowest
#define MaxRPM 600L      // 600 RPM Speed fastest

#define PotMeter A0       //Potmeter pin, must be analog, not A4 or A5 on arduino (SDA SCL)
#define AverageCount 5
#define MinAdc 50           //add dead band on start
#define MaxAdc 975          //add dead band on end
#define PotMeter A0         //Potmeter pin, must be analog, not A4 or A5 on arduino (SDA SCL)
#define AverageCount 25

#define PulsesPerRevolution 4
#define PulseHighMin 10

#define IndexPin 13
#define SyncPin 12

//#define DebugInfo
//define one or none of the fixed speed simulation setting
//#define FixSpeed30RPM
//#define FixSpeed120RPM
//#define FixSpeed600RPM

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

long CalculateDelayTime(long RPM)
{
  return  60000L / (RPM * PulsesPerRevolution );
}

long MapPotToRPM(long Pot)
{
  long RPM = map(Pot, MinAdc, MaxAdc, MinRPM, MaxRPM);
  if (RPM < MinRPM) return MinRPM;
  if (RPM > MaxRPM) return MaxRPM;
  return RPM;
}

long MapRPMtoDelay(long RPM)
{
  if (RPM < MinRPM) RPM = MinRPM;
  if (RPM > MaxRPM) RPM = MaxRPM;
  return  CalculateDelayTime(RPM);
}

long MapPotToDelay(long Pot)
{
  if (Pot < MinAdc) Pot = MinAdc;
  if (Pot > MaxAdc) Pot = MaxAdc;
  return MapRPMtoDelay(MapPotToRPM(Pot));
}

void ShowInfo()
{
#ifdef DebugInfo
  int ADC;
  ADC = ReadPotmeter();
  Serial.print("ADC: "); Serial.print(ADC);
  Serial.print(" RPM: "); Serial.print(MapPotToRPM(ADC));
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
  int PulseLow;
  int PulseHigh;
  for (;;)
  {
    for (int i = 0; i < PulsesPerRevolution; i++)
    {
      PulseTime = MapPotToDelay(ReadPotmeter());
      // if a fixed RPM is selecte, set speed accordinly
#if defined (FixSpeed30RPM)
      PulseTime = MapRPMtoDelay(30);
#elif defined FixSpeed120RPM
      PulseTime = MapRPMtoDelay(120);
#elif defined FixSpeed600RPM
      PulseTime = MapRPMToDelay(600);
#endif
      PulseTime -= PulseHighMin;
      if (i == 0) digitalWrite(IndexPin, HIGH); // Turn the LED on (Note that LOW is the voltage level
      else digitalWrite(IndexPin, LOW);         // To keep the pulses symetric set low to give the same delay between pulses
      digitalWrite(SyncPin, HIGH);     // Turn the LED on (Note that LOW is the voltage level
      delay(PulseHighMin);                         // Wait for a second
      digitalWrite(SyncPin, LOW);    // Turn the LED off by making the voltage HIGH
      digitalWrite(IndexPin, LOW);   // Turn the LED off by making the voltage HIGH
      delay(PulseTime);    // Wait for two seconds (to demonstrate the active low LED)
    }
#ifdef DebugInfo
    ShowInfo();
#endif
  }
}
