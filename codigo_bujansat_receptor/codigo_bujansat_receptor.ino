String cadena;
void setup(){
Serial.begin(9600); // Establecemos la velocidad del puerto serie (Igual que APC220)
Serial1.begin(9600); // Establecemos la velocidad del puerto serie (Igual que APC220)
}
void loop(){
if (Serial1.available()){
cadena = String("");
while (Serial1.available()){
cadena = cadena + char(Serial1.read());
}
}else{
//Serial.println("No disponible");
delay(4000);
}
Serial.println(cadena);
}
