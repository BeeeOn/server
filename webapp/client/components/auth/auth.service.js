'use strict';

angular.module('beeeOnWebApp')
  .factory('Auth', ['$log','$cookieStore','$q','$http','User', function ($log,$cookieStore,$q,$http,User) {
    var currentUser = {};
    if($cookieStore.get('token')){
      $log.debug('Gettings user from stored cookie.');
      currentUser = User.get();
    }
    return {

      login: function(properties,callback){
        var cb = callback || angular.noop;
        var deferred = $q.defer();
        var args = {
          bt: ''
        };
        /* is user trying to start DEMO MOD ? */
        if(angular.isDefined(properties) && angular.isDefined(properties.demo) && properties.demo === true){
          args = {
            bt: 'demo'
          };
          $log.info('Demo mod');
        }
        $http.post('api/v1/auth/local', args).
          success(function(data) {
            $cookieStore.put('token', data.token);
            $log.debug('Gettings user from login.');
            currentUser = User.get();
            deferred.resolve(data);
            return cb();
          }).
          error(function(err) {
            this.logout();
            deferred.reject(err);
            return cb(err);
          }.bind(this));
        return deferred.promise;
      },

      logout: function(callback){
        var cb = callback || angular.noop;
        var deferred = $q.defer();
        $log.debug('Logging out');
        $http.post('api/v1/auth/logout',{}).
          success(function(data){
            $cookieStore.remove('token');
            currentUser = {};
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
        $log.debug('Checking, if user is logged in (SYNC)!');
        return currentUser.hasOwnProperty('uid');
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
        } else if($cookieStore.get('token') && currentUser.hasOwnProperty('uid')) {
          cb(true);
        } else {
          cb(false);
        }
      }
    }
  }]);
