'use strict';

angular.module('beeeOnWebApp')
  .controller('NoGatewaysCtrl',['$scope','dialogs', function ($scope,dialogs) {
    $scope.showAddGatewayModal = function(){
      //show about modal
      dialogs.create(
        'components/tutorial/add-gateway/add-gateway.html',
        'AddGatewayCtrl',
        {},
        {
          'keyboard': false,
          'backdrop': true,
          'size': 'lg',
          'copy': false
        }
      );
    };
  }]);
