# <a href="https://github.com/MajeedMirza/HAMS"><img border="0" alt="The Home Monitoring and Automation System" src="../Resources/icon.png" width="34" height="34" /></a> HAMS Server
Primary Developer: [Majeed Mirza](https://github.com/MajeedMirza)

## About
The main logic for the server is located in the [node.service.js](services/node.service.js) file.
The [nodes.controller.js](controllers/api/nodes.controller.js) file handles api calls and routes them to the correct function in [node.service.js](services/node.service.js). The file which initiates the server is [server.js](server.js).

## Installation
Get [Node.js](https://nodejs.org/en/)

Clone the project

npm is the default package manager for Node (included in Node installation) and is used to install the [dependencies](package.json) for the project. 

From within the project folder run this command in the terminal to install all required packages:
```
npm install
``` 

To start the server use this command in the terminal:
```
node server.js
```

Go to [http://localhost:3001](http://localhost:3001) in your browser to confirm that the HAMS Server is up and running.

## Testing
To run tests change the TEST variable to true in [config.json](config/config.json) then restart the server.
