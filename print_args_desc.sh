# Print command arguments in descending order
#
# Example
#
# sh print_args_desc.sh arg1 arg2 arg3
# arg3 arg2 arg1
#
# sh print_args_desc.sh
#

arg=$# # number of args
output=echo # string containing the output

while test $arg -gt 0 # loop through args in desc order
do
	output="$output \$$arg" # add $arg to output
	arg=`expr $arg - 1` # $arg -= 1
done

eval $output # eval output string
