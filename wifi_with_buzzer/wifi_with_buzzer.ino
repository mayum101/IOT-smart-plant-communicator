#define BLYNK_TEMPLATE_ID "TMPL3CHcpRF8U"
#define BLYNK_TEMPLATE_NAME "soil humidity"
#define BLYNK_AUTH_TOKEN "ieoBhE8CmPYXouR72zHMlQvdSdU3Pi0O"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi 9i";  // type your wifi name
char pass[] = "88888888";  // type your wifi password

BlynkTimer timer;

#define DHTPIN 4 // D2 on NodeMCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

#define BUZZER_PIN D5  // Buzzer connected to D5 (GPIO14)

void sendSensor()
{
  int value = analogRead(A0);
  value = map(value, 400, 1023, 100, 0);  // Soil moisture %

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, value);  // Soil
  Blynk.virtualWrite(V1, t);      // Temperature
  Blynk.virtualWrite(V2, h);      // Humidity

  Serial.print("Soil Moisture : ");
  Serial.print(value);
  Serial.print("    Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

  // Buzzer Logic
  if (value < 30) {  // Dry soil => buzzer ON
    digitalWrite(BUZZER_PIN, HIGH);
  } else {           // Wet soil => buzzer OFF
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup()
{   
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin mode

  timer.setInterval(100L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
