'use strict';

describe('Service: loadingModal', function () {

  // load the service's module
  beforeEach(module('beeeOnWebApp'));

  // instantiate service
  var loadingModal;
  beforeEach(inject(function (_loadingModal_) {
    loadingModal = _loadingModal_;
  }));

  it('should do something', function () {
    expect(!!loadingModal).toBe(true);
  });

});
