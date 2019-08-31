## Code Analysis

1. Using the experience
2. Libfuzzer
3. Static Analysis (Infer, and Frama-C)


### Using the Experience: 
 1.  Malloc is depending on public parameter n which is of type int, so passing 
 	  negative or large value could lead to no memory allocation

 ```c
 void calc_and_print(double *v, double *w, float *z, float a, int n) {
 		double *b;
 		
     	b = malloc(n * sizeof(float));
     
     }
 
 ```
 and returning NULL. It would be good idea to do a NULL check to make sure that 
 that it is successful or not.
 
 [Common Weakness Enumeration](https://cwe.mitre.org/data/definitions/789.html)
 
2. The other problem with this allocation is 4 * n bytes (assuming float as 4 byte. 
   If b is double and we interpret this 4 * n bytes as double place holder, then we
   would just have n/2 memory places for double values (4 * n / 8). Also, 
   depending on compiler(clang, or clang++), you would get the type cast error. The 
   memory block return by malloc is void* and C silently casts it type of b, but C++
   is more strict and asks you explicitly type case it. 
 


 ```c
  keep-learnings-MacBook-Pro:Ccodeanalysis keep_learning$ clang Helloworld.c 
  keep-learnings-MacBook-Pro:Ccodeanalysis keep_learning$ clang++ Helloworld.c 
      clang: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is      deprecated [-Wdeprecated]
Helloworld.c:24:9: error: assigning to 'double *' from incompatible type 'void *'
    b = malloc(n * sizeof(float));
        ^~~~~~~~~~~~~~~~~~~~~~~~~
1 error generated.

 ```
 
3. Using the memory after freeing it. Assuming that n was greater than 10, then if 
   statement would free the memory, and for loop would access the invalid memory location, 
   possible reading garbage data or program crash. 
   [Common Weakness Enumeration](https://cwe.mitre.org/data/definitions/416.html)

```c
 if (n > 10) {
      free(b);
  }

 
  for ( int i = 0; i < n; i++ ) {
    printf("%f\n", b[i]);
  }
```
 
4. In magic_products, off by one error in b[n - i] = ( v[i] * z[n - i] ) / a; when i is 0, 
   and no check on a. What if a is very small and near to 0 or 0.


```c
void magic_product(double *v, double *w, float *z, float a, int n, double *b) 
{
  for (int i = 0; i < n/2; i++) 
  {

    b[i] = (v[i] * w[i] * z[i]) / a;
    b[n - i] = (v[i] * z[n - i]) / a;

  }
}
```

My C programming experience got me this far, but now it is good time to  [fuzz](https://
en.wikipedia.org/wiki/Fuzzing) it.

### libFuzzer and AddressSanitizer:
	This space is for libfuzzer
  

### Static Analysis
Static analysis is a program analysis technique in which we approximate the run time 
behaviour of program without executing it. I am using [infer](https://fbinfer.com/). 

```c
keep-learnings-MacBook-Pro:Ccodeanalysis keep_learning$ infer run -- clang Helloworld.c 
Capturing in make/cc mode...
Found 1 source file to analyze in /Users/keep_learning/Mukesh/Github/Ccodeanalysis/infer-out
Starting analysis...

legend:
  "F" analyzing a file
  "." analyzing a procedure

F....
Found 3 issues

Helloworld.c:34: error: MEMORY_LEAK
  memory dynamically allocated by call to `malloc()` at line 24, column 9 is not reachable after line 34, column 20.
  32.   
  33.    
  34. >   for ( int i = 0; i < n; i++ ) {
  35.       printf("%f\n", b[i]);
  36.     }

Helloworld.c:27: error: NULL_DEREFERENCE
  pointer `b` last assigned on line 24 could be null and is dereferenced by call to `magic_product()` at line 27, column 3.
  25.     }
  26.   
  27. >   magic_product(v, w, z, a, n, b);
  28.   
  29.     if (n > 10) {

Helloworld.c:35: error: USE_AFTER_FREE
  pointer `b` last assigned on line 24 was freed by call to `free()` at line 30, column 7 and is dereferenced or freed at line 35, column 20.
  33.    
  34.     for ( int i = 0; i < n; i++ ) {
  35. >     printf("%f\n", b[i]);
  36.     }
  37.   


Summary of the reports

       MEMORY_LEAK: 1
  NULL_DEREFERENCE: 1
    USE_AFTER_FREE: 1
keep-learnings-MacBook-Pro:Cco
```

It has found 3 issues which is kind of nice. When I ran the infer on Improved.c, then it 
showed

```

keep-learnings-MacBook-Pro:Ccodeanalysis keep_learning$ infer run -- clang -c Improved.c 
Capturing in make/cc mode...
Found 1 source file to analyze in /Users/keep_learning/Mukesh/Github/Ccodeanalysis/infer-out
Starting analysis...

legend:
  "F" analyzing a file
  "." analyzing a procedure

F...
  No issues found  
```

Also, it would be good to prove some properties about these functions in Frama-c.
