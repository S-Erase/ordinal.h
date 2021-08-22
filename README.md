# ordinal.h
This is a header made to allow the creaton, comparison and arithmetic of ordinal numbers up to &epsilon;<sub>0</sub>.

## What is an ordinal number?
An ordinal number is a generalization of the natural numbers used to describe the ordering of well-ordered sets.

### What is a well-ordered set?
A totally-ordered set is a set where any element can be described as either "less than" or "greater than" any other element. A well-ordered set is a totally-ordered set such that any non-empty subset has a "least" element.

### The Natural Numbers
The de-facto definition of the Natural numbers is known as the Von Neumann ordinals, which is as follows:
* Define '0' as the empty set &emptyset; = {}.
* Define the successor function S(x) = x &cup; {x}.
* Define '1' as S(0) = {0}, '2' as S(1) = {0,1}, '3' as S(2) = {0,1,2}, and so on.

This definition has two useful properties:
1. The natural number 'n' has n elements inside it.
  
    This property makes defining the size or 'cardinality' of a set easy in set theory. A set has size 'n' if every element can be mapped one-to-one to an element in 'n'. The set is sometimes said to be 'bijective' with n.
    
2. A natural number is the set of all naturals preceding it.

    This property makes defining the order between numbers easy. n < m if n is an element of m; equivalently if n is a subset of m. It also helps in defining certain functions
    * max(a,b) = a &cup; b
    * sup{ a<sub>i</sub> | i &in; I } = &cup;<sub>i &in; I</sub> a<sub>i</sub>

### Transfinite Ordinals
The set of natural numbers &Nopf; is thus defined as {0,1,2,3,...}. The definition of size for the natural numbers also applies to &Nopf; itself. A set that is 'bijective' with &Nopf; is called 'countably infinite'.

If the successor function is applied to &Nopf;, we get S(&Nopf;) = {0,1,2,...,&Nopf;}. This is where the concept of ordinals begins to show use. S(&Nopf;) actually has the same cardinality as &Nopf;, as &Nopf; in S(&Nopf;) can be mapped to 0 in &Nopf; and n in S(&Nopf;) can be mapped to S(n) in &Nopf;. S(&Nopf;) and &Nopf; cannot be mapped together in a way that preserves order however, as S(&Nopf;) has a 'greatest' element while &Nopf; does not. This is refered to as the order type. The successor function can be applied to &Nopf; repeatedely to get different 'countably infinite' sets with different order types. These sets are called 'transfinite ordinals'.
From here on, the set &Nopf; is referred to as &omega;.
* &omega; = {0,1,2,...}
* &omega;+1 = {0,1,2,...,&omega;}
* &omega;+2 = {0,1,2,...,&omega;,&omega;+1}

The initial definition of order for naturals holds in general for ordinals.

## Limit Ordinals and Induction
There are two types of natural numbers: '0', and successor naturals. Every natural number that is not zero is preceded by another natural. This is not the case when generalizing to ordinals however, as the ordinal &omega; is not preceded by anything. An ordinal that is not zero and not a successor to another ordinal is called a limit ordinal.

### Induction
There are two versions of induction.
* Weak induction

    This is the induction you would be taught in secondary school. Given a property P(n), if P(0) and P(n)&#x21D2;P(n+1) then P holds for all n.
    
* Strong induction

    Given a property P(n), if P(0) and (&forall;m<n P(m))&#x21D2;P(n) then P holds for all n.
    
The Weak and Strong forms of induction are equivalent when only working with natural numbers, but Weak induction cannot prove P(&omega;). Strong induction is required if induction on the ordinals (Transfinite induction) is to be attempted.

## Arithmetic
Addition, Multiplication, and Exponentiation (but not subtraction, division, roots or logarithms) can be defined recursively on the ordinals similar to how they are recursively defined on the naturals, however they lose certain properties once you transcend the naturals.
### Addition
Addition is defined as follows:
* For zero, a+0 = a
* For successors, a+S(b) = S(a+b)
* For limit ordinals, a+b = sup{ a+c | c < b }

Ordinal addition retains associativity ((a+b)+c = a+(b+c)) but loses commutivity. For example, 2+&omega; = sup{ 2+n | n&in;&Nopf; } = &omega; &ne; &omega;+2. Imagine taking the two sets, placing them next to each other and ordering them such that elements on the right set are greater than elements on the left set.

&omega; + 2 = {0,1,...} + {0,1} = {0,1,...,0',1'} = &omega;+2

2 + &omega; = {0,1} + {0,1,...} = {0,1,0',1',...} &equiv; {0,1,2,3,...} = &omega;

&omega;+&omega; = {0,1,...,&omega;,&omega;+1,...} = sup{ &omega;+n | n&in;&Nopf; }

### Multiplication
Multiplication is defined as follows:
* For zero, a\*0 = 0
* For successors, a\*S(b) = a\*b + a
* For limit ordinals, a\*b = sup{ a\*c | c < b }

Ordinal multiplication retains associativity, but loses commutivity and right-distributivity.

Examples:
* &omega;\*2 = &omega;\*1 + &omega; = &omega;+&omega;
* 2\*&omega; = sup{ 2\*n | n&in;&Nopf; } = &omega;
* &omega;\*&omega; = sup{ &omega;\*n | n&in;&Nopf; }

### Exponentiation
Exponentiation is defined as follows:
* For zero, a<sup>0</sup> = 1
* For successors, a<sup>S(b)</sup> = a<sup>b</sup> \* a
* For limit ordinals, a<sup>b</sup> = sup{ a<sup>c</sup> | c < b }

Ordinal exponentiation loses the property (a\*b)<sup>c</sup> = a<sup>c</sup>\*b<sup>c</sup>

Examples:
* (&omega;\*2)<sup>2</sup> = (&omega;\*2)\*(&omega;\*2) = &omega;<sup>2</sup>\*2
* (&omega;)<sup>2</sup>\*(2)<sup>2</sup> = &omega;<sup>2</sup>\*4
* a<sup>&omega;</sup> = sup{ a<sup>n</sup> | n&in;&Nopf; } = &omega;
* &omega;<sup>&omega;</sup> = sup{ &omega;<sup>n</sup> | n&in;&Nopf; }

## Epsilon zero and the Cantor Normal Form
As addition is defined by repeated succession, multiplication by repeated addition, and exponentation by repeated multiplication, a function can be defined by repeated exponentiation. This function is called tetration. Repeated tetration will give what is called pentation, repeated pentation gives hexation, and so on. These are not standard operations however, and so the tetration of &omega; with itself is given a new name entirely:

&epsilon;<sub>0</sub> = sup{ &omega;, &omega;<sup>&omega;</sup>, &omega;<sup>&omega;<sup>&omega;</sup></sup>, ...}

Given only &omega;, &epsilon;<sub>0</sub> cannot be reached without the use of tetration. With only addition, multiplication, and exponentiation, the ordinals you can compute can be represented in "Cantor Normal Form": a sum of &omega; terms raised to the power of strictly decreasing ordinal terms multiplied by a natural coefficient in a manner similar to a polynomial or n-base representation.
