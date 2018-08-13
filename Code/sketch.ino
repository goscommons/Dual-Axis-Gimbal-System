#include<Stepper.h>

int steps = 2048; // steps in 1 revolution, including gear ratio

float val = 0;
float val2=0;
float x_scale = 0;
float val_scale;
float y_scale=0;
int previous = 0;
int previousx = 0;
int previousy = 0;
int z=0;
float angle = 0;
float anglex;
float angley;
int mode=0;
int attempt=0;
int lastx;
int lasty;
float s1;
float s1x;
float s2x;
float s1y;
float s2y;
float s2;
int t = 0;
// initialize the stepper library on pins 10-13 
Stepper myStepper1(steps/64,10,12,11,13); 
Stepper myStepper2(steps/64,8,5,6,4);     

void setup() 
{
  Serial.begin(9600); // initialize serial port
  while(mode!=1&&mode!=2)
  {
    Serial.flush();
    Serial.print("Select the operation mode by entering appropriate single digit number: \n 1. Move to commanded angle or \n 2. Trace between zero degrees to given points \n Your Input: ");
    while(Serial.available()==0);
    mode = Serial.parseInt();
    Serial.print(mode);
    if(mode!=1&&mode!=2)
    {  
      Serial.println("\nInvalid input, please try again...");
    }
  }
}

void loop() 
{

  if (mode==1||mode==2)
  {
     myStepper1.setSpeed(200); // setting stepper speed
     myStepper2.setSpeed(200); // setting stepper speed
     val_scale=0;
     if (mode==1)
      {
        Serial.println("\nX-Axis: Please enter the target angle in degrees (0 to 360): ");
        while(Serial.available()==0);
        val = Serial.parseFloat(); // read Serial input value
        val_scale = map(val,0,360,0,steps);
        s1= 0.17578125 * val_scale;
        s2= 0.17578125 * (val_scale + 1);
        Serial.print(s1);
        Serial.print("\t");
        Serial.println(s2);
        if((s2-val)<(val-s1))
        {
          val_scale=val_scale+1;
        }
        myStepper2.step(val_scale-previousx); // steps the motor takes with direction
        Serial.print("Steps moved in this cycle: ");
        Serial.println(val_scale-previousx);
        previousx = val_scale; 
        angle = 0.17578125 * previousx;  
        Serial.print("X-Axis: Angle achieved: ");
        Serial.println(angle);

        val_scale=0;
        Serial.println("\nY-Axis: Please enter the target angle in degrees (0 to 360): ");
        while(Serial.available()==0);
        val2 = Serial.parseFloat(); // read Serial input value
        val_scale = map(val2,0,360,0,steps);
        s1= 0.17578125 * val_scale;
        s2= 0.17578125 * (val_scale + 1);
        Serial.print(s1);
        Serial.print("\t");
        Serial.println(s2);
        if((s2-val2)<(val2-s1))
        {
          val_scale=val_scale+1;
        }
        myStepper1.step(val_scale-previousy); // steps the motor takes with direction
        Serial.print("Y-Axis: Steps moved in this cycle: ");
        Serial.println(val_scale-previousy);
        previousy = val_scale; 
        angle = 0.17578125 * previousy;  
        Serial.print("Y-Axis: Angle achieved: ");
        Serial.println(angle);
      }
       if (mode == 2)
      {
        Serial.println("\nX-Axis: Please enter the target-end angle in degrees (0 to 360): ");
        while(Serial.available()==0);
        lastx = Serial.parseFloat(); // read Serial input value
        //lastx_scale = map(lastx,0,360,0,steps);
        //Serial.print("Steps moved in this cycle: "+x_scale-previousx);
             
        Serial.println("\nY-Axis: Please enter the target-end angle in degrees (0 to 360): ");
        while(Serial.available()==0);
        lasty = Serial.parseFloat(); // read Serial input value
        //lasty_scale = map(lasty,0,360,0,steps);

        Serial.println("\nPlease enter the amount of delay between each reading: ");
        while(Serial.available()==0);
        t = Serial.parseInt(); // read Serial input value

        //Tracing the interim angles:
        
        for (int x = 0; x <= lastx; x=x+10)
        {
          x_scale= map(x,0,360,0,steps);
          s1x= 0.17578125 * x_scale;
          s2x= 0.17578125 * (x_scale + 1);
          if((s2x-x)<(x-s1x))
          {
            x_scale=x_scale+1;
          }
          myStepper2.step(x_scale-previousx); // steps the motor takes with direction
          for (int y=0; y <= lasty; y=y+10)
          {
            z=z+1;
            y_scale= map(y,0,360,0,steps);
            s1y= 0.17578125 * y_scale;
            s2y= 0.17578125 * (y_scale + 1);
            if((s2y-y)<(y-s1y))
            {
              y_scale=y_scale+1;
            }
            myStepper1.step(y_scale-previousy); // steps the motor takes with direction
            delay(1000+t);
            previousx = x_scale; 
            anglex = 0.17578125 * previousx; 
            previousy = y_scale; 
            angley = 0.17578125 * previousy;  
            Serial.print(z);
            Serial.print(": Angles achieved: ");
            Serial.print(anglex);
            Serial.print(", ");
            Serial.println(angley);
          }
         }
         Serial.print("-------End of operation!-------");
      }
  }
  delay(1000+t);
}
