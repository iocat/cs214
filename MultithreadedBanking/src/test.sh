#!/bin/sh

# Make sure the binary files exist
CLIENT=banking
SERVER=start-server
TEST_RESULT=test_result.log
if [ ! -f $CLIENT ] || [ ! -f $SERVER ]
then
    make
fi

# Create the server that runs in the background
./start-server &
echo ""
echo "Warning: The result of these test is too long and the outputs are the mixture of the server and client program."
echo ""
# Test 1: Try to open 21 different client accounts
echo "Test #1: 21 different accounts"
echo "-------------------------------------------------------"
echo "Expected output: Database is full (when the client requests the 21 accounts)"
for i in {1..21}
do 
    printf "open account_${i}\nexit" | ./banking localhost &
done

