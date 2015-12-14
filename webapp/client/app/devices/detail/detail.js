'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('devices.detail', {
        url: '/detail/:deviceId',
        views: {
          'right@devices' : {
            templateUrl: 'app/devices/detail/detail.html',
            controller: 'DeviceDetailCtrl'
          }
        }
      })
      .state('modules.detail', {
        url: '/detail/:deviceId',
        views: {
          'right@modules' : {
            templateUrl: 'app/devices/detail/detail.html',
            controller: 'DeviceDetailCtrl'
          }
        }
      });
  });