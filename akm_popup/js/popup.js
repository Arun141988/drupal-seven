/**
 * @file
 * Provide functionality for popup.
 */

(function ($) {

  Drupal.GcePopup = Drupal.GcePopup || {};

  Drupal.behaviors.gcepopup = {
    attach: function (context, settings) {

      // Popup handler
      var popup_id = '#gce-popup';
      if ($(popup_id).length) {
        var tiomeout = Drupal.settings.gcePopupTimeOffset;

        $(popup_id).once(function () {
          if (tiomeout) {
            setTimeout(Drupal.GcePopup.fancyboxOpen, tiomeout * 1000, popup_id);
          }
          else {
            Drupal.GcePopup.fancyboxOpen(popup_id);
          }
        });

        $(popup_id).find("a.agree-btn").on("click", function (e) {
          Drupal.GcePopup.setCookie('ageGateConfirm', 1);
          Drupal.GcePopup.fancyboxClose();
          e.preventDefault();
        });
      }

      // Video popup handler
      var video_popup_id = '#gce-video-popup';
      if ($(video_popup_id).length) {
        $(video_popup_id).once(function () {
          setTimeout(Drupal.GcePopup.onYouTubeIframeAPIReady, 1000);
          Drupal.GcePopup.onVimeoIframeAPIReady();
          setTimeout(Drupal.GcePopup.onJWPlayerAPIReady, 3000);
          //Drupal.GcePopup.onDailymotionAPIReady();
        });

        $(video_popup_id).find("a.agree-btn").on("click", function (e) {
          Drupal.GcePopup.fancyboxClose();
        });
      }

    }
  };

  Drupal.GcePopup.youtubeplayers = new Array();
  Drupal.GcePopup.startpaly;

  window.addEventListener ? window.addEventListener("message", onDailymotionAPIReady, !1) : window.attachEvent("message", onDailymotionAPIReady);

  function onDailymotionAPIReady(event) {

    if (!/^http?:\/\/www.dailymotion.com/.test(event.origin)) {
      return !1;
    }

    var query = {};
    var parsed = event.data.split('&');

    parsed.forEach(function (elem, iter, arr) {
      var vals = arr[iter].split('=');
      query[vals[0]] = vals[1];
    });

    if (query.event === 'timeupdate') {
      var video_tiomeout = Drupal.settings.gceVideoPopupTimeOffset;

      if (!Drupal.GcePopup.startpaly) {
        Drupal.GcePopup.startpaly = parseFloat(query.time);
      }

      if (query.time > parseFloat(Drupal.GcePopup.startpaly) + parseFloat(video_tiomeout)) {
        event.source.postMessage('pause', event.origin);
        event.source.postMessage('seek=0', event.origin);
        Drupal.GcePopup.fancyboxOpen('#gce-video-popup');
        Drupal.GcePopup.startpaly = null;
      }
    }
  }

  /**
   * Helper function stoped video.
   */
  Drupal.GcePopup.stopDailymotionVideo = function (e) {
    console.log(e, 'stopDailymotionVideo');
    //Drupal.GcePopup.fancyboxOpen('#gce-video-popup');
  }

  /**
   * Helper called when Iframe youtube API ready.
   */
  Drupal.GcePopup.onVimeoIframeAPIReady = function (player_id) {
    $('iframe.vimeo-iframe').each(function (n, e) {
      $f(e).addEvent('ready', function (player_id) {
        $f(e).addEvent('playProgress', Drupal.GcePopup.stopVimeoVideo);
      });
    });
  }

  /**
   * Helper function stoped video.
   */
  Drupal.GcePopup.stopVimeoVideo = function (data, player_id) {
    if (data.seconds && data.seconds > Drupal.settings.gceVideoPopupTimeOffset) {
      $f(player_id).api('pause');
      $f(player_id).api('seekTo', 0);
      Drupal.GcePopup.fancyboxOpen('#gce-video-popup');
    }
  }

  /**
   * Helper function called when Iframe youtube API ready.
   */
  Drupal.GcePopup.onYouTubeIframeAPIReady = function () {
    jQuery('iframe.youtube-iframe').each(function (index, value) {
      var id = this.getAttribute('id');
      Drupal.GcePopup.youtubeplayers[index] = new window.YT.Player(id, {
        videoId: jQuery('#' + id).val(),
        events: {
          onStateChange: Drupal.GcePopup.onYouTubePlayerStateChange
        }
      });
    });
  }

  /**
   * Helper function called when iframe play.
   */
  Drupal.GcePopup.onYouTubePlayerStateChange = function (event) {
    var video_tiomeout = Drupal.settings.gceVideoPopupTimeOffset;

    if (event.data == window.YT.PlayerState.PLAYING) {
      setTimeout(Drupal.GcePopup.stopYouTubeVideo, video_tiomeout * 1000);
    }
  }

  /**
   * Helper function stoped video.
   */
  Drupal.GcePopup.stopYouTubeVideo = function () {
    for (var h = 0; h < Drupal.GcePopup.youtubeplayers.length; h++) {
      Drupal.GcePopup.youtubeplayers[h].seekTo(0, true);
      Drupal.GcePopup.youtubeplayers[h].stopVideo();
    }
    Drupal.GcePopup.fancyboxOpen('#gce-video-popup');
  }

  /**
   * Helper function called when JWplayer ready.
   */
  Drupal.GcePopup.onJWPlayerAPIReady = function () {
    var jwplayers = $('div.field-video-item>div[id^=jwplayer], div.field-video-item>div>div>div>div[id^=jwplayer]');
    $(jwplayers).each(function (n, e) {
      var playerInstance = jwplayer(e);
      playerInstance.onPlay(function () {
        if (typeof playerInstance.thumbnailIsSet !== 'undefined') {
          var video_tiomeout = Drupal.settings.gceVideoPopupTimeOffset;
          setTimeout(Drupal.GcePopup.stopJWPlayer, video_tiomeout * 1000, playerInstance);
        }
      });
    });
  }

  /**
   * Helper function called when JWplayer ready.
   */
  Drupal.GcePopup.stopJWPlayer = function (playerInstance) {
    playerInstance.stop();
    Drupal.GcePopup.fancyboxOpen('#gce-video-popup');
  }

  /**
   * Helper function close fancybox popup.
   */
  Drupal.GcePopup.fancyboxClose = function () {
    $.fancybox.close();
  }

  /**
   * Helper function open fancybox popup.
   */
  Drupal.GcePopup.fancyboxOpen = function (popup_id) {
    $.fancybox({
      href: popup_id,
      closeBtn: false,
      maxWidth: 600,
      closeEffect: 'none',
      openEffect: 'fade',
      helpers: {
        overlay: {
          css: {
            background: '#ee84c1'
          }
        }
      }
    });
  }
})(jQuery);
