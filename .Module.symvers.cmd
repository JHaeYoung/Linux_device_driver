cmd_/home/go/driver/bh1705/Module.symvers :=  sed 's/ko$$/o/'  /home/go/driver/bh1705/modules.order | scripts/mod/modpost -m -a    -o /home/go/driver/bh1705/Module.symvers -e -i Module.symvers -T - 
