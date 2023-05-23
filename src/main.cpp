#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int LED = 2;

const char *ssid = "CLARO-B612-9B21";
const char *pass = "3nDRaqQDFD";

// Credenciales MQTT
const char *brokerUser = "dev3";
const char *brokerPass = "pass3";
const char *broker = "192.168.1.109";
const char* mqttTopic = "testb/Luz_Patio1";

// const char *broker = "192.168.0.110";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // Convierte el payload en una cadena de caracteres
  char receivedMessage[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    receivedMessage[i] = (char)payload[i];
  }
  receivedMessage[length] = '\0';

  // Verifica el parámetro reemplazado en el tópico
  String topicString = String(topic);
  int parameterIndex = topicString.indexOf('/') + 1;
  String parameter = topicString.substring(parameterIndex);
  // Ejecuta diferentes acciones según el parámetro
  if (parameter == "Luz_Patio1") {
      if (strcmp(receivedMessage, "1") == 0) {
      digitalWrite(LED, LOW);  // Enciende el LED
      Serial.println("Encendiendo el LED");
      } else if (strcmp(receivedMessage, "0") == 0) {
      digitalWrite(LED, HIGH);  // Apaga el LED
      Serial.println("Apagando el LED");
      } else {
      Serial.println("Mensaje no reconocido");
      }

    // Acción para el parámetro 1
    Serial.println("Ejecutando acción para el parámetro 1");
  // } else if (parameter == "parametro2") {
  //   // Acción para el parámetro 2
  //   Serial.println("Ejecutando acción para el parámetro 2");
  // } else if (parameter == "parametro3") {
  //   // Acción para el parámetro 3
  //   Serial.println("Ejecutando acción para el parámetro 3");
  } else {
      

    Serial.println("Parámetro no reconocido");
  }

  // Realiza cualquier otra acción necesaria con el mensaje recibido
}
void setupWifi()
{
  delay(100);
  Serial.println("\nConectando a");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("-");
  }
  Serial.print("\nConectado a");
  Serial.println(ssid);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("\nConectando a");
    Serial.print(broker);
    if (client.connect("Patio", brokerUser, brokerPass))
    {
      Serial.print("\nConectado a");
      Serial.print(broker);
      
      
      client.subscribe(mqttTopic);  // Suscribirse al tópico especificado
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);

    }
    
}

}

void setup() {
  setupWifi();
  client.setServer(broker, 1883); 
  client.setCallback(callback);

  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop() {
     if (!client.connected())
  {
    reconnect();
  }
 
  client.loop();
}

