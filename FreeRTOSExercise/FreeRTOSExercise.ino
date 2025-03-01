#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Definición de la cola
QueueHandle_t xQueue;
 // Inicio de la config
void setup() {
  Serial.begin(9600);

  // Creación de la cola
  xQueue = xQueueCreate(10, sizeof(int));

  if (xQueue != NULL) {
    // Crear tareas
    xTaskCreate(TaskSender, "Sender", 128, NULL, 1, NULL);
    xTaskCreate(TaskReceiver, "Receiver", 128, NULL, 1, NULL);
  } else {
    Serial.println("Error");
  }
}

void loop() {
  // No se necesita código en loop() cuando se usa FreeRTOS
}
//Se encarga de enviar 
void TaskSender(void *pvParameters) {
  int value = 0;
  for (;;) {
    // Incrementar el valor y enviarlo a la cola
    value++;
    xQueueSend(xQueue, &value, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Retardo de 1 segundo
  }
}
//Sección que se encarga de recibir
void TaskReceiver(void *pvParameters) {
  int receivedValue;
  for (;;) {
    // Recibir valor de la cola
    if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdPASS) {
      // Mostrar el valor recibido en el puerto serie
      Serial.print("Recepción: ");
      Serial.println(receivedValue);
    }
  }
}
