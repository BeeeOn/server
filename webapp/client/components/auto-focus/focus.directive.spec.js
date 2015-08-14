'use strict';

describe('Directive: autoFocus', function () {

  // load the directive's module
  beforeEach(module('beeeOnWebApp'));

  var element,
    scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<auto-focus></auto-focus>');
    element = $compile(element)(scope);
    expect(element.text()).toBe('this is the autoFocus directive');
  }));
});