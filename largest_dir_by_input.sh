# Print largest directory and the number of files for directories
# read through the command line until no input is provided
#
# Example
# 
# sh largest_dir_by_input.sh 
# Enter directories:
# .
# ..
# 
# Largest directory is .. with 20 files

input='Enter directories:'

echo $input

until test "$input" = "" # read until no input
do 
	read input

	if [ -d "$input" ] # test if input is dir
	then
		count=0
		for f in `ls "$input"` # list all files...
		do
			count=`expr $count + 1` # ...and count them
		done

		if [ $count -gt ${files:-0} ] # update output if required
		then
			dir="$input"
			files=$count
		fi
	fi
done

echo "Largest directory is $dir with $files files"