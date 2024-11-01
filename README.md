# OS Assignment 2


## Pre-requisites

This usually occurs when the kernel headers haven't been installed properly.
Run in terminal:
```bash
sudo apt update
```
1. Check if kernel headers are installed:
    ```bash
    apt search linux-headers-$(uname -r)
    ```
    This command will search for the installed headers related to your kernel version. If it doesn't return anything, you may need to install the headers.

2. Install headers for specific kernel version:
    ```bash
    sudo apt install linux-headers-$(uname -r)

    ```

3. Verify installation
- List contents

    ```bash
    ls -l /lib/modules/$(uname -r)/

    ```
    Check for a `build` symbolic link pointing to the appropriate headers directory.
    
4. Create symbolic links
If `build` dir is still not available,manually create the symbolic links again after confirming the headers are installed: 
    ```bash
    sudo ln -s /usr/src/linux-headers-$(uname -r) /lib/modules/$(uname -r)/build

    ```

5. Prepare kernel headers
    ```bash
    cd /usr/src/linux-headers-$(uname -r)
    sudo make oldconfig
    sudo make prepare

    ```
6. Recompile the driver:
    Continue the steps as usual for installing the driver.
 
   
## Installation

### 1. Build the driver
Navigate to the folder `src` and run the command:
```bash
sudo make
```

We can now observe new files created in the folder. Check if `os_driver.ko` file is present.

### 2. Loading the driver
Run this command in terminal:
```bash
sudo insmod os_driver.ko kernel_version=6,1,112
```

Here, `6,1,112` was identified using `uname -a` command

### 3. Device Read/Write Operations

#### Using commands
For running the following commands, one might need sudo access. Run `sudo -s` and type in the password which will now allow us to run commands with sudo permissions.

1. To write into the device:
    ```
    echo “NANDANA_B220424CS” > /dev/os_driver_device
    ```
2. To read from the device:
    ```
    cat /dev/os_driver_device
    ```
#### Using C Program
The user program directly writes `NANDANA_B220424CS (<FIRSTNAME>_<ROLLNO>)` into the device. 
Run the following commands in the main folder to get the desired results:
```
gcc user.c
sudo ./a.out

```


### 4. Unloading the driver
To un-load the module, use rmmod command with root priveleges
```bash
sudo rmmod os_driver 
```
OR 
```bash
sudo rmmod os_driver.ko 
```

### Checks

1. After step 2, we can try running the command:
- List modules
    ```bash
    sudo lsmod
    ```
    This will list all the modules along with our module for the driver
    ```bash
    Module                  Size  Used by
    os_driver                 16384  0

    ```

- Run dmesg to see the log messages
    ```bash
    sudo dmesg
    ```

    Required output: 
    ```
    [ 1116.992705] Kernel version matches. Proceeding with module insertion.
    [ 1116.992940] Kernel Module Successfully Inserted..
    [ 1116.992944] MajorNo=237	MinorNo=0

    ```
- Use this command to get the general information of the driver moduler (when in src directory):
    ```bash
    sudo modinfo os_driver.ko
    ```

- If any command doesn't work try running with `sudo` at the beginning of the command