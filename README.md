# Peridot
Pronounced "pair-uh-dot". A toy language experiment.

## Building from source
You'll need:
* `bison` 3.0.4+ (verify by running `bison --version`)
* `flex` 2.6.0+ (verify by running `flex --version`)
* `gcc` 5+ (required for c++14 support; verify by running `gcc -v`)

Execute
```
make parser && make lexer && make all
./peridot
```
