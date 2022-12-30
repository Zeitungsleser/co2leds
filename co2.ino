#include <ErriezMHZ19B.h>
#include <SoftwareSerial.h>
#include <FastLED.h>

#define MHZ19B_TX_PIN 16
#define MHZ19B_RX_PIN 17
#define LED_PIN 22

SoftwareSerial mhzSerial(MHZ19B_TX_PIN, MHZ19B_RX_PIN);
ErriezMHZ19B mhz19b(&mhzSerial);

const uint8_t NUM_LEDS = 12;
CRGB leds[NUM_LEDS];

int werte[NUM_LEDS];
int pos = 0;

double average(){
  int sum=0;
  for(int i=0;i<NUM_LEDS;++i){
    sum += werte[i];
  }
  return ((double) sum / NUM_LEDS);
}

void setup() {
  Serial.begin(115200);
  mhzSerial.begin(9600);
  while (!mhz19b.detect()){
    delay(2000);
  };
  while (mhz19b.isWarmingUp()){
    delay(2000);
  }
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
  CRGB rot{127,0,0};
  CRGB gruen{0,127,0};
  CRGB aus{0,0,0};

  if(mhz19b.isReady()){
    pos = (pos+1)%12;    
    werte[pos] = mhz19b.readCO2();
    Serial.print(werte[pos]);
    Serial.println(average());
  }
  double avg = average();
  for(int i = 0;i<NUM_LEDS;++i){
    if(werte[i] > avg){
      leds[i] = rot;
    }else{
      leds[i] = gruen;
    }
    FastLED.show();
  }
  delay(200);
  leds[pos] = aus;
  FastLED.show();
  delay(200);
}
