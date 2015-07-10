'use strict';

angular.module('beeeOnWebApp')
  .factory('User', function () {
    var demoUser = {
      bt: 11111,
      name: "Demo",
      surname: "Mod",
      mail: "demo@mod.com",
      role: 'demo',
      picture: null,
      pictureURL: '../assets/images/icons-svg/ic_person_white_48px.svg'
    }

    var useDemo = false;

    return {
      get: function(){
        if(useDemo) return demoUser;
      },
      setUseDemo: function(use){
        useDemo = use;
      }
    }
  });
