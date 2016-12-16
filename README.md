# Peridot
Pronounced "pair-uh-dot". A toy language experiment.

## Building from source
You'll need:
* `bison` 3.0.4+ (verify by running `bison --version`)
* `flex` 2.6.0 (verify by running `flex --version`)
* `gcc` 4.7+ (required for c++11 support; verify by running `gcc -v`)

If your distribution features packages for older versions, build from source.

Run
```
make parser && make lexer && make all
./peridot
```
