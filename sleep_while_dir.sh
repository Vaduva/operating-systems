# Print a message and sleep while a given filename is a directory
#
# Example
# 
# sh sleep_while_dir.sh .
# Will sleep now (ctrl-C to exit)
# ^C

dir=$1

while test -d $dir
do echo "Will sleep now (ctrl-C to exit)"; sleep 2
done
