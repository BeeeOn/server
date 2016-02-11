'use strict';

angular.module('beeeOnWebApp')
  .controller('LoginCtrl',['$rootScope','$translate','$scope','$location','$log','$timeout','$window','dialogs','Modal','Auth','loadingModal',
    function ($rootScope,$translate,$scope,$location,$log,$timeout,$window,dialogs,Modal,Auth,loadingModal){

      $scope.logInMode = function(mode,token){
        $translate('LOGIN.MODAL_TEXT').then(function(text){
          loadingModal.show(text);
        });
        Auth.login({mode:mode,token:token})
          .then( function() {
            // Logged in, redirect to home
            $log.debug('LoginCtrl - Redirecting to devices');
            $location.path('/devices');
          })
          .catch( function(err) {
            $scope.errors = err;
            $log.error('LoginCtrl - '+err);
          })
          .finally(function() {
            loadingModal.hide();
          });
      };

      $scope.loginWithOAuth = function(provider,token){
        var address = '/api/v1/auth/' + provider;
        if(token){
          address = address + '?token='+token;
        }
        $window.location.href = address;
      };

      $scope.noAccountDialog = function(provider,token){
        dialogs.create(
          'components/modal/accounts/register/register.html',
          'RegisterAccountCtrl',
          {
            'keyboard': false,
            'backdrop': true,
            'size': 'sm',
            'copy': true
          }
        ).result.then(function(doRegister){
            register(provider,token);
          });
      };

      var register = function(provider,token){
        $translate('LOGIN.REGISTERING').then(function(text){
          loadingModal.show(text);
        });
        Auth.register({provider:provider,token:token})
          .then( function(data){
            //login after registration
            $log.debug("registration successful");
            $scope.logInMode(provider,token);
          })
          .catch( function(err){
            $scope.errors = err;
            $log.error('LoginCtrl - '+err);
          })
          .finally(function(){
            loadingModal.hide();
          });
      };

      var error = $location.search().error;
      if(error){
        switch (error){
          case "24":
            var token = $location.search().token;
            var provider = $location.search().provider;
            if(provider && token){
              //detected unregistered user, show register dialog
              $scope.noAccountDialog(provider,token);
            }
            break;
          default:
            $log.error("UI_server error: " + error);
            break;
        }
      }
  }
]);
