#!/bin/sh

# Make sure the binary files exist
CLIENT=banking
SERVER=bank
TEST_RESULT=test_result.log

if [ ! -f $CLIENT ] || [ ! -f $SERVER ]
then
    make
fi

echo ""
echo "Test #1: Start the server."
./$SERVER & 
sleep 3
killall $SERVER -SIGINT
echo ""

echo "Test #2: Terminate the server."
echo "Please CTRL-C to terminate the running server and see the result"
sleep 1
./$SERVER
echo ""

echo "Test #3: Run the server and client on localhost"
./$SERVER &
./$CLIENT localhost &
sleep 3
killall $SERVER $CLIENT -SIGINT
echo ""

echo "Test #4: Client connects with the server running on the remote host"
echo "Please set up the remote host on the server on another terminal (ssh)"
echo "Then enter the remote host here: "
read REMOTE_HOST
./$CLIENT $REMOTE_HOST
echo ""

echo "Test #5: Terminate the server while the client is connected"
./$SERVER &
./$CLIENT localhost &
sleep 2
killall $SERVER -SIGINT
sleep 1
echo ""

echo "Test #6: Terminate the client while the server is on."
./$SERVER &
./$CLIENT &
sleep 2
killall $CLIENT
sleep 2
killall $SERVER -SIGINT

echo ""
echo "Test #7: Maximum number of accounts reached."
./$SERVER &
for i in {1..21}
do 
    printf "open account_${i}\nexit" | ./banking localhost &
done
sleep 6
killall $SERVER -SIGINT
