
a b 
;; a & b are nil

(set a 10) 
;; a is 10

(defun add (a b) 
	(+ a b))	
;; a & b point to same heap data
;; as the a & b declared previously
	
(add 10 20) 
;; this sets a to 10 and b to 20
;; after returning 30, the function 
;; sets a back to 10 and b back to nil

(let 
	(a 10
    b 20)
	
	(+ a b))
;; same as above, a and b 
;; are altered then set back 
;; to their previous values 
;; after exit

(setc c 20)
;; const variable c

(let 
	(c 10)
	
	(+ c 100))
;; 'let' doesn't care what c
;; was before, it sets it, and
;; after execution revertes it 
;; back to 'const 20'

(def *variable* 100)

(let 
	(*variable* 10)
	
	(+ *variable* 10))
;; this would raise an error
;; '*variable*' is dynamically
;; scoped so its impossible to
;; redefine it locally 

(let 
	(set nil)
	
	(not (= set nil)))
;; furthering the last example, the 
;; function 'set' is dynamically scoped
;; and cannot be redefined locally
;; otherwise the local scope would be 
;; unable to utilize the 'set' function

(let 
	((x 10)
	 (y 20))
	
	(+ x y))
	
(defun func ((a 1) (b 2) c) ;; a = 1, b = 2, c = nil
	(...))
	
;; if local value is in parenthesis then 
;; the second value can be assumed to be
;; its designated value, if value is not
;; in a list then its initial value will be nil

(defun bizbuz (range)
	(let
		((n 0))
		
		(loop (< n range)
			(print n " - ")
			(if (= (mod n 3) 0) (print biz) nil)
			(if (= (mod n 5) 0) (print buz) nil)
			(print "\n")
			(+= n 1))))