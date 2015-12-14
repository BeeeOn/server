'use strict';

angular.module('beeeOnWebApp')
  .directive("scroll", function ($window) {
    return function(scope, element, attrs) {
      var ele = angular.element(element);
      ele.bind("scroll", function() {
        //when scrolled from the top of element, set given attribute tu true, else set to false
        scope[attrs.scroll] = ele[0].scrollTop > 0;
        //console.log(element);
        scope.$apply();
      });
    };
  })
  .directive('ngMouseWheelDown', function() {
    return function(scope, element, attrs) {
      element.bind("DOMMouseScroll mousewheel onmousewheel", function(event) {

        // cross-browser wheel delta
        var event = window.event || event; // old IE support
        var delta = Math.max(-1, Math.min(1, (event.wheelDelta || -event.detail)));

        if(delta < 0) {
          scope.$apply(function(){
            scope.$eval(attrs.ngMouseWheelDown);
          });

          // for IE
          event.returnValue = false;
          // for Chrome and Firefox
          if(event.preventDefault)  {
            event.preventDefault();
          }

        }
      });
    };
  })
  .directive('ngMouseWheelUp', function() {
    return function(scope, element, attrs) {
      element.bind("DOMMouseScroll mousewheel onmousewheel", function(event) {

        // cross-browser wheel delta
        var event = window.event || event; // old IE support
        var delta = Math.max(-1, Math.min(1, (event.wheelDelta || -event.detail)));

        if(delta > 0) {
          scope.$apply(function(){
            scope.$eval(attrs.ngMouseWheelUp);
          });

          // for IE
          event.returnValue = false;
          // for Chrome and Firefox
          if(event.preventDefault) {
            event.preventDefault();
          }

        }
      });
    };
  });