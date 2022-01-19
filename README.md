# Jester Script
 A Lisp-inspired scripting language
***

Jester Scipt is a hobby-project intended to make a minimal yet flexible language
- This project is by no means professionally developed or maintained

### Info

- Types are dynamic and with minimal type enforcement
- Jester Script uses a ```progn``` model for every expression: the last thing evaluated is also returned 
- By default Jester Scipt isn't lazily evaluated but can achieve the same result through quoting
- Jester Script can integrate to bring C++ source code
- Jester Script's runtime is comparable in speed to that of python, although due to C++ extensibility this can vary

## Examples

```
; define a sum function
(defun sum (a b)
 (+ a b))

; call function
(sum 2 2)

; assign function result
(set x (sum 2 2))
```

In Jester Script variables are declared simply by typing out their name. Any symbol not previously defined is immediately declared with a type and value of ```nil```.

```
; declare variables 'a', 'b', and 'c' to nil
a b c

; print - "nil nil nil"
(println a b c)
```

In the same manner lists are declared by default by writing out ```(elements...)``` - this reflects how everything in Jester Script is just a linked list of values.

```
; declare list {1, 2, 3}
(1 2 3)

(set list (1 2 3))

; print - "(1 2 3)"
(println list)
```

Move semantics are pretty simple: everything is copied by value unless a String or a List.

```
(set val 0)
(set val-cpy val)

; print "0"
(println val-cpy)

(+= val 1)

; print "0"
(println val-cpy)

; VS.

(set list (1 2 3))
(set list-cpy lst)

; print - "(1 2 3)"
(println list-cpy)

(append 4 list)

; print - "(1 2 3 4)"
(println list-cpy)
```

Lastly, scopes are dynamic, meaning they can be freely created or detsroyed such as with the case of functions of ```let``` bindings.

```
(set x 0)

(let
 ( (x 10) )
 
 ; print - "10"
 (println x))
 
; print - "0"
(println x)
```

Once the ```let``` scope terminated, x reverted to its previous value of 0. The same would occur with function arguments.

## Read-Eval-Print-Loop (REPL)

The REPL mode is a form of i/o programming and validating unique to Lisp's ```progn``` model. If unfamiliar, since every expression in Lisp must evaluate to something (essentially meaning there are no ```void``` evaluations/returns), then every line of code can be (1) Read (2) Evaluated and (3) Printed... in a (4) user Loop.

In Jester Script it looks something like this:

```
---------- Welcome to Jester-Script ----------

[0]>> x <-- user input
nil     <-- computer output
[1]>> (set x 10)
10
[2]>> (+= x 10)
20
[3]>> (defun sum (a b) (+ a b))
sum
[4]>> (sum 2 2)
4

etc...
```

This mode is useful for testing new ideas and seeing the immediate evaluation of your queries

## More Info to Come...
