#include <stdio.h>

// 超音波センサー
#define TRIG_PIN 2
#define ECHO_PIN 3
#define TRIG_DELAY 10
#define US_SAMPLING_COUNT 20
// 赤外線センサー
#define IR_INPUT_PIN 0
#define IR_SAMPLING_COUNT 100
// 水センサー
#define WATER_INPUT_PIN 1
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
  int water_level = 0;
  
  ultrasonic_distance = get_ultrasonic_distance();
  ir_distance = get_ir_distance();
  water_level = get_water_level();
  
  char output[13];
  snprintf(output, sizeof(output), "%04d%04d%04d", ir_distance, ultrasonic_distance, water_level);
  char output_with_digit[sizeof(output) + 2];
  add_check_digit(output_with_digit, output);
  Serial.println(output_with_digit);
  
  output_led(6 - ultrasonic_distance / 70);
  
  delay(100);
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

    interval = pulseIn(ECHO_PIN, HIGH, 2000000);
    distance_total += (int)((double)interval * 0.17325);
    
    delay(1);
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

// 水センサーの値を取得
int get_water_level() {
  int value = analogRead(WATER_INPUT_PIN);
  return value;
}

// チェックディジットを付加します
void add_check_digit(char added_value[], char value[]) {
  int len = strlen(value);
  
  char digit = calc_check_digit(value);
  
  strcpy(added_value, value);
  
  added_value[len] = digit;
  added_value[len + 1] = '\0';
}

// チェックディジットをチェックします
boolean check_check_digit(char value[]) {
  int len = strlen(value);
  char check_value[len];
  strncpy(check_value, value, len - 1);
  check_value[len - 1] = '\0';
  char digit = calc_check_digit(check_value);
  return digit == value[len - 1];
}

// チェックディジットを計算
char calc_check_digit(char value[]) {
  int i;
  int odd = 0;
  int even = 0;
  
  int len = strlen(value);
  
  // 奇数桁
  for(i = 0; i < len; i += 2) {
    odd += value[i] - '0';
  }
  for(i = 1; i < len; i+= 2) {
    even += value[i] - '0';
  }
  return '0' + (10 - ((odd * 3 + even) % 10)) % 10;
}

