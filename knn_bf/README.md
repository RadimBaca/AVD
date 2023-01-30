# Brute force KNN

Překlad s pomocí CMake:

```bash
$ mkdir -p build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make -j8
```

## Řešení

Řešení postavte na setřízeném poli, kde bude jen k nejbližších sousedů. Sekvenčně projdete pole vektorů a pro každý vektor dat spočítáme vzdálenost vektoru k dotazu. Vektor dat na `i`-té pozici nalezneme s pomocí příkazu `data[i * dim]`. Pro počítání vzdálenosti můžete využít funkci `distance`, která se v kódu již nachází.

Implementace naměří průměrný čas dotazu a také spočítá recall, který by měl být jedna.
