# readclockd (2004)

This is an old program I made around 2004 to synchronize the HP-UX system clock with a GPS device connected to the serial port.

## History

At the begining there was a discussion on how to do the clock synchronization, the first attemp to use the ntpd which came with hp-ux was unsuccessfull, it seems it didn't support the device our client had connected to the server.

I said make it in `c` it should be easy, someone said, it was easier to make using a `bash` script and he made it.

Time passed and the script took longer to make and made some mistakes to the system. For example, the client GPS device sometimes would send a wrong date, which the script didn't check, due to this reason the system was doing special discounts to the passengers on a certain Sunday which made all of us to go back on-site to fix the problem (the system was an AFC system for Metro).

Another problem was that the script would finish for unknown reason and wouldn't restart normally, the second time it restarted it wouldn't read the serial port correctly.

## Motivation

I though it would be interesting to make a c application for hp-ux. I also wanted some experience with serial ports and I didn't want to be called on Sundays to go to the control center to check someone else's problems, so I made this application.

It is to note that at that time I was not part of the software team of that project and I made this application myself, nobody asked for it. Later the final client decided to use this version instead of the bash script one.

For a funny end, a year later I was asked permission to use this program on another project but later was not used because the programmer didn't know how to port some of the hp-ux only functions to Linux (I think there was only one or two).

## Final notes

The code is not so good, but it is a complete application, including the script to launch the service, to install it, an easy configuration file and the client could more or less understand the code.

Once thing now seems weird is that function names, variables and some comments are in Spanish, I think I haven't write Spanish on my code for many years.