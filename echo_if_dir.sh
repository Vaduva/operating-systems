# Print message if given filename is a directory
#
# Example:
#
# sh echo_if_dir.sh .
# Argument is dir
#
# sh echo_if_dir.sh ./echo_if_dir.sh 
#

dir=$1

if test -d $dir
then echo "Argument is dir"
fi
