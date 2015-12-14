'use strict';

describe('Filter: moduleValue', function () {

  // load the filter's module
  beforeEach(module('beeeOnWebApp'));

  // initialize a new instance of the filter before each test
  var moduleValue;
  beforeEach(inject(function ($filter) {
    moduleValue = $filter('moduleValue');
  }));

  it('should return the input prefixed with "moduleValue filter:"', function () {
    var text = 'angularjs';
    expect(moduleValue(text)).toBe('moduleValue filter: ' + text);
  });

});
