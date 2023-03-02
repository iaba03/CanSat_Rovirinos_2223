//Librerías para el manejo del GPS
#include <SoftwareSerial.h>
#include <TinyGPS.h>

//Librería lectura/escritura tarjeta microSD
#include <SD.h>

//Librería sensor BME280
#include <Adafruit_BME280.h>

//Variables utilizadas para el control del GPS
TinyGPS gps;
SoftwareSerial softSerial(4, 3); //RXD a pin D4; TXD a pin D3

//Variables utilizadas para el control del módulo BME280
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
Colaboradores de organización:
33
float altitude;

void setup() {
Serial.begin(9600); //Abrimos el puerto serie y establecemos la velocidad de datos en 9600 bps
Serial1.begin(9600);//Abrimos el puerto serie (para transmitir a través de RF) y establecemos la velocidad de datos en 9600 bps
Serial.println(F("Inicio Rovirinos"));//Iniciamos el programa

//Comprobamos si el sensor BME280 está disponible
if (!bme.begin(0x76)) {
Serial.println(F("No se encuentra el sensor BME280!"));
while (1) delay(10);
}
//Recuperamos las variables proporcionadas por el sensor
bme_temp->printSensorDetails();
Colaboradores de organización:
34
bme_pressure->printSensorDetails();
bme_humidity->printSensorDetails();

void loop() {
//Recuperamos la información del sensor BME280 (temperatura, presión y humedad)
sensors_event_t temp_event, pressure_event, humidity_event;
bme_temp->getEvent(&temp_event);
bme_pressure->getEvent(&pressure_event);
bme_humidity->getEvent(&humidity_event);
//Escribimos los datos a través del puerto serie
Serial.print(F("Temperatura = "));
Serial.print(temp_event.temperature);
Serial.println(" ºC");
Serial.print(F("Humedad = "));
Serial.print(humidity_event.relative_humidity);
Serial.println(" %");
Colaboradores de organización:
35
Serial.print(F("Presión = "));
Serial.print(pressure_event.pressure);
Serial.println(" hPa");
Serial.print(F("Altitud = "));
Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
Serial.println(" m");
//Calculamos la altitud en función de la presión directamente con la fórmula para verificar el resultado
altitude=(pow(10.0, (log10(((bme.readPressure()))/101325.0)/5.25588))-1.0)/-
0.0000225577;
Serial.print(F("Altitud con fórmula = "));
Serial.println(altitude);
Serial.println();//Añadimos un retorno de carro para separar la información
//Código de recuperación de datos del gps
bool newData = false;
unsigned long chars;
unsigned short sentences, failed;
// Intentar recibir secuencia durante un segundo
for (unsigned long start = millis(); millis() - start < 1000;)
{
while (softSerial.available())
{
char c = softSerial.read();
if (gps.encode(c)) // Nueva secuencia recibida
newData = true;
}
Colaboradores de organización:
36
}
if (newData)
{
float flat, flon;
unsigned long age;
gps.f_get_position(&flat, &flon, &age);
Serial.print("LAT=");
Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
Serial.print(" LON=");
Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
Serial.print(" SAT=");
Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 :
gps.satellites());
Serial.print(" PREC=");
Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
}
gps.stats(&chars, &sentences, &failed);
Serial.print(" CHARS=");
Serial.print(chars);
Serial.print(" SENTENCES=");
Serial.print(sentences);
Serial.print(" CSUM ERR=");
Serial.println(failed);

//Enviamos los datos de temperatura, humedad, presión y altitud a través del módulo
RF220 (Serial1)
Serial1.print("ROVIRINOS");
Serial1.print(';');
Serial1.print(millis());
Serial1.print(';');
Serial1.print(temp_event.temperature);//Temperatura
Serial1.print(';');
Serial1.print(humidity_event.relative_humidity);//Humedad
Serial1.print(';');
Serial1.print(pressure_event.pressure);//Presión
Serial1.print(';');
Serial1.print(bme.readAltitude(SEALEVELPRESSURE_HPA));//Altitud
Serial1.print(';');
Serial1.print('\r');

}
