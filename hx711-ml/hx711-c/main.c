#include "hx711.h"
#include <time.h>
#include <wiringPi.h>
#include <stdio.h>

int main(void)
{
  initHX711(-43000, 200000.0);

  int i;

  for (i = 0; i < 3000; i++)
    {
      delay(100);
      // printf("DT_PIN: %d, SCK_PIN: %d\n", getAlt(DT_PIN), getAlt(SCK_PIN));
      printf ("Reading: %5.2f %d Avg: %3ld Drop: %ld\n",
              getReading(),
              getRawReading(),
              getAverageReadingTime (),
              getDropCount ());

    }
}
