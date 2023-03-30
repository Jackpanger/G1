## Homework 1 (20 Points)

The deadline for Homework 1 is Tuesday, September 13, 10pm. The late
submission deadline is Monday, September 19, 10pm.

### Getting the code template

Before you perform the next steps, you first need to create your own
private copy of this git repository. To do so, click on the link
provided in the announcement of this homework assignment on
Brightspace. After clicking on the link, you will receive an email from
GitHub, when your copy of the repository is ready. It will be
available at
```https://github.com/nyu-pl-fa22/hw01-<YOUR-GITHUB-USERNAME>```.
Note that this may take a few minutes.

Here are some Git-related resources:
* If you are unfamiliar with Git, watch the [first two git basics video](http://git-scm.com/videos).
* If you are unfamiliar with Github, watch [this YouTube video](https://www.youtube.com/watch?v=0fKg7e37bQE).
* A [simple git cheatsheet](http://rogerdudler.github.io/git-guide/).
* A [complete reference](http://www.git-scm.com/book/en/v2).
* I suggest using the command line to interact with Git/Github, but in a pinch [this GUI](https://desktop.github.com/) might be useful.

* Open a browser at `https://github.com/nyu-pl-fa22/hw01-<YOUR-GITHUB-USERNAME>` with your Github username inserted at the appropriate place in the URL.
* Choose a place on your computer for your homework assignments to reside and open a terminal to that location.
* Execute the following git command: <br/>
  `git clone https://github.com/nyu-pl-fa22/hw01-<YOUR-GITHUB-USERNAME>.git`<br/>
  `cd hw01`


### Submitting your solution

Put your solution into a plain text file called `solution.md` or a PDF
file called `solution.pdf`. Readable scans or photos of hand-written
solutions can be submitted as PDF.

Once you have completed the assignment, you can submit your solution
by pushing the modified code template to GitHub. This can be done by
opening a terminal in the root directory of your local copy of the
repository and executing the following commands in the terminal:

```bash
git add .
git commit -m "solution"
git push
```

You can replace "solution" by a more meaningful commit message.

Refresh your browser window pointing at
```
https://github.com/nyu-pl-fa22/hw01-<YOUR-GITHUB-USERNAME>/
```
and double-check that your solution has been uploaded correctly.

You can resubmit an updated solution anytime by reexecuting the above
git commands. Though, please remember the late submission penalty
rules.

Please note that we will be able to track commit times via GitHub
Classroom. Any attempt to manipulate your repository's commit history
to hide a late submission will be detected and treated as a violation
of academic integrity.


### Problem 1: Regular Expressions (12 Points)

For all subproblems below we consider a fixed alphabet consisting of the set of all unicode characters.

Write regular expressions to capture the following.

1. Identifiers in C. An identifier starts with a letter `A` to
  `Z`, `a` to `z`, or an underscore `_` followed by zero or more letters,
  underscores, and digits (`0` to `9`). **(2 Points)**

2. US phone numbers. These consist of a three digit *area
   code* whose first digit may not be a `0` or `1`, a three digit
   *central office code* whose first digit may not be a `0` or `1`,
   and a four digit *subscriber number*. The area code may be
   enclosed by optional parenthesis and the digit groups are separated
   by either a dash `-` or a dot `.` symbol. For instance, the
   following are valid phone numbers: `(234)-567-8901` and
   `234.567.8901` while the following are not:
   `(123)-456-7890` and `234.567-8901`. **(4 Points)**

3. String literals in Java. These are (possibly empty) sequences of unicode characters
   delimited by double-quotes `"` that satisfy the following additional
   constraints:
  
   A.  The backslash character `\` may only occur in one of the
       following *escape sequences*
       <ul>
       <li>`\b` `\t` `\n` `\f` `\r` `\"` `\'` `\\`</li>
       <li>a unicode escape which takes the form `\uhhhh` where each
         `h` stands for a hexadecimal digit (`0` to
         `9`, `A` to `F`, `a` to `f`). Example: `\u10ff`</li>
       <li>an octal escape which take the form `\o`, `\oo`, or `\too` where `t`
         is one of `0`, `1`, `2`, `3` and each `o` is an octal digit (`0` to
         `7`). Examples: `\237`, `\7`.</li>
       </ul>

   B.  Any double-quote occurring between the delimiting
       double-quotes must be preceeded by a backslash.

   C.  The final delimiting double-quote must not be preceded by a
       backslash unless that backslash is part of an escape sequence `\\`.

   D.  There must be no linefeed characters (`'\n'`) or
       carriage return characters (`'\r'`) in the string, neither the
       unicode escape sequences `\u00ff` and `\u000a` of these
       characters are allowed to occur. **(6 Points)**

Please use the syntax of regular expressions provided in the [course
notes](https://github.com/nyu-pl-fa22/class01/blob/main/syntax.pdf)
and
[slides](https://github.com/nyu-pl-fa22/class01/blob/main/class01.pdf)
of Class 1. In particular, do not use any extensions that some regular
expression libraries provide such as look-ahead/behind.

However, you *are* allowed to use the following notational shorthands:

* You can use the dot symbol `.` as a shorthand for a *wildcard*
  regular expression that stands for any symbol in the considered
  alphabet.

* You can use notation `[a-z]`, `[0-9]` etc. for regular expressions
  that match any character in the specified character range.

* You can use the notation `[^a1,...,an]` as shorthand for a regular
  expression that stands for any symbol in the alphabet that is not
  one of the symbols `a1`,...,`an`. For example, if the alphabet is
  the set of all unicode characters, then `[^0,1]` matches any unicode
  character except `0` and `1`. Note that the `a1` to `an` can only be
  symbols from the alphabet and not general regular expressions.

* You may use shorthand definitions to give names to regular
  expressions that you can then use to build more complex regular
  expressions.
  
  Example: `Digit ::= [0-9]`

**Hint:** Use `'.'` to denote the character (i.e., terminal symbol)
`.` to avoid confusion with the dot symbol used to denote an arbitrary
character in a regular expressions. Similarly, use the notation `'\n'`
to denote the linefeed character (in contrast to `\n` which stands for
the sequence of the two characters `\` and `n`). Use similar notation
for carriage return characters `'\r'`.


### Problem 2: Context-Free Grammars (8 Points)

1. Consider the following context-free grammar with start symbol `S`,
   non-terminal symbols `{S,M,A,B,C}`, terminal symbols `{a,b}`, and productions:
   ```
    S ::= MB 
    M ::= S | ε
    A ::= bAA | aC
    B ::= bC | aBB
    C ::= bA | aB | ε
   ```
   A.  Describe in English (or using mathematical notation) the
       precise language that the grammar generates. That is, what are the
       words that `L(G)` contains?</br>      
       **Hint:** As another example, consider the grammar:</br>
         `S ::= aSa | ε`</br>
       Here, the language of the grammar consists of all words over the terminal symbol `a` that have even length.
       Your task is to identify a similar property describing the words in the language generated by `G` above.</br>
      
   B.  Show a parse tree for the string `bbab`. **(2 + 3 Points)**

2. Consider the following context-free grammar for describing an
   expression language built from the binary infix operators `-`, `+`,
   `*`, `/`, and `**`:
   ```
    E ::= EOE | (E) | N
    O ::= + | - | * | / | **
    N ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 
   ```

   Show the (unique) parse tree for the word `3 + 5 * 6 / 8 ** 2 ** 2` under
   the following assumptions:
   
   A.  `**` has precedence 1
   
   B.  `*,/` have precedence 2
   
   C.  `+,-` have precedence 3
   
   D.  `**` is right-associative, and
   
   E. all other operators are left-associative.
  
   Here operators with smaller precedence values have *higher*
   precedence (i.e. bind stronger) than operators with larger
   precedence values.  **(3 Points)**

#### Hint

The following website provides a web interface for a tool that you can
use to play with context-free grammars:

https://web.stanford.edu/class/archive/cs/cs103/cs103.1156/tools/cfg/

In particular, you can use the tool to see the derivation that shows
whether a given word is in the language defined by a given grammar.
