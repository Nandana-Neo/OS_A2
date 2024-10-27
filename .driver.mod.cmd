cmd_/home/nandana/OS_A2/driver.mod := printf '%s\n'   driver.o | awk '!x[$$0]++ { print("/home/nandana/OS_A2/"$$0) }' > /home/nandana/OS_A2/driver.mod
