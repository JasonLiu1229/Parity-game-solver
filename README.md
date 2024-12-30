# SV-Project

This project is based on a project made by [Guillermo Perez](https://github.com/gaperez64/).

See: [HOA-TOOLS](https://github.com/SYNTCOMP/hoa-tools).

## Scripts

### How to run

```sh
cd src/scripts

./main.sh
```

### How to test

```sh
cd src/scripts

./test.sh
```

## Code explanationed

In the `tools/` directory you can find several libraries made making use of `spot`.

The parser will parse any hoa file and return an automaton. Several values are manually parsed because these values couldn't be find in spot or at least I didn't know how.

These values can be inserted in the solver to create an arena and then we solve this afterwards.

There is also an arena render library that shows a simplified version of the arena. The `main.cc` file has an small code explanation for you to play with.

We also have a benchmarking tester, this needs an input directory where all the files are placed that needs to be tested. Be aware that there is no file testing, in case these files can be parsed or not.

## Packages

Spot parser

BuDDy BDD objects
