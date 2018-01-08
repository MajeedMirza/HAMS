# <a href="https://github.com/MajeedMirza/HAMS"><img border="0" alt="The Home Monitoring and Automation System" src="Resources/icon.png" width="34" height="34" /></a> HAMS 
The **H**ome **A**utomation and **M**onitoring **S**ystem

## Index
The HAMS **App**, **Hardware Scripts**, and **Server** source code and installation instructions can be found at:
- [App](App)  
- [Scripts](HWScripts)  
- [Server](Server)  

## About
The Home Monitoring and Automation System or HAMS is an **open source** project that provides a starting point for home monitoring and automation. HAMS includes code for a fully functional server (Node.js) and database (MongoDB), a hybrid mobile app (Ionic), and scripts (Python and C) for nodes (Raspberry Pi + Aruduino). 

The system provides the software needed to monitor the following:
- Temperature
- Humidity
- Smoke, Gas presence
- Flame (fire)
- Flooding
- Garage door status and control

In case of an emergency all units of the system will sound an alarm and flash a red LED to alert residents in the house, as well as send mobile notifications to notify home owners of any events.

## Why HAMS?
HAMS gives you a starting point for a customized home monitoring solution. All data is handled by you and the way the code handles the data is fully transparent. By providing code that works for a variety of devices, new devices can easily be integrated by using existing code as an example. HAMS allows you to control your safety, security, and privacy.

## What exactly can HAMS do?
- HAMS sounds alarms within the house if an emergency is detected
- HAMS notifies users by pushing notifications to their phones if an emergency is detected 
- HAMS allows users constant monitoring of their homes through a mobile app with live readings from sensors
- HAMS provides users control of their garage doors through the HAMS app
- HAMS logs sensor readings every 10 seconds in a database to allow users to look through their home’s history
