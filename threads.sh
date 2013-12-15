# Generates threads.txt file with 5000 numbers, one on each line
#
# Must be run with bash: bash threads.sh

if [ -f threads.txt ]
    then rm threads.txt
fi

touch threads.txt

for i in {1..5000}
do
   echo "$i" >> threads.txt
done

echo "threads.txt file generated"
