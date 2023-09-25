# Projet Buildroot : follow crow movement

This projet use the movement data of a population of crow from Paris region using *moveBank.org*.
Every hour, new data are retrieve and a new PNG file is created for each individual.
It aim to see the live movement of this population of crow.

[Installation](#installation)
[Utilisation](#utilisation)
# Installation

**Clone**
```
git clone https://github.com/valpaz/projetBuildroot.git
```
**C++ library dependance:**  
-Cmake (cmake)  
-curl (libcurl4-openssl-dev)  
-cryptopp (libcrypto++-dev)  
-libGD (libgd-dev)  
-cron

# Utilisation


## **- On your linux :**
First you need to compile :  
>mkdir build  
>cd build  
>cmake ..  
>make  

Now :  
>./launcher  
>./updateEvent 
(when you want to update your data)  
>./graph 
(when you want to retrieve new graph from your data)  

## **- On your embedded machine : (here an exemple with a virtual ARM64 architecture (with Qemu) build with buildroot)**

**First you need to compile :**  

In the CMakeLists.txt, you need to set the path to your specific compileur. In this exemple :  
*set(CMAKE_CXX_COMPILER /path/to/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++)*  
>mkdir build  
>cd build  
>cmake ..  
>make  

**Setting up your modified file system :**  
In your buildroot folder :  
>mkdir overlay  

**Setting up your user :**  
in buildroot folder, do :  
>mkdir myConfig;cd myConfig  
>touch users.txt  

Here you need to add the user you want to create for your machine.  
For exemple :   
*username 1000 username 1000 =password /home/username /bin/sh -Raspberry Pi User*  
with this done, do :  

>make xconfig  

Search BR2_ROOTFS_USERS_TABLES and add the path to users.txt.  
Now in your overlay folder :  

>mkdir home;cd home;mkdir username  

**copying your executables to your username folder**  

>cp graph launcher updateEvent ~/your/buildroot/overlay/home/username/  
>cp map.png ~/your/buildroot/overlay/home/username/  

**Setting up cron**  
Next, you need to add cron so that it can update data every hour and create new graph.  
First, in your overlay folder :  

>mkdir etc;cd etc;mkdir init.d;cd init.d  
>touch S80Cron  

And add :  
*#!/bin/sh*  
*/usr/sbin/crond*  

Now,  
>mkdir var;cd var;mkdir spool;cd spool;mkdir cron;cd cron;mkdir crontabs  

Here you need to create a file named by the username you just create and will log in with. So in crontabs folder :  

>touch user  
In the file, add this :  
*0 * * * * /your/home/path/updateEvent*  
*0 * * * * /your/home/path/graph*  
This will make your updateEvent and graph execute every hours.   

And you are done.  
Now in your embedded machine, you just have to launch ./launcher one time from your HOME folder and the cron will launch both updateEvent and graph every hours. The data are stored in moveBankProjet folder. You will just have to retrieve the .png using ssh for exemple.  

Note : If you use a virtual machine, you could need to add some RAM in your start-Qemu file : -m 512   


