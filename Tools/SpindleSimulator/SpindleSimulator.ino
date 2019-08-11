#define MinRPM 30UL        //   30 RPM Speed slowest
#define MaxRPM 3000UL      // 3000 RPM Speed fastest

#define PotMeter A0       //Potmeter pin, must be analog, not A4 or A5 on arduino (SDA SCL)
//#define AverageCount 10
#define MinAdc 50UL           //add dead band on start
#define MaxAdc 975UL          //add dead band on end

#define PulsesPerRevolution 4UL
#define PulseHighMin 10UL

#define IndexPin 13
#define SyncPin 12

//#define DebugInfo
//define one or none of the fixed speed simulation setting
//#define FixSpeed30RPM
//#define FixSpeed120RPM
//#define FixSpeed600RPM

int ReadPotmeter()
{
  //  int Sum = 0;
  //  for (int i = 0; i < AverageCount; i++)
  //  {
  //    Sum += analogRead(PotMeter);
  //  }
  //  return Sum / AverageCount;
  return analogRead(PotMeter);
}

unsigned long CalculateDelayTime(unsigned long RPM)
{
  return  60000UL / (RPM * PulsesPerRevolution );
}

unsigned long MapPotToRPM(unsigned long Pot)
{
  unsigned long RPM = map(Pot, MinAdc, MaxAdc, MinRPM, MaxRPM);
  if (RPM < MinRPM) return MinRPM;
  if (RPM > MaxRPM) return MaxRPM;
  return RPM;
}

unsigned long MapRPMtoDelay(unsigned long RPM)
{
  if (RPM < MinRPM) RPM = MinRPM;
  if (RPM > MaxRPM) RPM = MaxRPM;
  return  CalculateDelayTime(RPM);
}

unsigned long MapPotToDelay(unsigned long Pot)
{
  if (Pot < MinAdc) Pot = MinAdc;
  if (Pot > MaxAdc) Pot = MaxAdc;
  return MapRPMtoDelay(MapPotToRPM(Pot));
}

void ShowInfo()
{
#ifdef DebugInfo
  unsigned long ADC;
  ADC = (unsigned long) ReadPotmeter();
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
      if (i == 0) digitalWrite(IndexPin, HIGH); // Turn the index pin high on the first sync pulse
      else digitalWrite(IndexPin, LOW);         // Turn the index pin low on all other sync pulses
      digitalWrite(SyncPin, HIGH);              // Turn the sync pin high
      delay(PulseHighMin);                      // Wait for the pulse high time
      digitalWrite(IndexPin, LOW);              // Turn the index pin Low
      digitalWrite(SyncPin, LOW);               // Turn the sync pin Low
      delay(PulseTime);                         // Wait for the pulse low time
    }
#ifdef DebugInfo
    ShowInfo();
#endif
  }
}
