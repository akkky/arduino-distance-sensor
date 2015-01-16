// 超音波センサー
#define TRIG_PIN 2
#define ECHO_PIN 3
#define TRIG_DELAY 100
#define US_SAMPLING_COUNT 20
// 赤外線センサー
#define IR_INPUT_PIN 0
#define IR_SAMPLING_COUNT 100
// LED
#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10
#define LED4_PIN 11
#define LED5_PIN 12
#define LED6_PIN 13

// 初期化
void setup() {
  // 超音波センサー
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // LED
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
  pinMode(LED6_PIN, OUTPUT);
  
  Serial.begin(9600);
}

// メインループ
void loop() {
  int ultrasonic_distance = 0;
  int ir_distance = 0;
  
  ultrasonic_distance += get_ultrasonic_distance();
  ir_distance = get_ir_distance();
  
  Serial.print(ir_distance);
  Serial.print(",");
  Serial.println(ultrasonic_distance);
  
//  output_led(6 - ir_distance / 133);
  output_led(6 - ultrasonic_distance / 100);
  
  delay(500);
}

// LED に出力
void output_led(int level) {
  int led[6] = {LOW, LOW, LOW, LOW, LOW};
  int i;
  for(i = 0; i < level && i < 6; i++) {
    led[i] = HIGH;
  }
  
  digitalWrite(LED1_PIN, led[0]);
  digitalWrite(LED2_PIN, led[1]);
  digitalWrite(LED3_PIN, led[2]);
  digitalWrite(LED4_PIN, led[3]);
  digitalWrite(LED5_PIN, led[4]);
  digitalWrite(LED6_PIN, led[5]);
}

// 超音波センサーから距離を取得(mm)
int get_ultrasonic_distance() {
  unsigned long distance_total = 0;
  unsigned long interval = 0;
  int i = 0;
  
  for(i = 0; i < US_SAMPLING_COUNT; i++) {
    // トリガーのパルスを送信
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(TRIG_DELAY);
    digitalWrite(TRIG_PIN, LOW);

    interval = pulseIn(ECHO_PIN, HIGH);
    distance_total += (int)((double)interval * 0.17);
  }
  
  return (int)(distance_total / US_SAMPLING_COUNT);
}

// 赤外線センサーから距離を取得
int get_ir_distance() {
  int ans;
  float volts;

  ans = get_serial_average(IR_INPUT_PIN);
  volts = analog_to_volts(ans);
  return volts_to_mm(volts);
}

// アナログポートからの入力値の平均値を取得
int get_serial_average(int pin_no) {
  long sum;
  int i;

  sum = 0;

  for(i = 0; i < IR_SAMPLING_COUNT; i++) {
    sum = sum + analogRead(pin_no);
  }

  return sum / IR_SAMPLING_COUNT;
}

// アナログポートからの入力値を電圧に変換
float analog_to_volts(int analog_value) {
  return (float)analog_value * 5.0 / 1024.0;
}

// 電圧から距離に変換
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

// 電圧から距離を計算
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

