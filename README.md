# Modeling and Analysis of Concurrent Systems 2 Lab

## Introduction
This repository contains all files necessary for the MACS2 lab classes.
This README will first explain how to configure this autotools project
on your machine. Then it will explain how to compile the project.
When successful, you should be able to load a Petri net and
print the size of the Petri net specification.
Do not hesitate to send me an e-mail at j.j.g.meijer [at] utwente [dot] nl,
if you at any step below run into an issue. Furthermore, there is also a
[Travis CI build script](https://github.com/Meijuh/MACS2/blob/master/.travis.yml).
which may help guide you in your journey described in the next sections 
(Build statusses, with expected output can be found e.g. 
[here](https://travis-ci.org/Meijuh/MACS2/builds/227652263)).

## Configuring
To configure this autotools project on your machine, make sure
to meet the following requirements:
 *  Ubuntu 64-bit >= 14.04, (or any other Linux distro with similar
    packages). If you do not have Linux installed, easiest is to download 
    a Virtual box virtual machine from http://www.osboxes.org/ubuntu/.
    Furthermore, any recent enough OSX version also works for MACS2.
 *  This project depends on the following packages:
    * GNU automake: 
    * GNU autoconf:
    * GNU libtool:
    * flex:
    * bison:
    * pkgconf:
    * GNU make:
    * libnuma: http://oss.sgi.com/projects/libnuma/
    * cmake (>= 3.0)
    * gmplib
    * hwloc

The easiest way to install these packages (on Ubuntu) is by running:
 * `apt-get install build-essential bison flex automake autoconf pkgconf
   libnuma-dev libhwloc-dev libgmp-dev`.

If you are using Ubuntu 14.04 you need to add an extra repository to
get a recent enough version of `cmake` do:
 * `add-apt-repository ppa:george-edison55/cmake-3.x`
 * `apt-get update`
 * `apt-get install cmake`    

## Building
We first compile and install the dependency Sylvan, next we will compile this
project.

### Sylvan
 * Download the Sylvan 1.2.0 release from 
    https://github.com/utwente-fmt/sylvan/releases: 
    `wget https://github.com/utwente-fmt/sylvan/archive/v1.2.0.tar.gz`
 * Extract: `tar xf v1.2.0.tar.gz`
 * Enter Sylvan directory: `cd sylvan-1.2.0`
 * Create build directory: `mkdir build`
 * Enter build dir: `cd build`
 * Configure Sylvan: `cmake .. -DBUILD_SHARED_LIBS=OFF  
    -DSYLVAN_BUILD_EXAMPLES=OFF -DSYLVAN_BUILD_DOCS=OFF`
 * Compile Sylvan: `make`
 * Install Sylvan: `make install`

### MACS2 lab files
 * Clone this git repository: `git clone https://github.com/Meijuh/MACS2.git`
 * Enter the git repository: `cd MACS2`
 * Generate config files: `./macs2reconf`
 * Configure lab class: `./configure`
 * Compile the lab files: `make`
 * Parse a simple Petri net in ANDL format: `src/macs2 examples/model.andl`,
    if successful you should see something similar to:        

        2017-05-01 16:38:45: Successful parse of file 'examples/model.andl' :)         
        2017-05-01 16:38:45: The name of the Petri net is: Philosophers_PT_000005
        2017-05-01 16:38:45: There are 25 transitions
        2017-05-01 16:38:45: There are 25 places
        2017-05-01 16:38:45: There are 45 in arcs
        2017-05-01 16:38:45: There are 35 out arcs

## Exercise 1
Read Sylvan's documentation at: https://trolando.github.io/sylvan/.

## Exercise 2
Familiarize yourself with
[Petri nets](https://en.wikipedia.org/wiki/Petri_net).
In this course, many example Petri nets are provided as
[PNML](http://www.pnml.org/), and 
[ANDL](http://www-dssz.informatik.tu-cottbus.de/track/download.php?id=187).
The latter URL contains a document containing an EBNF grammar for
Petri net specifications, it is only for reference, you do not need to read it 
now. 

## Exercise 2
The first programming exercise is to compute the marking graph symbolically,
this means computing the state space with BDDs. The main idea is to use Sylvan's
[sylvan_satcount](https://github.com/utwente-fmt/sylvan/blob/v1.2.0/src/sylvan_bdd.h#L161)
function, to compute the number of markings, and
[sylvan_relnext](https://github.com/utwente-fmt/sylvan/blob/v1.2.0/src/sylvan_bdd.h#L111)
to compute the next marking of a particular transition.
It is advised to "consult"
[this file](https://github.com/utwente-fmt/sylvan/blob/v1.2.0/examples/mc.c),
which contains code for computing a state space.

The lab files in this repository already provide three important aspects:
 * A lexer for ANDL: https://github.com/Meijuh/MACS2/blob/master/src/andl-lexer.l
 * A parser for ANDL: https://github.com/Meijuh/MACS2/blob/master/src/andl-parser.y
 * Some borderplate code for printing the size of the Petri net specification:
   https://github.com/Meijuh/MACS2/blob/master/src/macs2.c.

For MACS2 we think it is much easer to extend the ANDL parser, than it is to
create a PNML parser. Feel free to create a PNML parser though.

On Blackboard, under `Course Materials | Lab files | Petri nets` many
example Petri nets have been made available in an archive. Every directory
in the archive contains a Petri net specification. Important files for
this exercise in each directory are:
 * `1-safe`: if this file exists the Petri net is 1-safe.
 * `model.pnml`: the Petri net specified in PNML, which is not irrelevant if
   you use the ANDL parser.
 * `model.andl`: the Petri net specified in ANDL.

Whenever you have finished writing your symbolic state space generator you
can consult
[raw-results-analysis.csv](https://github.com/Meijuh/MACS2/blob/master/MCC/raw-result-analysis.csv).
This CSV file, contains all known answers from last year's
[Petri net Model Checking Contest](http://mcc.lip6.fr/), more specifically the 
column:
 * Input: the name of the Petri net. **Note** the Petri nets with `-COL-`,
   are irrelevant for this MACS2 course.
 * estimated result: contains the known answer for Petri nets.
 * Examination: the category to which the answer belongs.

Relevant for this exercise is the `StateSpace`, and `Examination` column.
For example, if we want to know the size of the marking graph of a Petri net
specification with 5 dining philosophers, we look at:
 * Input = Philosophers-PT-000005,
 * Examination = StateSpace,

This shows *243 945 1 10* in the column *estimated result*, meaning that
 1. the marking graph has 243 vertices,
 1. the marking graph has 945 edges,
 1. the maximum number of tokens that any place can have is 1 (hence
    it is 1-safe),
 1. the maximum sum of tokens never exceeds 10 tokens.

So, if you want to verify if the answer given by `sylvan_satcount` is correct,
you are interested in the first value (243).

The exercise is now as follows.
 1. Think about what data structures you require while parsing the
    ANDL files, e.g. what do you need to map names of places to BDD variables,
    or how are you going to store the initial marking?
 1. Write/download whatever code for these datastructures is necessary, and
    declare those data structures in
    [`andl_context_t`](https://github.com/Meijuh/MACS2/blob/master/src/andl.h),
    feel free to change any existing code there.
    The `andl_context_t` is a structure that is available while parsing ANDL files.
 1. Modify the [parser](https://github.com/Meijuh/MACS2/blob/master/src/andl-parser.y)
    to fill the data structures accordingly.
 1. Implement a *Breadth-first* symbolic state space generator, starting
    [here](https://github.com/Meijuh/MACS2/blob/master/src/macs2.c#L79).
 1. Make sure the return value of `sylvan_satcount` corresponds with the known
    answers in `raw-result-analysis.csv`, for as many Petri nets as possible
    (which you downloaded from Blackboard).
