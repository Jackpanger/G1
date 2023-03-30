# Solution for Problem 1

## Part 1

#### i.  

**Ans:** 

>t' = $\lambda$ a. ($\lambda$ b. a ($\lambda$ c. ($\lambda$ d. d) b)) z ($\lambda$ e. e x) a

#### ii.

**Ans:** 

>As shown above, variables not being renamed are free variables, i.e., z x 

## Part 2

*`iszero` (`mult` `0` `1`) `2` `3`*

= *(λ n. n (λ x. `false`) `true`) (`mult` `0` `1`) `2` `3`*

→ *((`mult` `0` `1`) (λ x. `false`) `true`) `2` `3`*

= *(((λ m n. m (`plus` n) `0`) `0` `1`) (λ x. `false`) `true`) `2` `3`*

→ *((`0` (`plus` `1`) `0` ) (λ x. `false`) `true`) `2` `3`*

= *((λ s z. z) (`plus` `1`) `0` ) (λ x. `false`) `true`) `2` `3`*

→ *(`0` (λ x. `false`) `true`) `2` `3`*

= *(λ s z. z) (λ x. `false`) `true`) `2` `3`*

→ *`true` `2` `3`*

= `2`


## Part 3

*`exp`*  = *(λ m n. n (`mult` m) `1`)*