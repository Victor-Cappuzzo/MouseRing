#include <BleMouse.h>

BleMouse bleMouse;
const int BUTTON_PIN_UP = 10; // Up Scroll
const int BUTTON_PIN_DOWN = 9;  // Down Scroll
const int BUTTON_PIN_LEFT = 8;  // Left Scroll
const int BUTTON_PIN_RIGHT = 7; // Right Scroll

void setup() {
  Serial.begin(115200);
  Serial.println("Waiting for device to connect...");

  // Set button pins as inputs with internal pull-down resistors enabled
  pinMode(BUTTON_PIN_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_LEFT, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_RIGHT, INPUT_PULLDOWN);

  bleMouse.begin();
}

void loop() {

  if (bleMouse.isConnected()) {

    Serial.println("Mouse Ring Connected!");

    // Read the state of each button
    int buttonState1 = digitalRead(BUTTON_PIN_UP);
    int buttonState2 = digitalRead(BUTTON_PIN_DOWN);
    int buttonState3 = digitalRead(BUTTON_PIN_LEFT);
    int buttonState4 = digitalRead(BUTTON_PIN_RIGHT);

    // Check if any button is pressed (HIGH indicates the button is pressed due to pull-down resistors)
    if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH || buttonState4 == HIGH) {

      if (buttonState1 == HIGH) {
        bleMouse.move(0,0,1); // Button 1 (up scroll) is pressed
        Serial.println("Up pressed");
      }
      //if (buttonState2 == HIGH) {
        //bleMouse.move(0,0,-1); // Button 2 (down scroll) is pressed
        //Serial.println("Down pressed");
      //}
      if (buttonState3 == HIGH) {
        bleMouse.move(0,0,0,-1); // Button 3 (left scroll) is pressed
        Serial.println("Left pressed");
      }
      //if (buttonState4 == HIGH) {
        //bleMouse.move(0,0,0,1); // Button 4 (right scroll) is pressed
        //Serial.println("Right pressed");
      //}

      delay(100); // Delay for stability
    }

  delay(200);

  }

  else {
    Serial.println("Waiting for device to connect...");
  }

}

/*

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t mouseState = 0; // Variable to store the mouse button states

const int BUTTON_PIN_1 = 10; // Up Scroll
const int BUTTON_PIN_2 = 9;  // Down Scroll
const int BUTTON_PIN_3 = 8;  // Left Scroll
const int BUTTON_PIN_4 = 7; // Right Scroll

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "bfb3d0c2-592f-4fab-aeca-43034e99e7ab"
#define CHARACTERISTIC_UUID "cc5756c6-3f99-4a2f-a593-0c68e24db724"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
        uint8_t buttons = 0; // Variable to store mouse button states

        // Read button states
        if (digitalRead(BUTTON_PIN_3) == LOW) {
            buttons |= 0x01; // Bit 0: Left button (Up)
        }
        if (digitalRead(BUTTON_PIN_4) == LOW) {
            buttons |= 0x02; // Bit 1: Right button (Down)
        }
        if (digitalRead(BUTTON_PIN_1) == LOW) {
            buttons |= 0x04; // Bit 2: Up button (Left)
        }
        if (digitalRead(BUTTON_PIN_2) == LOW) {
            buttons |= 0x08; // Bit 3: Down button (Right)
        }

        // Prepare the mouse report (4 bytes: x, y, wheel, buttons)
        uint8_t mouseReport[4] = {0, 0, 0, buttons};

        // Set the characteristic value to send the mouse report
        pCharacteristic->setValue(mouseReport, sizeof(mouseReport));
        pCharacteristic->notify();

        delay(10); // Delay for stability, adjust as needed based on your requirements
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }

    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
*/

/*
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLE2902.h>

const int BUTTON_PIN_1 = 10; // Up Scroll
const int BUTTON_PIN_2 = 9;  // Down Scroll
const int BUTTON_PIN_3 = 8;  // Left Scroll
const int BUTTON_PIN_4 = 7; // Right Scroll

BLEHIDDevice* hid;
BLECharacteristic* inputMouse;

bool buttonState1 = false;
bool buttonState2 = false;
bool buttonState3 = false;
bool buttonState4 = false;

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("ESP32 Mouse");
  hid = new BLEHIDDevice(BLEHIDDevice::HID_MOUSE);
  inputMouse = hid->inputReport(1); // <-- input report ID must be 1
  hid->manufacturer()->setValue("Manufacturer");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x01);

  // Set up buttons
  pinMode(BUTTON_PIN_1, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_2, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_3, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_4, INPUT_PULLDOWN);
}

void loop() {
  // Read button states
  bool newButtonState1 = digitalRead(BUTTON_PIN_1);
  bool newButtonState2 = digitalRead(BUTTON_PIN_2);
  bool newButtonState3 = digitalRead(BUTTON_PIN_3);
  bool newButtonState4 = digitalRead(BUTTON_PIN_4);

  // If button state changes, send mouse event
  if (newButtonState1 != buttonState1 || newButtonState2 != buttonState2 || newButtonState3 != buttonState3 || newButtonState4 != buttonState4) {
    buttonState1 = newButtonState1;
    buttonState2 = newButtonState2;
    buttonState3 = newButtonState3;
    buttonState4 = newButtonState4;

    // Prepare mouse report
    uint8_t mouseReport[4] = {0, 0, 0, 0}; // dx, dy, wheel, buttons

    if (buttonState1) {
      mouseReport[3] |= 0x01; // Button 1 (up scroll) is pressed
    }
    if (buttonState2) {
      mouseReport[3] |= 0x02; // Button 2 (down scroll) is pressed
    }
    if (buttonState3) {
      mouseReport[3] |= 0x04; // Button 3 (left scroll) is pressed
    }
    if (buttonState4) {
      mouseReport[3] |= 0x08; // Button 4 (right scroll) is pressed
    }

    // Send mouse report
    inputMouse->setValue(mouseReport, sizeof(mouseReport));
    inputMouse->notify();
    delay(10); // Delay for stability
  }

  // You can add additional code here if needed
  delay(20); // Adjust the delay according to your needs
}
*/