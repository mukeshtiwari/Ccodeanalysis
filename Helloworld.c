#include <stdio.h>
#include <stdlib.h>

void magic_product(double *v, double *w, float *z, float a, int n, double *b) 
{
  for (int i = 0; i < n/2; i++) 
  {

    b[i] = (v[i] * w[i] * z[i]) / a;
    b[n - i] = ( v[i] * z[n - i] ) / a;

  }
}


void calc_and_print(double *v, double *w, float *z, float a, int n) {
  double buf[10] = {0};
  double *b;

  if (n < 10) {
    b = buf;
    } 
  else {
    b = malloc(n * sizeof(float));
  }

  magic_product(v, w, z, a, n, b);

  if (n > 10) {
      free(b);
  }

 
  for ( int i = 0; i < n; i++ ) {
    printf("%f\n", b[i]);
  }

}

int LLVMFuzzerTestOneInput(double *v, double *w, float *z, float a, int n) {
  calc_and_print(v, w, z, a, n);
  return 0;
}

int main()
  {
    //double v[10], w[10];


  }