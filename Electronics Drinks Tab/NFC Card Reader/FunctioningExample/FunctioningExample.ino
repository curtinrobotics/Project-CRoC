
// Libraries
#include <Adafruit_PN532.h>

// Global Variables
Adafruit_PN532 nfc(2,3);

void setup()
{
  Serial.begin(9600);
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  nfc.SAMConfig();

}

boolean readBlock(uint8_t block, char *data, uint8_t *uid, uint8_t uidLength, uint8_t *key)
{
  boolean authd = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, key);
  if( authd )
  {
    boolean readSuccess = nfc.mifareclassic_ReadDataBlock(block, data);
    if( readSuccess )
    {
      return true;
    }
  }
  return false;
}

void loop()
{
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  char data[16];                         // Array to store block data during reads

  // Keyb on NDEF and Mifare Classic should be the same
  uint8_t blockkey[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  uint8_t block = 56;

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success)
  {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    success = readBlock(block, data, uid, uidLength, blockkey);
    if( success )
    {
      char* userId = (data);
      Serial.println(userId);
      //flash(5, 255, 0, 0);
      //snake(4, 255, 0, 0);
      //christmas(4, 255, 0, 0, 0, 255, 255);
      //staffEffect(255, 255, 255);
    }
  }
  Serial.flush();
}
