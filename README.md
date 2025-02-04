# SV-Project

This project is based on a project made by [Guillermo Perez](https://github.com/gaperez64/).

See: [HOA-TOOLS](https://github.com/SYNTCOMP/hoa-tools).

## Scripts

### How to run

Small code example to test.

```sh
cd src/scripts

./main.sh
```

### How to test

The test script is used to run all the examples files provided in the repository. It will give an output with an duration of how long this process took. Not only that, it will also provide an output stating if Eve can win in this parity game.

```sh
cd src/scripts

./test.sh
```

## Code explained

In the `tools/` directory you can find several libraries made making use of `spot`.

The parser will parse any hoa file and return an automaton. Several values are manually parsed because these values couldn't be find in spot or at least I didn't know how.

These values can be inserted in the solver to create an arena and then we solve this afterwards.

There is also an arena render library that shows a simplified version of the arena. The `main.cc` file has an small code explanation for you to play with.

We also have a benchmarking tester, this needs an input directory where all the files are placed that needs to be tested. Be aware that there is no file testing, in case these files can be parsed or not.

## Packages

These are some packages I made use of. The BuDDy package is preinstalled with spot, so that should work without installing BuDDy itself.

1. Spot parser

2. BuDDy BDD objects

## Help

Check the `CmakeList.txt` just in case to check where I install the packages. In case this is different from your download path, make sure you change this before running the scripts or else this will not work. I installed based on the installation guide of Spot, so make sure you check that out first before running the project.
