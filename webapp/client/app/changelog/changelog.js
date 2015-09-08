'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('changelog', {
        url: '/changelog',
        views: {
          'main' : {
            templateUrl: 'app/changelog/changelog.html',
            controller: 'ChangelogCtrl'
          }
        }
      });
  }]);