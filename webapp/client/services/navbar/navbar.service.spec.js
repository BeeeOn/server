'use strict';

describe('Service: navbar', function () {

  // load the service's module
  beforeEach(module('beeeOnWebApp'));

  // instantiate service
  var navbar;
  beforeEach(inject(function (_navbar_) {
    navbar = _navbar_;
  }));

  it('should do something', function () {
    expect(!!navbar).toBe(true);
  });

});
