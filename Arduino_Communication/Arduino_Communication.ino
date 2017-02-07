String incoming_data;

void setup() {
  pinMode(30, OUTPUT);
  pinMode(40, OUTPUT);
  Serial.begin(9600,SERIAL_8N1 );
  //Serial1.begin(9600,SERIAL_8N1 );
}

//-----------------------------------------------------------------------------------------------------------------
void loop() {
  Serial.print(incoming_data);
  
  if       (incoming_data.equals("FUNCTION1ON\r\n")){
    Serial.print("func1off");
    //if (bitRead(PORTD,30) == LOW){
      digitalWrite(30, HIGH);
    //}
  }
  else if (incoming_data.equals("FUNCTION1OFF\r\n")){
    Serial.print("func1off");
   // if (bitRead(PORTD,30) == HIGH){
      digitalWrite(30, LOW);
       Serial.print("JO");
    //}
  }
  else if (incoming_data.equals("FUNCTION2ON\r\n")){
    Serial.print("func2on");
   // if (bitRead(PORTD,40) == LOW){
      digitalWrite(40, HIGH);
 //   }
  }
  else if (incoming_data.equals("FUNCTION2OFF\r\n")){
    Serial.print("func2off");
   // if (bitRead(PORTD,40) == HIGH){
      digitalWrite(40, LOW);
 //   }
  }
}

//-----------------------------------------------------------------------------------------------------------------
void serialEvent() {
  incoming_data = Serial.readString();
}



