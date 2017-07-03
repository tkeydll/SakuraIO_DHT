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

  if(sakuraio.enqueueTx(0,t) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(1,h) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  if(sakuraio.enqueueTx(2,cnt) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  sakuraio.send();
  delay(60000);

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
