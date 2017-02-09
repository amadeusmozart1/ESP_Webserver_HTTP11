String incoming_data;

void setup() {
  pinMode(30, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(23, OUTPUT);
  delay(1);
  digitalWrite(23, LOW);
  Serial.begin(9600,SERIAL_8N1 );
  Serial1.begin(9600,SERIAL_8N1 );
}

//-----------------------------------------------------------------------------------------------------------------
void loop() {
  
  if       (incoming_data.equals("FUNCTION1ON\r\n")){
      digitalWrite(30, HIGH);
      digitalWrite(23, HIGH);
      delay(50);
      digitalWrite(23, LOW);
      incoming_data = "";
  }
  else if (incoming_data.equals("FUNCTION1OFF\r\n")){
      digitalWrite(30, LOW);
      digitalWrite(23, HIGH);
      delay(50);
      digitalWrite(23, LOW);
      incoming_data = "";
  }
  else if (incoming_data.equals("FUNCTION2ON\r\n")){
      digitalWrite(40, HIGH);
      digitalWrite(23, HIGH);
      delay(50);
      digitalWrite(23, LOW);
      incoming_data = "";
  }
  else if (incoming_data.equals("FUNCTION2OFF\r\n")){
      digitalWrite(40, LOW);
      digitalWrite(23, HIGH);
      delay(50);
      digitalWrite(23, LOW);
      incoming_data = "";
  }
  
}

//-----------------------------------------------------------------------------------------------------------------
void serialEvent1() {
  incoming_data = Serial1.readString();
  Serial.print(incoming_data);
}



