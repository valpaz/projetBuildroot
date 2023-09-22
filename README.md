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
-Cmake (cmake)  
-curl (libcurl4-openssl-dev)  
-cryptopp (libcrypto++-dev)  
-libGD (libgd-dev)  


## Utilisation

**Compilation use CMakeLists.txt :**  
>mkdir build  
>cd build  
>cmake ..  
>make  

This program is intended to be used with cron as it can be launch every hours like this :  
*1 * * * * /your/path/updateEvent*  
*1 * * * * /your/path/graph*  
(don't forget to launch *launcher* first as it initiate the whole program.

**But you can use it without it too**, just by executing :  
 -*Launcher*  
 -*updateEvent* when you want to update your data  
 -*graph* when you want to retrieve new graph from your data  

