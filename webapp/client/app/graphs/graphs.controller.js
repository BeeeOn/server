'use strict';

angular.module('beeeOnWebApp')
  .controller('GraphsCtrl',['$scope','$translate','navbar', function ($scope,$translate,navbar) {
    navbar.setLocation('MENU.GRAPHS');
  }]);
