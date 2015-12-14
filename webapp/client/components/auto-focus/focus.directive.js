'use strict';

angular.module('beeeOnWebApp')
  .directive('focus',['$timeout', function ($timeout) {
    return {
      restrict: 'A',
      scope : {
        trigger : '@focus'
      },
      link : function(scope, element) {
        scope.$watch('trigger', function(value) {
          if (value === "true") {
            $timeout(function() {
              element[0].focus();
            },100);
          }
        });
      }
    };
  }]);