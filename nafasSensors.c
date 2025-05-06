#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define mq8 A0
#define mq7 A1
#define mq4 A2

int vodo;
int ugar;
int prir;
int moodValue1 = 1; // try -1, 0, or 1
int moodValue2 = 1; // try -1, 0, or 1
int moodValue3 = 1; // try -1, 0, or 1
int moodValue4 = 1; // try -1, 0, or 1
int co2ppm;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// WiFi credentials and Google Script URL
const char* ssid = "Hidden_for_security";
const char* password = "Hidden_for_security";
const String scriptURL = "https://script.google.com/macros/s/AKfycbzUlYEXALfmEo4gdxF6dNeUYwJ-RKwDZT4KZ4kNwMjQFsKfPgzPfYfJt4gROA_o35cE/exec"; // replace with actual


void setup() {
  Serial.begin(9600); // и для монитора, и для датчика
  Serial.println("CO2 Sensor on Serial");

  pinMode(mq8, INPUT);
  pinMode(mq7, INPUT);
  pinMode(mq4, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  if (Serial.available() >= 6) {
    byte b1 = Serial.read();
    byte b2 = Serial.read();
    byte b3 = Serial.read();
    byte b4 = Serial.read();
    byte b5 = Serial.read();
    byte b6 = Serial.read();

    byte checksum = (b1 + b2 + b3 + b4 + b5) & 0xFF;

    if (b1 == 0x2C && b6 == checksum) {
      co2ppm = b2 * 256 + b3;
      Serial.print("CO2: ");
      Serial.print(co2ppm);
      Serial.println(" ppm");
    } else {
      Serial.println("Invalid data or checksum error");
    }

    vodo = analogRead(mq8);
    ugar = analogRead(mq7);
    prir = analogRead(mq4);
    Serial.print(vodo);
    Serial.print(", ");
    Serial.print(ugar);
    Serial.print(", ");
    Serial.print(prir);
    Serial.println(", ");

    // Send data to Google Sheets
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = scriptURL + "?vodo=" + String(vodo)
                              + "&ugar=" + String(ugar)
                              + "&prir=" + String(prir)
                              + "&co2=" + String(co2ppm);
      http.begin(url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Data sent: " + payload);
      } else {
        Serial.println("Failed to send data");
      }
      http.end();
    } else {
      Serial.println("WiFi not connected");
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("H2");
  lcd.setCursor(4, 0);
  lcd.print("CO");
  lcd.setCursor(8, 0);
  lcd.print("CH4");
  lcd.setCursor(12, 0);
  lcd.print("CO2");

  if (vodo <= 250) {
    moodValue1 = 1;
  } else if (vodo >= 400) {
    moodValue1 = -1;
  } else moodValue1 = 0;

  if (ugar <= 70) {
    moodValue2 = 1;
  } else if (vodo >= 120) {
    moodValue2 = -1;
  } else moodValue2 = 0;

  if (prir <= 300) {
    moodValue3 = 1;
  } else if (prir >= 500) {
    moodValue3 = -1;
  } else moodValue3 = 0;

  if (co2ppm <= 450) {
    moodValue4 = 1;
  } else if (co2ppm >= 2000) {
    moodValue4 = -1;
  } else moodValue4 = 0;

  if (moodValue1 == -1) {
    lcd.setCursor(0, 1);
    lcd.print(";_;");
  } else if (moodValue1 == 0) {
    lcd.setCursor(0, 1);
    lcd.print("-_-");
  } else if (moodValue1 == 1) {
    lcd.setCursor(0, 1);
    lcd.print("^_^");
  } else {
    lcd.print("Err");
  }

  if (moodValue2 == -1) {
    lcd.setCursor(4, 1);
    lcd.print(";_;");
  } else if (moodValue2 == 0) {
    lcd.setCursor(4, 1);
    lcd.print("-_-");
  } else if (moodValue2 == 1) {
    lcd.setCursor(4, 1);
    lcd.print("^_^");
  } else {
    lcd.print("Err");
  }

  if (moodValue3 == -1) {
    lcd.setCursor(8, 1);
    lcd.print(";_;");
  } else if (moodValue3 == 0) {
    lcd.setCursor(8, 1);
    lcd.print("-_-");
  } else if (moodValue3 == 1) {
    lcd.setCursor(8, 1);
    lcd.print("^_^");
  } else {
    lcd.print("Err");
  }

  if (moodValue4 == -1) {
    lcd.setCursor(12, 1);
    lcd.print(";_;");
  } else if (moodValue4 == 0) {
    lcd.setCursor(12, 1);
    lcd.print("-_-");
  } else if (moodValue4 == 1) {
    lcd.setCursor(12, 1);
    lcd.print("^_^");
  } else {
    lcd.print("Err");
  }

  delay(1000);
}
