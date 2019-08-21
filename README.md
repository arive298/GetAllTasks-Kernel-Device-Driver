# GetAllTasks-Kernel-Device-Driver
Uses the kernel macro for_each_task() to access every task and obtain specific fields of the task_struct.
To compile the device driver run the following commands from within the folder. Note: Before compiling make sure you already have the kernel headers(Explained in the next bottom section).
```
sudo su
make
```

## Installing requirements for building driver on raspberri py
For all the command you have to be the root account.
First, you need to get the latest update. 
```
- sudo su
- apt-get update && apt-get upgrade -y
```
Then install the necessay dependencies.
```
- apt-get install bison flex libss-dev
```
Update the Kernel using the rpi-update tool
```
- rpi-update
- wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
```
Install the headers
```
- rpi-source
```
