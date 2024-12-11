C++ Compiler for My Test Language 
=========================

Compiles "Testing" language for x86 native machines.


Building
---

Just run CMake like this:

```sh
# in build dir
cmake <src-dir>
```

and then

```sh
cmake --build . --target main
```


Usage
---

Run a simple example:

```sh
main "file"
```

