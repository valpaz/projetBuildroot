# Projet Buildroot : follow crow movement

This projet use the movement data of a population of crow from Paris region using *moveBank.org*.
Every hour, new data are retrieve and a new PNG file is created for each individual.
It aim to see the live movement of this population of crow.

[Installation](#installation)
[Utilisation](#utilisation)
## Installation

**Clone**
```
git clone https://github.com/valpaz/projetBuildroot.git
```
**C++ library dependance:**
-curl (libcurl4-openssl-dev)
-cryptopp (libcrypto++-dev)
-libGD (libgd-dev)

## Utilisation

**Compilation :**
g++ -std=c++17 -o testGraph graph.cpp -lgd  
g++ -std=c++17 -o testP main.cpp -lcurl -lcryptopp  

This program is intended to be used with cron as it can be launch every hours like this :
*1 * * * * /your/path/main*
*1 * * * * /your/path/graph*

**But you can use it without it too**, just by executing *./main* then *./graph*

