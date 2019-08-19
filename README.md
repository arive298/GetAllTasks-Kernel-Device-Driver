# GetAllTasks-Kernel-Device-Driver
Uses the kernel macro for_each_task() to access every task and obtain specific fields of the task_struct
To compile the device driver run the following commands
'''
sudo su
make
'''

## Installing requirements for building driver on raspberri py

- sudo su
- apt-get update && apt-get upgrade -y
- apt-get install bison flex libss-dev
- rpi-update
- wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
- rpi-source
