'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('devices.detail.edit', {
        url: '/edit',
        views: {
          'right@devices' : {
            templateUrl: 'app/devices/edit/edit.html',
            controller: 'EditDeviceCtrl'
          }
        }
      })
      .state('modules.detail.edit', {
        url: '/edit',
        views: {
          'right@modules' : {
            templateUrl: 'app/devices/edit/edit.html',
            controller: 'EditDeviceCtrl'
          }
        }
      });
  });