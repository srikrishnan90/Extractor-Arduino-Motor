#include <Wire.h>

const int b_en = 0;
const int b_stp = 1;
const int b_dir = 2;

const int s_en = 3;
const int s_stp = 4;
const int s_dir = 5;

const int m_en = 12;
const int m_stp = 13;
const int m_dir = 14;


const int b_hm = 24;
const int s_hm = 25;
const int m_hm = 26;
const int dsen = 27;
const int tsen = 28;

const int uvlamp = 22;
const int wtlamp = 23;
int uv_off_stat=0;

const int buzzer = 15; //buzzer to arduino pin 9

String de;
String te;
int pos = 0; //positions
int dur = 0; //mixing or idle or magnetic duration
int spd = 0; //mixing speed



void setup() {

  pinMode(b_en, OUTPUT);
  pinMode(b_stp, OUTPUT);
  pinMode(b_dir, OUTPUT);

  pinMode(s_en, OUTPUT);
  pinMode(s_stp, OUTPUT);
  pinMode(s_dir, OUTPUT);

  pinMode(m_en, OUTPUT);
  pinMode(m_stp, OUTPUT);
  pinMode(m_dir, OUTPUT);

  pinMode(b_hm, INPUT_PULLUP);
  pinMode(s_hm, INPUT_PULLUP);
  pinMode(m_hm, INPUT_PULLUP);
  pinMode(dsen, INPUT_PULLUP);
  pinMode(tsen, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(uvlamp, OUTPUT);
  pinMode(wtlamp, OUTPUT);

  Serial.begin(9600);
  Wire.begin(0x07); //Set Arduino up as an I2C slave at address 0x07
  Wire.onRequest(requestEvent); //Prepare to send data
  Wire.onReceive(receiveEvent); //Prepare to recieve data
  tsen_stat();

}

void loop()
{
  //Serial.println("yes");
  delay(50);
  //door_sensor();
  //analogWrite(uvlamp,255);
  //delay(20000);
  //analogWrite(uvlamp,0);
  //delay(2000);
  if (te.substring(0, 3) == "bhm")
  {
    //Serial.println("in" + de.substring(0,3));
    b_home();
    base_init();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "shm")
  {
    //Serial.println("in" + de.substring(0,3));
    s_home();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "mhm")
  {
    //Serial.println("in" + de.substring(0,3));
    m_home();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "bmf")
  {
    //Serial.println("in" + de.substring(0,3));
    for (int i = 0; i < pos; i++)
    {
      base_forward();
    }
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "bmb")
  {
    //Serial.println("in" + de.substring(0,3));
    for (int i = 0; i < pos; i++)
    {
      base_backward();
    }
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "smf")
  {
    //Serial.println("in" + de.substring(0,3));
    for (int i = 0; i < pos; i++)
    {
      sleeve_forward();
    }
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "mmf")
  {
    //Serial.println("in" + de.substring(0,3));
    //for(int i=0;i<pos;i++)
    //{
    // magnet_forward();
    //}
    sleeve_backward();
    magnet_forward();
    m_s_forward();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "msf")
  {
    //Serial.println("in" + de.substring(0,3));
    for (int i = 0; i < pos; i++)
    {
      m_s_forward();
    }
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "msb")
  {
    //Serial.println("in" + de.substring(0,3));
    for (int i = 0; i < pos; i++)
    {
      m_s_backward();
    }
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "idl")
  {
    //Serial.println("in" + de.substring(0,3));
    idle();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "mag")
  {
    //Serial.println("in" + de.substring(0,3));
    idle();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "mix")
  {
    // Serial.println("in" + de.substring(0,3));
    mix();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "mis")
  {
    // Serial.println("in" + de.substring(0,3));
    mis();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "buz")
  {
    // Serial.println("in" + de.substring(0,3));
    buzzer_demo();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "ini")
  {
    // Serial.println("in" + de.substring(0,3));
    initz();
    delay(500);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "dss")
  {
    // Serial.println("in" + de.substring(0,3));
    dsen_stat();
    delay(200);
    de = "done";
    te = "";
  }
  else if (te.substring(0, 3) == "UVN")
  {
    // Serial.println("in" + de.substring(0,3));
    uv_on();
    delay(200);
    de = "done";
    te = "";
  }
}

void(* resetFunc) (void) = 0;

void tsen_stat()
{
  while (1)
  {
    int stat = digitalRead(tsen);
    if (stat == 0)
    {
      de = "tlock";
    }
    else
    {
      de = "done";
      break;
    }
  }
  
}

void dsen_stat()
{
  int stat = digitalRead(dsen);
  if (stat == 1)
  {
    de = "dopen";
  }
  else
  {
    de = "done";
  }

}

void initz()
{
  m_home();
  s_home();
  b_home();
  base_init();
}

void m_home() {
  magnet_home(); magnet_home(); magnet_home(); magnet_home();
}
void s_home() {
  sleeve_home(); sleeve_home(); sleeve_home(); sleeve_home();
}
void b_home() {
  base_home(); base_home(); base_home(); base_home();
}


void base_home()
{
  digitalWrite(b_en, LOW);
  digitalWrite(b_dir, HIGH);
  for (int i = 0; i < 1000; i++)
  {
    int stat = digitalRead(b_hm);
    if (stat == 0)
      break;
    else
    {
      digitalWrite(b_stp, HIGH);
      delayMicroseconds(2000);
      digitalWrite(b_stp, LOW);
      delayMicroseconds(2000);
    }
  }
  //digitalWrite(b_en, HIGH);
}

void sleeve_home()
{
  digitalWrite(s_en, LOW);
  digitalWrite(s_dir, HIGH);
  for (int i = 0; i < 1000; i++)
  {
    int stat = digitalRead(s_hm);
    if (stat == 0)
      break;
    else
    {
      digitalWrite(s_stp, HIGH);
      delayMicroseconds(1000);
      digitalWrite(s_stp, LOW);
      delayMicroseconds(1000);
    }
  }
  //digitalWrite(s_en, HIGH);
}
void magnet_home()
{
  digitalWrite(m_en, LOW);
  digitalWrite(m_dir, LOW);
  for (int i = 0; i < 1700; i++)
  {
    int stat = digitalRead(m_hm);
    if (stat == 0)
      break;
    else
    {
      digitalWrite(m_stp, HIGH);
      delayMicroseconds(1000);
      digitalWrite(m_stp, LOW);
      delayMicroseconds(1000);
    }
  }
  //digitalWrite(m_en, HIGH);
}




void base_forward1()
{
  digitalWrite(b_en, LOW);
  digitalWrite(b_dir, LOW);
  for (int i = 0; i < 210; i++)
  {
    digitalWrite(b_stp, HIGH);
    delayMicroseconds(2000);
    digitalWrite(b_stp, LOW);
    delayMicroseconds(2000);
  }
  //digitalWrite(b_en, HIGH);
}

void base_init()
{
  digitalWrite(b_en, LOW);
  digitalWrite(b_dir, LOW);
  for (int i = 0; i < 290; i++)
  {
    digitalWrite(b_stp, HIGH);
    delayMicroseconds(2000);
    digitalWrite(b_stp, LOW);
    delayMicroseconds(2000);
  }
  //digitalWrite(b_en, HIGH);
}


void sleeve_forward()
{
  digitalWrite(s_en, LOW);
  digitalWrite(s_dir, LOW);
  for (int i = 0; i < 795; i++)//770
  {

    digitalWrite(s_stp, HIGH);
    delayMicroseconds(1500);
    digitalWrite(s_stp, LOW);
    delayMicroseconds(1500);
  }
  //digitalWrite(s_en, HIGH);
}
//new finction
void sleeve_backward()
{
  digitalWrite(s_en, LOW);
  digitalWrite(s_dir, HIGH);
  for (int i = 0; i < 645; i++)//620
  {

    digitalWrite(s_stp, HIGH);
    delayMicroseconds(1500);
    digitalWrite(s_stp, LOW);
    delayMicroseconds(1500);
  }
  //digitalWrite(s_en, HIGH);
}
//new function
void magnet_forward()
{
  digitalWrite(m_en, LOW);
  digitalWrite(m_dir, HIGH);
  for (int i = 0; i < 838; i++)
  {
    digitalWrite(m_stp, HIGH);
    delayMicroseconds(1500);
    digitalWrite(m_stp, LOW);
    delayMicroseconds(1500);
  }
  //digitalWrite(m_en, HIGH);
}
/*void magnet_forward()
  {
  digitalWrite(m_en, LOW);
  digitalWrite(m_dir, HIGH);
  for (int i = 0; i < 1520; i++)
  {
    digitalWrite(m_stp, HIGH);
    delayMicroseconds(1500);
    digitalWrite(m_stp, LOW);
    delayMicroseconds(1500);
  }
  //digitalWrite(m_en, HIGH);
  }*/

void m_s_backward()
{
  digitalWrite(m_en, LOW);
  digitalWrite(s_en, LOW);
  digitalWrite(m_dir, LOW);
  digitalWrite(s_dir, HIGH);
  for (int i = 0; i < 645; i++)//620
  {
    digitalWrite(m_stp, HIGH);
    digitalWrite(s_stp, HIGH);
    delayMicroseconds(1500);
    digitalWrite(m_stp, LOW);
    digitalWrite(s_stp, LOW);
    delayMicroseconds(1500);
  }
  //digitalWrite(m_en, HIGH);
}
void m_s_forward()
{
  digitalWrite(m_en, LOW);
  digitalWrite(s_en, LOW);
  digitalWrite(m_dir, HIGH);
  digitalWrite(s_dir, LOW);
  for (int i = 0; i < 645; i++)//620
  {
    digitalWrite(m_stp, HIGH);
    digitalWrite(s_stp, HIGH);
    delayMicroseconds(5000);
    digitalWrite(m_stp, LOW);
    digitalWrite(s_stp, LOW);
    delayMicroseconds(5000);
  }
  //digitalWrite(m_en, HIGH);
}

void base_forward()
{
  digitalWrite(b_en, LOW);
  digitalWrite(b_dir, LOW);
  for (int i = 0; i < 120; i++)
  {
    digitalWrite(b_stp, HIGH);
    delayMicroseconds(2000);
    digitalWrite(b_stp, LOW);
    delayMicroseconds(2000);
  }
  //digitalWrite(b_en, HIGH);
}

void base_backward()
{
  digitalWrite(b_en, LOW);
  digitalWrite(b_dir, HIGH);
  for (int i = 0; i < 120; i++)
  {
    digitalWrite(b_stp, HIGH);
    delayMicroseconds(2000);
    digitalWrite(b_stp, LOW);
    delayMicroseconds(2000);
  }
  //digitalWrite(b_en, HIGH);
}

void mix()
{
  uint32_t period = dur * 1000L;       // 5 minutes
  int del = spd * 300;
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {

    {
      digitalWrite(s_dir, HIGH);
      for (int i = 0; i < 150; i++)
      {
        digitalWrite(s_stp, HIGH);
        delayMicroseconds(del);
        digitalWrite(s_stp, LOW);
        delayMicroseconds(del);
      }
      delay(10);
      digitalWrite(s_dir, LOW);
      for (int i = 0; i < 150; i++)
      {
        digitalWrite(s_stp, HIGH);
        delayMicroseconds(del);
        digitalWrite(s_stp, LOW);
        delayMicroseconds(del);
      }
    }
  }
}

void mis()
{
  uint32_t period = dur * 1000L;       // 5 minutes
  int del = spd * 450;
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {

    {
      digitalWrite(s_dir, HIGH);
      for (int i = 0; i < 30; i++)
      {
        digitalWrite(s_stp, HIGH);
        delayMicroseconds(del);
        digitalWrite(s_stp, LOW);
        delayMicroseconds(del);
      }
      delay(10);
      digitalWrite(s_dir, LOW);
      for (int i = 0; i < 30; i++)
      {
        digitalWrite(s_stp, HIGH);
        delayMicroseconds(del);
        digitalWrite(s_stp, LOW);
        delayMicroseconds(del);
      }
    }
  }
}

void idle()
{
  uint32_t period = dur * 1000L;       // 5 minutes
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {

  }
}

void uv_on()
{
  digitalWrite(uvlamp, HIGH);
  uint32_t period = dur * 1000L;       // 5 minutes
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {
    if(uv_off_stat==1)
    {
      break;
    }

  }
  uv_off_stat=0;
  digitalWrite(uvlamp, LOW);
  
  //Buzzer for alarm after completion
  period = 2 * 1000L; 
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }
}

