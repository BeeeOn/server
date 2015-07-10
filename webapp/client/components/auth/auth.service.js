'use strict';

angular.module('beeeOnWebApp')
  .factory('Auth', ['$log','$cookieStore','User', function ($log,$cookieStore,User) {
    var currentUser = {};
    if($cookieStore.get('token')){
      //if demo mod, load demo user
      if($cookieStore.get('token') === 'demo'){
        User.setUseDemo(true);
        currentUser = User.get();
      }
    }
    return {
      login: function(){

      },
      demoLogin: function(){
        User.setUseDemo(true);
        ga('set', '&uid', User.bt); // Set the user ID using signed-in user_id.
        currentUser = User.get();
        $cookieStore.put('token','demo');
        $log.debug('Demo mod');
      },
      logout: function(){
        $cookieStore.remove('token');
        currentUser = {};
      },
      getCurrentUser: function(){
        return currentUser;
      },
      isLoggedIn: function(cb){
        if(currentUser.hasOwnProperty('role')){
          cb(true);
        }else{
          cb(false);
        }
      }
    }
  }]);
