'use strict';

describe('Directive: devicesList', function () {

  // load the directive's module and view
  beforeEach(module('beeeOnWebApp'));
  beforeEach(module('components/devices-list/devices-list.html'));

  var element, scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<devices-list></devices-list>');
    element = $compile(element)(scope);
    scope.$apply();
    expect(element.text()).toBe('this is the devicesList directive');
  }));
});