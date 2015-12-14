'use strict';

describe('Directive: deviceDetail', function () {

  // load the directive's module and view
  beforeEach(module('beeeOnWebApp'));
  beforeEach(module('components/device-detail/device-detail.html'));

  var element, scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<device-detail></device-detail>');
    element = $compile(element)(scope);
    scope.$apply();
    expect(element.text()).toBe('this is the deviceDetail directive');
  }));
});