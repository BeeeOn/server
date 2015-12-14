'use strict';

angular.module('beeeOnWebApp')
  .directive('locationButton',['$log','$state','SidePanel', function ($log,$state,SidePanel) {
    /**
     * Directive for location button can have two options in attribute type (menu,location).
     *
     * menu = location button will hide/show menu panel. Uses Menu service to determine, if menu is shown/hidden
     * location = location button will return to previous location/state
     */
    return {
      templateUrl: 'components/location-button/location-button.html',
      restrict: 'E',
      scope: {
        options: '='
      },
      link : function(scope, element, attrs) {
        var init = function(){
          scope.hideBtn = true;
          if(!angular.isDefined(scope.options)){
            return;
          }
          if(angular.isDefined(scope.options.hideBtn)){
            scope.hideBtn = scope.options.hideBtn;
          }
          scope.icon = 'menu_stripes';
          scope.action = angular.noop;
          switch (scope.options.type){
            case 'panel_toggle_left':
              scope.action = function(){
                SidePanel.toggleLeft();
              };
              break;
            case 'panel_close_left':
              scope.icon = 'arrow_back';
              scope.action = function(){
                SidePanel.closeLeft();
              };
              break;
            case 'panel_close_right':
              scope.icon = 'arrow_back';
              scope.action = function(){
                SidePanel.closeRight();
              };
              SidePanel.setRightMainAction(scope.action);
              break;
            case 'change_state_right':
              scope.icon = 'arrow_back';
              scope.action = function(){
                SidePanel.closeRight();
                if(!angular.isDefined(scope.options.changeStateOptions)){
                  $log.error('locationButton - Did not provide new state for location-button');
                  return;
                }
                $state.go(scope.options.changeStateOptions.newState,scope.options.changeStateOptions.params);
              };
              SidePanel.setRightMainAction(scope.action);
              break;
            case 'cancel_and_return':
              scope.icon = 'close';
              scope.action = function(){
                SidePanel.closeRight();
                if(!angular.isDefined(scope.options.changeStateOptions)){
                  $log.error('locationButton - Did not provide new state for location-button');
                  return;
                }
                $state.go(scope.options.changeStateOptions.newState,scope.options.changeStateOptions.params);
              };
              SidePanel.setRightMainAction(scope.action);
              break;
          }
        };
        //initialize button settings when button type changes
        scope.$watch('options.type',function(){
          init();
        });
      }
    };
  }]);