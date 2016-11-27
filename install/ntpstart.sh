/sbin/init.d/xntpd stop
sleep 5
/usr/sbin/xntpd -c /etc/ntp.conf.server
