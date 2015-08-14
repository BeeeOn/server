'use strict';

angular.module('beeeOnWebApp')
  .factory('loadingModal',['$rootScope','$timeout','dialogs', function ($rootScope,$timeout,dialogs) {
    var message = '';
    var modal = null;
    var openModal = function (){
      modal = dialogs.create(
        'components/modal/loading/loading-modal.html',
        'LoadingModalCtrl',
        {},
        {
          'keyboard': false,
          'backdrop': 'static',
          'size': 'sm',
          'windowClass': 'loadingModal',
          'copy': false
        }
      );
    };
    var hide = function(){
      $timeout(function(){
        if(modal) {
          modal.close();
          modal = null;
        }
      },300);
    };
    // Public API here
    return {
      getMessage : function(){
        return message;
      },
      setMessage : function(msg){
        message = msg;
        if(modal){
          $rootScope.$broadcast('dialogs.loading.message',{'msg' : message});
        }
      },
      show : function (msg){
        this.setMessage(msg);
        if(!modal){
          openModal();
          //fail safe, automatically hide after 10s
          $timeout(function(){
            hide();
          },10000);
        }
      },
      hide : hide
    };
  }]);
