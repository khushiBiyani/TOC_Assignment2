# TOC Assignment 2

Our submission for the second assignment done under the course Theory of Computation @ BITS Pilani, Hyderabad Campus

## Group Members

- Ankesh Pandey: 2020A7PS0104H
- Khooshrin Aspi Pithawalla: 2020A7PS2067H
- Khushi Biyani: 2020A7PS0194H
- Kavyanjali Agnihotri: 2020A7PS0185H
- Tushar Brijesh Chenan: 2020A7PS0253H

## Grammer Of Basic C

The **Formal Context Free Grammer** of our language is:

$$ G = (V, T, P, S) $$
$$ V = \{ PR, FS, MS, SS, FOR, AWS, A, EWS, E, RE, V, T, F, W, R, D, VL\} $$
$$ T = \{;\} \cup \{,\} \cup \{SPACE\} \cup \{int, for, read, write, +, -, *, /, >, ==,\} \cup \{(\} \cup \{)\} \cup \{\{\} \cup \{\}\} \cup \{VAR, INT\_LITERAL\} $$
$$ S = PR $$

The elements of $V$ are correlated with the following real world entities:

$$ PR = Program  $$
$$ FS = First \ Statement $$
$$ MS = Many \ Statements $$
$$ SS = Single \ Statement $$
$$ FOR = For \ Statement $$
$$ AWS = Assignment \ Statement \ Without \ Semicolon $$
$$ A = Assignment \ Statement \ With \ Semicolon $$
$$ EWS = Expression \ Without \ Semicolon $$
$$ E = Expression \ With \ Semicolon $$
$$ RE = Relational \ Expression $$
$$ V = Value $$
$$ T = Term $$
$$ F = Factor $$
$$ W = Write \ Statement $$
$$ R = Read \ Statement $$
$$ D = Declaration \ Statement $$
$$ VL = Variable \ List $$

The elements of $T$ are correlated with the following real world entities:

$$ VAR = Variable \ token $$
$$ INT\_LITERAL = Integer \ Constant $$
$$ SPACE = Space \ Token $$

The regular expression for $VAR$ is: `\b(?!(for|int|read|write)\b)[a-z]+\b`

We don't have to handle the case of $for$, $int$, $read$ or $write$ explicitly because we are **manually checking** for those while tokenizing.

The regular expression for $INT\_LITERAL$ is: $\{0-9\}^{+}$

The set of **Productions** $P$ is:

$$ PR \rightarrow FS \ | \ FS \ MS $$
$$ MS \rightarrow SS \ | \ SS \ MS $$
$$ FS \rightarrow D \ | \ R \ | \ W \ | \ FOR \ | \ A $$
$$ SS \rightarrow R \ | \ W \ | \ FOR \ | \ A $$
$$ FOR \rightarrow for \ ( A \ E \ AWS \ ) \ \{MS\} \ ;$$
$$ W \rightarrow write \ SPACE \ VAR \ ; \ | \ write \ SPACE \ INT\_LITERAL \ ;$$
$$ R \rightarrow read \ SPACE \ VAR \ ; $$
$$ D \rightarrow int \ SPACE \ VL \ ;$$
$$ VL \rightarrow VAR \ | \ VAR \ , \ VL$$
$$ AWS \rightarrow VAR \ = \ EWS $$
$$ A \rightarrow AWS; $$
$$ EWS \rightarrow RE \ | \ RE == EWS $$
$$ RE \rightarrow V \ | \ V > RE $$
$$ V \rightarrow T \ | \ T + V \ | \ T - V $$
$$ T \rightarrow F \ | \ F * T \ | \ F / T  $$
$$ F \rightarrow VAR \ | \ INT\_LITERAL \ | \ (EWS) $$
$$ E \rightarrow EWS \ ; $$

## Run Locally

Clone the project

```bash
  git clone https://github.com/khushiBiyani/TOC_Assignment2.git
```

Go to the project directory

```bash
  cd TOC_Assignment2
```

Compile the main file

```bash
  gcc main.c -o main
```

Run the app

```bash
  ./main input.txt
```
