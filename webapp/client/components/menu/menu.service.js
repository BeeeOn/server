'use strict';

angular.module('beeeOnWebApp')
  .service('Menu', [ '$timeout', '$mdSidenav', '$mdUtil', '$log',
    function ($timeout, $mdSidenav, $mdUtil, $log) {

    var toggler = buildToggler('left');
      var close = angular.noop;
      /**
       * Build handler to open/close a SideNav; when animation finishes
       * report completion in console
       */
      function buildToggler(navID) {
        var debounceFn = $mdUtil.debounce(function () {
          $mdSidenav(navID)
            .toggle()
            .then(function () {
              //action after menu is toggled
              //$log.debug("toggle " + navID + " is done");
            });
        }, 300);
        return debounceFn;
      }
    return {
      toggle : function(){
        toggler();
      },
      close : function(){
        $mdSidenav('left').close()
          .then(function () {
            //action after menu is closed
            //$log.debug("close LEFT is done");
          });
      },
      open : function(){
        $mdSidenav('left').open()
          .then(function () {
            //action after menu is opened
            //$log.debug("open LEFT is done");
          });
      }
    }
  }]);
