String incoming_data;

void setup() {
  pinMode(30, OUTPUT);
  pinMode(40, OUTPUT);
  Serial.begin(9600,SERIAL_8N1 );
  Serial1.begin(9600,SERIAL_8N1 );
}

//-----------------------------------------------------------------------------------------------------------------
void loop() {
  Serial.print(incoming_data);
  
  if       (incoming_data.equals("FUNCTION1ON\r\n")){
      digitalWrite(30, HIGH);
  }
  else if (incoming_data.equals("FUNCTION1OFF\r\n")){
      digitalWrite(30, LOW);
  }
  else if (incoming_data.equals("FUNCTION2ON\r\n")){
      digitalWrite(40, HIGH);
  }
  else if (incoming_data.equals("FUNCTION2OFF\r\n")){
      digitalWrite(40, LOW);
  }
}

//-----------------------------------------------------------------------------------------------------------------
void serialEvent1() {
  incoming_data = Serial1.readString();
}



