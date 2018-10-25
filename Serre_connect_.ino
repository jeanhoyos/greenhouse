#include <SimpleDHT.h>
#include <Servo.h>


// SETUP DATA
/*************/
SimpleDHT11 dht11;
int pinDHT11 = 2;   // humidity and temperature of ambient air
byte temperature = 0;
byte humidity = 0;
byte data[40] = {0};
/**************/
int sensor_pin = A0;    // Humidity sensor
int moist_value;
/**************/
int pumpPin= 3;       // Pump
/**************/
Servo myservo;

// CONTROL DATA

int moist_opt = 50;     // moist
int moist_delta = 0;
int moist_max = 80;








void DHT11_data(){
    if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
      return;
    }
   Serial.print("Temperature:"); Serial.print((int)temperature); Serial.print(" *C, ");
   Serial.print("Humidity:"); Serial.print((int)humidity); Serial.println(" %");  
  }

void moistur(){
  // Revoir la valeur en %
   moist_value= analogRead(sensor_pin);
   moist_value = map(moist_value,550,0,0,100);//*2;

//   if (moist_value > 100){
//      moist_value = 100;
//    }
//   else if (moist_value < 0){
//      moist_value = 0;
//    }
   Serial.print("Moisture : "); Serial.print(moist_value); Serial.println("%");
}

void open_motorPump(){  
    int speed = 255;
    analogWrite(pumpPin, speed);
  }

void close_motorPump(){  
    int speed = 0;
    analogWrite(pumpPin, speed);
  }



void start_fan(){
  /*
   * Should add a timer or fan until out of limit
   */
    Serial.println("Tkt ya un ventilo qui marche");
  }  

void open_window(){
  // at servo
    move_to_pos(90);
    Serial.println("Window openned");
  }

void close_window(){
  // at servo
    move_to_pos(0);
    Serial.println("Window closed");
  }


void feed_water(){
   open_motorPump();
   delay(3000);
   close_motorPump();
  }
//=======================================================================================

void moist_supervisor(){
    if (moist_value < (moist_opt - moist_delta) ){
      Serial.println("a");
      feed_water();
    }
    else if (moist_value > moist_max){
      Serial.println("b");  
      open_window();
      start_fan();  
      close_window();
    }
    else{
      Serial.println("No need to feed water man nor wetness");      
    }
  }

void move_to_pos(int pos){
  int current_pos = myservo.read();
  Serial.println(current_pos);
  if (pos < current_pos){
      for (int position = current_pos; position >= pos; position--) {
        myservo.write(position);
        delay(15);
      }    
  }
  else{
       for (int position = current_pos; position <= pos; position++) {
        myservo.write(position);
        delay(15);
      }  
    }
  }
  

/********************************/
  

void setup() {
   Serial.begin(9600);
   pinMode(pumpPin, OUTPUT);
   myservo.attach(9);
}


/*********************************/

void loop() {

   DHT11_data();
   moistur();
   moist_supervisor();
   Serial.print("TESTTTTTT");
   



   delay(1000);
}
