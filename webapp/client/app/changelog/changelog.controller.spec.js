'use strict';

describe('Controller: ChangelogCtrl', function () {

  // load the controller's module
  beforeEach(module('beeeOnWebApp'));

  var ChangelogCtrl, scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    ChangelogCtrl = $controller('ChangelogCtrl', {
      $scope: scope
    });
  }));

  it('should ...', function () {
    expect(1).toEqual(1);
  });
});
