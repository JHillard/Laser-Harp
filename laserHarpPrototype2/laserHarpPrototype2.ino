//Laser Harp
//Jake Hillard Spring 2016

    #define VREF (3.266)         // ADC reference voltage (= power supply)
    #define VINPUT (2.171)       // ADC input voltage from resistive divider to VREF
    #define ADCMAX (65535)       // maximum possible reading from ADC
    #define EXPECTED (ADCMAX*(VINPUT/VREF))     // expected ADC reading
    #define SAMPLES (10000)      // how many samples to combine for pp, std.dev statistics

    const int handPin = 14;  // Analog input is AIN0 (Teensy3 pin 14, next to LED)
    const int LED1 = 13;         // output LED connected on Arduino digital pin 13
    const int clockPin = 0;
    const int flagPin = 1; //digital output pin for Oscilloscope to see. 
    const int b1 = 2;
    const int b2 = 3;
    const int b3 = 4;
    const int b4 = 5;

    int lightThreshold = 40000; //Threshhold for triggering the second pulse.
    int lowNotes = 0;
    int highNotes = 0;
    double buttonNote = 0;
    double noteRatio = 0;
    int analogVal = 0;
    unsigned long curTime = 0;
    unsigned long handTime = 0;
    unsigned long lastTime = 0;
    bool clockHigh = 0; //Prevent clock thinking a full cycle has passed.



    
    void setup() {    // ==============================================================
      pinMode(LED1,OUTPUT);       // enable digital output for turning on LED indicator
      pinMode(clockPin, INPUT_PULLDOWN);
      pinMode(b1, INPUT_PULLUP);
      pinMode(b2, INPUT_PULLUP);
      pinMode(b3, INPUT_PULLUP);
      pinMode(b4, INPUT_PULLUP);
      
      pinMode(flagPin, OUTPUT);
      analogReference(INTERNAL);  // set analog reference to internal ref
      analogReadRes(16);          // Teensy 3.0: set ADC resolution to this many bits
     
      Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
      digitalWrite(LED1,HIGH);   delay(1000);   // LED on for 1 second
      digitalWrite(LED1,LOW);    delay(3000);   // wait for slow human to get serial capture running
     
      Serial.println("# Clock Search running... ");
      
    } // ==== end setup() ===========

    void loop() {  // ================================================================ 
      
        if(digitalRead(clockPin)){
          if (!clockHigh){
            lastTime = curTime;
            curTime = micros();
            clockHigh = 1; 
          }
        }else{clockHigh = 0;}
        
        analogVal = analogRead(handPin);
        if (analogVal > lightThreshold){
          digitalWrite(flagPin, HIGH); //For the oscilloscope to track when we find the hand. 
          delay(1);
          digitalWrite(flagPin, LOW);
          handTime = micros();
        }

        if (handTime > curTime){
          noteRatio = double(handTime-curTime)/double(curTime-lastTime);
          Serial.println(noteRatio*100);
          
        }else{
          noteRatio = 0;
        }

        
        if(!digitalRead(b1)){
          digitalWrite(LED1, HIGH);
          delay(5);
          digitalWrite(LED1,LOW);
        }
        if (!digitalRead(b2)){
          digitalWrite(LED1, HIGH);
          delay(5);
          digitalWrite(LED1,LOW);
        }
        
        
      }

