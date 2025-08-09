#define BLYNK_TEMPLATE_ID "TMPL3yKWLkUY8"
#define BLYNK_TEMPLATE_NAME "esp32"
#define BLYNK_DEVICE_NAME "esp32"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Replace with your Blynk credentials
char auth[] = "__________________";
char ssid[] = "_______________";
char pass[] = "_______________";

// Pin definitions
#define MQ2_ANALOG A0         // AOUT
#define MQ2_DIGITAL D6        // DOUT (GPIO12)
#define BUZZER_PIN D5         // Buzzer (GPIO14)
#define LED_PIN D7            // LED (GPIO13)

BlynkTimer timer;

void sendSensorData() {
  int analogVal = analogRead(MQ2_ANALOG);
  int digitalVal = digitalRead(MQ2_DIGITAL);

  Serial.print("MQ2 DIGITAL OUTPUT: ");
  Serial.println(digitalVal);
  Serial.print("MQ2 ANALOG VALUE: ");
  Serial.println(analogVal);

  Blynk.virtualWrite(V1, analogVal);  // Send analog value to Gauge (V1)

  // Alert condition
  if (digitalVal == LOW || analogVal > 290) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Blynk.logEvent("gas_alert", "🚨 Gas Leak Detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(MQ2_ANALOG, INPUT);
  pinMode(MQ2_DIGITAL, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensorData);  // Every 1 second
}

void loop() {
  Blynk.run();
  timer.run();
}
