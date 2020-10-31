/*
 * ESP32のタイマ割込みを使う
*/

#define LED_PIN 32 // LED出力ピン

// variables
volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// ISR
void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    // タイマ初期化 (タイマ番号(0~3), プリスケーラ(Source:80MHz), カウントアップ(bool));
    timer = timerBegin(0, 80, true);
    
    // 割込み関数のアサイン 最後のtrueはエッジ割込みをするかどうか
    timerAttachInterrupt(timer, &onTimer, true);
    
    // タイマ割込みタイミング、繰り返すかどうかを設定
    timerAlarmWrite(timer, 1000, true);

    // タイマ割込み有効
    timerAlarmEnable(timer);
}

void loop() {
    if (interruptCounter > 0) {
   
      portENTER_CRITICAL(&timerMux);
      interruptCounter--;
      portEXIT_CRITICAL(&timerMux);
   
      totalInterruptCounter++;

      digitalWrite(LED_PIN, (totalInterruptCounter % 2) == 0);
   
    }
}
