#include <EtherCard.h>

static byte mymac[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F };
static byte dstip[] = {192, 168, 81, 1};
byte Ethernet::buffer[700];
static uint32_t timer = 0;

const int dstPort PROGMEM = 8080;

const int srcPort PROGMEM = 4321;

void setup () {
  Serial.begin(9600);

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

}

char textToSend[] = "test 123";

void loop () {
    if (millis() > timer) {
      Serial.print("timer: ");
      Serial.println(timer);
      timer = millis() + 5000;
     //static void sendUdp (char *data,uint8_t len,uint16_t sport, uint8_t *dip, uint16_t dport);
     ether.sendUdp(textToSend, sizeof(textToSend), srcPort, dstip, dstPort );
  }
}
