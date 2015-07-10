**********************************************************************
**  BeeeOnWeb application
**  see: https://ant-2.fit.vutbr.cz/projects/server/wiki/Web_interface
**********************************************************************
**********************************************************************
INSTALLATION:

 - prerequisites
 --- nodejs( https://nodejs.org/download/ ) 
 --- npm ( https://www.npmjs.com/ ) - part of nodejs
 --- yeoman ( http://yeoman.io/ ),bower ,grunt - installation command: (possibly sudo) npm install -g yo bower grunt-cli gulp


 1. STEP - install Angular dependencies

 - in console move to directory with bower.json
 - run command: bower install

 2. STEP - install Express dependencies

 - in console move to directory with package.json
 - run command: npm install

 3. STEP - install Yeoman generator

 - run command: npm install -g generator-angular-fullstack
**********************************************************************
**********************************************************************
 RUN SERVER

 1. LOCAL DEVELOPMENT SERVER

 A)
 - open file server/config/enviroment/development.js
 - change property "ip" to your local adress
 --- "0.0.0.0" = localhost
 --- "192.168.x.x" = if you set actual IP address of your computer, you can acces server on local network ( for example your phone)
 B)
 -open file Gruntfile.js
 - find property "ip" and change it to ip selected in step (A):
//////////////////////////////////////////////////////////
 grunt.initConfig({
    // Project settings
    pkg: grunt.file.readJSON('package.json'),
    yeoman: {
      // configurable paths
      client: require('./bower.json').appPath || 'client',
      dist: 'dist'
    },
    express: {
      options: {
        port: process.env.PORT || 9000,
        ip: '192.168.1.175' <----------------------------- 
      },
//////////////////////////////////////////////////////////
C)
- in console run command: grunt serve
**********************************************************************
**********************************************************************
ADD NEW COMPONENT TO APPLICATION

!!!!!!!!!!!ALWAYS USE YEOMAN!!!!!!!!!!!!!!!!!!!!!

- commands can be found here: https://github.com/DaftMonk/generator-angular-fullstack

**********************************************************************
**********************************************************************
EDIT CHANGELOG

- open file "client/changelog.json"
- add new log to file and save

**********************************************************************
**********************************************************************
BUILD PRODUCTION VERSION

- run command: grunt build
- production version of application is in directory "dist"

**********************************************************************
**********************************************************************

TESTING

- both client and server -> run command: grunt test
- just client -> run command: grunt test:client
- just server -> run command: grunt test:server

**********************************************************************
**********************************************************************