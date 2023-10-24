#include <IRremote.hpp>

const int irReceiverPin = 4;
const int ledPin = 2;

#define GreenLedPin     2
#define BlueLedPin      23
#define WhiteLedPin     18

#define 	Zero		(0x19)
#define 	One			(0x45)
#define 	Two			(0x46)
#define 	Three		(0x47)
#define 	Four		(0x44)
#define 	Five		(0x40)
#define 	Six			(0x43)
#define 	Seven		(0x07)
#define 	Eight		(0x15)
#define 	Nine		(0x09)
#define 	UP			(0x18)
#define 	Down		(0x52)
#define 	Right		(0x5A)
#define 	Left		(0x08)
#define 	OK			(0x1C)

uint8_t GreenLedCommand = Two;
uint8_t WhiteLedCommand = Five;
uint8_t BlueLedCommand  = Eight;
uint8_t ReProgrammingCommand = OK;
bool buttonPressed = false;

void toggle(uint8_t pin) 
{
  auto state = digitalRead(pin);
  digitalWrite(pin, !state);
}

void ReProgrammingFunction()
{
    Serial.println("\nStarting Re Programming LEDs");

    Serial.println("Re Programming Green LED");
    buttonPressed = false;
    while (!buttonPressed) 
    {
        if(IrReceiver.decode()) 
        {
            GreenLedCommand = IrReceiver.decodedIRData.command;
            buttonPressed = true;
            delay(100);
            IrReceiver.resume();
        }
    }
    Serial.println("Green LED Re Programmed Succsefuly\n");

    Serial.println("Re Programming White LED");
    buttonPressed = false;
    while (!buttonPressed) 
    {
        if(IrReceiver.decode()) 
        {
            WhiteLedCommand = IrReceiver.decodedIRData.command;
            buttonPressed = true;
            delay(100);
            IrReceiver.resume();
        }
    }
    Serial.println("White LED Re Programmed Succsefuly\n");

    Serial.println("Re Programming Blue LED");
    buttonPressed = false;
    while (!buttonPressed) 
    {
        if(IrReceiver.decode()) 
        {
            BlueLedCommand = IrReceiver.decodedIRData.command;
            buttonPressed = true;
            delay(100);
            IrReceiver.resume();
        }
    }
    Serial.println("Blue LED Re Programmed Succsefuly\n");
}

void setup() 
{
    Serial.begin(9600);
    pinMode(GreenLedPin, OUTPUT);
    pinMode(BlueLedPin, OUTPUT);
    pinMode(WhiteLedPin, OUTPUT);
    IrReceiver.begin(irReceiverPin, ENABLE_LED_FEEDBACK);
}

void loop() 
{
    if (IrReceiver.decode()) 
    {
        uint16_t command = IrReceiver.decodedIRData.command;

        if (command == ReProgrammingCommand) 
        {
            IrReceiver.resume();
            ReProgrammingFunction();
        }
        
        
        else if (command == GreenLedCommand) toggle(GreenLedPin);
        else if (command == WhiteLedCommand) toggle(WhiteLedPin);
        else if (command == BlueLedCommand) toggle(BlueLedPin);

        delay(100);
        IrReceiver.resume();
    }
}