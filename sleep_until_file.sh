# Print a message and sleep until a given filename is a file
#
# Example
#
# sh sleep_until_file.sh .
# Will sleep now (ctrl-C to exit)
# ^C

file=$1

until test -f $file
do echo "Will sleep now (ctrl-C to exit)"; sleep 2
done
