# Brute force KNN

Překlad s pomocí CMake:

```bash
$ mkdir -p build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make -j8
```

## Řešení

Řešení postavte na setřízeném poli, kde bude jen k nejbližších sousedů. Sekvenčně projdete pole vektorů.
