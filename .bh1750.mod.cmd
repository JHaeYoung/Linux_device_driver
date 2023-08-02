cmd_/home/go/driver/bh1705/bh1750.mod := printf '%s\n'   bh1750.o | awk '!x[$$0]++ { print("/home/go/driver/bh1705/"$$0) }' > /home/go/driver/bh1705/bh1750.mod
