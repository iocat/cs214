#!/bin/sh

# Make sure the binary files exist
CLIENT=banking
SERVER=bank
TEST_RESULT=test_result.log

if [ ! -f $CLIENT ] || [ ! -f $SERVER ]
then
    make
fi

# Create the server that runs in the background
./bank &

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
# Sleep to guarantee that the client process exits
sleep 4

# Test 2: Two clients access a same account concurrently
echo ""
echo "Test #2: It seems we are sharing a same account."
echo "-------------------------------------------------------"
echo "In this test, two accounts try to access a same account_1"
echo "at the same time. But the server does not allow"
echo "The expected output would be:"
echo "Error: Requested account 'account_1' has been started on another session."
for i in {1..3}
do
    echo "Try #${i}"
    echo "-------------------------------------------------------"
    printf "start account_1\nexit" | ./banking localhost & printf "start account_1\nexit" | ./banking localhost
done

#Test 3: 3 clients using 3 different accounts but the server still handle every
echo ""
echo "Test #3: Let's do a three...-client-at-the-same-time"
echo "-------------------------------------------------------"
echo "In this test, we have 3 clients connecting to the server concurrently"
echo "which are account_2, account_3, account_4"
echo "The expected result would be:"
echo "account_2 \$20"
echo "account_3 \$30"
echo "account_4 \$40"
for i in {2..4}
do
    printf "start account_${i}\ncredit ${i}${i}\ndebit ${i}\nexit" | ./banking localhost &
done
sleep 21

cat $TEST_RESULT

killall $SERVER
