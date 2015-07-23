'use strict';

describe('Directive: rightPanel', function () {

  // load the directive's module and view
  beforeEach(module('beeeOnWebApp'));
  beforeEach(module('components/right-panel/right-panel.html'));

  var element, scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<right-panel></right-panel>');
    element = $compile(element)(scope);
    scope.$apply();
    expect(element.text()).toBe('this is the rightPanel directive');
  }));
});