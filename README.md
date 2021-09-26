# Jester-Script
 A Lisp-inspired scripting language
***

## What is This Project

I've restarted Jester Script multiple times now (1st iteration is private and stashed, but 2nd iteration is still public, but archived). This being the 3rd iteration I finally came to understand and embrace the power of a Lisp-inspired scripting language and from there the rest is easy...

I hope to reintegrate ```C-Bridge Classes/Functions``` again soon, which is a feature I successfully coded in the 2nd iteration through a lot of templating magic (you can check the code out there if you're interested)

The end goal of Jester Script is to integrate it with my potential game engine -- Jester Engine. That's why I started this scripting language in the first place, yet it took ver and pushed the engine aside, which I'm completely okay with. Jester Script has grown to become it's own unique entity, and I'm plan on progressively improving. (Jester Engine is a public repository, but I wouldn't really recommend looking at the code...)

I hope to make Jester Script cross platform for Windows, Mac and Linux. Regardless of whether people find interest in it or not, I also plan on starting a small devlog-YouTube channel to improve myself as a programmer, get other people's opinions and meet other people who share my interests in programming 
***

## What is Lisp?

### Structure
Lisp (list-processing) is a very simple langugae in concept.  
All of Lisp is structured using one syntax: ```(action -> arguments)```  This translates to ```(print 1)``` or ```(+ 1 2 3)```

Additionally, all strctures are ```Objects```. Since every item is an object, I refer to variables and such as ```symbols```, as variables become literal key-value pairs between a given ID and any possible data in Jester Script
***

### Typing
The second part of Lisp is **typing** and a little thing called a ```progn```. In Jester-Script specifically, you can introduce a new variable by simply typing out a variable name:

``` 
x            # initiates new symbol 'x' with value 'null' simply by writing it out 
(set x 10)   # sets x to an integer '10'
(set x 10.5) # sets x to float '10.5'

(set y true) # initiates new symbol 'y' and also sets it to a boolean 'true'
```

Now, ```progn```'s

Imagine writing a loop in a modern language (C/C++/C# etc...):
```
for (int i = 0; i < 10; ++i)
{...}
```

now imagine that you want to return a number from this ```for``` loop... well... you can't really

what you *can* do is encapsulate the loop in a function with a static return type and return a number from there:

```
int GetMagicNumber() {
 for (int i = 0; i < 10; ++i) {
   return 42;
 }
}
```

Even in Python, where types aren't statically enfored, this is impossible... Enter ```progn```

This idea of ```progn``` is essentially this: *return the last thing you evaluated*
Now what does that mean?

Imagine we had this loop in Jester-Script

```
(set i 0)

(loop (< i 10) 
 (++i))
```

Now, although I'm not directly specifying any return value, I still return... the last thing I evaluate. In this case I would iterate through the loop until ```i``` reaches 11, at which point ```(++ i)``` would be the last thing I evaluated and therefore my return value.

So I could say this:

```
(set i 0)

(set val (loop (< i 10) 
 (++i)))
```

and it would work just fine
***

### REPL Mode

Building off of ```progn```s is the REPL-mode, or the "Read-Evaluate-Print-Loop" mode. Remember that *everything* has a return value. this means that you could write out any function to the parser, and get in response some sort of fitting evaluation.

In REPL mode, you're directly speaking and seeing what the parses sees.
When you write to the console, it evaluates and returns the ```progn``` of your statement:

```
[1]>> (+ 5 5)
10 # response
[2]>> (set x (+ 5 5))
10
[3]>> x
10
[4]>> (++ x)
11
[5]>> (println x)
11
11
```

Notice the last exampe: ```(println x)``` and how it outputs the same answer twice. The issue here is that there is no need for a print statement, it is obselete. So what happens is that ```(println)``` prints ```x``` to the console, returns ```x```, and then ```x``` is printed again due to the REPL integration
***

### Quoting

The last pillar of Lisp - quoting. 
If you're familiar with C or C++ you might have a slight idea of what Quotes are -- Macros.

Lets say you wanted to write out literally 2 of some number:

```
//C++ code
#define TWO_OF(num) num num

TWO_OF(2) // expands to '2 2'
```

When calling a macro like ```TWO_OF```, you essentially ask the processor to make textual subtitution, and to write out what's literally there, or ```num num```.

Quotes are very much alike this but much more integrated into the actual processing of Lisp, rather than being just textual insertions.

Let's say I want to create a function that evaluates a statement only if a conditional is correct:

```
(defn when (cond result) 
 (if cond
  (eval result)
  nil))
```

Now's important to remember how ```progn```s work. Essentially I'm saying:

1) create function "when"
2) "when" takes two parameters: a condition and a result
3) if conditionaly is true, evaluate (and implicity therefore return) the result
4) otherwise return nil

Now let's call our function:

```
(when false (print false))
```
Well, there's an issue.

The result parameter, ```(print false)``` evaluates immediately and prints 'false', regardless of if its evaluated or not.

This is because when you pass parameters to functions, they get evaluated immediately. The pupose of quoting is to tell the parser, 'hey, don't evaluate this unless I expressly tell you to' (more or less).

When you create a quote: ```(quote (+ 1 2 3))``` like this it creates a list of quoted values. Since '+' is now quoted, its type is not a function but rather just type 'quote', therefore the list ```(+ 1 2 3)``` is not evaluated and remains just a list (remember that everything here is made of Objects, which means that you can create, manipulate and evaluate lists at runtime and treate them as if they were just regular code)

Back to the example, let's try and pass in a quote for our problematic ```print``` statement:

```
(when false (quote (print false)))
```

and just like that we get our expected result. ```(print false)``` here is evaluated if and only if the conditional is true rather than being evaluated when passed into the function... and the rest is up to you...
***

## Some More Things

- You can run Jester Script in REPL mode by running the executable in the root directory/releases
     - Some special REPL operations ('--' denotes special operation):
     - --parse  params: path --> takes path, compiles and runs corresponsing .jts file

## Goals

- [x] Quoting  
- [x] Lists  
- [x] User-Functions  
- [ ] Scoping  
- [ ] C-Bridge Functions  
- [ ] User-Defined Classes  
- [ ] C-Bridge Classes  
- [ ] Memory management (reference counting)  
