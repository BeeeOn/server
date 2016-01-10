'use strict';

angular.module('beeeOnWebApp')
  .directive('userRole',[ '$translate','$compile', function ($translate,$compile) {
    return {
      template: '',
      restrict: 'A',
      scope : {
        type : '=userRole'
      },
      link: function (scope, element, attrs) {
        scope.$watch('type', function(value) {
          switch (value){
            case 'owner':
              $translate('ROLES.SUPERUSER').then(function(text){
                element.text(text);
              });
              break;
            case 'admin':
              $translate('ROLES.ADMIN').then(function(text){
                element.text(text);
              });
              break;
            case 'user':
              $translate('ROLES.USER').then(function(text){
                element.text(text);
              });
              break;
            case 'guest':
              $translate('ROLES.GUEST').then(function(text){
                element.text(text);
              });
              break;
          }
        });
      }
    };
  }]);
