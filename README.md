# Leaker
A leak checking library for C, for when you cannot use valgrind. It works by using macros to intercept calls to the primiary memory management functions and tracking the life of the pointers they create.
For this reason there are a couple of drawbacks to leaker, the primary being that it can only find leaks within files that include it's header, thus leaks from libraries or their missuse cannot be found. It also adds quite a bit of overhead if you allocate memory a lot.
 
 To use leaker, simply inlcude it's header in any file you allocate or free memory and call the start_leaker() and end_leaker() functions at the start and end of your program respectively.
While there is no problem including stdlib, if you do it is recommended to include leaker _after_ stdlib. 
 
 Example: 
 ``` c
 #include <stdio.h>
 #include <leaker.h>
 
 int main() {
    start_leaker();
    int * i = malloc(sizeof(int));
    end_leaker();
    return 0;
}
 ```
