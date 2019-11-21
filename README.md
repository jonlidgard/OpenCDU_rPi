About 2 years ago I built a standalone CDU Box for use with the PMDG 737 aircraft on Microsoft FSX / Lockheed Martin P3D. It works really well and could easily be modified to work with their B747 & B777 aircraft.


I will try to describe here how I did it.



This is the Client code for a Raspberry pi 3. I've got it connected to an old FDS version 1 CDU & Sony PsOne Screen.

The FDS v1 CDU in it's original form was basically just a USB keyboard and an S-Video connection to your PC. The ide was that you would drag the aircrafts CDU window down onto the screen. I didn't like this method for various reasons and set out to design something that was more plug-n-play.

The server code is loaded as a module when P3D starts up. It detects when the PMDG 737 aircraft is loaded and connects to the Raspberery pi over the network. I use an ethernet link but wireless works too.

