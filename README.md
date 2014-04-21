steins-gate
===========

steins-gate is free (GPL'd) reimplementation of Steins;Gate visual novel.
This project only aims to replace executable; you will still have to buy a copy of game files.

Building
--------

It is the choice of the Steins Gate if you can build from master branch today:
[![Travis Build Status](https://travis-ci.org/FGRE/steins-gate.svg?branch=master)][1]

    $ git clone https://github.com/FGRE/steins-gate.git
    $ cd steins-gate

optinally, check out latest tag, if master branch status is red:

    $ git checkout `git describe --tags --abbrev=0`

now compile:

    $ cmake .
    $ make

[1]: https://travis-ci.org/FGRE/steins-gate
