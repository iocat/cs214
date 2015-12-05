#!/bin/sh

# Make sure the binary files exist
CLIENT=banking
SERVER=bank
TEST_RESULT=test_result.log
TEST_CASE_NO=10
if [ ! -f $CLIENT ] || [ ! -f $SERVER ]
then
    make
fi

case $1 in
    1)
        echo ""
        echo "Test #1: Start the server."
        ./$SERVER & 
        sleep 3
        killall $SERVER -SIGINT
        echo ""
        ;;
    2)
        echo ""
        echo "Test #2: Terminate the server."
        echo "|| PLEASE CTRL-C to TERMINATE THE RUNNING SERVER"
        sleep 1
        ./$SERVER
        echo ""
        ;;
    3)
        echo ""
        echo "Test #3: Run the server and client on localhost"
        ./$SERVER &
        ./$CLIENT localhost &
        sleep 3
        echo "AUTO kill client!"
        killall $CLIENT 
        echo "AUTO kill server!"
        killall $SERVER -SIGINT
        echo ""
        ;;
    4)
        echo ""
        echo "Test #4: Client connects with the server running on the remote host"
        echo "|| Please set up the server process on a remote machine in another terminal (ssh)"
        printf "|| Then PLEASE PUT THE REMOTE_MACHINE_NAME HERE: "
        read REMOTE_HOST
        echo "Thanks."
        echo "|| Now TEST THE CLIENT whatever you want. See the output on the"
        echo "other terminal"
        ./$CLIENT $REMOTE_HOST
        echo ""
        ;;
    5)
        echo ""
        echo "Test #5: Terminate the server while the client is connected"
        ./$SERVER &
        # Push a bunch of new lines to the client to reserve it.
        ./$CLIENT localhost &
        sleep 3
        echo "NOW I WILL KILL THE SERVER"
        killall $SERVER -SIGINT
        sleep 1
        echo ""
        ;;
    6)
        echo ""
        echo "Test #6: Terminate the client while the server is on."
        ./$SERVER &
        ./$CLIENT localhost &
        sleep 2
        echo "NOW I KILL THE CLIENT"
        killall $CLIENT
        sleep 2
        killall $SERVER -SIGINT
        echo ""
        ;;
    7)
        echo ""
        echo "SORRY FOR THE SUPER LONG OUTPUT OF 20 CLIENTS on a same terminal"
        echo ""
        echo "Test #7: Maximum number of accounts reached."
        echo ""
        ./$SERVER &
        for i in {1..21}
        do 
            printf "open account_${i}\nexit" | ./$CLIENT localhost &
        done
        sleep 6
        killall $SERVER -SIGINT

        echo ""
        ;;
    8)
        echo ""
        echo "Test #8: Two clients access a same account at the same time whether"
        echo "concurently or sequentially"
        ./$SERVER &
        echo "First create a shared account"
        printf "open acc\nexit\n" | ./$CLIENT localhost
        echo "Now let's share!"
        printf "start acc\ncredit 100\nexit\n" | ./$CLIENT localhost & printf "start acc\ncredit 100\nexit\n" | ./$CLIENT localhost &
        sleep 7
        killall $SERVER -SIGINT

        echo ""
        ;;
    9)
        echo ""
        echo "Test #9: Three clients access three different accounts at the same time and the server still handles them normally"
        echo ""
        ./$SERVER &
        printf "open account_1\nopen account_2\nopen account_3\nexit\n" |./$CLIENT localhost
        for i in {1..3}; do
            printf "start account_${i}\ncredit ${i}0\nbalance\nexit\n" | ./$CLIENT localhost &
        done
        echo "|| WAIT AND HOLD for server to print the result."
        sleep 25
        killall $SERVER -SIGINT
        ;;
    10)
        echo ""
        echo "Test #10: The client debits an amount exceeding the balance."
        echo ""
        ./$SERVER &
        printf "open acc\nstart acc\ndebit 10.12\nexit\n" | ./$CLIENT localhost
        killall $SERVER -SIGINT
        ;;
    11)
        echo ""
        echo "Test #11: The client starts without an server opened."
        echo ""
        ./$CLIENT localhost &
        sleep 5
        ./$SERVER &
        sleep 2
        killall $SERVER -SIGINT
        ;;
    *)  echo "Usage: $0 [1..${TEST_CASE_NO}]"
        ;;
esac

