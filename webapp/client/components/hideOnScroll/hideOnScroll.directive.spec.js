'use strict';

describe('Directive: hideOnScroll', function () {

  // load the directive's module
  beforeEach(module('beeeOnWebApp'));

  var element,
    scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<hide-on-scroll></hide-on-scroll>');
    element = $compile(element)(scope);
    expect(element.text()).toBe('this is the hideOnScroll directive');
  }));
});