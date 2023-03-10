# Brute force KNN

Překlad s pomocí CMake:

```bash
$ mkdir -p build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make -j8
```
Vašim úkolem je dopsat metodu `brute_force`, která hledá k nejbližších sousedů pro dotaz.

## Řešení

Řešení postavte na setřízeném poli, kde bude jen k nejbližších sousedů. Sekvenčně projdete pole vektorů a pro každý vektor dat spočítáme vzdálenost vektoru k dotazu. Vektor dat na `i`-té pozici nalezneme s pomocí příkazu `data[i * dim]`. Pro počítání vzdálenosti můžete využít funkci `distance`, která se v kódu již nachází.

Implementace naměří průměrný čas dotazu a také spočítá recall, který by měl být jedna.

## Bonus task

Zkuste změnit datový typ prvku vektoru z `float` na `int8` a změřte co to udělá s průměrným časem jednoho dotazu a recall. 
