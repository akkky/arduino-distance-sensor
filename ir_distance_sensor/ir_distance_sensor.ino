#define SAMPLING_COUNT 100

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ans;
  float volts;
  int mm;

  ans = get_serial_average(0);
  volts = analog_to_volts(ans);
  mm = volts_to_mm(volts);
  Serial.print(mm);
  Serial.println(" mm");
  delay(500);
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
