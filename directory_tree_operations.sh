# Builds test tree for directory_tree_operations.c
#
# Tree looks like:
# - source
#   - file1.txt
#   - file2.txt
#   - folder1
#     - file1folder1.txt
#     - file2folder1.txt
#   - folder2
#     - file1folder2.txt
#     - file2folder2.txt

mkdir source
cd source
echo "file1" > file1.txt
echo "file2" > file2.txt

mkdir folder1
cd folder1
echo "file1folder1" > file1folder1.txt
echo "file2folder1" > file2folder1.txt

cd ..
mkdir folder2
cd folder2
echo "file1folder2" > file1folder2.txt
echo "file2folder2" > file2folder2.txt