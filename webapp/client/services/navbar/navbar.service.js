'use strict';

angular.module('beeeOnWebApp')
  .factory('navbar', [ '$translate', function ($translate) {
    var location = '';

    // Public API here
    return {
      setLocation: function (newLocation) {
        location = newLocation;
      },
      getLocation: function (){
        return $translate(location);
      }
    };
  }]);