void buzzer_demo()
{
  uint32_t period = dur * 1000L;       // 5 minutes
  for ( uint32_t tStart = millis();  (millis() - tStart) < period;  )
  {
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(100);        // ...for 1sec
  }
}

void requestEvent()
{
  Serial.println(de);
  char buf[30];
  de.toCharArray(buf, 30);
  Wire.write(buf, 30); //Write String to Pi.
  delay(100);
}


void receiveEvent(int numBytes)
{
  unsigned char rc[30] = "";
  int count = 0;
  while (Wire.available())
  {
    char c = Wire.read();
    rc[count] = c;
    count++;
    delay(10);
  }

  de = rc;
  te = de;
  Serial.println(de);
  //Serial.println(de.substring(0, 3));
  if (te.substring(0, 3) == "bmf" || te.substring(0, 3) == "bmb" || te.substring(0, 3) == "mmf" || te.substring(0, 3) == "mmb" || te.substring(0, 3) == "smf" || te.substring(0, 3) == "smb" || te.substring(0, 3) == "msf" || te.substring(0, 3) == "msb")
  {
    char buf[30];
    te.toCharArray(buf, 30);
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, " ", &p)) != NULL) // delimiter is the space
    {
      if (i == 1)
      {
        pos = atoi(str);
      }
      i++;
    }
  }
  else if (te.substring(0, 3) == "idl" || te.substring(0, 3) == "mag"|| te.substring(0, 3) == "buz" || te.substring(0, 3) == "UVN")
  {
    char buf[30];
    te.toCharArray(buf, 30);
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, " ", &p)) != NULL) // delimiter is the space
    {
      if (i == 1)
      {
        dur = atoi(str);
      }
      i++;
    }
  }
  else if (te.substring(0, 3) == "mix" || te.substring(0, 3) == "mis")
  {
    char buf[30];
    te.toCharArray(buf, 30);
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, " ", &p)) != NULL) // delimiter is the space
    {
      if (i == 1)
      {
        dur = atoi(str);
      }
      if (i == 2)
      {
        spd = atoi(str);
      }
      i++;
    }
  }
   else if (te.substring(0, 3) == "stp")
   {
    initz();
    delay(100);
    de = "done";
    resetFunc();
   }
   else if (te.substring(0, 3) == "UVF")
   {
    uv_off_stat=1;
   }

}
