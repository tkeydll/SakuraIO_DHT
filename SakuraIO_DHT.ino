#include <SakuraIO.h>
#include <DHT.h>

SakuraIO_I2C sakuraio;

// DHT
#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

uint32_t cnt = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  dht.begin();
  Serial.println("");
}

void loop() {
  cnt++;
  Serial.println(cnt);

  float t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(t);

  float h = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(h);

  // Sound
  //analogRead(0);
  //delay(10);
  uint32_t vol = analogRead(2);
  Serial.print("Volume: ");
  Serial.println(vol);

  uint32_t yubi = analogRead(3);
  Serial.print("Yubi: ");
  Serial.println(yubi);

  if(sakuraio.enqueueTx(0,cnt) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(1,t) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(2,h) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(3,vol) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(4,yubi) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }

  sakuraio.send();
  delay(10000);

  uint8_t available;
  uint8_t queued;
  if(sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE){
    Serial.println("[ERR] get tx queue length error");
  }

  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);
}
