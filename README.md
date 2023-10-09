# MIT Scheme Interpreter
A Scheme interpreter written in C++

## REPL
```bash
g++ -std=c++17 *.cpp -o main
./main
```

## Example
```bash
3
   -> 3
   Pointers: 0
x
   -> error
   Pointers: 0
()
   -> ()
   Pointers: 0
(+ 3 4)
   -> 7
   Pointers: 5
(define x 4)
   -> ()
   Pointers: 8
x
   -> 4
   Pointers: 8
(+ x x)
   -> 8
   Pointers: 13
(if 3 4 5)
   -> 4
   Pointers: 17
(if () 4 5)
   -> 5
   Pointers: 21
(define abs (lambda (x) (if (< x 0) (- x) x)))
   -> ()
   Pointers: 38
(abs 3)
   -> 3
   Pointers: 43
(abs -3)
   -> 3
   Pointers: 49
```
