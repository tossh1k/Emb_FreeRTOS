#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#define LED_PIN 2
int state = HIGH;

char str1[] = "AAAA";
char str2[] = "BBBB";

char ch1[] = "+";
char ch2[] = "*";
char ch3[] = "$";

void LedTask(void* Arg) {
    pinMode(LED_PIN, OUTPUT);
    for (;;) {
        state = !state;
        digitalWrite(LED_PIN, state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}
void UARTTask(void* Arg) {
    for (int i = 0; i < 5; i++) {
        Serial.println((char*)Arg);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}
SemaphoreHandle_t Semaphore = NULL;
void SemTask(void* Arg) {
    xSemaphoreTake(Semaphore, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
        Serial.print((char*)Arg);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    xSemaphoreGive(Semaphore);
    vTaskDelete(NULL);
}
SemaphoreHandle_t SemaphoreBin = NULL;
void SemBinTask(void* Arg) {
    for (int i = 0; i < 5; i++) {
        xSemaphoreTake(SemaphoreBin, portMAX_DELAY);
        Serial.println((char*)Arg);
        xSemaphoreGive(SemaphoreBin);
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}
SemaphoreHandle_t mutex = NULL; 
void UartTaskMutex(void* Arg) {
    for (int i = 0; i < 5; i++) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        Serial.println((char*)Arg);
        xSemaphoreGive(mutex);
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}

TaskHandle_t waitTask = NULL;
void WaitTask(void* Arg) {  
    Serial.println("Waiting...");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Waiting has been notified!");
    vTaskDelete(NULL);
} 

void NotifyTask(void* Arg) {
    vTaskDelay(5000/portTICK_PERIOD_MS);
    xTaskNotifyGive(*((TaskHandle_t*)Arg));
    vTaskDelete(NULL);
}

TaskHandle_t waitTask2 = NULL;
void WaitTask2(void* Arg) {
    
    Serial.println("Waiting...");
    xTaskNotifyWait(ULONG_MAX, ULONG_MAX, NULL,portMAX_DELAY);
    Serial.println("Waiting has been notified!");
    vTaskDelete(NULL);
}
void NotifyTask2(void* Arg) {
    vTaskDelay(5000/portTICK_PERIOD_MS);
    xTaskNotify(*((TaskHandle_t*)Arg), 0, eNoAction);
    vTaskDelete(NULL);
}

TaskHandle_t timer = NULL;
int timerID1 = 7;
int timerID2 = 8;

void TimerCallBack(TimerHandle_t xTimer)
{
  Serial.println("Time Interval elapsed");
  Serial.print("TimerID: ");
  Serial.println(*((int*)pvTimerGetTimerID(xTimer)));
}

void setup() {

    Serial.begin(115200);

    //Semaphore = xSemaphoreCreateCounting(2, 2);

    //SemaphoreBin = xSemaphoreCreateBinary();
    //xSemaphoreGive(SemaphoreBin);

    //mutex = xSemaphoreCreateMutex();

    xTaskCreate(LedTask, "ledstate", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);

    //xTaskCreate(UARTTask, "uart1", 1024, str1, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate(UARTTask, "uart2", 1024, str2, tskIDLE_PRIORITY + 1, NULL);

    //xTaskCreate (SemTask, "uart1", 1024, ch1, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate (SemTask, "uart2", 1024, ch2, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate (SemTask, "uart3", 1024, ch3, tskIDLE_PRIORITY + 1, NULL);

    //xTaskCreate (SemBinTask, "uart1", 1024, str1, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate (SemBinTask, "uart2", 1024, str2, tskIDLE_PRIORITY + 1, NULL);

    //xTaskCreate (UartTaskMutex, "uart1", 1024, str1, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate (UartTaskMutex, "uart2", 1024, str2, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(WaitTask, "waittask", 1024, NULL, tskIDLE_PRIORITY + 1, &waitTask);
    xTaskCreate(NotifyTask, "noyifytask", 1024, &waitTask, tskIDLE_PRIORITY + 1, NULL);
    
    //xTaskCreate(WaitTask2, "waittask", 1024, NULL, tskIDLE_PRIORITY + 1, &waitTask2);
    //xTaskCreate(NotifyTask2, "noyifytask", 1024, &waitTask2, tskIDLE_PRIORITY + 1, NULL);

    //timer = xTimerCreate("myTimer", 3000/portTICK_PERIOD_MS, pdTRUE, &timerID1, TimerCallBack);
    //xTimerStart(timer, portMAX_DELAY);
    //timer = xTimerCreate("myTimer", 3000/portTICK_PERIOD_MS, pdTRUE, &timerID2, TimerCallBack);
    //xTimerStart(timer, portMAX_DELAY);
}

void loop() {

}

