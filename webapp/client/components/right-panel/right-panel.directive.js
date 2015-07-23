'use strict';

angular.module('beeeOnWebApp')
  .directive('rightPanel', function () {
    return {
      templateUrl: 'components/right-panel/right-panel.html',
      restrict: 'E',
      scope: {
        options: '=',
        lockedMedia: '@'
      },
      transclude: true,
      controller: function($scope){

      }
    };
  });