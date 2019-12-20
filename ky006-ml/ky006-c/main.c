#include <stdio.h>
#include "ky006.h"

int main(void)
{
  initKY006();

  while(1){
    printf("music is being played...\n");
    buzz();
  }

  return 0;
}
