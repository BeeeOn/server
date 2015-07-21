'use strict';

angular.module('beeeOnWebApp')
  .service('SidePanel', [ '$timeout', '$mdSidenav', '$mdUtil', '$log',
    function ($timeout, $mdSidenav, $mdUtil, $log) {
      var togglerLeft = buildToggler('left');
      var togglerRight = buildToggler('right');
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
	    /**
	     * Toggle left panel. This panel is registered in $mdSidenav as \"left\"
	     */
      toggleLeft : function(){
        togglerLeft();
      },
	    /**
	     * Toggle right panel. This panel is registered in $mdSidenav as \"right\"
	     */
      toggleRight : function(){
        togglerRight();
      },
	    /**
	     * Close left panel. This panel is registered in $mdSidenav as \"left\"
	     */
      closeLeft : function(){
        $mdSidenav('left').close()
          .then(function () {
            //action after menu is closed
            //$log.debug("close LEFT is done");
          });
      },
	    /**
	     * Close right panel. This panel is registered in $mdSidenav as \"right\"
	     */
      closeRight : function(){
        $mdSidenav('right').close()
          .then(function () {
            //action after menu is closed
            //$log.debug("close LEFT is done");
          });
      },
	    /**
	     * Open left panel. This panel is registered in $mdSidenav as \"left\"
	     */
      openLeft : function(){
        $mdSidenav('left').open()
          .then(function () {
            //action after menu is opened
            //$log.debug("open LEFT is done");
          });
      },
	    /**
	     * Open right panel. This panel is registered in $mdSidenav as \"right\"
	     */
      openRight : function(){
        $mdSidenav('right').open()
          .then(function () {
            //action after menu is opened
            //$log.debug("open LEFT is done");
          });
      }
    }
  }]);
