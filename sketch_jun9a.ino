#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

WebServer server(80); // FIXED: Added missing Server declaration

// ultrasonic sensor
#define Trigpin 5
#define echopin 18
#define buzzer 4

#include <IRremote.hpp>

//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// touch 
#define touch_sensors 26
#define ir_sensorS 35

//DHT11
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Galaxy M30s441F";
const char* password = "lftn7687";

// Eye coordinates
int leftEyeX = 40, rightEyeX = 88, eyeY = 32, eyeRadius = 14;

//Soil Sensor 
#define SOIL_PIN 34

//Relay
#define RELAY_PIN 15

// Threshold for dry soil
int threshold = 3000;
String irresult = "NOTHING";

// Global variables
int soilValue;
float temperature;
float humidity;

//function for touch print
bool lasttouchstate = LOW;
int touchstate = 0;
float globalDistance;

// 2. HTML & CSS Webpage Design
void handleRoot() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<meta http-equiv='refresh' content='2'>"; // Auto-refresh page every 2 seconds
    html += "<title>ESP32 Dashboard</title>";
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f4f6f9; color: #333; margin: 0; padding: 20px; text-align: center; }";
    html += "h1 { color: #2c3e50; margin-bottom: 30px; }";
    html += ".container { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 20px; max-width: 1000px; margin: 0 auto; }";
    html += ".card { background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); border-top: 5px solid #3498db; transition: transform 0.2s; }";
    html += ".card:hover { transform: translateY(-5px); }";
    html += ".card.alert { border-top-color: #e74c3c; }";
    html += "h3 { margin: 0 0 10px 0; color: #7f8c8d; font-size: 14px; text-transform: uppercase; letter-spacing: 1px; }";
    html += ".value { font-size: 28px; font-weight: bold; color: #2c3e50; margin: 10px 0; }";
    html += ".footer { margin-top: 40px; font-size: 12px; color: #bdc3c7; }";
    html += "</style></head><body>";
    
    html += "<h1>ESP32 Live Sensor Dashboard</h1>";
    html += "<div class='container'>";
    
    // Cards for each sensor
    html += "<div class='card'><h3>Temperature</h3><div class='value'>" + String(temperature,1) + " &deg;C</div></div>";
    html += "<div class='card'><h3>Humidity</h3><div class='value'>" + String(humidity,1) + " %</div></div>";
    html += "<div class='card'><h3>Distance</h3><div class='value'>" + String(globalDistance,1) + " cm</div></div>";
    html += "<div class='card'><h3>Soil Moisture</h3><div class='value'>" + String(soilValue) + " %</div></div>";
    html += "<div class='card'><h3>object detection</h3><div class='value'>" + irresult + " %</div></div>";
    
    html += "</div>";
    html += "<div class='footer'>ESP32 Station | Updates automatically every 2 seconds</div>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

void ir_sensor() {
  bool ir = digitalRead(ir_sensorS);
  if(ir == HIGH)
    irresult = "OBJECT DETECTED";
  else
    irresult = "no object";
}

void showexpression(int state) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);

  switch(state) {
    case 0: // Expression 1: Neutral/Normal Eyes
      display.fillCircle(leftEyeX, eyeY, eyeRadius, SSD1306_WHITE);
      display.fillCircle(rightEyeX, eyeY, eyeRadius, SSD1306_WHITE);
      break;
      
    case 1: // Expression 2: Happy Eyes (^)
      display.setCursor(leftEyeX - 10, eyeY - 10);
      display.print("^");
      display.setCursor(rightEyeX - 10, eyeY - 10);
      display.print("^");
      break;
      
    case 2: // Expression 3: Wink (One open, one closed)
      display.fillCircle(leftEyeX, eyeY, eyeRadius, SSD1306_WHITE); // Left open
      display.fillRect(rightEyeX - eyeRadius, eyeY - 2, eyeRadius * 2, 4, SSD1306_WHITE); // Right flat line
      break;
      
    case 3: // Expression 4: Sleeping / Sad Eyes (v)
      display.setCursor(leftEyeX - 10, eyeY - 10);
      display.print("v");
      display.setCursor(rightEyeX - 10, eyeY - 10);
      display.print("v");
      break;
  }
  
  display.display();
}

void touch() {
  bool touch_sensor = digitalRead(touch_sensors);
  if(touch_sensor == HIGH && lasttouchstate == LOW) {
    touchstate++;
    if(touchstate > 3)
      touchstate = 0;
    showexpression(touchstate);
    delay(200);
  }
  lasttouchstate = touch_sensor;
}

// Function to read soil moisture
void readSoilMoisture() {
  soilValue = analogRead(SOIL_PIN);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);
}

void ultrasonic() {
  digitalWrite(Trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigpin, HIGH);
  delayMicroseconds(10); // Must be at least 10 microseconds
  digitalWrite(Trigpin, LOW);
  
  long duration = pulseIn(echopin, HIGH);
  float distance = (duration * 0.0343) / 2;
  globalDistance = distance;
  Serial.println(distance);
  
  if(distance > 6)
   { digitalWrite(buzzer, HIGH);
   delay(100); 
   }
  else
  digitalWrite(buzzer, LOW);
}

// Function to read DHT11
void readDHTSensor() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

// Function to control pump
void controlPump() {
  if (soilValue > threshold) {
    digitalWrite(RELAY_PIN, LOW); // Pump ON
    Serial.println("Pump ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Pump OFF
    Serial.println("Pump OFF");
  }
}

// Function to display data on OLED
void displayData() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Soil: ");
  display.print(soilValue);

  display.setCursor(0,20);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0,40);
  display.print("Humidity: ");
  display.print(humidity);
  display.print("%");

  display.display();
}

// Setup
void setup() {
  //Initialise Serial Print
  Serial.begin(115200);
  
  //ultrasonic sensor and buzzer
  pinMode(buzzer, OUTPUT);
  pinMode(Trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  //Initialise Relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  pinMode(touch_sensors, INPUT);
  pinMode(ir_sensorS, INPUT);
 
  //Initialise DHT
  dht.begin();

  //Initialise OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }
  
  // Initialize Remote Receiver
  WiFi.begin(ssid, password);
  Serial.println("System Started");
  
  // Set up Web Server Routes
  server.on("/", handleRoot);
  server.begin();
  showexpression(0); 
}
 
String globalIR = "0x0"; // FIXED: Added missing globalIR variable

// Loop
void loop() {
  server.handleClient(); // Process incoming web requests
  touch();
  
  // 3. Process Remote Control Inputs
  readSoilMoisture(); // Button 1
  readDHTSensor();    // Button 2
  controlPump();      // Button 3
  displayData();      // Button 4
  ir_sensor();
       
  static unsigned long lastUltraTime = 0;
  if (millis() - lastUltraTime > 400) {
     lastUltraTime = millis();
     ultrasonic();
     
     // Automatically pull background environment sensor cache for the webpage dashboard layout
     soilValue = analogRead(SOIL_PIN);
     float tempRead = dht.readTemperature();
     float humiRead = dht.readHumidity();
     if(!isnan(tempRead)) temperature = tempRead;
     if(!isnan(humiRead)) humidity = humiRead;
  }
}
