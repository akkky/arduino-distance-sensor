#define TRIG_PIN 2
#define ECHO_PIN 3
#define TRIG_DELAY 100


void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    int interval = 0;
    double distance = 0;
    // トリガーのパルスを送信
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(TRIG_DELAY);
    digitalWrite(TRIG_PIN, LOW);

    interval = pulseIn(ECHO_PIN, HIGH);
    distance = (float)interval * 0.00017;

    Serial.print(distance);
    Serial.println(" m");

    delay(500);
}
