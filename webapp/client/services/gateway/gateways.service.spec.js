'use strict';

describe('Service: gateways', function () {

  // load the service's module
  beforeEach(module('beeeOnWebApp'));

  // instantiate service
  var Gateways;
  beforeEach(inject(function (_Gateways_) {
    Gateways = _Gateways_;
  }));

  it('should do something', function () {
    expect(!!Gateways).toBe(true);
  });

});
