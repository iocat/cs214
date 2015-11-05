
# Test number 1
echo "========================================="
echo "Test 1: Wanna overwrite an existed file?"
./index ./test/exists.txt ./test

# Test number 2
echo "========================================="
echo "Test 2: Writing to a directory?"
./index ./test ./test

# Test number 3
echo "========================================="
echo "Test 3: Let's try the project example"
./index ./test3.txt ./test/project_example
cat ./test3.txt
rm -r ./test3.txt

# Test number 4
echo "========================================="
echo "Test 4: An invalid file to be indexed"
./index ./test4.txt ./test/link

#Test number 5
echo "========================================="
echo "Test 5: A larger data set"
./index ./test5.txt ./test/extra
cat ./test5.txt
rm -f ./test5.txt
