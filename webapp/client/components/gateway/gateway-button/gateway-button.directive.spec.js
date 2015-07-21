'use strict';

describe('Directive: gatewayButton', function () {

  // load the directive's module and view
  beforeEach(module('beeeOnWebApp'));
  beforeEach(module('components/gateway/gateway-button/gateway-button.html'));

  var element, scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<gateway-button></gateway-button>');
    element = $compile(element)(scope);
    scope.$apply();
    expect(element.text()).toBe('this is the gatewayButton directive');
  }));
});