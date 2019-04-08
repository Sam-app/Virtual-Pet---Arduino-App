  
  #include <Adafruit_RGBLCDShield.h>
  #include <utility/Adafruit_MCP23017.h>
  #include <EEPROM.h>
  #include <Time.h>
  
  Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
  
  unsigned long startTime4Hap;
  unsigned long startTime4Ful;
  
  uint8_t development = 0;
  uint8_t happiness = 2;
  uint8_t fullness = 3;
  uint8_t age = 0;

  //states of the App
  // 1 -> playing state
  // 2 -> Menu selected & puase pet
    uint8_t state;
  

  void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);  //start Serial in case we need to print debugging info
    //read the pet statics
       EEPROM.get(50,development);
       EEPROM.get(100,happiness);
       EEPROM.get(150 ,fullness);
       EEPROM.get(200, age);

     
    lcd.begin(16, 2);
    lcd.setBacklight(0x6);
    welcome();

    state = 1;
    
  }
  
  void loop() {
    // put your main code here, to run repeatedly:

    uint8_t buttons = lcd.readButtons();

    if(state == 1){
            updateLcd();
            if (age == 5) {
                development = 1;
                 startTime4Hap = millis();
                 startTime4Ful = millis();
              } 
 
              if (age >= 5) {
            every_7sec_update_happiness();
            everty_11sec_update_fullness();
              }
    
          //id dev stage >1 listen input
        if (development >= 1) {
          if (buttons & BUTTON_LEFT) {
            feedPet();
          }else if (buttons & BUTTON_RIGHT) {
            playPet();
          }else  if (buttons & BUTTON_UP) {
            growPet();
          }
          
      
        }
   delay(998);
   age += 1;

     // menu selected
    }else if(state == 2){
      if(buttons & BUTTON_UP){
        savePet();
        menuSelected();
      }else if(buttons & BUTTON_DOWN){
        deletePet();
        state = 1;
      }else if (buttons & BUTTON_RIGHT) {
            restart();
            state = 1;
       }else if (buttons & BUTTON_LEFT) {
            leave();
            lcd.clear();
            state = 1;
        }
      
    }


   if (buttons & BUTTON_SELECT) {
        state = 2;
        menuSelected();
        
      }
    
    
   
  }
  
  
  void every_7sec_update_happiness() {
  
    if (millis() - startTime4Hap >= 7000) {
      // if age atleast 5
      if (age >= 5) {
        if (happiness > 0) {
          happiness -= 1;
        }
        startTime4Hap = millis();
      }
    }
  } // end of updHap7Sec()
  
  
  // every 11 sec update fullness
  void everty_11sec_update_fullness() {
    if (millis() - startTime4Ful > 11000) {
      if (fullness > 0) {
        fullness -= 1;
      }
      //
      if (fullness == 0) {
        happiness = 0;
      }
      startTime4Ful = millis();//
    }
  }// end of everty_11sec_update_fullness(
  
  
  //Eventhandlers 
  // BUTTON_lEFT handler -feedPet
  void feedPet() {
    if (fullness < 3) {
      fullness += 1;
      startTime4Ful = millis();//
    }else if (fullness == 3) {
      fullness += 1;
      happiness = 0;
      startTime4Ful = millis();//
    }
  
  }
  //BUTTON_RIGHT pressed handler - playPet
  void playPet() {
    if ((happiness < 2) && (fullness >= 2)) {
      happiness += 1;
      startTime4Hap = millis();
    }
  }
  
  // //BUTTON_up pressed handler - growPet
  void growPet(){
    if((age>35)&&(happiness >= 1)&& fullness >= 3){
      development = 2;
    }
  }
  
  
  // update the lcd
  void updateLcd() {
    
    uint8_t minutes = age/60;
    uint8_t sec = age%60;
    lcd.setCursor(0, 0);
    lcd.print("Dev:");
    lcd.setCursor(4, 0);
    lcd.print(development);
  
    // Pet happiness
    lcd.setCursor(7, 0);
    lcd.print("Hap:");
    lcd.print(happiness);

// pet fullness 
    lcd.setCursor(0, 1);
    lcd.print("Ful:");

    lcd.print(fullness);
  
    lcd.setCursor(7, 1);
    lcd.print("Age:");

    if(minutes<10){
      lcd.print(0);
    }
    lcd.print(minutes);
    
    lcd.print(":");
    
    if(sec<10){
      lcd.print(0);
    }
    lcd.print(sec);
  
    
  }

   void menuSelected(){

//    state = 2;
//    for(int i = 8; i>0; i-=2){
//    lcd.clear(); 
//    lcd.setCursor(i,0);
//    lcd.print(" Main Menu");
//    lcd.setCursor(i,1);
//    lcd.print("------------");
//    delay(500);   
//     }
//     for(int i = -0; i<5; i+=1){
//    lcd.clear(); 
//    lcd.setCursor(i,0);
//    lcd.print(" Main Menu");
//    lcd.setCursor(i,1);
//    lcd.print("------------");
//    delay(400);   
//     }
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Main Menu");
    lcd.setCursor(5,1);
    lcd.print("-----------");  
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start:R");
    lcd.setCursor(8,0);
    lcd.print("Leave:L");
    lcd.setCursor(0,1);
    lcd.print("Save:U");
    lcd.setCursor(8,1);
    lcd.print("Delete:D");
  }


  void savePet(){
    EEPROM.put(50,development);
    EEPROM.put(100,happiness);
    EEPROM.put(150,fullness);
    EEPROM.put(200,age);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pet Saved");
    delay(1000);
  }

  void deletePet(){
    EEPROM.put(50,0);
    EEPROM.put(100,2);
    EEPROM.put(150,3);
    EEPROM.put(200,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pet Deleted");
    delay(500);
    lcd.clear();

     EEPROM.get(50,development);
     EEPROM.get(100,happiness);
     EEPROM.get(150 ,fullness);
     EEPROM.get(200, age);
    
  }

  void restart(){
    deletePet();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pet Restarted");
    delay(1000);
    lcd.clear();
    state = 1;
     EEPROM.get(50,development);
     EEPROM.get(100,happiness);
     EEPROM.get(150 ,fullness);
     EEPROM.get(200, age);
  }

  void leave(){  
    state = 1;
  }

  void welcome(){
    for(int i = 17; i>-2; i-=2){   
      lcd.clear();
      lcd.setCursor(i,0);
      lcd.print("Virtual Pet ...");
      delay(350);
      
    }
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Starting...");
    delay(1000);
    lcd.clear();
  }
  
  
  
  

