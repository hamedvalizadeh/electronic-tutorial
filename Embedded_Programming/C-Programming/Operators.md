# Types of Operators in C

- Arithmetic Operator.
- Increment/Decrement Operator.
- Assignment Operator.
- Logical Operator.
- Bitwise Operator.
- Misc Operator.



# Bitwise Operator

there are some operators to work with bit values in c; these operators support `AND`, `OR`, `NOT`, and `XOR`.

this operators first change the value to the base 2 number number, and execute the operator on each of its bits. 

### `NOT` Operator

the operator is `~` that is used to convert the bit to it's not. 

```c
#include <stdio.h>

void printb(unsigned int v) {
    unsigned int i, s = 1<<((sizeof(v)<<3)-1); // s = only most significant bit at 1
    for (i = s; i; i>>=1) printf("%d", v & i || 0 );
}

int main()
{
    int a   = 25;
    int b   = ~a;
    int c = a + b;
    printf("a value: ");
    printf("%d", a);
    printf("\n");
    printf("a binary: ");
    printb(a);
    printf("\n");
    printf("b value: ");
    printf("%d", b);
    printf("\n");
    printf("b binary: ");
    printb(b);
    printf("\n");
    printf("c value: ");
    printf("%d", c);
    printf("\n");
    printf("c binary: ");
    printb(c);
    return 0;
}
```

the output would be as following:

```
a value: 25
a binary: 00000000000000000000000000011001
b value: -26
b binary: 11111111111111111111111111100110
c value: -1
c binary: 11111111111111111111111111111111
```



**Note:** first bit in bit array in computers are reserved to specify the sign of the number, if it is 1 the number is negative otherwise it is positive.



### `AND` Operator

the symbol is `&`.



### `OR` Operator 

the symbol is `|`.



### `XOR` Operator

the symbol is `^`.