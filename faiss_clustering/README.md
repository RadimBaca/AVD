# FAISS Clustering

Překlad s pomocí CMake:

```bash
$ mkdir -p build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make -j8
```
Vašim úkolem je dopsat rozdělení `sift_base.fvecs` vektorů do jednotlivých shluků a následně napsat dotazování, které bude shluky využívat:

1. Pro dotaz najde nejbližší centroid
2. Projde sekvečně vektory tohoto centroidu

