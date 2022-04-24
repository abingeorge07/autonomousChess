//Max distance in the x direction is 16
//Max distance in the y is 15

#define cell_size 48

#define steps_temp 200
#define cell_d 1
#define y_dist_max 400
#define x_dist_max 381

#define d 50
#define d_x 40
#define d_extruder 100
#define d_mov 800

#define step_extruder 3000
#define step_extruder_initial 3000

#define X1_STEP_PIN         54
#define X1_DIR_PIN          55
#define X1_ENABLE_PIN       38
#define X1_CS_PIN           53

#define X2_STEP_PIN         60
#define X2_DIR_PIN          61
#define X2_ENABLE_PIN       56
#define X2_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#define E0_CS_PIN          42

int data_e0;
int extruder_flag=0;


int len;
char in;

int X_pos=1;
int Y_pos=1;

void deadMotion()
{
  double dist_x= 8.5-X_pos;
  double dist_y= 5.5- Y_pos;

  X_mov(0.5*cell_size);
  delay(d_mov);

 if(dist_y<0)
 {
  Y_mov(-0.5*cell_size);
  delay(d_mov);
  X_mov((dist_x-0.5)*cell_size);
  delay(d_mov);
  Y_mov((dist_y+0.5)*cell_size);
  delay(d_mov);
 }
 else
 {
  Y_mov(0.5*cell_size);
  delay(d_mov);
  X_mov((dist_x-0.5)*cell_size);
  delay(d_mov);
  Y_mov((dist_y-0.5)*cell_size);
  delay(d_mov);
 }

if(extruder_flag==1)
{
  extruder_down();
  extruder_flag=0;
}

  X_mov((-0.5)*cell_size);
  delay(d_mov);
  Y_mov((-0.5)*cell_size);
  delay(d_mov);
  
}

void robot_motionPlanning(int x_temp, int y_temp)
{
  double dist_x= x_temp-X_pos;
  double dist_y= y_temp- Y_pos;

  if(extruder_flag==0)
  {
    X_mov(dist_x*cell_size);
    delay(d_mov);
    Y_mov(dist_y*cell_size);
    delay(d_mov);

    X_pos= x_temp;
    Y_pos= y_temp;
  }
else
{
  if(x_temp==9)
  {
    
    deadMotion();

    X_pos=8;
    Y_pos=5;
  }

  else if(dist_x==0)
  {
    X_mov(-0.5*cell_size);
    delay(d_mov);

    Y_mov(dist_y*cell_size);
    delay(d_mov);

    X_mov(0.5*cell_size);
    delay(d_mov);

      X_pos= x_temp;
      Y_pos= y_temp;
  }

  else if(dist_y==0)
  {
    Y_mov(-0.5*cell_size);
    delay(d_mov);

    X_mov(dist_x*cell_size);
    delay(d_mov);

    Y_mov(0.5*cell_size);
    delay(d_mov);
      X_pos= x_temp;
      Y_pos= y_temp;
  }
  else
  {
    if(dist_x<0)
    {
      X_mov(-0.5*cell_size);
      dist_x= dist_x+0.5;

        if(dist_y<0)
      {
        Y_mov(-0.5*cell_size);
        delay(d_mov);
        dist_y= dist_y+0.5;
        X_mov((dist_x+0.5)*cell_size);
        delay(d_mov);
        Y_mov((dist_y+0.5)*cell_size);
        delay(d_mov);
        X_mov(-0.5*cell_size);
        delay(d_mov);
        Y_mov(-0.5*cell_size);
        delay(d_mov);
      }
      else
      {
        Y_mov(0.5*cell_size);
        delay(d_mov);
        dist_y= dist_y-0.5;
        X_mov((dist_x+0.5)*cell_size);
        delay(d_mov);
        Y_mov((dist_y-0.5)*cell_size);
        delay(d_mov);
        X_mov(-0.5*cell_size);
        delay(d_mov);
        Y_mov(0.5*cell_size);
        delay(d_mov);
      }
    }
    else
    {
      X_mov(0.5*cell_size);
      delay(d_mov);
      dist_x= dist_x-0.5;
          if(dist_y<0)
          {
            Y_mov(-0.5*cell_size);
            delay(d_mov);
            dist_y= dist_y+0.5;
            X_mov((dist_x-0.5)*cell_size);
            delay(d_mov);
            Y_mov((dist_y+0.5)*cell_size);
            delay(d_mov);
            X_mov(0.5*cell_size);
            delay(d_mov);
            Y_mov(-0.5*cell_size);
            delay(d_mov);
          }
          else
          {
            Y_mov(0.5*cell_size);
            delay(d_mov);
            dist_y= dist_y-0.5;
            X_mov((dist_x-0.5)*cell_size);
            delay(d_mov);
            Y_mov((dist_y-0.5)*cell_size);
            delay(d_mov);
            X_mov(0.5*cell_size);
            delay(d_mov);
            Y_mov(0.5*cell_size);
            delay(d_mov);
          }
    } 

   X_pos= x_temp;
   Y_pos= y_temp;
    
  }
}
}

