'use strict';

describe('Service: modules', function () {

  // load the service's module
  beforeEach(module('beeeOnWebApp'));

  // instantiate service
  var modules;
  beforeEach(inject(function (_modules_) {
    modules = _modules_;
  }));

  it('should do something', function () {
    expect(!!modules).toBe(true);
  });

});
