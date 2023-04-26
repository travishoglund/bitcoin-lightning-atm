#include <Hash.h>
#include "qrcoded.h"
#include "Bitcoin.h"
#include <TFT_eSPI.h>
#include <JC_Button.h>
#define BTN1 39 // Screen tap button

// LNURLDevices ATM details - edit these as needed
String baseURLATM = "https://legend.lnbits.com/lnurldevice/api/v1/lnurl/DEVICE_ID";
String secretATM = "SECRET";
String currencyATM = "USD";
float coinValue = 0.01;
int charge = 10; // % you will charge people for service, set in LNbits extension
float maxAmount = 1.00; // max amount per withdraw

// Program Variables - do not change these
boolean coinInserted = false;
unsigned long lastInterrupt;
String qrData;
float total;

// LCD Display & Clicking Settings
TFT_eSPI tft = TFT_eSPI();
Button BTNA(BTN1);

void setup() {
  // Initialize LCD Screen
  BTNA.begin();
  tft.init();
  tft.setRotation(2);
  tft.invertDisplay(false);
  showATMStartScreen();

  // Listen for coin insertions on Pin 2, and track latest interrupt
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), coinInterrupt, CHANGE);
  lastInterrupt = millis();
}

void loop() {

  // Credit user when coin is inserted
  if(coinInserted) {
    coinInserted = false;
    lastInterrupt = millis();
    total = total + coinValue;
    Serial.println("coin detected!");

    // Update the UI to reflect the coin has been inserted
    showATMDepositScreen();
  }

  // When button is pushed, show the QR code for redemption
  BTNA.read();
  if((total > 0 && BTNA.wasReleased()) || total == maxAmount) {
    makeLNURL();
    showATMRedemptionScreen("SCAN, THEN TAP WHEN FINISHED");
  }

}

/*
 * Triggered when voltage changes on Pin 2
 */
void coinInterrupt() {
  if(millis() - lastInterrupt > 250) {
    coinInserted = true;
  }
}

/*
 * Prints out our basic UI
 */
void printMessage(String text1, String text2, String text3, int ftcolor, int bgcolor)
{
  tft.fillScreen(bgcolor);
  tft.setTextColor(ftcolor, bgcolor);
  tft.setTextSize(3);
  tft.setCursor(24, 30);
  tft.println(text1);
  tft.setCursor(24, 70);
  tft.println(text2);
  tft.setCursor(24, 420);
  tft.setTextSize(2);
  tft.println(text3);
}

/*
 * The first screen for our Bitcoin ATM - this welcomes the user
 */
void showATMStartScreen() {
  total = 0;
  tft.fillScreen(TFT_BLACK);
  printMessage("Bitcoin ATM", String(charge) + "% charge", "TRADE PENNIES FOR SATS!", TFT_WHITE, TFT_BLACK);
}

/*
 * The second screen for our Bitcoin ATM - this shows the users deposited funds
 */
void showATMDepositScreen() {
  printMessage("Bitcoin ATM", "Total: " + String(total) + currencyATM, "TAP SCREEN WHEN FINISHED", TFT_WHITE, TFT_BLACK);
}

/*
 * The final screen shows our QR code once user deposits are finished, pre-signed to be redeemed on Lightning
 */
void showATMRedemptionScreen(String message)
{
  tft.fillScreen(TFT_WHITE);
  qrData.toUpperCase();
  const char *qrDataChar = qrData.c_str();
  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];
  qrcode_initText(&qrcoded, qrcodeData, 11, 0, qrDataChar);

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(40 + 4 * x, 40 + 4 * y, 4, 4, TFT_BLACK);
      }
      else
      {
        tft.fillRect(40 + 4 * x, 40 + 4 * y, 4, 4, TFT_WHITE);
      }
    }
  }

  tft.setCursor(60, 420);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.println(message);

  bool waitForTap = true;
  while(waitForTap){
    BTNA.read();
    if (BTNA.wasReleased()) {
      waitForTap = false;
      showATMStartScreen();
    }
  }
}

////////////////////////////////////////////
///////////////LNURL STUFF//////////////////
////USING STEPAN SNIGREVS GREAT CRYTPO//////
////////////THANK YOU STEPAN////////////////
////////////////////////////////////////////

void to_upper(char *arr)
{
  for (size_t i = 0; i < strlen(arr); i++)
  {
    if (arr[i] >= 'a' && arr[i] <= 'z')
    {
      arr[i] = arr[i] - 'a' + 'A';
    }
  }
}

void makeLNURL()
{
  int randomPin = random(1000, 9999);
  byte nonce[8];
  for (int i = 0; i < 8; i++)
  {
    nonce[i] = random(256);
  }

  byte payload[51]; // 51 bytes is max one can get with xor-encryption

    size_t payload_len = xor_encrypt(payload, sizeof(payload), (uint8_t *)secretATM.c_str(), secretATM.length(), nonce, sizeof(nonce), randomPin, float(total * 100));
    String preparedURL = baseURLATM + "?atm=1&p=";
    preparedURL += toBase64(payload, payload_len, BASE64_URLSAFE | BASE64_NOPADDING);

  Serial.println(preparedURL);
  char Buf[200];
  preparedURL.toCharArray(Buf, 200);
  char *url = Buf;
  byte *data = (byte *)calloc(strlen(url) * 2, sizeof(byte));
  size_t len = 0;
  int res = convert_bits(data, &len, 5, (byte *) url, strlen(url), 8, 1);
  char *charLnurl = (char *) calloc(strlen(url) * 2, sizeof(byte));
  bech32_encode(charLnurl, "lnurl", data, len);
  to_upper(charLnurl);
  qrData = charLnurl;
  Serial.println(charLnurl);
}

int xor_encrypt(uint8_t *output, size_t outlen, uint8_t *key, size_t keylen, uint8_t *nonce, size_t nonce_len, uint64_t pin, uint64_t amount_in_cents)
{
  // check we have space for all the data:
  // <variant_byte><len|nonce><len|payload:{pin}{amount}><hmac>
  if (outlen < 2 + nonce_len + 1 + lenVarInt(pin) + 1 + lenVarInt(amount_in_cents) + 8)
  {
    return 0;
  }

  int cur = 0;
  output[cur] = 1; // variant: XOR encryption
  cur++;

  // nonce_len | nonce
  output[cur] = nonce_len;
  cur++;
  memcpy(output + cur, nonce, nonce_len);
  cur += nonce_len;

  // payload, unxored first - <pin><currency byte><amount>
  int payload_len = lenVarInt(pin) + 1 + lenVarInt(amount_in_cents);
  output[cur] = (uint8_t)payload_len;
  cur++;
  uint8_t *payload = output + cur;                                 // pointer to the start of the payload
  cur += writeVarInt(pin, output + cur, outlen - cur);             // pin code
  cur += writeVarInt(amount_in_cents, output + cur, outlen - cur); // amount
  cur++;

  // xor it with round key
  uint8_t hmacresult[32];
  SHA256 h;
  h.beginHMAC(key, keylen);
  h.write((uint8_t *) "Round secret:", 13);
  h.write(nonce, nonce_len);
  h.endHMAC(hmacresult);
  for (int i = 0; i < payload_len; i++)
  {
    payload[i] = payload[i] ^ hmacresult[i];
  }

  // add hmac to authenticate
  h.beginHMAC(key, keylen);
  h.write((uint8_t *) "Data:", 5);
  h.write(output, cur);
  h.endHMAC(hmacresult);
  memcpy(output + cur, hmacresult, 8);
  cur += 8;

  // return number of bytes written to the output
  return cur;
}
