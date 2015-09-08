'use strict';

angular.module('beeeOnWebApp')
  .controller('GatewayBottomSheetCtrl',[ '$scope','$translate','$mdBottomSheet', function ($scope,$translate,$mdBottomSheet) {
    $scope.items = [
      {
        id: 'edit',
        icon : 'edit',
        title: ''
      },
      {
        id: 'users',
        icon : 'group',
        title: ''
      },
      {
        id: 'delete',
        icon : 'delete',
        title: ''
      }
    ];
    $translate(['GLOBAL.EDIT','GLOBAL.DELETE','GLOBAL.USERS']).then(function(translations){
      $scope.items[0].title = translations['GLOBAL.EDIT'];
      $scope.items[1].title = translations['GLOBAL.USERS'];
      $scope.items[2].title = translations['GLOBAL.DELETE'];
    });
    $scope.listItemClick = function($index) {
      var clickedItem = $scope.items[$index];
      $mdBottomSheet.hide(clickedItem);
    };
  }]);
