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
  .directive('gatewayButton',['Gateways','lodash', function (Gateways,lodash) {
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
          $log.debug('gatewayButton - Current gateway -> ' + Gateways.getSelectedId() + ' trying to change to -> ' + newGateway);
          //if new gateway isn't equal to current
            //check, if state has .show at it's end
            var currentState = $state.current.name.split('.')[0];
            /*if(!lodash.endsWith(currentState,'.show')){
              currentState = currentState.split('.')[0] + '.show';
            }*/

            //change state, works only if state is in "management" or "applications" section
            $scope.changeLocation(currentState,newGateway);
        };
      }
    };
  }]);