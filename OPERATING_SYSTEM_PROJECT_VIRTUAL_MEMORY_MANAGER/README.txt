# complie
> make

# clean
> make clean

# output result
> ./manager addresses.txt



# compare results
# Normalize the line endings
> unix2dos correct.txt result.txt
> sh compare.sh

# OR

# compare results (Linux)
> dos2unix correct.txt             # to convert in same format
> cmp correct.txt result.txt       # now check
> sh compare.sh
