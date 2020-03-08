bool mright = false;
bool lastPower = false;
uint8_t mode = -1;
int pwm;
void loop()
{

    if (ds4.readGamepad())
    {
      if (ds4.buttonPressed(PS))
      {
        powerOn = !powerOn;
        digitalWrite(powerLed, powerOn);
        debug_msg("POWER : " + String((powerOn) ? "ON" : "OFF"));
  
      }
  
  
      if (powerOn)
      {
        Lx = map(ds4.axis(LX), 0, 65535, -32768, 32767);
        Ly = map(ds4.axis(LY), 0, 65535, -32768, 32767);
        Rx = map(ds4.axis(RX), 0, 65535, -32768, 32767);
        Ry = map(ds4.axis(RY), 0, 65535, -32768, 32767);
        Serial.print("Lx :");
        Serial.print(Lx);
        Serial.print("\tLy :");
        Serial.println(Ly);
  
        if (checkForRoutines())
        {
          return;
        }
  
        else if (!(Rx > -AXIS_DEAD_ZONE && Rx < AXIS_DEAD_ZONE) || !(Ry > -AXIS_DEAD_ZONE && Ry < AXIS_DEAD_ZONE))
        {
          double theta = toDegree(atan((double)Ry / (double)Rx));
          Rx = map(Rx, -32768, 32767, -128, 127);
          Ry = map(Ry, -32768, 32767, -128, 127);
          Serial.print("Rx :");
          Serial.println(Rx);
          unsigned int R = constrain(map2((double)((Rx * Rx) + (Ry * Ry)), 0.0, 16384.0, 0.0, 100.0), 0, 100);
          if (Ry >= 0)
          {
            if (theta <= 0)
              theta += 180.0;
          }
          else
          {
            if (theta >= 0)
              theta += 180.0;
            else
              theta += 360.0;
          }
  
          // debug_msg("Rx : " + String(Rx) + "\tRy : " + String(Ry) + "\tR : " + String(R) + "\tTheta : " + String(theta));
          if (R <= 25)
          {
            rotational.SetTunings(0.021, 0.015, 0.0032);
            if (mode != 0)
            {
              rotational.resetIntegral();
              mode = 0;
            }
          }
          else //if (R <= 60)
          {
            //0.01325, 0.002, 0.01
            rotational.SetTunings(0.021, 0.015, 0.0032);
            if (mode != 1)
            {
              rotational.resetIntegral();
              mode = 1;
            }
          }
          bot.move(R, theta);
          //testRoutine();
        }
        else if (ds4.button(SQUARE))
        {
          servo();
          Serial.println("s");
        }
        else if (ds4.button(CIRCLE))
        {
          servo1();
          Serial.println("s1");
        }
        else if (!(Lx > -AXIS_DEAD_ZONE && Lx < AXIS_DEAD_ZONE) || !(Ly > -AXIS_DEAD_ZONE && Ly < AXIS_DEAD_ZONE))
        {
          Serial.println("Left Axis Triggered\t");
          //Serial.println(String(Lx));
          pwm = map(Lx, -32768, 32767, -25, 25);
          bot.Rotate_AK(pwm);
          //resetBNO();
        }
        else if (ds4.button(TRIANGLE))
              {
                  bot.move(30, 90);
              }
        else if (ds4.button(CROSS))
              {
                  bot.move(30, 270);
              }
        else {
          //Serial.println("Stopping");
          bot.stopAll();
        }
  
      }
      else
        bot.stopAll();
    }
  }

//  if (tuner.update())
//  {
//    if (lastPower == 0 && powerOn == 1) {
//      //resetBNO();
//    mright = !mright;}
//    rotational.SetTunings(kp, ki, kd);   // Serial.println(String(kp) + "\t" + String(ki) + "\t" + String(kd) + "\t" + String(powerOn));
//    //Serial.println(String(powerOn));
//    if (powerOn)
//    {
//      
//      (mright) ? bot.move(40, 0) : bot.move(40, 180);
//      //Serial.println("moving");
//     // bot.move(30, 90);
////      powerOn = !powerOn;
//      
////  Serial.println(String(kp) + "\t" + String(ki) + "\t" + String(kd) + "\t" + String(powerOn));
//    }
//    else
//    {// Serial.println("Stopping");
//      bot.stopAll();
//    }
//    lastPower = powerOn;
//
//  }

//Serial.println(lastPower);
