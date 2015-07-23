'use strict';
/**
 * Directive for showing gateway switch button.
 *
 * Requires in parent scope:
 * - variable - "selectedGateway" -> current gateway number
 * - array - "gateways" - array of all existing gateways
 *            gateways  = [
 *                          { id:'11111',name: 'Adapter#1',role:'Superuser'},
 *                          { id:'22222',name: 'Adapter#2',role:'Admin'},
 *                          { id:'33333',name: 'Adapter#3',role:'Admin'}
 *                        ]
 * - function - "changeLocation(newState)" -> changes state
 */
angular.module('beeeOnWebApp')
  .directive('gatewayButton',['Gateway','lodash', function (Gateway,lodash) {
    return {
      templateUrl: 'components/gateway/gateway-button/gateway-button.html',
      restrict: 'EA',
      scope: false,
      controller: function($scope,$stateParams,$log,$state) {
        /**
         * Changes ui-router state to: "/current_state/given_parameter"
         * If current state doesn't have ".show" at end, it is added.
         * @param newGateway new gateway number
         */
        $scope.select = function(newGateway){
          $log.debug('Current gateway -> ' + Gateway.getSelectedId() + ' trying to change to -> ' + newGateway);
          //if new gateway isn't equal to current
          if(Gateway.getSelectedId() !== newGateway){
            //show new gateway number
            $scope.selectedGateway = newGateway;
            //check, if state has .show at it's end
            var currentState = $state.current.name;
            if(!lodash.endsWith(currentState,'.show')){
              currentState = currentState.split('.')[0] + '.show';
            }
            //save new gateway number
            Gateway.setSelected(newGateway);
            //change state, works only if state is in "management" or "applications" section
            $scope.changeLocation(currentState);
          }else {
            $log.debug('Redundant state change - do nothing.');
          }
        };

        $scope.settings = function(gateway){
          $log.debug('Settings for adapter: ' + gateway);
        };
      }
    };
  }]);