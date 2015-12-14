'use strict';

describe('Directive: modulesList', function () {

  // load the directive's module and view
  beforeEach(module('beeeOnWebApp'));
  beforeEach(module('components/modules-list/modules-list.html'));

  var element, scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<modules-list></modules-list>');
    element = $compile(element)(scope);
    scope.$apply();
    expect(element.text()).toBe('this is the modulesList directive');
  }));
});