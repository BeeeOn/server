'use strict';

angular.module('beeeOnWebApp')
  .controller('NotificationsCtrl', ['$scope','$translate','navbar', function ($scope,$translate,navbar) {
    navbar.setLocation('GLOBAL.NOTIFICATIONS');
  }]);
