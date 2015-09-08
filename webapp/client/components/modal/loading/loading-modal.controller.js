'use strict';

angular.module('beeeOnWebApp')
  .controller('LoadingModalCtrl',[ '$scope','loadingModal', function ($scope,loadingModal) {
    $scope.message = loadingModal.getMessage();

    $scope.$on('dialogs.loading.message',function(evt,args){
      $scope.message = (angular.isDefined(args.msg)) ? args.msg : $scope.msg;
    });
  }]);
