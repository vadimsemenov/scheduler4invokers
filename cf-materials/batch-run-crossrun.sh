for i in {1..10}
	do
		TEST="00"
		if [ $i == 10 ]; then
			TEST="10"
		else
			TEST="0$i"
		fi
		echo Testing $TEST

		java -jar CrossRun.jar "./interactor tests/$TEST output.txt" "$1"

		if [ "$?" != "0" ]; then
			echo Some error occured
			exit 1
		fi

		echo The calculated value is:
		cat output.txt
	done

echo Processes are finished.
