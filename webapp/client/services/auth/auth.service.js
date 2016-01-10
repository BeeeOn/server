'use strict';

angular.module('beeeOnWebApp')
  .factory('Auth', ['$log','$cookieStore','$q','$http','User','Gateways','Devices','Locations', function ($log,$cookieStore,$q,$http,User,Gateways,Devices,Locations) {
    var currentUser = {};
    var roles = ['guest','user','admin','owner'];
    if($cookieStore.get('token')){
      $log.debug('Auth - Getting user from stored cookie.');
      currentUser = User.get();
    }
    var self = this;
    return {

      login: function(properties,callback){
        var cb = callback || angular.noop;
        var deferred = $q.defer();
        var args = {
          bt: ''
        };
        /* is user trying to start DEMO MOD ? */
        if(angular.isDefined(properties) && angular.isDefined(properties.mode)){
          switch (properties.mode){
            case 'demo':
              args = {
                bt: 'demo',
                url: 'local',
                provider: 'demo'
              };
              $log.info('Demo mod');
              break;
            case 'google':
              $log.info('Trying to login with google account');
              args = {
                url: 'local',
                provider: 'google',
                access_token: properties.token
              };
              break;
            default:
              deferred.reject("Unknown provider");
              return deferred.promise;
          }

        }
        $http.post('api/v1/auth/'+args.url, args).
          success(function(data) {
            $cookieStore.put('token', data.token);
            $log.debug('Auth - Getting user from login.');
            currentUser = User.get();
            currentUser.currentProvider = properties.mode;
            deferred.resolve(data);
            return cb();
          }).
          error(function(err) {
            //this.logout();
            deferred.reject(err);
            return cb(err);
          }.bind(this));
        return deferred.promise;
      },

      register : function(options,callback){
        var cb = callback || angular.noop;
        var deferred = $q.defer();
        $log.debug('Auth - Registering');
        $http.post('api/v1/auth/register',options).
          success(function(data){
            $cookieStore.put('token', data.token);
            deferred.resolve(data);
            return cb();
          }).
          error(function(err){
            deferred.reject(err);
            return cb(err);
          }.bind(this));
        return deferred.promise;
      },

      logout: function(callback){
        var cb = callback || angular.noop;
        var deferred = $q.defer();
        $log.debug('Auth - Logging out');
        $http.post('api/v1/auth/logout',{}).
          success(function(data){
            $cookieStore.remove('token');
            $log.info('Logout successful');
            deferred.resolve(data);
            return cb();
          }).
          error(function(err){
            deferred.reject(err);
            return cb(err);
          }.bind(this));
        return deferred.promise;
      },

      getCurrentUser: function(){
        return currentUser;
      },
      isLoggedIn: function(){
        $log.debug('Auth - Checking, if user is logged in (SYNC)!');
        return currentUser.hasOwnProperty('id');
      },

      isLoggedInAsync: function(cb){
        //$log.debug('Checking, if user is logged in (ASYNC)!');
        if(currentUser.hasOwnProperty('$resolved') && currentUser.$resolved === false) {
          //$log.debug('promise exists, waiting for it to resolve.');
          currentUser.$promise.then(function() {
            cb(true);
          }).catch(function() {
            cb(false);
          });
        } else if($cookieStore.get('token') && currentUser.hasOwnProperty('id')) {
          cb(true);
        } else {
          cb(false);
        }
      },
      getRoles : function (){
        return roles;
      },
      clearLocalData : function (){
        currentUser = {};
        Gateways.clearLocalData();
        Devices.clearLocalData();
        Locations.clearLocalData();
      }
    }
  }]);
