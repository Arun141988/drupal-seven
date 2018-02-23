/**
 * @file
 * Provide functionality for change location.
 */

(function ($) {

  Drupal.GcePopup = Drupal.GcePopup || {};

  Drupal.behaviors.gcepopupchangelocation = {
    attach: function (context, settings) {
      // Location handler
      if ($('#change-user-location').length) {
        $('#change-user-location').on("click", function (e) {
          Drupal.GcePopup.getUserPosition();
        });
      }
      // Popup handler
      var popup_id = '#gce-popup';
      if ($(popup_id).length) {
        $(popup_id).find("a.agree-btn").on("click", function (e) {
          Drupal.GcePopup.getUserPosition();
        });
      }
    }
  };

  /**
   * Helper function remove cookie.
   */
  Drupal.GcePopup.removeCookie = function (cookieName) {
    $.cookie(cookieName, null, {path: '/'});
  }

  /**
   * Helper function set cookie.
   */
  Drupal.GcePopup.setCookie = function (cookieName, value) {
    $.cookie(cookieName, value, {path: '/'});
  }

  /**
   * Helper function get user current possition to cookie.
   */
  Drupal.GcePopup.getUserPosition = function () {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(Drupal.GcePopup.setUserPosition, Drupal.GcePopup.showBrowserError);
    }
    else {
      alert("Geolocation is not supported by this browser.");
    }
  }

  /**
   * Helper function set user current possition to cookie.
   */
  Drupal.GcePopup.setUserPosition = function (position) {
    // Serialize curent position
    var gceCurrentPosition = JSON.stringify({
      latitude: position.coords.latitude,
      longitude: position.coords.longitude,
    });
    Drupal.GcePopup.setCookie('gceCurrentPosition', gceCurrentPosition);
    Drupal.GcePopup.removeCookie('gceUserLocation');
    // Reload page with locations
    location.reload();
  }

  /**
   * Helper function set user current possition to cookie.
   */
  Drupal.GcePopup.showBrowserError = function (error) {
    switch (error.code) {
      case error.PERMISSION_DENIED:
        errorMessage = "User denied the request for Geolocation."
        break;
      case error.POSITION_UNAVAILABLE:
        errorMessage = "Location information is unavailable."
        break;
      case error.TIMEOUT:
        errorMessage = "The request to get user location timed out."
        break;
      case error.UNKNOWN_ERROR:
        errorMessage = "An unknown error occurred."
        break;
    }
    alert(errorMessage);
  }
})(jQuery);
