// Analog input test for Teensy 3.0    Oct 4 2012 J.Beale
// Setup: https://picasaweb.google.com/109928236040342205185/Electronics#5795546092126071650
//From dangerous prototpyes; http://dangerousprototypes.com/forum/viewtopic.php?t=4606


    #define VREF (3.266)         // ADC reference voltage (= power supply)
    #define VINPUT (2.171)       // ADC input voltage from resistive divider to VREF
    #define ADCMAX (65535)       // maximum possible reading from ADC
    #define EXPECTED (ADCMAX*(VINPUT/VREF))     // expected ADC reading
    #define SAMPLES (10000)      // how many samples to combine for pp, std.dev statistics

    const int analogInPin = 14;  // Analog input is AIN0 (Teensy3 pin 14, next to LED)
    const int LED1 = 13;         // output LED connected on Arduino digital pin 13
    const int clockPin = 0;
    const int flagPin = 1; //digital output pin for Oscilloscope to see. 
    const int b1 = 2;
    const int b2 = 3;
    const int b3 = 4;
    const int b4 = 5;

    int sensorValue = 0;        // value read from the ADC input
    int handThreshold = 40000; //Threshhold for triggering the second pulse.
    long oldT;
    int hand = 0;  //Location of the hand
    
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
      int lowNotes = 0;
      int highNotes = 0;
      int buttonNote = 0;
      int handLocation = 0;
      while(1){
        //Serial.println(analogRead(analogInPin));
        
        if(digitalRead(clockPin)){
          //Serial.println("ClockFound!");
          hand = findHand();
          //Serial.print("Hand found at ");
          if ((lowNotes != 0) && (highNotes != 0) && (hand != 0)) handLocation = (hand-lowNotes)/(highNotes-lowNotes);
        }
        if (digitalRead(b1) && (hand != 0)){
          lowNotes = hand;
          digitalWrite(LED, HIGH);
          delay(5);
          digitalWrite(LED,LOW);
        }
        if (digitalRead(b2) && (hand != 0)){
          highNotes = hand;
          digitalWrite(LED, HIGH);
          delay(5);
          digitalWrite(LED,LOW);
        }
        
        if ((lowNotes != 0) && (highNotes != 0) ){Serial.println(handLocation); }
        
      }
     
//     while (true) {}
    } // end main()  =====================================================

    int findHand(void){
        int x;
        int hand =0;
        int maximum;
        int ave;
        int QuicknessThreshold = 7500;
        
        for (int i=0;i<SAMPLES;i++) {
        x = analogRead(analogInPin);
//      Serial.println(x,0);
        if (x> maximum){maximum = x;}
        //Serial.println(x);
        if (x > handThreshold){ 
          digitalWrite(flagPin, HIGH); //For the oscilloscope to track when we find the hand. 
          delay(1);
          digitalWrite(flagPin, LOW);
          if (i>QuicknessThreshold) hand = i;
          }
        }
        //Serial.print("Ave of hand Samples: ");
        //Serial.println(ave);
        //Serial.print("Max of hs: ");
        //Serial.println(maximum);
        
        return hand;
    }
