'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('changelog', {
        url: '/changelog',
        templateUrl: 'app/routes/changelog/changelog.html',
        controller: 'ChangelogCtrl'
      });
  }]);