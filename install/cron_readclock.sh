#! /bin/sh

myname=`hostname`
clmaster=`cmviewcl -p pkg1 | awk '/pkg1/{print $5}'`

readclock status > /dev/null 2>&1
rclockstat=$?


if [ $myname = $clmaster ]; then
	# I am the master
	if [ rclockstat -ne 1 ]; then
		# readclock is not running
		readclock start
	fi

else
	#I am not the master
	if [ rclockstat -ne 0 ]; then
		# readclock is running, kill it!
		readclock stop
	fi
fi

