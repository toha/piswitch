/**
 * Copyright 2012-2013 Tobias Hartwich
 */
int pin = 8;

unsigned long network0 = (B00001011 << 8) | (B00000000 << 0);

// start      network          payload     CRC
//   1      101100000000        110100      1

// payload:
// lamp address    Broadcast     On/Off    Dimmer     Immer 0
//      11             1            1         1          0

void setup() {
  pinMode(pin, OUTPUT);
  Serial.begin(9600);

  digitalWrite(pin, LOW);
  delay(500);

}


void sendSignal(unsigned long signal) {

  Serial.print("Signal: ");
  Serial.println(signal, BIN);
  for (int j=0; j<4; j++) {

    for (long i=20; i>=0; i--) {
      int oneBit = (signal & (1L << i)) >> i;

      if (oneBit == 1) {
        send1();
      }
      else {
        send0();
      }
    }

    // Abstand der 4 Frames
    delay(85);
  }

  // Nach dem ganzen Signal nochmal 100ms warten bis zum nächsten 
  // Befehl um den Empfänger nicht zu verwirren 
  delay(100);
}

void switchOnOff(unsigned long network, long button, long on) {
  unsigned long startbit = 1;
  unsigned long payload = 0;
  payload = (button << 4) | (on << 2);


  unsigned long checksum = calculateChecksum(payload);
  unsigned long signal = 0;
  signal = (startbit << 20) |(network << 8) | (payload << 2) | (checksum << 0);

  sendSignal(signal);

}

unsigned long calculateChecksum(unsigned long payload) {
  unsigned long button = (payload & B00110000) >> 4;
  unsigned long broadcastAndOnOff = (payload & B00001100) >> 2;
  unsigned long dimmerAndMystery = payload & B00000011;

  unsigned long cs = (button ^ broadcastAndOnOff) ^ dimmerAndMystery;


  Serial.print("checksum: ");
  Serial.println(String(cs, BIN));  
  return cs; 

}

void send0() {

  delayMicroseconds(655);

  digitalWrite(pin, HIGH);
  delayMicroseconds(1355);
  digitalWrite(pin, LOW);

}

void send1() {

  delayMicroseconds(1355);

  digitalWrite(pin, HIGH);
  delayMicroseconds(655);
  digitalWrite(pin, LOW);

}

void readCommand() {
  // Prüfen wieviele Bytes da sind
  if (Serial.available() >= 7) {

    // Erstes Zeichen prüfen
    if (Serial.read() != 's') {
      // Wenn kein Befehl anfängt, nichts machen
      Serial.println("error: invalid command start");
      return;
    }

    Serial.println("----------------------------");

    // Netzwerk-ID lesen (4 Stellen)
    unsigned long netid = 0;
    for (int i=4; i>0; i--) {
      unsigned long digit = Serial.read() - '0';
      // Befehl abbrechen, wenns keine zahl ist
      if (digit < 0 || digit > 9) {
        Serial.println("error: no number");
        return;
      }
      for (int j=i-1; j>0; j--) {
        digit *= 10;  
      }
      netid += digit;
    }

    if (netid < 0 || netid > 4096) {
      Serial.println("error: invalid netid");
      return;
    }

    // Lampen Id lesen (eine Stelle)
    unsigned long lampid = Serial.read() - '0'; 
    if (lampid < 0 || lampid > 3) {
      Serial.println("error: invalid lampid");
      return;
    }   

    // An/Aus lesen (eine Stelle)
    unsigned long onoff = Serial.read() - '0'; 
    if (onoff < 0 || onoff > 1) {
      Serial.println("error: invalid onoff");
      return;
    }       



    Serial.print("netid: ");
    Serial.print(netid, DEC);        
    Serial.print(" ");
    Serial.println(netid, BIN);

    Serial.print("lampid: ");
    Serial.println(lampid, DEC);   
    Serial.print("onoff: ");
    Serial.println(onoff, DEC);       

    switchOnOff(netid, lampid, onoff);

  }
}


void loop()
{
  readCommand();
  delay(5);
}