void X_mov(double dist)
{
  if(dist<0)
  {
    dist=dist*-1;
    digitalWrite(X1_DIR_PIN, LOW);
    digitalWrite(X2_DIR_PIN, LOW);
  }
  else
  {
    digitalWrite(X1_DIR_PIN, HIGH);
    digitalWrite(X2_DIR_PIN, HIGH);
  }

  int steps_x= map(dist, 0,x_dist_max, 0, 29860);


  for(int i =0; i<steps_x; i++)
  {
    digitalWrite(X1_STEP_PIN, HIGH);
    delayMicroseconds(d_x);
    digitalWrite(X1_STEP_PIN, LOW);
    delayMicroseconds(d_x);

    delayMicroseconds(1);

    digitalWrite(X2_STEP_PIN, HIGH);
    delayMicroseconds(d_x);
    digitalWrite(X2_STEP_PIN, LOW);
    delayMicroseconds(d_x);

    delayMicroseconds(1);
  }
}


void Y_mov(double dist)
{

  if(dist<0)
  {
    dist= dist*-1;
    digitalWrite(Z_DIR_PIN, LOW);
  }
  else
  {
    digitalWrite(Z_DIR_PIN, HIGH);
  }
  
  int steps_y = map(dist,0,y_dist_max,0, 32000);

  for(int i=0; i<steps_y; i++)
  {
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(d);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(d);
  }
}
void extruder_set()
{
   digitalWrite(E0_DIR_PIN,LOW);

  for(int i=0; i<step_extruder_initial; i++)
  {
    digitalWrite(E0_STEP_PIN , LOW);
    delayMicroseconds(d_extruder);
    digitalWrite(E0_STEP_PIN , HIGH);
    delayMicroseconds(d_extruder);
  }
}

void extruder_down()
{
   digitalWrite(E0_DIR_PIN,HIGH);

  for(int i=0; i<step_extruder; i++)
  {
    digitalWrite(E0_STEP_PIN , LOW);
    delayMicroseconds(d_extruder);
    digitalWrite(E0_STEP_PIN , HIGH);
    delayMicroseconds(d_extruder);
  }
}

void extruder_up()
{
   digitalWrite(E0_DIR_PIN,LOW);

  for(int i=0; i<step_extruder; i++)
  {
    digitalWrite(E0_STEP_PIN , LOW);
    delayMicroseconds(d_extruder);
    digitalWrite(E0_STEP_PIN , HIGH);
    delayMicroseconds(d_extruder);
  }
}


 
void setup() {
  // Sets the two pins as Outputs
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN , OUTPUT);
  digitalWrite(Z_ENABLE_PIN , LOW);

  pinMode(X1_STEP_PIN  , OUTPUT);
  pinMode(X1_DIR_PIN, OUTPUT);
  pinMode(X1_ENABLE_PIN , OUTPUT);
  digitalWrite(X1_ENABLE_PIN , LOW);

  pinMode(X2_STEP_PIN  , OUTPUT);
  pinMode(X2_DIR_PIN, OUTPUT);
  pinMode(X2_ENABLE_PIN , OUTPUT);
  digitalWrite(X2_ENABLE_PIN , LOW);

  pinMode(E0_STEP_PIN  , OUTPUT);
  pinMode(E0_DIR_PIN, OUTPUT);
  pinMode(E0_ENABLE_PIN , OUTPUT);
  digitalWrite(E0_ENABLE_PIN , LOW);

  Serial.begin(9600);
  Serial.setTimeout(.1);

  //extruder_set();

}

  int counter=0;
  int pyth;
  int temp_ul;
  int x_temp;
  int y_temp;
  
void loop() {


/*
X_mov(0.5*cell_size);
Y_mov(7*cell_size);
X_mov(1*cell_size);

Y_mov(-7*cell_size);
X_mov(1*cell_size);

Y_mov(7*cell_size);
X_mov(1*cell_size);

Y_mov(-7*cell_size);
X_mov(1*cell_size);

Y_mov(7*cell_size);
X_mov(1*cell_size);

Y_mov(-7*cell_size);
X_mov(1*cell_size);

Y_mov(7*cell_size);
X_mov(1*cell_size);
Y_mov(-7*cell_size);
X_mov(-7.5*cell_size);
*/

 temp_ul= Serial.readString().toInt();
  
 while (!Serial.available());
 pyth = Serial.readString().toInt();
 Serial.println(pyth);
  
 if(counter==0)
 {
  
  if((pyth==1) && (extruder_flag==0))
  {
    extruder_up();
    extruder_flag=1;
 
  }
  else if((pyth==0)&& (extruder_flag==1))
  {
    extruder_down();
    extruder_flag=0;
  }
  

  counter++;
  Serial.println(String(69));
 }

 else if(counter ==1)
 {
  x_temp=pyth;
  counter++;
  Serial.println(String(69));
 }

 else if(counter ==2)
 {
  y_temp=pyth;
  //delay(500);
  robot_motionPlanning(x_temp, y_temp);
  Serial.println(String(69));
  counter++;
  
 }
 else if(counter==3)
 {
  
  if((pyth==1) && (extruder_flag==0))
  {
    extruder_up();
    extruder_flag=1;

  }
  else if((pyth==0)&& (extruder_flag==1))
  {
    extruder_down();
    extruder_flag=0;
  }

  counter=0;
  Serial.println(String(69));
 }
 
}
