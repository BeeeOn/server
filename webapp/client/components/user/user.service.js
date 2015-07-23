'use strict';

angular.module('beeeOnWebApp')
  .factory('User',[ '$resource' , function ($resource) {
    return $resource('/api/v1/user/:controller', {

      },
      {
        get: {
          method: 'GET'
        }
      });
  }]);
