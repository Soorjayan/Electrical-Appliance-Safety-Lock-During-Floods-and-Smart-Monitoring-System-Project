#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "HONOR 200 Lite";
const char* password = "12345678";

WebServer server(80);
HardwareSerial mySerial(2);

// Relay pin
#define RELAY_PIN 25

int waterVal = 0;
int moistureVal = 0;

String waterStatus = "UNKNOWN";
String moistureStatus = "UNKNOWN";
String powerStatus = "OFF";

// HTML page
String getHTML()
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Smart Monitoring</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { font-family: Arial; text-align:center; }
.safe { color:green; font-weight:bold; }
.danger { color:red; font-weight:bold; }
button { padding:10px; margin:10px; font-size:16px; }
</style>
</head>

<body>

<h1>Smart Monitoring System</h1>

<h2>Water: <span id="water"></span></h2>
<h3 id="waterStatus"></h3>

<h2>Moisture: <span id="moisture"></span></h2>
<h3 id="moistureStatus"></h3>

<h2>Power: <span id="power"></span></h2>

<button onclick="fetch('/on')">Turn ON</button>
<button onclick="fetch('/off')">Turn OFF</button>

<canvas id="chart" width="300" height="150"></canvas>

<script>
let waterData = [];
let moistureData = [];
let labels = [];

const ctx = document.getElementById('chart').getContext('2d');
const chart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: labels,
    datasets: [
      { label: 'Water', data: waterData, borderWidth: 2 },
      { label: 'Moisture', data: moistureData, borderWidth: 2 }
    ]
  }
});

function updateData() {
  fetch('/data')
    .then(res => res.json())
    .then(data => {
      document.getElementById('water').innerText = data.water;
      document.getElementById('moisture').innerText = data.moisture;
      document.getElementById('power').innerText = data.power;

      let ws = document.getElementById('waterStatus');
      let ms = document.getElementById('moistureStatus');

      ws.innerText = data.waterStatus;
      ms.innerText = data.moistureStatus;

      ws.className = data.waterDanger ? "danger" : "safe";
      ms.className = data.moistureDanger ? "danger" : "safe";

      labels.push('');
      waterData.push(data.water);
      moistureData.push(data.moisture);

      if(labels.length > 20){
        labels.shift();
        waterData.shift();
        moistureData.shift();
      }

      chart.update();
    });
}

setInterval(updateData, 2000);
</script>

</body>
</html>
)rawliteral";

  return html;
}

void handleRoot()
{
  server.send(200, "text/html", getHTML());
}

void handleData()
{
  bool waterDanger = (waterVal > 625);
  bool moistureDanger = (moistureVal < 400);

  String json = "{";
  json += "\"water\":" + String(waterVal) + ",";
  json += "\"moisture\":" + String(moistureVal) + ",";
  json += "\"waterStatus\":\"" + waterStatus + "\",";
  json += "\"moistureStatus\":\"" + moistureStatus + "\",";
  json += "\"power\":\"" + powerStatus + "\",";
  json += "\"waterDanger\":" + String(waterDanger ? "true":"false") + ",";
  json += "\"moistureDanger\":" + String(moistureDanger ? "true":"false");
  json += "}";

  server.send(200, "application/json", json);
}

void handleOn()
{
  digitalWrite(RELAY_PIN, HIGH);
  powerStatus = "ON";
  server.send(200, "text/plain", "ON");
}

void handleOff()
{
  digitalWrite(RELAY_PIN, LOW);
  powerStatus = "OFF";
  server.send(200, "text/plain", "OFF");
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
}

void loop()
{
  if (mySerial.available())
  {
    String data = mySerial.readStringUntil('\n');

    int wIndex = data.indexOf("W:");
    int mIndex = data.indexOf("M:");

    if (wIndex != -1 && mIndex != -1)
    {
      waterVal = data.substring(wIndex+2, data.indexOf(",", wIndex)).toInt();
      moistureVal = data.substring(mIndex+2).toInt();

      if (waterVal < 100) waterStatus = "LOW";
      else if (waterVal <= 600) waterStatus = "SAFE";
      else if (waterVal <= 625) waterStatus = "MEDIUM";
      else waterStatus = "DANGER";

      if (moistureVal < 400) moistureStatus = "DRY";
      else if (moistureVal <= 700) moistureStatus = "NORMAL";
      else moistureStatus = "WET";
    }
  }

  server.handleClient();
}