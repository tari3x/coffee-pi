/*
*   Author: Tosatto Davide
*
*   Module that controls sample reading from the HX711 sensor.
*
*   Due to process switches, it can happen that a reading fails sometimes (on average one over 70).
*
*   Since to avoid this kernel mode operation is needed but I have no time to write down a device driver,
*   I simply throw away bad readings, identified by a reading time higher than average. Algorithm below.
*/

#include "hx711.h"
#include "precisionTiming.h"
#include <time.h>
#include <wiringPi.h>
#include <stdio.h>

// #define DEBUG //Uncomment to activate debug printfs

static int DT_PIN = 15;
static int SCK_PIN = 7;

static int readValue = 0;
static int _offset = 0;
static double _div = 0;
static long int dropCount = 0;

static volatile int reading = 0; //Keeps the read data

static precisionTime_t readAverage = 70; //Keeps the average reading time of a single sample

static float getMillisDiff (clock_t t1, clock_t t2);
static float getDistFromTime (float timeDiff);

static void edge ();

void initHX711 (int offset, double div)
{
  // This one doesn't work:
  // wiringPiSetupGpio() ;
  wiringPiSetup();

  //printf("DT_PIN: %d, SCK_PIN: %d\n", getAlt(DT_PIN), getAlt(SCK_PIN));
  pinMode(DT_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  wiringPiISR (DT_PIN, INT_EDGE_FALLING, edge);
  // printf("DT_PIN: %d, SCK_PIN: %d\n", getAlt(DT_PIN), getAlt(SCK_PIN));

  setupHX711(offset, div);

  digitalWrite(SCK_PIN, LOW);
}

int extendSign (int val)
{
  if ((val & (1<<23))>0)
    {
      val += 0xFF<<24;
    }

  return val;
}

double getReading ()
{
  int val = extendSign(readValue);

  return (val + _offset)/_div;
}

int getRawReading ()
{
  return extendSign(readValue);
}

long getAverageReadingTime ()
{
  return readAverage;
}

long int getDropCount ()
{
  return dropCount;
}

//Callback function called at falling edge on DT_PIN
static void edge ()
{
  if (reading)
    {
#ifdef DEBUG
      printf("already reading\n");
#endif
      return;
    }

  if(digitalRead(DT_PIN))
    {
#ifdef DEBUG
      printf("unexpected high DT_PIN\n");
#endif
      return;
    }

#ifdef DEBUG
  printf("Reading \n");
#endif

  precisionTime_t microS = getTimeMicro();

  reading = 1;

  int read = 0;

  int i;

  int j = 0;

  precisionDelayMicro (1);

  for (i = 0; i < 24; i++)
    {
      digitalWrite(SCK_PIN, HIGH);
      read <<= 1;

      precisionDelayMicro (1);

      digitalWrite(SCK_PIN,LOW);

      precisionDelayMicro (1);

      if (digitalRead(DT_PIN)){
#ifdef DEBUG
        printf ("%d ", i);
#endif
        read++;}
    }

  digitalWrite(SCK_PIN,HIGH);
  precisionDelayMicro (1);
  digitalWrite(SCK_PIN,LOW);

  //Calculates the reading time for this sample
  precisionTime_t readTime = getTimeMicro() - microS;

  //Calculates a weighted average through shifts to waste less time
  //It's like doing (readAverage*7 + readTime)/8
  //Probably the copiler would do this as well, but for being sure I used shifts
  readAverage = ((readAverage << 2)  + (readAverage << 1) + readAverage + readTime)>>3;

  //This sample is valid if its reading time is less than average + 25% of the average
  int valid = !(readTime > (readAverage + (readAverage>>2)));

#ifdef DEBUG
  printf("Averages: %lld, Read Time: %lld, valid: %d\n", readAverage, readTime, valid);
#endif

  if (valid)
    {
      readValue = read;
    }
  else
    {
      dropCount++;
#ifdef DEBUG
      printf("DROP: %d\n", read);
#endif
    }

  reading = 0;
}

void setupHX711 (int offset, double div)
{
  _offset = offset;
  _div = div;
}

