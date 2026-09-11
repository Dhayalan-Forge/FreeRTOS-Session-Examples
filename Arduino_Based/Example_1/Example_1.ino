#define LED       6
#define BUTTON    8
#define SENSOR    10

volatile uint32_t customDelay = 200;

void blinkLed()
{
  digitalWrite(LED, HIGH);
  delay(customDelay);

  digitalWrite(LED, LOW);
  delay(customDelay);
}

void readButton()
{
  int data = digitalRead(BUTTON);
  delay(50);

  (data == 0) ? Serial.println("Button Pressed") : NULL;
}

void readSensor()
{
  int data = digitalRead(SENSOR);
  delay(50);

  (data == 0) ? Serial.println("Object Detected") : NULL;
}

void readUART()
{
    if (Serial.available() > 0)
    {
        int val = Serial.parseInt();

        while (Serial.available())
            Serial.read();

        if (val <= 0 || val > 3000)
            val = 2000;

        customDelay = val;

        Serial.print("New delay: ");
        Serial.println(customDelay);
    }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(SENSOR, INPUT);

  Serial.println("-----------------------------");
}

void loop() {

  blinkLed();
  readButton();
  readSensor();
  readUART();
}
