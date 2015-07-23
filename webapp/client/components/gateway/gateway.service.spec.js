'use strict';

describe('Service: gateway', function () {

  // load the service's module
  beforeEach(module('beeeOnWebApp'));

  // instantiate service
  var gateway;
  beforeEach(inject(function (_gateway_) {
    gateway = _gateway_;
  }));

  it('should do something', function () {
    expect(!!gateway).toBe(true);
  });

});
