'use strict';

angular.module('beeeOnWebApp')
  .service('SidePanel', [ '$rootScope','$timeout', '$mdSidenav','$mdComponentRegistry', '$mdUtil', '$log',
    function ($rootScope,$timeout, $mdSidenav,$mdComponentRegistry, $mdUtil, $log) {
      var togglerLeft = buildToggler('left');
      var togglerRight = buildToggler('right');
      var close = angular.noop;
      var rightPanelOptions = {
        lockedMedia: '',
        location: '',
        type: 'change_state_right',
        changeStateOptions : {
          newState: 'overview',
          params:{
          }
        },
        hideBtn: false
      };
      var rightMainAction = null;
      /**
       * Build handler to open/close a SideNav; when animation finishes
       * report completion in console
       */
      function buildToggler(navID) {
        var debounceFn = $mdUtil.debounce(function () {
          $mdSidenav(navID)
            .toggle()
            .then(function () {

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
        $mdComponentRegistry.when('left').then(function(leftSidenav){
          leftSidenav.close();
        });
      },
	    /**
	     * Close right panel. This panel is registered in $mdSidenav as \"right\"
	     */
      closeRight : function(){
        $mdComponentRegistry.when('right').then(function(rightSidenav){
          rightSidenav.close();
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
            $log.debug("open RIGHT is done");
          });
      },
      getRightPanelOptions : function () {
        return rightPanelOptions;
      },
      setRightPanelOptions : function (options) {
        rightPanelOptions = options;
      },
      setRightMainAction : function(action){
        rightMainAction = action;
      },
      getRightMainAction : function(){
        return rightMainAction;
      }
    }
  }]);
