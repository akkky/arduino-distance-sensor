// ultrasonic distance sensor
#define TRIG_PIN 2
#define ECHO_PIN 3
#define TRIG_DELAY 100
// IR distance censor
#define IR_INPUT_PIN 0
#define SAMPLING_COUNT 100
// LED
#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10
#define LED4_PIN 11
#define LED5_PIN 12

void setup() {
  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // LED
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  double ultrasonic_distance = 0;
  int ir_distance = 0;
  
  ultrasonic_distance = get_ultrasonic_distance();
  ir_distance = get_ir_distance();
  
  Serial.print("Ultrasonic : ");
  Serial.print(ultrasonic_distance);
  Serial.println(" m");
  Serial.print("IR : ");
  Serial.print(ir_distance);
  Serial.println(" mm");
  
  output_led(5 - ir_distance / 160);
  
  delay(500);
}

void output_led(int level) {
  int led[5] = {LOW, LOW, LOW, LOW, LOW};
  int i;
  for(i = 0; i < level && i < 5; i++) {
    led[i] = HIGH;
  }
  
  digitalWrite(LED1_PIN, led[0]);
  digitalWrite(LED2_PIN, led[1]);
  digitalWrite(LED3_PIN, led[2]);
  digitalWrite(LED4_PIN, led[3]);
  digitalWrite(LED5_PIN, led[4]);
  
  Serial.print("Level : ");
  Serial.println(level);
}

double get_ultrasonic_distance() {
    unsigned long interval = 0;

    // トリガーのパルスを送信
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(TRIG_DELAY);
    digitalWrite(TRIG_PIN, LOW);

    interval = pulseIn(ECHO_PIN, HIGH);
    return (double)interval * 0.00017;
}

int get_ir_distance() {
  int ans;
  float volts;

  ans = get_serial_average(IR_INPUT_PIN);
  volts = analog_to_volts(ans);
  return volts_to_mm(volts);
}

int get_serial_average(int pin_no) {
  long sum;
  int i;

  sum = 0;

  for(i = 0; i < SAMPLING_COUNT; i++) {
    sum = sum + analogRead(pin_no);
  }

  return sum / SAMPLING_COUNT;
}

float analog_to_volts(int analog_value) {
  return (float)analog_value * 5.0 / 1024.0;
}

int volts_to_mm(float volts) {
  int mm;
  if(volts > 3.0) {
    return 70;
  }
  else if(0.4 >= volts) {
    return 800;
  }
  else {
    mm = calc_volts_to_mm(volts, 3.0, 2.3, 100, 70);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 2.3, 1.7, 150, 100);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 1.7, 1.3, 200, 150);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 1.3, 1.1, 250, 200);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 1.1, 0.9, 300, 250);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 0.9, 0.75, 400, 300);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 0.75, 0.6, 500, 400);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 0.6, 0.5, 600, 500);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 0.5, 0.45, 700, 600);
    if(mm >= 0) {
      return mm;
    }
    mm = calc_volts_to_mm(volts, 0.45, 0.4, 800, 700);
    if(mm >= 0) {
      return mm;
    }
  }
  return -1;
}

int calc_volts_to_mm(float volts, float volts_max, float volts_min, int mm_max, int mm_min) {
  float temp;
  if(volts_max >= volts && volts > volts_min) {
    temp = (float)(mm_max - mm_min) / (volts_max - volts_min);
    return (int)(mm_min + (volts - volts_min) * temp);
  }
  else {
    return -1;
  }
}

