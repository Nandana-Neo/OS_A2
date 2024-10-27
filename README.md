# OS Assignment 2

## Incase make doesn't work -- can't find build 

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
    
## Implementing

### 1. Build the driver
Navigate to the folder `src` and run the command:
```bash
sudo make
```

We can now observe new files created in the folder. Check if `driver.ko` file is present.

### 2. Loading the driver
Run this command in terminal:
```bash
sudo insmod driver.ko
```

### 3. Unloading the driver
To un-load the module, use rmmod command with root priveleges
```bash
sudo rmmod diver 
```
OR 
```bash
sudo rmmod diver.ko 
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
    driver                 16384  0

    ```

- Run dmesg to see the log messages
    ```bash
    sudo dmesg
    ```
- Use this command to get the general information of the driver moduler(when in src directory):
    ```bash
        sudo modinfo driver.ko
    ```