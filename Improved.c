#include <stdio.h>
#include <stdlib.h>

// Also, it would be good idea to prove this code using Frama-c
void magic_product(double *v, double *w, double *z, double a, int n, double *b)
{
  // Check if a is near zero.
  for (int i = 0; i < n / 2; i++)
  {
    b[i] = (v[i] * w[i] * z[i]) / a;
    b[n - i - 1] = (v[i] * z[n - i - 1]) / a;
  }
}

void calc_and_print(double *v, double *w, double *z, double a, int n)
{
  if (n <= 0)
  {
    printf("The value of %d <= 0", n);
    return;
  }

  double *b = malloc(n * sizeof(double));
  if (b == NULL)
  {
    // Do based on your desing. I am going to abort it
    abort();
  }

  magic_product(v, w, z, a, n, b);

  for (int i = 0; i < n; i++)
    printf("%f\n", b[i]);

  free(b);
}

int main()
{
}
