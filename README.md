# Modeling and Analysis of Concurrent Systems 2 Lab

## Introduction
This repository contains all files necessary for the MACS2 lab classes.
This README will first explain how to configure this autotools project
on your machine. Then it will explain how to compile the project.
When successful, you should be able to load a Petri net and
print the size of the Petri net specification.
Do not hesitate to send me an e-mail at j.j.g.meijer [at] utwente [dot] nl,
if you at any step below run into an issue.

## Configuring
To configure this autotools project on your machine, make sure
to meet the following requirements:
 *  Ubuntu 64-bit >= 14.04, (or any other Linux distro with similar
    packages). If you do not have Linux installed, easiest is to download 
    a Virtual box virtual machine from http://www.osboxes.org/ubuntu/.
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
 * Clone this git repository
 * Enter the git repository
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


