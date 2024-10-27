cmd_/home/nandana/OS_A2/Module.symvers :=  sed 's/ko$$/o/'  /home/nandana/OS_A2/modules.order | scripts/mod/modpost -m      -o /home/nandana/OS_A2/Module.symvers -e -i Module.symvers -T - 
