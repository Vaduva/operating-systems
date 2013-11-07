# Builds test tree for testing copy_tree.c
#
# Tree looks like:
# - outer
#   - inner
#     - inner.txt
#     - innersymlink
#   - outer.txt
#   - outersymlink
# - outer-copy

mkdir outer
cd outer
echo "outer file" > outer.txt
ln -s outer.txt outersymlink

mkdir inner
cd inner
echo "inner file" > inner.txt
ln -s inner.txt innersymlink

cd ../..
mkdir outer-copy