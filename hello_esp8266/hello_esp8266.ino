#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// start asynchronous web server on port 80
AsyncWebServer server(80);

// constants
const char* ssid = "ESP8266";
const char* passphrase = "nullnull";
const int pinToUnoSignal = 4; // D2/GPIO4, digital communication to Uno

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        body {
            margin: 0;
            height: 100vh;
            display: flex;
            gap: 1rem;
            box-sizing: border-box;
        }
        
        body > div {
            flex-grow: 1;
            height: 100%;
            display: grid;
            place-content: center;
        }

        body > div > div {
            text-align: center;
        }
    </style>
</head>
<body>
    <div id="up-dn">
        <div id="up-dn-output">0</div>
        <input id="up-dn-range" type="range" min="0" max="255" value="0" orient="vertical">
    </div>

    <div>
        <div id="up-dn-output">128</div>
        <input type="range" min="0" max="255" value="128">
    </div>

    <script>
        const upDnRange = document.getElementById('up-dn-range')
        const upDnOutput = document.getElementById("up-dn-output")
        upDnRange.addEventListener('input', function fly () {
            upDnOutput.innerText = this.value
            
            const body = new FormData()
            body.append('UPDN', this.value)
            fetch(`http://192.168.4.1/updn`, {
                method: 'POST',
                body: body
            })
        })
    </script>
</body>
</html>
)rawliteral";


// Define the control inputs
#define MOT_A1_PIN 12
#define MOT_A2_PIN 14
#define MOT_B1_PIN 13
#define MOT_B2_PIN 15

#define MOT_C1_PIN 16
#define MOT_C2_PIN 5
#define MOT_D1_PIN 0
#define MOT_D2_PIN 4



void setup() {
  // Set all the motor control inputs to OUTPUT
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);
  
  pinMode(MOT_C1_PIN, OUTPUT);
  pinMode(MOT_C2_PIN, OUTPUT);
  pinMode(MOT_D1_PIN, OUTPUT);
  pinMode(MOT_D2_PIN, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);
  
  digitalWrite(MOT_C1_PIN, LOW);
  digitalWrite(MOT_C2_PIN, LOW);
  digitalWrite(MOT_D1_PIN, LOW);
  digitalWrite(MOT_D2_PIN, LOW);
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, passphrase);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });
  
  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/updn", HTTP_POST, [](AsyncWebServerRequest *request){
    int speed;
    speed = request->getParam("UPDN", true)->value().toInt();
    Serial.print("UPDN: ");
    Serial.println(speed);
    set_motor_currents(speed, speed, speed, speed);
    request->send(200, "text/plain", "asd");
  });

  // Start server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}


/// Set the current on a motor channel using PWM and directional logic.
///
/// \param pwm    PWM duty cycle ranging from -255 full reverse to 255 full forward
/// \param IN1_PIN  pin number xIN1 for the given channel
/// \param IN2_PIN  pin number xIN2 for the given channel
void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0) {  // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);

  } else { // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}

/// Set the current on both motors.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
void set_motor_currents(int pwm_A, int pwm_B, int pwm_C, int pwm_D)
{
  set_motor_pwm(pwm_A, MOT_A1_PIN, MOT_A2_PIN);
  set_motor_pwm(pwm_B, MOT_B1_PIN, MOT_B2_PIN);
  set_motor_pwm(pwm_C, MOT_C1_PIN, MOT_C2_PIN);
  set_motor_pwm(pwm_D, MOT_D1_PIN, MOT_D2_PIN);

  // Print a status message to the console.
  Serial.print("Set motor A PWM = ");
  Serial.print(pwm_A);
  Serial.print(" motor B PWM = ");
  Serial.println(pwm_B);
}

/// Simple primitive for the motion sequence to set a speed and wait for an interval.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
/// \param duration delay in milliseconds
void spin_and_wait(int pwm_A, int pwm_B, int duration)
{
  // set_motor_currents(pwm_A, pwm_B);
  delay(duration);
}
