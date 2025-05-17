#include "FLOAT.h"

#include <stdio.h>
#include <unistd.h>

float F2float(FLOAT F_val) { return (float)F_val / (1 << 16); }

int main()
{

}
